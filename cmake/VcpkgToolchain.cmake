set(NUGET_SOURCE "https://nuget.pkg.github.com/opengisch/index.json")
set(NUGET_OWNER "qfield-fairy" CACHE STRING "Nuget user")
# Obfuscate a (read only) github token, if it's here clear text it will be invalidated
# Python: print(*(ord(i) for i in token), sep=";")
set(NUGET_TOKEN_ASCII
    103;104;112;95;116;76;65;120;74;80;120;70;84;109;122;112;85;103;68;114;106;90;52;74;55;85;117;111;107;74;48;113;70;112;51;100;52;77;113;102
)
string(ASCII ${NUGET_TOKEN_ASCII} NUGET_TOKEN_DEFAULT)
set(NUGET_TOKEN ${NUGET_TOKEN_DEFAULT} CACHE STRING "Nuget token")

string(COMPARE EQUAL "${CMAKE_HOST_SYSTEM_NAME}" "Windows" _HOST_IS_WINDOWS)
option(WITH_VCPKG "Use the vcpkg submodule for dependency management."
       ${_HOST_IS_WINDOWS})
if(NOT WITH_VCPKG)
  return()
endif()

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/base/scripts/buildsystems/vcpkg.cmake" CACHE STRING "" FORCE)

function(_qfield_vcpkg_generate_nuget_config)
  cmake_parse_arguments(
    _pvgnc "" "OUT_FILE;NUGET_NAME;NUGET_SOURCE;NUGET_OWNER;NUGET_TOKEN" ""
    ${ARGN})
  configure_file("${CMAKE_SOURCE_DIR}/cmake/NuGet.Config.in"
                 "${_pvgnc_OUT_FILE}" @ONLY)
endfunction()

function(_qfield_vcpkg_setup_binarycache)
  cmake_parse_arguments(_pvsb "" "NAME;PREFIX" "" ${ARGN})

  if(NOT ${_pvsb_PREFIX}_SOURCE
     OR NOT ${_pvsb_PREFIX}_OWNER
     OR NOT ${_pvsb_PREFIX}_TOKEN)
    return()
  endif()

  set(_CONFIG_PATH "${CMAKE_BINARY_DIR}/${_pvsb_NAME}-NuGet.Config")
  _qfield_vcpkg_generate_nuget_config(
    OUT_FILE ${_CONFIG_PATH}
    NUGET_SOURCE ${${_pvsb_PREFIX}_SOURCE}
    NUGET_OWNER ${${_pvsb_PREFIX}_OWNER}
    NUGET_TOKEN ${${_pvsb_PREFIX}_TOKEN}
  )

  file(TO_NATIVE_PATH "${_CONFIG_PATH}" _CONFIG_PATH_NATIVE)
  set(ENV{VCPKG_BINARY_SOURCES} "$ENV{VCPKG_BINARY_SOURCES};nugetconfig,${_CONFIG_PATH_NATIVE},readwrite")
endfunction()

# Binarycache can only be used on Windows or if mono is available.
find_program(_VCPKG_MONO mono)
if(_HOST_IS_WINDOWS OR EXISTS "${_VCPKG_MONO}")
  _qfield_vcpkg_setup_binarycache(NAME mainline PREFIX NUGET)
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
