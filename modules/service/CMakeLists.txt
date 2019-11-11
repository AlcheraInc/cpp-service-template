
add_executable(service
    launcher.cpp
    loader.h
    loader.cpp
    grpc_shared.h
    grpc_completion_queue.cpp
    grpc_service.cpp
    grpc_handlers.cpp
)

set_target_properties(service
PROPERTIES
    CXX_STANDARD 20
)
target_include_directories(service
PRIVATE
    ${PROJECT_SOURCE_DIR}/static
)
target_link_libraries(service
PRIVATE
    core
)
