set(VCPKG_TARGET_ARCHITECTURE arm)
set(VCPKG_CRT_LINKAGE static)
set(VCPKG_LIBRARY_LINKAGE static)
set(VCPKG_CMAKE_SYSTEM_NAME Android)
set(VCPKG_BUILD_TYPE release)

set(ENV{VCPKG_ANDROID_NATIVE_API_LEVEL} "24")
set(VCPKG_ANDROID_NATIVE_API_LEVEL "24")
set(ANDROID_STL c++_static)
set(ANDROID_ARM_NEON TRUE)

set(VCPKG_CXX_FLAGS "-fstack-protector-strong")
set(VCPKG_C_FLAGS "-fstack-protector-strong")
