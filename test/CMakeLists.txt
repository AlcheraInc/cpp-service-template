
set(TEST_SUITE ${PROJECT_NAME}_test_suite)
add_executable(${TEST_SUITE}
    test_main.cpp
    test_env.cpp
    test_completion_queue.cpp
)

set_target_properties(${TEST_SUITE}
PROPERTIES
    CXX_STANDARD 20
)

target_include_directories(${TEST_SUITE}
PRIVATE
    ${PROJECT_SOURCE_DIR}/test
    ${PROJECT_SOURCE_DIR}/static
)

target_link_libraries(${TEST_SUITE}
PRIVATE
    core
    Catch2::Catch2
)
add_test(test_platform_abi  ${TEST_SUITE} "[abi]")
add_test(test_grpc_features ${TEST_SUITE} "[grpc]")
