# Find QCA (Qt Cryptography Architecture 2+)
# ~~~~~~~~~~~~~~~~
# When run this will define
#
#  Qca_FOUND - system has QCA
#  Qca_LIBRARY - the QCA library or framework
#  Qca_INCLUDE_DIR - the QCA include directory
#  Qca_VERSION_STR - e.g. "2.0.3"
#
# Copyright (c) 2006, Michael Larouche, <michael.larouche@kdemail.net>
# Copyright (c) 2014, Larry Shaffer, <larrys (at) dakotacarto (dot) com>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

if(ANDROID AND NOT WITH_VCPKG)
  set(Qca_INCLUDE_DIR ${OSGEO4A_STAGE_DIR}/${ANDROID_ABI}/include/Qca-qt5/QtCrypto CACHE PATH "")
  set(Qca_LIBRARY ${OSGEO4A_STAGE_DIR}/${ANDROID_ABI}/lib/libqca-qt5_${ANDROID_ABI}.so CACHE PATH "")
  set(Qca_FOUND TRUE)

else()
  if(Qca_INCLUDE_DIR AND Qca_LIBRARY)
    set(Qca_FOUND TRUE)
  else(Qca_INCLUDE_DIR AND Qca_LIBRARY)
    set(Qca_LIBRARY_NAMES qca-qt5 qca2-qt5 qca-qt6 qca qca-qt5_${ANDROID_ABI})

    find_library(Qca_LIBRARY
      NAMES ${Qca_LIBRARY_NAMES}
      PATHS
        ${LIB_DIR}
        $ENV{LIB}
        "$ENV{LIB_DIR}"
        $ENV{LIB_DIR}/lib
        /usr/local/lib
        ${OSGEO4A_STAGE_DIR}/${ANDROID_ABI}/lib
    )

    set(_qca_fw)
    if(Qca_LIBRARY MATCHES "/qca.*\\.framework")
      string(REGEX REPLACE "^(.*/qca.*\\.framework).*$" "\\1" _qca_fw "${Qca_LIBRARY}")
    endif()

    find_path(Qca_INCLUDE_DIR
      NAMES QtCrypto
      PATHS
        "${_qca_fw}/Headers"
        ${LIB_DIR}/include
        "$ENV{LIB_DIR}/include"
        $ENV{INCLUDE}
        /usr/local/include
        ${OSGEO4A_STAGE_DIR}/${ANDROID_ABI}/include
        PATH_SUFFIXES QtCrypto qt5/QtCrypto Qca-qt5/QtCrypto qt/Qca-qt5/QtCrypto qt5/Qca-qt5/QtCrypto Qca-qt6/QtCrypto
    )

    if(Qca_LIBRARY AND Qca_INCLUDE_DIR)
      set(Qca_FOUND TRUE)
    endif()
  endif(Qca_INCLUDE_DIR AND Qca_LIBRARY)

endif()

if(NOT Qca_FOUND)

  if(QCA_FIND_REQUIRED)
    message(FATAL_ERROR "Could not find QCA")
  else()
    message(STATUS "Could not find QCA")
  endif()

else(NOT Qca_FOUND)

  # Check version is valid (>= 2.0.3)
  # find_package(QCA 2.0.3) works with 2.1.0+, which has a QcaConfigVersion.cmake, but 2.0.3 does not

  # qca_version.h header only available with 2.1.0+
  set(_qca_version_h "${Qca_INCLUDE_DIR}/qca_version.h")
  if(EXISTS "${_qca_version_h}")
    file(STRINGS "${_qca_version_h}" _qca_version_str REGEX "^.*QCA_VERSION_STR +\"[^\"]+\".*$")
    string(REGEX REPLACE "^.*QCA_VERSION_STR +\"([^\"]+)\".*$" "\\1" Qca_VERSION_STR "${_qca_version_str}")
  else()
    # qca_core.h contains hexadecimal version in <= 2.0.3
    set(_qca_core_h "${Qca_INCLUDE_DIR}/qca_core.h")
    if(EXISTS "${_qca_core_h}")
      file(STRINGS "${_qca_core_h}" _qca_version_str REGEX "^#define +QCA_VERSION +0x[0-9a-fA-F]+.*")
      string(REGEX REPLACE "^#define +QCA_VERSION +0x([0-9a-fA-F]+)$" "\\1" _qca_version_int "${_qca_version_str}")
      if("${_qca_version_int}" STREQUAL "020003")
        set(Qca_VERSION_STR "2.0.3")
      endif()
    endif()
  endif()

  if(NOT Qca_VERSION_STR)
    set(Qca_FOUND FALSE)
    if(QCA_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find QCA >= 2.0.3")
    else()
      message(STATUS "Could not find QCA >= 2.0.3")
    endif()
  else()
    if(NOT QCA_FIND_QUIETLY)
      message(STATUS "Found QCA: ${Qca_LIBRARY} (${Qca_VERSION_STR})")
    endif()
  endif()

endif(NOT Qca_FOUND)
