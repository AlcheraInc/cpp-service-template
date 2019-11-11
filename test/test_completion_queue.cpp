#define CATCH_CONFIG_FAST_COMPILE
#include <catch2/catch.hpp>
#include <gsl/gsl>

#include <grpcpp/grpcpp.h>

void consume_user_data(void* user_data, bool) noexcept(false) {
    if (user_data == nullptr)
        FAIL("user_data from completion queue must not nullptr");
}

// fetch and resume without signal checking
void drain_queue(grpc::CompletionQueue& cq) noexcept(false) {
    void* user_data = nullptr;
    bool ok = false;
    while (cq.Next(&user_data, &ok))
        consume_user_data(user_data, ok);
}

void shutdown_queue(grpc::Server& s,
                    grpc::CompletionQueue& cq) noexcept(false) {
    auto on_return = gsl::finally([&cq]() {
        cq.Shutdown();
        // we have to drain the queue.
        // see https://github.com/grpc/grpc/issues/8610
        drain_queue(cq);
    });
    s.Shutdown();
    return;
}

void counting_serve_queue(grpc::Server& s, grpc::CompletionQueue& cq,
                          size_t repeat) {
    auto on_return_1 = gsl::finally([&cq]() {
        cq.Shutdown();
        drain_queue(cq); // drain on-going requests
    });
    auto on_return_2 = gsl::finally([&s]() {
        s.Shutdown(); // no more request
    });
    auto is_finished = [&repeat]() { return repeat-- > 0; };

    void* user_data = nullptr;
    bool ok = false;
    // fetch and resume with signal checking
    while (is_finished() == false && cq.Next(&user_data, &ok) == true)
        consume_user_data(user_data, ok);
}

TEST_CASE("completion queue drain", "[grpc]") {

    grpc::ServerBuilder builder{};
    builder.AddListeningPort("0.0.0.0:9090", grpc::InsecureServerCredentials());

    SECTION("no registration") {
        // no server object registration
    }

    auto cq = builder.AddCompletionQueue();
    REQUIRE(cq);
    auto s = builder.BuildAndStart();
    REQUIRE(s);

    REQUIRE_NOTHROW(shutdown_queue(*s, *cq));
}
