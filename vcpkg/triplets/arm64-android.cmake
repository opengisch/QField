set(VCPKG_TARGET_ARCHITECTURE arm64)
set(VCPKG_CRT_LINKAGE dynamic)
set(VCPKG_LIBRARY_LINKAGE static)
set(VCPKG_CMAKE_SYSTEM_NAME Android)
set(VCPKG_BUILD_TYPE release)

set(ENV{VCPKG_ANDROID_NATIVE_API_LEVEL} "detect")

set(ENV{CXXFLAGS} "-fstack-protector-strong -fsanitize=address -fno-omit-frame-pointer")
set(ENV{CFLAGS} "-fstack-protector-strong -fsanitize=address -fno-omit-frame-pointer")
set(ENV{LDFLAGS} "-fsanitize=address")

set(VCPKG_CXX_FLAGS "-fstack-protector-strong -fsanitize=address -fno-omit-frame-pointer")
set(VCPKG_C_FLAGS "-fstack-protector-strong -fsanitize=address -fno-omit-frame-pointer")
set(VCPKG_LINKER_FLAGS "-fsanitize=address")
