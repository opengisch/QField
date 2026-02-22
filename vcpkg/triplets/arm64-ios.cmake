set(VCPKG_TARGET_ARCHITECTURE arm64)
set(VCPKG_CRT_LINKAGE dynamic)
set(VCPKG_LIBRARY_LINKAGE static)
set(VCPKG_CMAKE_SYSTEM_NAME iOS)
set(VCPKG_OSX_ARCHITECTURES arm64)

set(VCPKG_OSX_DEPLOYMENT_TARGET 16.0)

if(DEFINED ENV{DEPLOYMENT_TARGET} AND NOT "$ENV{DEPLOYMENT_TARGET}" STREQUAL "${VCPKG_OSX_DEPLOYMENT_TARGET}")
  message(FATAL_ERROR "The deployment targets set via environment ($ENV{DEPLOYMENT_TARGET}) and in the vcpkg triplet (${VCPKG_OSX_DEPLOYMENT_TARGET}) do not match.")
endif()
set(VCPKG_BUILD_TYPE release)
set(VCPKG_CXX_FLAGS "-fvisibility=hidden -g")
set(VCPKG_C_FLAGS "-fvisibility=hidden -g")
