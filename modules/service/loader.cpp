#include <system_error>

#include "loader.h"

#if __has_include(<Windows.h>)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

loader_t::~loader_t() noexcept {
    ::FreeLibrary(static_cast<HMODULE>(handle));
}

loader_t loader_t::load(const char* file) noexcept(false) {
    if (auto m = ::LoadLibraryA(file))
        return loader_t{m};

    using namespace std;
    throw system_error{static_cast<int>(GetLastError()), system_category(),
                       "LoadLibraryA"};
}

auto loader_t::lookup(const char* symbol) const noexcept -> void* {
    return ::GetProcAddress(static_cast<HMODULE>(handle), symbol);
}

auto loader_t ::error() noexcept -> const char* {
    return strerror(static_cast<errno_t>(GetLastError()));
}

#elif __has_include(<dlfcn.h>)
#include <dlfcn.h>

loader_t::~loader_t() noexcept {
    ::dlclose(handle);
}

loader_t loader_t::load(const char* file) noexcept(false) {
    if (auto m = ::dlopen(file, RTLD_NOW))
        return loader_t{m};

    throw std::runtime_error{error()};
}

auto loader_t::lookup(const char* symbol) const noexcept -> void* {
    return ::dlsym(handle, symbol);
}

auto loader_t ::error() noexcept -> const char* {
    return ::dlerror();
}

#endif
