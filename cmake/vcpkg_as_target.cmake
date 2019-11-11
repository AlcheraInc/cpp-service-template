cmake_minimum_required(VERSION 3.13)

if(${CMAKE_TOOLCHAIN_FILE} MATCHES vcpkg.cmake)
    message(STATUS "Vcpkg")
    message(STATUS "  Path      \t: ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}")
    message(STATUS)

    set(VCPKG_INCLUDE_DIR ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include)
    set(VCPKG_LIB_DIR     ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib)
    # target_include_directories(vcpkg_meta
    # PRIVATE
    #     ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include
    # )
    # target_link_directories(vcpkg_meta
    # PRIVATE
    #     ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib
    # )
endif()
