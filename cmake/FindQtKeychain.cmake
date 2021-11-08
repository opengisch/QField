# Find QtKeychain
# ~~~~~~~~~~~~~~~
# Copyright (c) 2016, Boundless Spatial
# Author: Larry Shaffer <lshaffer (at) boundlessgeo (dot) com>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#
# CMake module to search for QtKeychain library from:
#    https://github.com/frankosterfeld/qtkeychain
#
# If it's found it sets QTKEYCHAIN_FOUND to TRUE
# and following variables are set:
#    QTKEYCHAIN_INCLUDE_DIR
#    QTKEYCHAIN_LIBRARY

if(ANDROID AND NOT WITH_VCPKG)
  set(QTKEYCHAIN_INCLUDE_DIR ${OSGEO4A_STAGE_DIR}/${ANDROID_ABI}/include/qt5keychain CACHE PATH "")
  set(QTKEYCHAIN_LIBRARY ${OSGEO4A_STAGE_DIR}/${ANDROID_ABI}/lib/libqt5keychain_${ANDROID_ABI}.so CACHE PATH "")

else()
  FIND_PATH(QTKEYCHAIN_INCLUDE_DIR keychain.h
    PATHS
    ${LIB_DIR}/include
    "$ENV{LIB_DIR}/include"
    $ENV{INCLUDE}
    /usr/local/include
    /usr/include
    ${OSGEO4A_STAGE_DIR}/${ANDROID_ABI}/include
    PATH_SUFFIXES qt5keychain qtkeychain qt6keychain
  )

  FIND_LIBRARY(QTKEYCHAIN_LIBRARY NAMES qt5keychain qtkeychain qt6keychain qt5keychain_${ANDROID_ABI}
    PATHS
    ${LIB_DIR}
    "$ENV{LIB_DIR}"
    $ENV{LIB_DIR}/lib
    $ENV{LIB}
    /usr/local/lib
    /usr/lib
    ${OSGEO4A_STAGE_DIR}/${ANDROID_ABI}/lib
  )

endif()

IF (QTKEYCHAIN_INCLUDE_DIR AND QTKEYCHAIN_LIBRARY)
  SET(QTKEYCHAIN_FOUND TRUE)
ELSE()
  SET(QTKEYCHAIN_FOUND FALSE)
ENDIF (QTKEYCHAIN_INCLUDE_DIR AND QTKEYCHAIN_LIBRARY)

IF (QTKEYCHAIN_FOUND)
   IF (NOT QTKEYCHAIN_FIND_QUIETLY)
      MESSAGE(STATUS "Found QtKeychain: ${QTKEYCHAIN_LIBRARY}")
   ENDIF (NOT QTKEYCHAIN_FIND_QUIETLY)
ELSE (QTKEYCHAIN_FOUND)
   IF (QTKEYCHAIN_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find QtKeychain")
   ENDIF (QTKEYCHAIN_FIND_REQUIRED)
ENDIF (QTKEYCHAIN_FOUND)
