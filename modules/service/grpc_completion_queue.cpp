
#include <csignal>
#include <gsl/gsl>
#include <service.grpc.pb.h>

#include "grpc_shared.h"

using namespace std;

//
// ... global signal status ...
//

volatile sig_atomic_t sig_status = 0;

void on_signal_change_sig_status(sig_atomic_t signum) {
    sig_status = signum;
};

bool is_signaled() noexcept {
    return sig_status != 0;
}

bool is_shutdowned() noexcept {
    return is_signaled();
}

//
// ... completion queue related works ...
//

void consume_user_data(void* user_data, bool failed) noexcept(false) {
    using promise_t = promise_service_coroutine_t;
    auto task = coroutine_handle<promise_t>::from_address(user_data);

    if (failed == false)
        task.promise().set_false();

    if (task && task.done() == false)
        task.resume();
}

// fetch and resume without signal checking
void drain_queue(grpc::CompletionQueue& cq) noexcept(false) {
    void* user_data = nullptr;
    bool ok = false;
    while (cq.Next(&user_data, &ok))
        consume_user_data(user_data, ok);
}

void consume_queue(grpc::Server& launcher,
                   grpc::CompletionQueue& cq) noexcept(false) {
    auto on_return_1 = gsl::finally([&cq]() {
        cq.Shutdown();
        // we have to drain the queue.
        // see https://github.com/grpc/grpc/issues/8610
        drain_queue(cq);
    });
    auto on_return_2 = gsl::finally([&launcher]() {
        // make sure of the shutdown
        launcher.Shutdown();
    });

    // hijack some signals. it will break the completion queue
    signal(SIGTERM, on_signal_change_sig_status);

    void* user_data = nullptr;
    bool ok = false;
    // fetch and resume with signal checking
    while (is_signaled() == false && cq.Next(&user_data, &ok) == true)
        consume_user_data(user_data, ok);
}
