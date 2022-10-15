set(NUGET_SOURCE "https://nuget.pkg.github.com/opengisch/index.json")
set(NUGET_USERNAME "qfield-fairy" CACHE STRING "Nuget user")
# Obfuscate a (read only) github token, if it's here clear text it will be invalidated
# Python: print(*(ord(i) for i in token), sep=";")

# Disable nuget token by default
# set(NUGET_TOKEN_ASCII
#     103;104;112;95;116;76;65;120;74;80;120;70;84;109;122;112;85;103;68;114;106;90;52;74;55;85;117;111;107;74;48;113;70;112;51;100;52;77;113;102
# )
# string(ASCII ${NUGET_TOKEN_ASCII} NUGET_TOKEN_DEFAULT)
set(NUGET_TOKEN "" CACHE STRING "Nuget token")

string(COMPARE EQUAL "${CMAKE_HOST_SYSTEM_NAME}" "Windows" _HOST_IS_WINDOWS)
set(_WITH_VCPKG_DEFAULT ${_HOST_IS_WINDOWS})
if(${VCPKG_TARGET_TRIPLET} MATCHES "android")
  message(STATUS "Building for Android --")
  set(_BUILD_FOR_ANDROID ON)
  set(_WITH_VCPKG_DEFAULT ON)
endif()
set(WITH_VCPKG ${_WITH_VCPKG_DEFAULT} CACHE BOOL "Use the vcpkg submodule for dependency management.")

if(NOT WITH_VCPKG)
  message(STATUS "Building with system libraries --")
  return()
endif()

if(NOT VCPKG_TAG STREQUAL VCPKG_INSTALLED_VERSION)
  message(STATUS "Updating vcpkg")
  include(FetchContent)
  FetchContent_Declare(vcpkg
      GIT_REPOSITORY https://github.com/microsoft/vcpkg.git
      GIT_TAG ${VCPKG_TAG}
  )
  FetchContent_MakeAvailable(vcpkg)
else()
  message(STATUS "Using cached vcpkg")
endif()
set(VCPKG_ROOT "${FETCHCONTENT_BASE_DIR}/vcpkg-src" CACHE STRING "")
set(CMAKE_TOOLCHAIN_FILE "${FETCHCONTENT_BASE_DIR}/vcpkg-src/scripts/buildsystems/vcpkg.cmake" CACHE FILEPATH "")

find_package(Git REQUIRED)
if(WIN32)
  execute_process(COMMAND cmd /C "${GIT_EXECUTABLE} -C ${VCPKG_ROOT} rev-parse HEAD" OUTPUT_VARIABLE VCPKG_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)
else()
  execute_process(COMMAND bash -c "${GIT_EXECUTABLE} -C ${VCPKG_ROOT} rev-parse HEAD" OUTPUT_VARIABLE VCPKG_VERSION ERROR_VARIABLE ERR OUTPUT_STRIP_TRAILING_WHITESPACE)
endif()

set(VCPKG_INSTALLED_VERSION ${VCPKG_VERSION} CACHE STRING "" FORCE)

message(STATUS "Building with vcpkg libraries version ${VCPKG_INSTALLED_VERSION}")

if(BUILD_WITH_QT6)
  set(VCPKG_MANIFEST_DIR "${CMAKE_SOURCE_DIR}/.qt6")
endif()

if(_BUILD_FOR_ANDROID)
  if(NOT DEFINED ENV{ANDROID_NDK_HOME})
    message(FATAL_ERROR "ANDROID_NDK_HOME environment variable is not set")
  endif()
  set(VCPKG_CHAINLOAD_TOOLCHAIN_FILE "$ENV{ANDROID_NDK_HOME}/build/cmake/android.toolchain.cmake")
endif()

# Binarycache can only be used on Windows or if mono is available.
find_program(_VCPKG_MONO mono)
if(NOT "${NUGET_TOKEN}" STREQUAL "" AND (_HOST_IS_WINDOWS OR EXISTS "${_VCPKG_MONO}"))
  # Early bootstrap, copied from the vcpkg toolchain, we need this to fetch nuget
  if(_HOST_IS_WINDOWS)
    set(_VCPKG_EXECUTABLE "${VCPKG_ROOT}/vcpkg.exe")
    set(_VCPKG_BOOTSTRAP_SCRIPT "${VCPKG_ROOT}/bootstrap-vcpkg.bat")
  else()
    set(_VCPKG_EXECUTABLE "${VCPKG_ROOT}/vcpkg")
    set(_VCPKG_BOOTSTRAP_SCRIPT "${VCPKG_ROOT}/bootstrap-vcpkg.sh")
  endif()

  if(NOT EXISTS "${_VCPKG_EXECUTABLE}")
    message(STATUS "Bootstrapping vcpkg before install")

    file(TO_NATIVE_PATH "${CMAKE_BINARY_DIR}/vcpkg-bootstrap.log" _VCPKG_BOOTSTRAP_LOG)
    execute_process(
      COMMAND "${_VCPKG_BOOTSTRAP_SCRIPT}" ${VCPKG_BOOTSTRAP_OPTIONS}
      OUTPUT_FILE "${_VCPKG_BOOTSTRAP_LOG}"
      ERROR_FILE "${_VCPKG_BOOTSTRAP_LOG}"
      RESULT_VARIABLE _VCPKG_BOOTSTRAP_RESULT)

    if(_VCPKG_BOOTSTRAP_RESULT EQUAL 0)
      message(STATUS "Bootstrapping vcpkg before install - done")
    else()
      message(STATUS "Bootstrapping vcpkg before install - failed")
      file(READ ${_VCPKG_BOOTSTRAP_LOG} MSG)
      message(FATAL_ERROR "vcpkg install failed. See logs for more information: ${MSG}")
    endif()
  endif()

  execute_process(
    COMMAND ${_VCPKG_EXECUTABLE} fetch nuget
    OUTPUT_STRIP_TRAILING_WHITESPACE
    OUTPUT_VARIABLE _FETCH_NUGET_OUTPUT)

  STRING(REGEX REPLACE "\n" ";" _FETCH_NUGET_OUTPUT "${_FETCH_NUGET_OUTPUT}")
  list(GET _FETCH_NUGET_OUTPUT -1 _NUGET_PATH)

  if(_HOST_IS_WINDOWS)
    set(_NUGET_EXE ${_NUGET_PATH})
  else()
    set(_NUGET_EXE ${_VCPKG_MONO} ${_NUGET_PATH})
  endif()

  set(_CONFIG_PATH "${CMAKE_BINARY_DIR}/github-NuGet.Config")

  configure_file(
    "${CMAKE_SOURCE_DIR}/cmake/NuGet.Config.in"
    "${_CONFIG_PATH}"
    @ONLY)
  execute_process(
    COMMAND ${_NUGET_EXE} setapikey "${NUGET_TOKEN}" -src "https://nuget.pkg.github.com/opengisch/index.json" -configfile ${_CONFIG_PATH}
    OUTPUT_VARIABLE _OUTPUT
    ERROR_VARIABLE _ERROR
    RESULT_VARIABLE _RESULT)
  if(_RESULT EQUAL 0)
    message(STATUS "Setup nuget api key - done")
  else()
    message(STATUS "Setup nuget api key - failed")
    message(STATUS "Output:")
    message(STATUS ${_OUTPUT})
    message(STATUS "Error:")
    message(STATUS ${_ERROR})
  endif()

  file(TO_NATIVE_PATH "${_CONFIG_PATH}" _CONFIG_PATH_NATIVE)
  set(ENV{VCPKG_BINARY_SOURCES} "$ENV{VCPKG_BINARY_SOURCES};nugetconfig,${_CONFIG_PATH_NATIVE},readwrite")
endif()

set(SYSTEM_QT OFF CACHE BOOL "Determines if system Qt libraries should be used or Qt should be built from source. If enabled, the 'Qt5_DIR' environment variable can be used to specify the Qt installation to use.")
if(SYSTEM_QT)
  list(APPEND VCPKG_OVERLAY_PORTS "${CMAKE_SOURCE_DIR}/vcpkg/overlay_system_qt")
  set($ENV{VCPKG_KEEP_ENV_VARS} "$ENV{VCPKG_KEEP_ENV_VARS};Qt5_DIR")
endif()
list(APPEND VCPKG_OVERLAY_PORTS "${CMAKE_SOURCE_DIR}/vcpkg/overlay")
list(APPEND VCPKG_OVERLAY_TRIPLETS "${CMAKE_SOURCE_DIR}/vcpkg/triplets")

if(APPLE)
  set(USE_MAC_BUNDLING OFF)
endif()

# Copies DLLs built by vcpkg when an install() command is run. Probably only
# works on Windows.
set(X_VCPKG_APPLOCAL_DEPS_INSTALL ON CACHE BOOL "Copy dependency DLLs on install")
