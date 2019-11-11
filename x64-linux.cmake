# https://github.com/microsoft/vcpkg/blob/master/docs/users/triplets.md
set(VCPKG_TARGET_ARCHITECTURE x64)
set(VCPKG_CRT_LINKAGE dynamic)
set(VCPKG_LIBRARY_LINKAGE dynamic)

set(VCPKG_CMAKE_SYSTEM_NAME Linux)

# linker flags
set(VCPKG_C_FLAGS "-lc++")
set(VCPKG_CXX_FLAGS "-std=c++17 -stdlib=libc++ ${VCPKG_C_FLAGS}")
