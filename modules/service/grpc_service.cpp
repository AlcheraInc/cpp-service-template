
#include <service.grpc.pb.h>

#include "grpc_shared.h"

using namespace std;

auto serve_invoke1(grpc_impl::ServerCompletionQueue& queue,
                   svc_v1::SH::AsyncService& service) noexcept(false)
    -> service_coroutine_t;

auto serve_invoke2(grpc_impl::ServerCompletionQueue& queue,
                   svc_v1::SH::AsyncService& service) noexcept(false)
    -> service_coroutine_t;

auto serve_invoke3(grpc_impl::ServerCompletionQueue& queue,
                   svc_v1::SH::AsyncService& service) noexcept(false)
    -> service_coroutine_t;

auto serve_invoke4(grpc_impl::ServerCompletionQueue& queue,
                   svc_v1::SH::AsyncService& service) noexcept(false)
    -> service_coroutine_t;

void consume_queue(grpc::Server& s, grpc::CompletionQueue& q) noexcept(false);

void serve(grpc::Server& launcher, //
           grpc_impl::ServerCompletionQueue& queue,
           svc_v1::SH::AsyncService& service) noexcept(false) {
    // we have 4 method. each will be served with coroutines
    // all coroutines will be destructed automatically

    // each coroutine will hold a context from the plugin
    serve_invoke1(queue, service);
    serve_invoke2(queue, service);
    serve_invoke3(queue, service);
    serve_invoke4(queue, service);

    // run with the completion queue.
    // the grpc server will be shutdowned when a signal raised
    return consume_queue(launcher, queue);
}
