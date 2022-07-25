## Once run this will define:
##
## QGIS_FOUND            = system has QGIS lib
##
## QGIS_CORE_LIBRARY     = full path to the CORE library
## QGIS_ANALYSIS_LIBRARY = full path to the ANALYSIS library
## QGIS_INCLUDE_DIR      = where to find headers
##
## QGIS_VERSION          = version as defined in qgsconfig.h, as major.minor.patch
##
## Definitions or ENV variables affecting search locations
##
## OSGEO4W_ROOT          = [A-Z]:/path/to/OSGeo4W/install/root
##                               (^ use forward slashes!)
## OSGEO4W_QGIS_SUBDIR   = qgis[-rel|-ltr][-dev], in OSGEO4W_ROOT/apps/
## QGIS_MAC_PATH         = /path/to/any/QGIS.app/Contents
## QGIS_BUILD_PATH       = [A-Z:]/path/to/QGIS/build/directory
##
## Tim Sutton
## Larry Shaffer (2017-01-31)

message(STATUS "Searching for QGIS")

IF(WIN32)
  # OSGEO4W_QGIS_SUBDIR relative install: qgis[-rel|-ltr][-dev], etc.
  IF (NOT OSGEO4W_QGIS_SUBDIR OR "${OSGEO4W_QGIS_SUBDIR}" STREQUAL "")
    IF (NOT "$ENV{OSGEO4W_QGIS_SUBDIR}" STREQUAL "")
      SET (OSGEO4W_QGIS_SUBDIR $ENV{OSGEO4W_QGIS_SUBDIR})
    ELSE ()
      SET (OSGEO4W_QGIS_SUBDIR qgis)
    ENDIF ()
  ENDIF ()

  #MESSAGE("Searching for QGIS in $ENV{PROGRAMFILES}/QGIS")
  IF (MINGW)
    FIND_PATH(QGIS_INCLUDE_DIR
      NAMES qgsapplication.h
      PATHS
        "$ENV{PROGRAMFILES}/QGIS/include"
    )
    FIND_LIBRARY(QGIS_CORE_LIBRARY
      NAMES qgis_core
      PATHS
        "$ENV{PROGRAMFILES}/QGIS/"
    )
  ENDIF (MINGW)

  IF (MSVC)
    FIND_PATH(QGIS_PLUGIN_DIR
      NAMES wmsprovider.dll
      PATHS
        "$ENV{OSGEO4W_ROOT}/apps/${OSGEO4W_QGIS_SUBDIR}/plugins"
        "$ENV{PROGRAMFILES}/QGIS/plugins"
        "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/tools/qgis/plugins"
    )
    FIND_PATH(QGIS_INCLUDE_DIR
      NAMES qgsapplication.h
      PATHS
        "$ENV{INCLUDE}"
        "$ENV{LIB_DIR}/include/qgis"
        "$ENV{OSGEO4W_ROOT}/include"
        "$ENV{OSGEO4W_ROOT}/apps/${OSGEO4W_QGIS_SUBDIR}/include"
        "$ENV{PROGRAMFILES}/QGIS/include"
    )
    FIND_LIBRARY(QGIS_CORE_LIBRARY
      NAMES qgis_core
      PATHS
        "$ENV{LIB_DIR}/lib/"
        "$ENV{LIB}"
        "$ENV{OSGEO4W_ROOT}/lib"
        "$ENV{OSGEO4W_ROOT}/apps/${OSGEO4W_QGIS_SUBDIR}/lib"
        "$ENV{PROGRAMFILES}/QGIS/lib"
    )
    FIND_LIBRARY(QGIS_ANALYSIS_LIBRARY
      NAMES qgis_analysis
      PATHS
        "$ENV{LIB_DIR}"
        "$ENV{LIB}"
        "$ENV{OSGEO4W_ROOT}/lib"
        "$ENV{OSGEO4W_ROOT}/apps/${OSGEO4W_QGIS_SUBDIR}/lib"
        "$ENV{PROGRAMFILES}/QGIS/lib"
    )
  ENDIF (MSVC)

elseif(ANDROID)
  find_path(
    QGIS_INCLUDE_DIR
    NAMES qgis.h
    HINTS ${OSGEO4A_STAGE_DIR}/${ANDROID_ABI}
    PATH_SUFFIXES /include/qgis
  )

  find_library(
    QGIS_CORE_LIBRARY
    qgis_core_${ANDROID_ABI}
    HINTS ${OSGEO4A_STAGE_DIR}/${ANDROID_ABI}
    PATH_SUFFIXES /lib
  )
  find_library(
    QGIS_ANALYSIS_LIBRARY
    qgis_analysis_${ANDROID_ABI}
    HINTS ${OSGEO4A_STAGE_DIR}/${ANDROID_ABI}
    PATH_SUFFIXES /lib
  )
ELSEIF(UNIX)
  #MESSAGE("Searching for QGIS in /usr/bin; /usr/local/bin")
  FIND_PATH(QGIS_INCLUDE_DIR
    NAMES qgis.h
    PATHS
      ${CMAKE_PREFIX_PATH}
      ${QGIS_BUILD_PATH}/output/lib/qgis_core.framework/Headers
      ${QGIS_MAC_PATH}/Frameworks/qgis_core.framework/Headers
      ${QGIS_PREFIX_PATH}/include/qgis
      /usr/include/qgis
      /usr/local/include/qgis
      /Library/Frameworks/qgis_core.framework/Headers
      "$ENV{LIB_DIR}/include/qgis"
    PATH_SUFFIXES QGIS.app/Contents/Frameworks/qgis_core.framework/Headers
  )

  # also get other frameworks' headers folders on OS X
  IF (APPLE AND USE_MAC_BUNDLING)
    FIND_PATH(QGIS_ANALYSIS_INCLUDE_DIR
      NAMES qgsanalysis.h
      PATHS
        ${CMAKE_PREFIX_PATH}
        ${QGIS_BUILD_PATH}/output/lib
        ${QGIS_MAC_PATH}/Frameworks
        /Library/Frameworks
      PATH_SUFFIXES
        qgis_analysis.framework/Headers
        QGIS.app/Contents/Frameworks/qgis_analysis.framework/Headers
    )
    SET(QGIS_INCLUDE_DIR
      ${QGIS_INCLUDE_DIR}
      ${QGIS_ANALYSIS_INCLUDE_DIR}
    )

    # qgis_core is an archive without extensions for ios
    SET(CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_FIND_LIBRARY_SUFFIXES} "")
  ENDIF (APPLE AND USE_MAC_BUNDLING)

  FIND_LIBRARY(QGIS_CORE_LIBRARY
    NAMES qgis_core
    PATHS
      ${CMAKE_PREFIX_PATH}
      ${QGIS_BUILD_PATH}/output/lib
      ${QGIS_MAC_PATH}/Frameworks
      ${QGIS_MAC_PATH}/lib
      ${QGIS_PREFIX_PATH}/lib/
      /usr/lib64
      /usr/lib
      /usr/local/lib
      /Library/Frameworks
      "$ENV{LIB_DIR}/lib/"
    PATH_SUFFIXES
      QGIS.app/Contents/Frameworks/qgis_core.framework
  )
  FIND_LIBRARY(QGIS_ANALYSIS_LIBRARY
    NAMES qgis_analysis
    PATHS
      ${CMAKE_PREFIX_PATH}
      ${QGIS_BUILD_PATH}/output/lib
      ${QGIS_MAC_PATH}/Frameworks
      ${QGIS_MAC_PATH}/lib
      ${QGIS_PREFIX_PATH}/lib/
      /usr/lib64
      /usr/lib
      /usr/local/lib
      /Library/Frameworks
      "$ENV{LIB_DIR}/lib/"
    PATH_SUFFIXES
      QGIS.app/Contents/Frameworks/qgis_analysis.framework
  )

  IF (APPLE AND USE_MAC_BUNDLING)
    FIND_PATH(QGIS_APPLE_RESOURCES_DIR
      NAMES FindQGIS.cmake
      PATHS
        ${CMAKE_PREFIX_PATH}
        ${QGIS_BUILD_PATH}/output/bin
        ${QGIS_MAC_PATH}/Frameworks
        /Library/Frameworks
      PATH_SUFFIXES
        QGIS.app/Contents/Resources
      REQUIRED
    )
  ENDIF()

ENDIF()

IF (QGIS_INCLUDE_DIR)
  SET(QGIS_VERSION QGIS_VERSION-NOTFOUND)
  FIND_FILE(_qgsconfig_h qgsconfig.h PATHS ${QGIS_INCLUDE_DIR})
  IF (_qgsconfig_h)
    FILE(READ ${_qgsconfig_h} _qgsconfig)
    IF (_qgsconfig)
      # version defined like #define VERSION "2.14.8-Essen"
      FILE(STRINGS "${_qgsconfig_h}" _qgsversion_str REGEX "^#define VERSION .*$")
      STRING(REGEX REPLACE "^#define VERSION +\"([0-9]+\\.[0-9]+\\.[0-9]+).*$" "\\1" _qgsversion "${_qgsversion_str}")
      IF (_qgsversion)
        SET(QGIS_VERSION ${_qgsversion})
      ELSE ()
        MESSAGE(WARNING "No QGIS version determined: failed to parse qgsconfig.h")
      ENDIF ()
    ELSE()
      MESSAGE(WARNING "No QGIS version determined: failed to read qgsconfig.h")
    ENDIF ()
  ELSE ()
    MESSAGE(WARNING "No QGIS version determined: failed to find qgsconfig.h")
  ENDIF ()
ENDIF ()

IF (QGIS_INCLUDE_DIR AND QGIS_CORE_LIBRARY AND QGIS_ANALYSIS_LIBRARY)
  SET(QGIS_FOUND TRUE)
  add_library(qgis_core INTERFACE)
  target_link_libraries(qgis_core INTERFACE ${QGIS_CORE_LIBRARY})
  target_include_directories(qgis_core INTERFACE ${QGIS_INCLUDE_DIR})
  add_library(QGIS::Core ALIAS qgis_core)

  add_library(qgis_analysis INTERFACE)
  target_link_libraries(qgis_analysis INTERFACE ${QGIS_ANALYSIS_LIBRARY})
  target_include_directories(qgis_analysis INTERFACE ${QGIS_ANYLSIS_INCLUDE_DIR})
  add_library(QGIS::Analysis ALIAS qgis_analysis)

  IF (NOT QGIS_FIND_QUIETLY)
    MESSAGE(STATUS "Found QGIS: ${QGIS_VERSION}")
    MESSAGE(STATUS "Found QGIS core: ${QGIS_CORE_LIBRARY}")
    MESSAGE(STATUS "Found QGIS analysis: ${QGIS_ANALYSIS_LIBRARY}")
  ENDIF (NOT QGIS_FIND_QUIETLY)
ELSE ()
  # Useful logs addressed to sdk mainteners
  IF(NOT QGIS_INCLUDE_DIR)
    MESSAGE(WARNING "Fail to find QGIS core include folder")
  ENDIF ()
  IF(NOT QGIS_CORE_LIBRARY)
    MESSAGE(WARNING "Fail to find QGIS core include folder")
  ENDIF ()
  IF(NOT QGIS_ANALYSIS_INCLUDE_DIR)
    MESSAGE(WARNING "Fail to find QGIS analysis include folder")
  ENDIF ()
  IF(NOT QGIS_ANALYSIS_LIBRARY)
    MESSAGE(WARNING "Fail to find QGIS analysis include folder")
  ENDIF ()
  IF (QGIS_FIND_REQUIRED)
     MESSAGE(FATAL_ERROR "Could not find QGIS. Make sure QGIS sdk is located in CMAKE_PREFIX_PATH")
  ENDIF (QGIS_FIND_REQUIRED)
ENDIF ()
