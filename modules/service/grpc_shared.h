#pragma once
#include <grpcpp/grpcpp.h>
#include <new>

#if __has_include(<coroutine>)
#include <coroutine>
using namespace std;

#elif __has_include(<experimental/coroutine>)
#include <experimental/coroutine>
using namespace std;
using namespace std::experimental;

#endif

using suspend_callback_t = function<void(coroutine_handle<void>)>;

class suspend_on_completion_queue_t {
  protected:
    suspend_callback_t callback;

  public:
    void set_false() noexcept {
        callback = nullptr;
    }

    bool await_ready() noexcept {
        return false;
    }
    void await_suspend(coroutine_handle<void> coro) noexcept(false) {
        return callback(coro);
    }
    bool await_resume() noexcept {
        return static_cast<bool>(callback);
    }
};

struct promise_service_coroutine_t : public suspend_on_completion_queue_t {
    auto initial_suspend() noexcept {
        return suspend_never{};
    }
    auto final_suspend() noexcept {
        return suspend_never{};
    }
    void unhandled_exception() noexcept(false) {
        throw;
    }

    void return_value(grpc::Status) noexcept {
        // do nothing with the value.
    }

    auto get_return_object() noexcept {
        return this;
    }

    auto await_transform(suspend_callback_t&& fn) noexcept
        -> suspend_on_completion_queue_t& {
        this->callback = move(fn);
        return *this;
    }
};

class service_coroutine_t final : public coroutine_handle<void> {
  public:
    using promise_type = promise_service_coroutine_t;

  public:
    service_coroutine_t(promise_type* prom) noexcept(false) {
        if (coroutine_handle<void>& ref = *this; prom)
            ref = coroutine_handle<promise_type>::from_promise(*prom);
    };
};
