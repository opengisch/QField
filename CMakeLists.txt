#############################################################
# CMake settings
CMAKE_MINIMUM_REQUIRED(VERSION 3.5)
SET(CMAKE_COLOR_MAKEFILE ON)
SET (APP_NAME "QField" CACHE STRING "Application Name")
SET (APK_VERSION_CODE "1" CACHE STRING "Apk Version Code (Example: 1)")
SET (APP_VERSION "1.0.0" CACHE STRING "Application Version (Example: 1.0.0)")
SET (APP_VERSION_NAME "1.0.0 - dev" CACHE STRING "Application Version Name (Example: 1.0.0 - Homerun)")
PROJECT(${APP_NAME}
  LANGUAGES CXX
  VERSION ${APP_VERSION}
)

SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

ADD_DEFINITIONS(-DVERSTR="${APP_VERSION}")
ADD_DEFINITIONS(-DVERSIONCODE="1.2.0")
ADD_DEFINITIONS(-DAPP_NAME="${APP_NAME}")

SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

ENABLE_TESTING()
FIND_PACKAGE(QGIS)

SET(QT_MIN_VERSION 5.13.0)

SET (DEFAULT_BIN_SUBDIR     bin)
SET (DEFAULT_CGIBIN_SUBDIR  bin)
SET (DEFAULT_LIB_SUBDIR     lib${LIB_SUFFIX})
SET (DEFAULT_INCLUDE_SUBDIR include/qfield)

SET (QFIELD_BIN_SUBDIR     ${DEFAULT_BIN_SUBDIR}     CACHE STRING "Subdirectory where executables will be installed")
SET (QFIELD_LIB_SUBDIR     ${DEFAULT_LIB_SUBDIR}     CACHE STRING "Subdirectory where libraries will be installed")
SET (QFIELD_INCLUDE_SUBDIR ${DEFAULT_INCLUDE_SUBDIR} CACHE STRING "Subdirectory where header files will be installed")

MARK_AS_ADVANCED (QFIELD_INCLUDE_SUBDIR QFIELD_BIN_SUBDIR QFIELD_LIB_SUBDIR)
SET (QFIELD_BIN_DIR ${QFIELD_BIN_SUBDIR})
SET (QFIELD_LIB_DIR ${QFIELD_LIB_SUBDIR})
SET (QFIELD_INCLUDE_DIR ${QFIELD_INCLUDE_SUBDIR})
SET (QT_ANDROID_APPLICATION_BINARY "qfield")

# set the default locations where the targets (executables, libraries) will land when compiled
# this is to allow running QField directly from the build directory.
IF (ANDROID)
  SET (QFIELD_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/android-build)
ELSE ()
  SET (QFIELD_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)
ENDIF ()

SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${QFIELD_OUTPUT_DIRECTORY}/${QFIELD_BIN_SUBDIR})
SET (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${QFIELD_OUTPUT_DIRECTORY}/${QFIELD_LIB_SUBDIR})

IF (ANDROID)
  ADD_DEFINITIONS(-DQGIS_INSTALL_DIR="") # TODO: do we need osgeo4a/[lib]/files here? see qgis.pri
  SET (ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_SOURCE_DIR}/android)
  SET (OSGEO4A_STAGE_DIR NONE CACHE DIRECTORY "OSGEO4A stage path (base path without architecture)")
  SET (OSGEO4A_LIB_DIR ${OSGEO4A_STAGE_DIR}/${ANDROID_ABI}/lib)
  SET(ANDROID_EXTRA_LIBS
    ${OSGEO4A_LIB_DIR}/libssl_1_1.so
    ${OSGEO4A_LIB_DIR}/libcrypto_1_1.so
    ${OSGEO4A_LIB_DIR}/libexpat.so
    ${OSGEO4A_LIB_DIR}/libgeos.so
    ${OSGEO4A_LIB_DIR}/libgeos_c.so
    ${OSGEO4A_LIB_DIR}/libgslcblas.so
    ${OSGEO4A_LIB_DIR}/libsqlite3.so
    ${OSGEO4A_LIB_DIR}/libcharset.so
    ${OSGEO4A_LIB_DIR}/libexiv2.so
    ${OSGEO4A_LIB_DIR}/libiconv.so
    ${OSGEO4A_LIB_DIR}/libopenjp2.so
    ${OSGEO4A_LIB_DIR}/libfreexl.so
    ${OSGEO4A_LIB_DIR}/libtiff.so
    ${OSGEO4A_LIB_DIR}/libgdal.so
    ${OSGEO4A_LIB_DIR}/libproj.so
    ${OSGEO4A_LIB_DIR}/libspatialindex.so
    ${OSGEO4A_LIB_DIR}/libpq.so
    ${OSGEO4A_LIB_DIR}/libspatialite.so
    ${OSGEO4A_LIB_DIR}/libqca-qt5_${ANDROID_ABI}.so
    ${OSGEO4A_LIB_DIR}/libqgis_core_${ANDROID_ABI}.so
    ${OSGEO4A_LIB_DIR}/libqgis_analysis_${ANDROID_ABI}.so
    ${OSGEO4A_LIB_DIR}/libqgis_native_${ANDROID_ABI}.so
    ${OSGEO4A_LIB_DIR}/libqt5keychain_${ANDROID_ABI}.so
    ${OSGEO4A_LIB_DIR}/libzip.so
    ${OSGEO4A_LIB_DIR}/libspatialiteprovider_${ANDROID_ABI}.so
    ${OSGEO4A_LIB_DIR}/libdelimitedtextprovider_${ANDROID_ABI}.so
    ${OSGEO4A_LIB_DIR}/libgpxprovider_${ANDROID_ABI}.so
    ${OSGEO4A_LIB_DIR}/libmssqlprovider_${ANDROID_ABI}.so
    ${OSGEO4A_LIB_DIR}/libowsprovider_${ANDROID_ABI}.so
    ${OSGEO4A_LIB_DIR}/libpostgresprovider_${ANDROID_ABI}.so
    ${OSGEO4A_LIB_DIR}/libspatialiteprovider_${ANDROID_ABI}.so
    ${OSGEO4A_LIB_DIR}/libvirtuallayerprovider_${ANDROID_ABI}.so
    ${OSGEO4A_LIB_DIR}/libwcsprovider_${ANDROID_ABI}.so
    ${OSGEO4A_LIB_DIR}/libwfsprovider_${ANDROID_ABI}.so
    ${OSGEO4A_LIB_DIR}/libwmsprovider_${ANDROID_ABI}.so
  )
  FIND_PACKAGE(Qt5 COMPONENTS AndroidExtras REQUIRED)

  INCLUDE(GetGitRevisionDescription)
  GET_GIT_HEAD_REVISION(GIT_REFSPEC GIT_REV)
  CONFIGURE_FILE(cmake/version.properties.in android-build/version.properties @ONLY)
  CONFIGURE_FILE(cmake/generated.xml.in android-build/res/values/generated.xml @ONLY)

  #  INCLUDE(CreateZip)
  #  CREATE_ZIP("${CMAKE_CURRENT_BINARY_DIR}/android-build/assets.zip" "${OSGEO4A_STAGE_DIR}/${ANDROID_ABI}/share/resources/ ${OSGEO4A_STAGE_DIR}/${ANDROID_ABI}/share/svg/" "${OSGEO4A_STAGE_DIR}/${ANDROID_ABI}/")
  #  ADD_CUSTOM_TARGET(assets ALL DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/android-build/assets.zip")
ENDIF (ANDROID)

FIND_PACKAGE(Qt5 COMPONENTS Test Core Qml Gui Xml Positioning Widgets PrintSupport Network Quick Svg OpenGL Sql PrintSupport Sensors REQUIRED)

ADD_SUBDIRECTORY(3rdparty/tessellate)
ADD_SUBDIRECTORY(src/qgsquick)
ADD_SUBDIRECTORY(src/core)
ADD_SUBDIRECTORY(src/app)
ADD_SUBDIRECTORY(test)
