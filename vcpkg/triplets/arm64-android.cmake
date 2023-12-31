set(VCPKG_TARGET_ARCHITECTURE arm64)
set(VCPKG_CRT_LINKAGE dynamic)
# set(DYNAMIC_PORTS
#   "zlib"
#   "egl-registry"
#   "bzip2"
#   "pthreads"
#   "opengl-registry"
#   "libpng"
#   "gettext-libintl"
#   "brotli"
#   "getopt"
#   "expat"
#   "pthread"
#   "openssl"
#   "opengl"
#   "lz4"
#   "libuuid"
#   "freetype"
#   "dirent"
#   "libiconv"
#   "gettext"
#   "gperf"
#   "double-conversion"
#   "egl"
#   "sqlite3"
#   "zstd"
#   "pcre2"
#   "fontconfig"
#   "libpq"
#   "libjpeg-turbo"
#   "libb2"
#   "icu"
#   "harfbuzz"
# )

include("${CMAKE_CURRENT_LIST_DIR}/stat.cmake")
message(STATUS "Building static: ${STATIC_PORTS}")
# set(STATIC_PORTS
#     catch2
#     libiconv
#     freexl
#     libspatialite
# )
if (PORT IN_LIST STATIC_PORTS)
    set(VCPKG_LIBRARY_LINKAGE static)
else()
    set(VCPKG_LIBRARY_LINKAGE dynamic)
endif()
set(VCPKG_CMAKE_SYSTEM_NAME Android)
set(VCPKG_BUILD_TYPE release)
set(VCPKG_MAKE_BUILD_TRIPLET "--host=aarch64-linux-android")
set(VCPKG_CMAKE_CONFIGURE_OPTIONS -DANDROID_ABI=arm64-v8a)
set(ANDROID_SDK_ROOT $ENV{ANDROID_SDK_ROOT})

set(VCPKG_CXX_FLAGS "-fstack-protector-strong")
set(VCPKG_C_FLAGS "-fstack-protector-strong")
