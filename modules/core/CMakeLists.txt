
add_custom_command(
    COMMENT     "generating from 'service.proto' ..."
    OUTPUT      ${PROJECT_SOURCE_DIR}/static/service.pb.h
                ${PROJECT_SOURCE_DIR}/static/service.pb.cc
                ${PROJECT_SOURCE_DIR}/static/service.grpc.pb.h
                ${PROJECT_SOURCE_DIR}/static/service.grpc.pb.cc
    # WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMAND     ${proto_compiler_path} --version
    COMMAND     ${proto_compiler_path} ${PROJECT_SOURCE_DIR}/service.proto 
                    --proto_path=${PROJECT_SOURCE_DIR}
                    --cpp_out=${PROJECT_SOURCE_DIR}/static
    COMMAND     ${proto_compiler_path} ${PROJECT_SOURCE_DIR}/service.proto 
                    --proto_path=${PROJECT_SOURCE_DIR}
                    --grpc_out=${PROJECT_SOURCE_DIR}/static
                    --plugin="protoc-gen-grpc=${grpc_cpp_plugin_path}"
)

add_library(core
    libmain.cpp
    ${PROJECT_SOURCE_DIR}/static/service.pb.h
    ${PROJECT_SOURCE_DIR}/static/service.pb.cc
    ${PROJECT_SOURCE_DIR}/static/service.grpc.pb.h
    ${PROJECT_SOURCE_DIR}/static/service.grpc.pb.cc
)

set_target_properties(core
PROPERTIES
    CXX_STANDARD 20
    WINDOWS_EXPORT_ALL_SYMBOLS True
)

# set(VCPKG_INCLUDE_DIR ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include)
# set(VCPKG_LIB_DIR     ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib)

target_include_directories(core
PRIVATE
    ${PROJECT_SOURCE_DIR}/static
PUBLIC
    ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>
)

target_link_directories(core
PUBLIC
    ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib
)

target_link_libraries(core
PUBLIC
    ${CMAKE_DL_LIBS}
    protobuf::libprotobuf # protobuf::libprotobuf-lite
    gRPC::grpc++ # gRPC::gpr gRPC::grpc gRPC::grpc_cronet
)

target_compile_definitions(core
PRIVATE
    PLATFORM_NAME=L"${CMAKE_SYSTEM_NAME}"
)

if(MSVC)
    target_compile_options(core
    PUBLIC
        /await
    )
    target_compile_definitions(core
    PUBLIC
        WIN32_LEAN_AND_MEAN NOMINMAX
        _WIN32_WINNT=0x0A00 # be explicit
    )
elseif(UNIX)
    target_compile_options(core
    PUBLIC
        -O0 -g
        -fPIC -stdlib=libc++ -fcoroutines-ts
    )
    target_link_libraries(core
    PUBLIC
        c++
    )
endif()
