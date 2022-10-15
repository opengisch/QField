set(VCPKG_TARGET_ARCHITECTURE x64)
set(VCPKG_CRT_LINKAGE dynamic)
set(VCPKG_LIBRARY_LINKAGE static)
set(VCPKG_CMAKE_SYSTEM_NAME Android)
set(VCPKG_BUILD_TYPE release)
set(VCPKG_MAKE_BUILD_TRIPLET "--host=x86_64-linux-android")

set(VCPKG_CXX_FLAGS "-fstack-protector-strong")
set(VCPKG_C_FLAGS "-fstack-protector-strong")
