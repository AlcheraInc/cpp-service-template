
cmake_minimum_required(VERSION 3.14)
project(cpp_service_template LANGUAGES CXX VERSION 1.0)

find_package(protobuf 3.6   CONFIG REQUIRED)
find_package(gRPC           CONFIG REQUIRED)

# run protobuf compiler and generate files
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include(project_info)
include(vcpkg_as_target)
include(grpc_codegen_targets)

add_subdirectory(modules/core)
add_subdirectory(modules/service)

#
# generate/install config & version info
#
include(CMakePackageConfigHelpers)
set(VERSION_FILE_PATH   ${CMAKE_BINARY_DIR}/cmake/${PROJECT_NAME}-config-version.cmake)
write_basic_package_version_file(${VERSION_FILE_PATH}
    VERSION             ${PROJECT_VERSION}
    COMPATIBILITY       SameMajorVersion
)
install(FILES           ${VERSION_FILE_PATH} 
        DESTINATION     ${CMAKE_INSTALL_PREFIX}/share/${PROJECT_NAME}
)

#
# export declared cmake targets
#
install(EXPORT      ${PROJECT_NAME}-config
        NAMESPACE   "${PROJECT_NAME}::"
        DESTINATION ${CMAKE_INSTALL_PREFIX}/share/${PROJECT_NAME}
)

#
# proto definition
#
install(FILES           ${PROJECT_SOURCE_DIR}/service.proto 
        DESTINATION     ${CMAKE_INSTALL_PREFIX}/share/${PROJECT_NAME}
)

#
# core module + launcher
#
install(FILES   ${PROJECT_SOURCE_DIR}/static/service.pb.h
                ${PROJECT_SOURCE_DIR}/static/service.grpc.pb.h
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include
)

# https://stackoverflow.com/questions/33991581/install-name-tool-to-update-a-executable-to-search-for-dylib-in-mac-os-x
install(TARGETS         core service
        EXPORT          ${PROJECT_NAME}-config
        RUNTIME  DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
        LIBRARY  DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        ARCHIVE  DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        INCLUDES DESTINATION ${CMAKE_INSTALL_PREFIX}/include
)

#
# ... testing comes here ...
#
if(NOT CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    return()
endif()

enable_testing()
find_package(Catch2 CONFIG REQUIRED)

add_subdirectory(test)
