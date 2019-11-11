//
//  Author
//      github.com/luncliff
//  Note
//      DLL loading + RAII
//
#pragma once
#ifndef LOADER_H
#define LOADER_H

class loader_t final {
    void* handle = nullptr;

  private:
    loader_t(const loader_t&) = delete;
    loader_t& operator=(const loader_t&) = delete;
    loader_t(loader_t&&) = delete;
    loader_t& operator=(loader_t&&) = delete;

    explicit loader_t(void* _mod) noexcept : handle{_mod} {
    }

  public:
    ~loader_t() noexcept;

  public:
    operator bool() const noexcept {
        return handle != nullptr;
    }

    template <typename FuncType>
    auto lookup(const char* symbol) const noexcept -> FuncType {
        return reinterpret_cast<FuncType>(lookup(symbol));
    }
    auto lookup(const char* symbol) const noexcept -> void*;

  public:
    static loader_t load(const char* file) noexcept(false);
    static auto error() noexcept -> const char*;
};

#endif // LOADER_H
