set(NUGET_SOURCE "https://nuget.pkg.github.com/opengisch/index.json")
set(NUGET_USERNAME "qfield-fairy" CACHE STRING "Nuget user")
# Obfuscate a (read only) github token, if it's here clear text it will be invalidated
# Python: print(*(ord(i) for i in token), sep=";")
set(NUGET_TOKEN_ASCII
    103;104;112;95;116;76;65;120;74;80;120;70;84;109;122;112;85;103;68;114;106;90;52;74;55;85;117;111;107;74;48;113;70;112;51;100;52;77;113;102
)
string(ASCII ${NUGET_TOKEN_ASCII} NUGET_TOKEN_DEFAULT)
set(NUGET_TOKEN ${NUGET_TOKEN_DEFAULT} CACHE STRING "Nuget token")

string(COMPARE EQUAL "${CMAKE_HOST_SYSTEM_NAME}" "Windows" _HOST_IS_WINDOWS)
set(WITH_VCPKG ${_HOST_IS_WINDOWS} CACHE BOOL "Use the vcpkg submodule for dependency management.")
if(NOT WITH_VCPKG)
  return()
endif()

set(VCPKG_ROOT "${CMAKE_SOURCE_DIR}/vcpkg/base" CACHE STRING "")
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")

message(STATUS "Host is windows: ${_HOST_IS_WINDOWS} (${CMAKE_HOST_SYSTEM_NAME})")
# Binarycache can only be used on Windows or if mono is available.
find_program(_VCPKG_MONO mono)
if(_HOST_IS_WINDOWS OR EXISTS "${_VCPKG_MONO}")
  # Early bootstrap, copied from the vcpkg toolchain, we need this to fetch mono
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
  #  execute_process(
  #    COMMAND ${_NUGET_EXE} sources add -source "${NUGET_SOURCE}" -username "${NUGET_USERNAME}" -password "${NUGET_TOKEN}" -storepasswordincleartext -name "github" -configfile ${_CONFIG_PATH}
  #    OUTPUT_VARIABLE _OUTPUT
  #    ERROR_VARIABLE _ERROR
  #    RESULT_VARIABLE _RESULT)
  #  if(_RESULT EQUAL 0)
  #    message(STATUS "Setup nuget source [github] - done")
  #  else()
  #    message(STATUS "Setup nuget source [github] - failed")
  #    message(STATUS "Output:")
  #    message(STATUS ${_OUTPUT})
  #    message(STATUS "Error:")
  #    message(STATUS ${_ERROR})
  #  endif()
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

set(SYSTEM_QT OFF CACHE BOOL "Determines if system Qt libraries should be used or Qt should be built from source.")
if(SYSTEM_QT)
  list(APPEND VCPKG_OVERLAY_PORTS "${CMAKE_SOURCE_DIR}/vcpkg/overlay_system_qt")
endif()
list(APPEND VCPKG_OVERLAY_PORTS "${CMAKE_SOURCE_DIR}/vcpkg/overlay")
# list(APPEND VCPKG_OVERLAY_TRIPLETS "${CMAKE_SOURCE_DIR}/vcpkg/triplets")

if(APPLE)
  set(USE_MAC_BUNDLING OFF)
endif()

# Copies DLLs built by vcpkg when an install() command is run. Probably only
# works on Windows.
set(X_VCPKG_APPLOCAL_DEPS_INSTALL ON)
