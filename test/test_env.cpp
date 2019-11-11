#define CATCH_CONFIG_FAST_COMPILE
#include <catch2/catch.hpp>

#if defined(_WIN32)
#else

#include <stdlib.h>
#include <string>

TEST_CASE("env variable", "[abi]") {
    const auto key = "SOME_KEY";
    const auto value = "~~~";

    if (auto failed = setenv(key, value, true); failed)
        REQUIRE(strerror(errno));

    REQUIRE(getenv(key) == std::string{value});
}

#endif
