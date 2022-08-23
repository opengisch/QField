# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file COPYING-CMAKE-SCRIPTS or https://cmake.org/licensing for details.

#[=======================================================================[.rst:
FindQtKeychain
---------

CMake module to search for QtKeychain library

IMPORTED targets
^^^^^^^^^^^^^^^^
This module defines the following :prop_tgt:`IMPORTED` target:
``QtKeychain::QtKeychain``

The macro sets the following variables:
``QtKeychain_FOUND``
  if the library found

``QtKeychain_LIBRARIES``
  full path to the library

``QtKeychain_INCLUDE_DIRS``
  where to find the library headers

``QtKeychain_VERSION_STRING``
  version string of QtKeychain

#]=======================================================================]

if(BUILD_WITH_QT6)
  set(QtKeychain_INCLUDE_SUFFIXES
            qt6keychain
            Qt6Keychain)
  set(QtKeychain_NAMES ${QtKeychain_NAMES} Qt6Keychain qt6keychain)
else()
  set(QtKeychain_INCLUDE_SUFFIXES
            Qt5keychain
            qt5keychain
            Qtkeychain
            qtkeychain)
  set(QtKeychain_NAMES ${QtKeychain_NAMES} Qt5keychain qt5keychain Qtkeychain qtkeychain)
endif()

find_path(QtKeychain_INCLUDE_DIR keychain.h
          PATHS
            ${QtKeychain_ROOT}/include/
          PATH_SUFFIXES
            ${QtKeychain_INCLUDE_SUFFIXES}
          DOC "Path to QtKeychain include directory")

if(NOT QtKeychain_LIBRARY)
  find_library(QtKeychain_LIBRARY_RELEASE NAMES ${QtKeychain_NAMES})
  find_library(QtKeychain_LIBRARY_DEBUG NAMES ${QtKeychain_NAMES})
  include(SelectLibraryConfigurations)
  select_library_configurations(QtKeychain)
  mark_as_advanced(QtKeychain_LIBRARY_RELEASE QtKeychain_LIBRARY_DEBUG)
endif()

unset(QtKeychain_NAMES)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(QtKeychain
                                  REQUIRED_VARS QtKeychain_LIBRARY QtKeychain_INCLUDE_DIR
                                  VERSION_VAR QtKeychain_VERSION_STRING)
mark_as_advanced(QtKeychain_INCLUDE_DIR QtKeychain_LIBRARY)

if(QtKeychain_FOUND)
  set(QtKeychain_LIBRARIES ${QtKeychain_LIBRARY})
  set(QtKeychain_INCLUDE_DIRS ${QtKeychain_INCLUDE_DIR})
  if(NOT TARGET QtKeychain::QtKeychain)
    add_library(QtKeychain::QtKeychain UNKNOWN IMPORTED)
    set_target_properties(QtKeychain::QtKeychain PROPERTIES
                          INTERFACE_INCLUDE_DIRECTORIES ${QtKeychain_INCLUDE_DIR}
                          IMPORTED_LINK_INTERFACE_LANGUAGES "C")
    if(EXISTS "${QtKeychain_LIBRARY}")
      set_target_properties(QtKeychain::QtKeychain PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES "C"
        IMPORTED_LOCATION "${QtKeychain_LIBRARY}")
    endif()
    if(EXISTS "${QtKeychain_LIBRARY_RELEASE}")
      set_property(TARGET QtKeychain::QtKeychain APPEND PROPERTY
        IMPORTED_CONFIGURATIONS RELEASE)
      set_target_properties(QtKeychain::QtKeychain PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
        IMPORTED_LOCATION_RELEASE "${QtKeychain_LIBRARY_RELEASE}")
    endif()
    if(EXISTS "${QtKeychain_LIBRARY_DEBUG}")
      set_property(TARGET QtKeychain::QtKeychain APPEND PROPERTY
        IMPORTED_CONFIGURATIONS DEBUG)
      set_target_properties(QtKeychain::QtKeychain PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C"
        IMPORTED_LOCATION_DEBUG "${QtKeychain_LIBRARY_DEBUG}")
    endif()
  endif()
endif()

