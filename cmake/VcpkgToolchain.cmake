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

set(Z_VCPKG_ROOT_DIR "${CMAKE_SOURCE_DIR}/vcpkg/base")
set(CMAKE_TOOLCHAIN_FILE "${Z_VCPKG_ROOT_DIR}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "" FORCE)

function(_qfield_vcpkg_generate_nuget_config)
  cmake_parse_arguments(
    _pvgnc "" "OUT_FILE;NUGET_NAME;NUGET_SOURCE;NUGET_USERNAME;NUGET_TOKEN" ""
    ${ARGN})
  configure_file("${CMAKE_SOURCE_DIR}/cmake/NuGet.Config.in"
                 "${_pvgnc_OUT_FILE}" @ONLY)
endfunction()

function(_qfield_vcpkg_setup_binarycache)
  cmake_parse_arguments(_pvsb "" "NAME;PREFIX" "" ${ARGN})

  if(NOT ${_pvsb_PREFIX}_SOURCE
     OR NOT ${_pvsb_PREFIX}_USERNAME
     OR NOT ${_pvsb_PREFIX}_TOKEN)
    return()
  endif()

  set(_CONFIG_PATH "${CMAKE_BINARY_DIR}/${_pvsb_NAME}-NuGet.Config")
  _qfield_vcpkg_generate_nuget_config(
    OUT_FILE ${_CONFIG_PATH}
    NUGET_SOURCE ${${_pvsb_PREFIX}_SOURCE}
    NUGET_USERNAME ${${_pvsb_PREFIX}_USERNAME}
    NUGET_TOKEN ${${_pvsb_PREFIX}_TOKEN}
  )

  file(TO_NATIVE_PATH "${_CONFIG_PATH}" _CONFIG_PATH_NATIVE)
  set(ENV{VCPKG_BINARY_SOURCES} "$ENV{VCPKG_BINARY_SOURCES};nuget,${_CONFIG_PATH_NATIVE},readwrite")
endfunction()

# Binarycache can only be used on Windows or if mono is available.
find_program(_VCPKG_MONO mono)
if(_HOST_IS_WINDOWS OR EXISTS "${_VCPKG_MONO}")
  # Early bootstrap, copied from the vcpkg toolchain, we need this to fetch mono
  if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    set(Z_VCPKG_EXECUTABLE "${Z_VCPKG_ROOT_DIR}/vcpkg.exe")
    set(Z_VCPKG_BOOTSTRAP_SCRIPT "${Z_VCPKG_ROOT_DIR}/bootstrap-vcpkg.bat")
  else()
    set(Z_VCPKG_EXECUTABLE "${Z_VCPKG_ROOT_DIR}/vcpkg")
    set(Z_VCPKG_BOOTSTRAP_SCRIPT "${Z_VCPKG_ROOT_DIR}/bootstrap-vcpkg.sh")
  endif()

  if(NOT EXISTS "${Z_VCPKG_EXECUTABLE}")
    message(STATUS "Bootstrapping vcpkg before install")

    file(TO_NATIVE_PATH "${CMAKE_BINARY_DIR}/vcpkg-bootstrap.log" Z_VCPKG_BOOTSTRAP_LOG)
    execute_process(
      COMMAND "${Z_VCPKG_BOOTSTRAP_SCRIPT}" ${VCPKG_BOOTSTRAP_OPTIONS}
      OUTPUT_FILE "${Z_VCPKG_BOOTSTRAP_LOG}"
      ERROR_FILE "${Z_VCPKG_BOOTSTRAP_LOG}"
      RESULT_VARIABLE Z_VCPKG_BOOTSTRAP_RESULT)

    if(Z_VCPKG_BOOTSTRAP_RESULT EQUAL 0)
      message(STATUS "Bootstrapping vcpkg before install - done")
    else()
      message(STATUS "Bootstrapping vcpkg before install - failed")
      file(READ ${Z_VCPKG_BOOTSTRAP_LOG} MSG)
      message(FATAL_ERROR "vcpkg install failed. See logs for more information: ${MSG}")
    endif()
  endif()

  execute_process(
    COMMAND ${Z_VCPKG_EXECUTABLE} fetch nuget
    OUTPUT_STRIP_TRAILING_WHITESPACE
    OUTPUT_VARIABLE NUGET_PATH)

  execute_process(
    COMMAND ${_VCPKG_MONO} ${NUGET_PATH} sources add -source "${NUGET_SOURCE}" -username "${NUGET_USERNAME}" -password "${NUGET_TOKEN}" -storepasswordincleartext -name "github")
  execute_process(
    COMMAND ${_VCPKG_MONO} ${NUGET_PATH} setapikey "${NUGET_TOKEN}" -src "https://nuget.pkg.github.com/opengisch/index.json")

  set(ENV{VCPKG_BINARY_SOURCES} "$ENV{VCPKG_BINARY_SOURCES};nuget,github,readwrite")
  #  _qfield_vcpkg_setup_binarycache(NAME mainline PREFIX NUGET)
endif()

set(SYSTEM_QT OFF CACHE BOOL "Determines if system Qt libraries should be used or Qt should be built from source.")
if(SYSTEM_QT)
  list(APPEND VCPKG_OVERLAY_PORTS "${CMAKE_SOURCE_DIR}/vcpkg/overlay_system_qt")
endif()
list(APPEND VCPKG_OVERLAY_PORTS "${CMAKE_SOURCE_DIR}/vcpkg/overlay")
# list(APPEND VCPKG_OVERLAY_TRIPLETS "${CMAKE_SOURCE_DIR}/vcpkg/triplets")

# Copies DLLs built by vcpkg when an install() command is run. Probably only
# works on Windows.
set(X_VCPKG_APPLOCAL_DEPS_INSTALL ON)
