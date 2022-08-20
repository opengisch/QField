# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file COPYING-CMAKE-SCRIPTS or https://cmake.org/licensing for details.

#[=======================================================================[.rst:
FindQca
---------

CMake module to search for Qca library

IMPORTED targets
^^^^^^^^^^^^^^^^
This module defines the following :prop_tgt:`IMPORTED` target:
``Qca::qca``

The macro sets the following variables:
``Qca_FOUND``
  if the library found

``Qca_LIBRARIES``
  full path to the library

``Qca_INCLUDE_DIRS``
  where to find the library headers

``Qca_VERSION_STRING``
  version string of Qca

#]=======================================================================]

if(BUILD_WITH_QT6)
  set(Qca_INCLUDE_SUFFIXES
            QtCrypto
            qt6/QtCrypto
            Qca-qt6/QtCrypto
            qt6/Qca-qt6/QtCrypto)
  set(Qca_NAMES ${Qca_NAMES} qca-qt6 qca)
else()
  set(Qca_INCLUDE_SUFFIXES
            QtCrypto
            qt5/QtCrypto
            Qca-qt5/QtCrypto
            qt5/Qca-qt5/QtCrypto)
  set(Qca_NAMES ${Qca_NAMES} qca-qt5 qca)
endif()
find_path(Qca_INCLUDE_DIR qca.h
          PATHS
            ${Qca_ROOT}/include/
          PATH_SUFFIXES
            ${Qca_INCLUDE_SUFFIXES}
          DOC "Path to Qca include directory")


if(NOT Qca_LIBRARY)
  find_library(Qca_LIBRARY_RELEASE NAMES ${Qca_NAMES})
  find_library(Qca_LIBRARY_DEBUG NAMES ${Qca_NAMES})
  include(SelectLibraryConfigurations)
  select_library_configurations(Qca)
  mark_as_advanced(Qca_LIBRARY_RELEASE Qca_LIBRARY_DEBUG)
endif()

unset(Qca_INCLUDE_SUFFIXES)
unset(Qca_NAMES)

if(Qca_INCLUDE_DIR)
  # qca_version.h header only available with 2.1.0+
  set(_qca_version_h "${Qca_INCLUDE_DIR}/qca_version.h")
  file(STRINGS "${_qca_version_h}" _qca_version_str REGEX "^.*QCA_VERSION_STR +\"[^\"]+\".*$")
  string(REGEX REPLACE "^.*QCA_VERSION_STR +\"([^\"]+)\".*$" "\\1" Qca_VERSION_STRING "${_qca_version_str}")
endif ()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Qca
                                  REQUIRED_VARS Qca_LIBRARY Qca_INCLUDE_DIR
                                  VERSION_VAR Qca_VERSION_STRING)
mark_as_advanced(Qca_INCLUDE_DIR Qca_LIBRARY)

if(Qca_FOUND)
  set(Qca_LIBRARIES ${Qca_LIBRARY})
  set(Qca_INCLUDE_DIRS ${Qca_INCLUDE_DIR})
  if(NOT TARGET Qca::qca)
    add_library(Qca::qca UNKNOWN IMPORTED)
    set_target_properties(Qca::qca PROPERTIES
                          INTERFACE_INCLUDE_DIRECTORIES ${Qca_INCLUDE_DIR}
                          IMPORTED_LINK_INTERFACE_LANGUAGES "C")
    if(EXISTS "${Qca_LIBRARY}")
      set_target_properties(Qca::qca PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES "C"
        IMPORTED_LOCATION "${Qca_LIBRARY}")
    endif()
    if(EXISTS "${Qca_LIBRARY_RELEASE}")
      set_property(TARGET Qca::qca APPEND PROPERTY
        IMPORTED_CONFIGURATIONS RELEASE)
      set_target_properties(Qca::qca PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
        IMPORTED_LOCATION_RELEASE "${Qca_LIBRARY_RELEASE}")
    endif()
    if(EXISTS "${Qca_LIBRARY_DEBUG}")
      set_property(TARGET Qca::qca APPEND PROPERTY
        IMPORTED_CONFIGURATIONS DEBUG)
      set_target_properties(Qca::qca PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C"
        IMPORTED_LOCATION_DEBUG "${Qca_LIBRARY_DEBUG}")
    endif()
  endif()
endif()

