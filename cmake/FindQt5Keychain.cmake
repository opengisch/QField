# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file COPYING-CMAKE-SCRIPTS or https://cmake.org/licensing for details.

#[=======================================================================[.rst:
FindQt5Keychain
---------

CMake module to search for Qt5Keychain library

IMPORTED targets
^^^^^^^^^^^^^^^^
This module defines the following :prop_tgt:`IMPORTED` target:
``Qt5Keychain::Qt5Keychain``

The macro sets the following variables:
``Qt5Keychain_FOUND``
  if the library found

``Qt5Keychain_LIBRARIES``
  full path to the library

``Qt5Keychain_INCLUDE_DIRS``
  where to find the library headers

``Qt5Keychain_VERSION_STRING``
  version string of Qt5Keychain

#]=======================================================================]

find_path(Qt5Keychain_INCLUDE_DIR keychain.h
          PATHS
            ${Qt5Keychain_ROOT}/include/
          PATH_SUFFIXES
            qt5keychain
            qtkeychain
          DOC "Path to Qt5Keychain include directory")

set(Qt5Keychain_NAMES ${Qt5Keychain_NAMES} qt5keychain qtkeychain)

if(NOT Qt5Keychain_LIBRARY)
  find_library(Qt5Keychain_LIBRARY_RELEASE NAMES ${Qt5Keychain_NAMES})
  find_library(Qt5Keychain_LIBRARY_DEBUG NAMES ${Qt5Keychain_NAMES})
  include(SelectLibraryConfigurations)
  select_library_configurations(Qt5Keychain)
  mark_as_advanced(Qt5Keychain_LIBRARY_RELEASE Qt5Keychain_LIBRARY_DEBUG)
endif()

unset(Qt5Keychain_NAMES)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Qt5Keychain
                                  REQUIRED_VARS Qt5Keychain_LIBRARY Qt5Keychain_INCLUDE_DIR
                                  VERSION_VAR Qt5Keychain_VERSION_STRING)
mark_as_advanced(Qt5Keychain_INCLUDE_DIR Qt5Keychain_LIBRARY)

if(Qt5Keychain_FOUND)
  set(Qt5Keychain_LIBRARIES ${Qt5Keychain_LIBRARY})
  set(Qt5Keychain_INCLUDE_DIRS ${Qt5Keychain_INCLUDE_DIR})
  if(NOT TARGET Qt5Keychain::Qt5Keychain)
    add_library(Qt5Keychain::Qt5Keychain UNKNOWN IMPORTED)
    set_target_properties(Qt5Keychain::Qt5Keychain PROPERTIES
                          INTERFACE_INCLUDE_DIRECTORIES ${Qt5Keychain_INCLUDE_DIR}
                          IMPORTED_LINK_INTERFACE_LANGUAGES "C")
    if(EXISTS "${Qt5Keychain_LIBRARY}")
      set_target_properties(Qt5Keychain::Qt5Keychain PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES "C"
        IMPORTED_LOCATION "${Qt5Keychain_LIBRARY}")
    endif()
    if(EXISTS "${Qt5Keychain_LIBRARY_RELEASE}")
      set_property(TARGET Qt5Keychain::Qt5Keychain APPEND PROPERTY
        IMPORTED_CONFIGURATIONS RELEASE)
      set_target_properties(Qt5Keychain::Qt5Keychain PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
        IMPORTED_LOCATION_RELEASE "${Qt5Keychain_LIBRARY_RELEASE}")
    endif()
    if(EXISTS "${Qt5Keychain_LIBRARY_DEBUG}")
      set_property(TARGET Qt5Keychain::Qt5Keychain APPEND PROPERTY
        IMPORTED_CONFIGURATIONS DEBUG)
      set_target_properties(Qt5Keychain::Qt5Keychain PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C"
        IMPORTED_LOCATION_DEBUG "${Qt5Keychain_LIBRARY_DEBUG}")
    endif()
  endif()
endif()

