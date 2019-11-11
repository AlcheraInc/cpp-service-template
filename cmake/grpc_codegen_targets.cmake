
# protobuf compiler
find_program(proto_compiler_path
    NAMES   protoc.exe protoc
    PATHS   ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/tools/protobuf
    # NO_DEFAULT_PATH 
)
message(STATUS "Tools")
message(STATUS "  protoc            : ${proto_compiler_path}")

# grpc plugin: c++
find_program(grpc_cpp_plugin_path
    NAMES   grpc_cpp_plugin.exe grpc_cpp_plugin
    PATHS   ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/tools/grpc
    # NO_DEFAULT_PATH 
)
message(STATUS "  grpc_cpp_plugin   : ${grpc_cpp_plugin_path}")

# code generation: c++
add_custom_target(run_protoc_cpp
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMAND     ${proto_compiler_path} --version
    COMMAND     ${proto_compiler_path} service.proto
                    --cpp_out=${PROJECT_SOURCE_DIR}/static
    COMMAND     ${proto_compiler_path} service.proto
                    --grpc_out=${PROJECT_SOURCE_DIR}/static
                    --plugin="protoc-gen-grpc=${grpc_cpp_plugin_path}"
)

# grpc plugn: python3
find_program(grpc_python_plugin_path
    NAMES   grpc_python_plugin.exe grpc_python_plugin
    PATHS   ${PROJECT_SOURCE_DIR}/bin
            ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/tools/grpc
    # NO_DEFAULT_PATH 
)
message(STATUS "  grpc_python_plugin: ${grpc_python_plugin_path}")

add_custom_target(run_protoc_python
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMAND     ${proto_compiler_path} --version
    COMMAND     ${proto_compiler_path} service.proto
                    --python_out=${PROJECT_SOURCE_DIR}/static
    COMMAND     ${proto_compiler_path} service.proto
                    --grpc_out=${PROJECT_SOURCE_DIR}/static
                    --plugin="protoc-gen-grpc=${grpc_python_plugin_path}"
)

# grpc plugn: node.js
find_program(grpc_node_plugin_path
    NAMES   grpc_node_plugin.exe grpc_node_plugin
    PATHS   ${PROJECT_SOURCE_DIR}/bin
            ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/tools/grpc
    # NO_DEFAULT_PATH 
)
message(STATUS "  grpc_node_plugin  : ${grpc_node_plugin_path}")

add_custom_target(run_protoc_nodejs
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMAND     ${proto_compiler_path} --version
    COMMAND     ${proto_compiler_path} service.proto 
                    --grpc_out=${PROJECT_SOURCE_DIR}/static
                    --plugin="protoc-gen-grpc=${grpc_node_plugin_path}"
)


# grpc plugn: csharp
find_program(grpc_csharp_plugin_path
    NAMES   grpc_csharp_plugin.exe grpc_csharp_plugin
    PATHS   ${PROJECT_SOURCE_DIR}/bin
            ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/tools/grpc
    # NO_DEFAULT_PATH 
)
message(STATUS "  grpc_csharp_plugin: ${grpc_csharp_plugin_path}")

add_custom_target(run_protoc_csharp
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMAND     ${proto_compiler_path} --version
    COMMAND     ${proto_compiler_path} service.proto
                    --csharp_out=${PROJECT_SOURCE_DIR}/static
    COMMAND     ${proto_compiler_path} service.proto
                    --grpc_out=${PROJECT_SOURCE_DIR}/static
                    --plugin="protoc-gen-grpc=${grpc_csharp_plugin_path}"
)

if (DEFINED $ENV{GOROOT})
    # $env:GOPATH=pwd
    # go get -u github.com/golang/protobuf/protoc-gen-go
    find_program(grpc_go_plugin_path
        NAMES   protoc-gen-go.exe protoc-gen-go
        PATHS   ${PROJECT_SOURCE_DIR}/bin
                ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/tools/grpc
        NO_DEFAULT_PATH 
    )
    message(STATUS "  grpc_go_plugin:     ${grpc_go_plugin_path}")

    add_custom_target(run_protoc_go
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMAND     ${proto_compiler_path} --version
        COMMAND     ${proto_compiler_path} service.proto
                        --go_out="${PROJECT_SOURCE_DIR}/static"
                        --plugin="protoc-gen-go=${grpc_go_plugin_path}"
        COMMAND     ${proto_compiler_path} service.proto
                        --go_out="plugins=grpc:${PROJECT_SOURCE_DIR}/static"
                        --plugin="protoc-gen-go=${grpc_go_plugin_path}"
    )
endif()

message(STATUS)