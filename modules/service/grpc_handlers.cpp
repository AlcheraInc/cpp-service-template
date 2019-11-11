
#include <gsl/gsl>
#include <service.grpc.pb.h>

#include "grpc_shared.h"

using namespace std;
using namespace grpc;

bool is_shutdowned() noexcept;

auto until(function<void(coroutine_handle<void>)>&& on_suspend) noexcept {
    return on_suspend;
}

auto serve_invoke1(grpc_impl::ServerCompletionQueue& queue,
                   svc_v1::SH::AsyncService& service) noexcept(false)
    -> service_coroutine_t {
    // serve next request unless the program is shutdowned
    auto on_return = gsl::finally([&]() {
        if (is_shutdowned() == false)
            serve_invoke1(queue, service);
    });
    // prepare to receive 1 request
    ServerContext sctx{};
    ServerAsyncResponseWriter<svc_v1::Response> responder{&sctx};
    svc_v1::Request req{};

    co_await until([&](coroutine_handle<void> coro) {
        service.RequestInvoke1(&sctx, &req, &responder, &queue, &queue,
                               coro.address());
    });

    svc_v1::Response res{};
    grpc::Status status{grpc::StatusCode::OK, svc_v1::SH::service_full_name()};
    res.set_blob(req.blob());

    co_await until([&](coroutine_handle<void> coro) {
        responder.Finish(res, status, coro.address());
    });
    co_return status;
}

auto serve_invoke2(grpc_impl::ServerCompletionQueue& queue,
                   svc_v1::SH::AsyncService& service) noexcept(false)
    -> service_coroutine_t {
    // serve next request unless the program is shutdowned
    auto on_return = gsl::finally([&]() {
        if (is_shutdowned() == false)
            serve_invoke1(queue, service);
    });

    ServerContext sctx{};
    ServerAsyncWriter<svc_v1::Response> writer{&sctx};
    svc_v1::Request req{};

    co_await until([&](coroutine_handle<void> coro) {
        service.RequestInvoke2(&sctx, &req, &writer, &queue, &queue,
                               coro.address());
    });

    svc_v1::Response res{};
    grpc::Status status{grpc::StatusCode::OK, svc_v1::SH::service_full_name()};
    res.set_blob(req.blob());

    for (auto i = 0u; i < 1u; ++i)
        co_await until([&](coroutine_handle<void> coro) {
            writer.Write(res, coro.address());
        });
    co_await until([&](coroutine_handle<void> coro) {
        writer.Finish(status, coro.address());
    });
    co_return status;
}

auto serve_invoke3(grpc_impl::ServerCompletionQueue& queue,
                   svc_v1::SH::AsyncService& service) noexcept(false)
    -> service_coroutine_t {
    // serve next request unless the program is shutdowned
    auto on_return = gsl::finally([&]() {
        if (is_shutdowned() == false)
            serve_invoke1(queue, service);
    });

    ServerContext sctx{};
    ServerAsyncReader<svc_v1::Response, svc_v1::Request> reader{&sctx};

    co_await until([&](coroutine_handle<void> coro) {
        service.RequestInvoke3(&sctx, &reader, &queue, &queue, coro.address());
    });

    svc_v1::Request req{};
    svc_v1::Response res{};
    grpc::Status status{grpc::StatusCode::OK, svc_v1::SH::service_full_name()};

    while (co_await until([&](coroutine_handle<void> coro) {
        reader.Read(&req, coro.address());
    })) {
        res.set_blob(req.blob()); // will return the last one
    }
    co_await until([&](coroutine_handle<void> coro) {
        reader.Finish(res, status, coro.address());
    });
    co_return status;
}

auto serve_invoke4(grpc_impl::ServerCompletionQueue& queue,
                   svc_v1::SH::AsyncService& service) noexcept(false)
    -> service_coroutine_t {
    // serve next request unless the program is shutdowned
    auto on_return = gsl::finally([&]() {
        if (is_shutdowned() == false)
            serve_invoke1(queue, service);
    });

    grpc::ServerContext sctx{};
    ServerAsyncReaderWriter<svc_v1::Response, svc_v1::Request> link{&sctx};

    co_await until([&](coroutine_handle<void> coro) {
        service.RequestInvoke4(&sctx, &link, &queue, &queue, coro.address());
    });

    svc_v1::Request req{};
    svc_v1::Response res{};
    grpc::Status status{grpc::StatusCode::OK, svc_v1::SH::service_full_name()};

    for (; // read one time ...
         co_await until([&](coroutine_handle<void> coro) {
             link.Read(&req, coro.address());
         });
         // write one time ...
         co_await until([&](coroutine_handle<void> coro) {
             link.Write(res, coro.address());
         })) {
        res.set_blob(req.blob()); // will return the last one
    }
    co_await until([&](coroutine_handle<void> coro) {
        // finish the serving ...
        link.Finish(status, coro.address());
    });
    co_return status;
}
