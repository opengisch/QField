SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "2")
SET(CPACK_PACKAGE_VERSION_PATCH "0")
SET(COMPLETE_VERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH})
SET(PROJECT_VERSION ${COMPLETE_VERSION})
CMAKE_POLICY(SET CMP0048 NEW)
PROJECT(qgis-mobile VERSION ${PROJECT_VERSION})
IF (APPLE)
  SET(QGIS_MOBILE_APP_NAME "QField")
ELSE (APPLE)
  SET(QGIS_MOBILE_APP_NAME "qfield")
ENDIF (APPLE)

# Note the version no is Mmmpp for Major/minor/patch, 0-padded, thus '10100' for 1.1.0
MATH(EXPR QGIS_MOBILE_VERSION_INT "${CPACK_PACKAGE_VERSION_MAJOR}*10000+${CPACK_PACKAGE_VERSION_MINOR}*100+${CPACK_PACKAGE_VERSION_PATCH}")
MESSAGE(STATUS "QField version: ${COMPLETE_VERSION} ${RELEASE_NAME} (${QGIS_MOBILE_VERSION_INT})")

#############################################################
# CMake settings

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.6)

SET(CMAKE_COLOR_MAKEFILE ON)

# set path to additional CMake modules
SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

# in generated makefiles use relative paths so the project dir is moveable
# Note commented out since it cause problems but it would be nice to resolve these and enable
#
# issue is caused by INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}) near the end of this file generating incorrect path
#SET (CMAKE_USE_RELATIVE_PATHS ON)

# try to configure and build POSTGRESQL support
# Compile flag. Make it possible to turn it off.
SET (PEDANTIC TRUE CACHE BOOL "Determines if we should compile in pedantic mode.")

# whether unit tests should be build
SET (ENABLE_TESTS TRUE CACHE BOOL "Build unit tests?")

#############################################################
# find QGIS and setup
FIND_PACKAGE(QGIS REQUIRED)
ADD_DEFINITIONS( "-DQGIS_PLUGIN_DIR=\"${QGIS_PLUGIN_DIR}\"" )

#############################################################
# search for Qt
SET(QT_MIN_VERSION 5.3.0)
FIND_PACKAGE(Qt5Core QUIET)
FIND_PACKAGE(Qt5Gui REQUIRED)
FIND_PACKAGE(Qt5Widgets REQUIRED)
FIND_PACKAGE(Qt5Network REQUIRED)
FIND_PACKAGE(Qt5Xml REQUIRED)
FIND_PACKAGE(Qt5Svg REQUIRED)
FIND_PACKAGE(Qt5Concurrent REQUIRED)
FIND_PACKAGE(Qt5PrintSupport REQUIRED)
FIND_PACKAGE(Qt5Positioning)
FIND_PACKAGE(Qt5Test REQUIRED)
FIND_PACKAGE(Qt5UiTools REQUIRED)
FIND_PACKAGE(Qt5Script REQUIRED)
FIND_PACKAGE(Qt5Sql REQUIRED)
FIND_PACKAGE(Qt5OpenGL REQUIRED)

MESSAGE(STATUS "Found Qt version: ${Qt5Core_VERSION_STRING}")

#############################################################
# search for other required packages
FIND_PACKAGE(GEOS)

#############################################################
# enable warnings

IF (PEDANTIC)
  MESSAGE (STATUS "Pedantic compiler settings enabled")
  IF(MSVC)
    IF (NOT USING_NMAKE)
      ADD_DEFINITIONS( /W4 )
    ENDIF (NOT USING_NMAKE)

    # disable warnings
    ADD_DEFINITIONS( /wd4100 )  # unused formal parameters
    ADD_DEFINITIONS( /wd4127 )  # constant conditional expressions (used in Qt template classes)
    ADD_DEFINITIONS( /wd4231 )  # nonstandard extension used : 'identifier' before template explicit instantiation (used in Qt template classes)
    ADD_DEFINITIONS( /wd4244 )  # conversion from '...' to '...' possible loss of data
    ADD_DEFINITIONS( /wd4251 )  # needs to have dll-interface to be used by clients of class (occurs in Qt template classes)
    ADD_DEFINITIONS( /wd4275 )  # non dll-interface class '...' used as base for dll-interface class '...'
    ADD_DEFINITIONS( /wd4505 )  # unreferenced local function has been removed (QgsRasterDataProvider::extent)
    ADD_DEFINITIONS( /wd4510 )  # default constructor could not be generated (sqlite3_index_info, QMap)
    ADD_DEFINITIONS( /wd4512 )  # assignment operator could not be generated (sqlite3_index_info)
    ADD_DEFINITIONS( /wd4610 )  # user defined constructor required (sqlite3_index_info)
    ADD_DEFINITIONS( /wd4706 )  # assignment within conditional expression (pal)
    ADD_DEFINITIONS( /wd4800 )  # 'int' : forcing value to bool 'true' or 'false' (performance warning)
  ELSE (MSVC)
    # add warnings via flags (not as definitions as on Mac -Wall can not be overridden per language )
    SET(_warnings "-Wall -Wextra -Wno-long-long -Wformat-security -Wno-strict-aliasing")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${_warnings}")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${_warnings}")

    # Qt produces lots of warnings with strict aliasing (as of Qt 4.4.0 & GCC 4.3)
    # There are redundant declarations in Qt and GDAL
    # ADD_DEFINITIONS( -fstrict-aliasing -Wstrict-aliasing=1 -Wredundant-decls )

    IF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
      SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-return-type-c-linkage -Wno-overloaded-virtual")
    ENDIF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")

    # add any extra CXXFLAGS flags set by user. can be -D CXX_EXTRA_FLAGS or environment variable
    # command line -D option overrides environment variable
    # e.g. useful for suppressing transient upstream warnings in dependencies, like Qt
    SET(CXX_EXTRA_FLAGS "" CACHE STRING "Additional appended CXXFLAGS")
    IF ("${CXX_EXTRA_FLAGS}" STREQUAL "" AND DEFINED $ENV{CXX_EXTRA_FLAGS})
      SET(CXX_EXTRA_FLAGS "$ENV{CXX_EXTRA_FLAGS}")
    ENDIF ("${CXX_EXTRA_FLAGS}" STREQUAL "" AND DEFINED $ENV{CXX_EXTRA_FLAGS})
    IF (NOT "${CXX_EXTRA_FLAGS}" STREQUAL "")
      MESSAGE (STATUS "Appending CXX_EXTRA_FLAGS")
      SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_EXTRA_FLAGS}")
    ENDIF (NOT "${CXX_EXTRA_FLAGS}" STREQUAL "")
  ENDIF (MSVC)

ENDIF (PEDANTIC)

IF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Qunused-arguments")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Qunused-arguments")
  SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Qunused-arguments")
  SET(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -Qunused-arguments")
  SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Qunused-arguments")
ENDIF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")

IF (CMAKE_BUILD_TYPE MATCHES Debug OR CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
  MESSAGE (STATUS "Debug output enabled")
  ADD_DEFINITIONS(-DQGISDEBUG=1)
  ADD_DEFINITIONS(-DQT_QML_DEBUG)
ENDIF (CMAKE_BUILD_TYPE MATCHES Debug OR CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)

IF(MSVC)
  SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -DQGISDEBUG=1")
  SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DQGISDEBUG=1")
  # disable macros that offend std::numeric_limits<T>::min()/max()
  ADD_DEFINITIONS(-DNOMINMAX)
ELSE(MSVC)
  INCLUDE(CheckCXXCompilerFlag)
  CHECK_CXX_COMPILER_FLAG("-std=c++1y" COMPILER_SUPPORTS_CXX1Y)
  IF(COMPILER_SUPPORTS_CXX1Y)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")
  ELSE(COMPILER_SUPPORTS_CXX1Y)
    MESSAGE(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++1y support. Please use a different C++ compiler.")
  ENDIF(COMPILER_SUPPORTS_CXX1Y)
ENDIF(MSVC)

#############################################################
# platform specific stuff

IF (WIN32)
  SET (DEFAULT_LIB_SUBDIR     lib)
  SET (DEFAULT_LIBEXEC_SUBDIR .)
  SET (DEFAULT_DATA_SUBDIR    .)
  SET (DEFAULT_PLUGIN_SUBDIR  plugins)
  SET (DEFAULT_INCLUDE_SUBDIR include)

  IF (MSVC)
    SET (DEFAULT_BIN_SUBDIR bin)
    # put all the build products into a single directory
    # under build (doesnt affect install target) to make for
    # easier debugging.

    # Turn on defines for non standard maths stuff
    ADD_DEFINITIONS(-D_USE_MATH_DEFINES)

    # Turn off deprecation warnings
    ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
    ADD_DEFINITIONS(-D_CRT_NONSTDC_NO_WARNINGS)

    IF (CMAKE_BUILD_TYPE MATCHES Debug OR CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
      IF (NOT USING_NMAKE)
        MESSAGE (STATUS "Generating browse files")
        ADD_DEFINITIONS( /FR )
      ENDIF (NOT USING_NMAKE)
    ENDIF (CMAKE_BUILD_TYPE MATCHES Debug OR CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)

    IF (INSTALL_DEPS)
      INSTALL(DIRECTORY ${INSTALL_DEPS} DESTINATION .)
    ENDIF (INSTALL_DEPS)
  ELSE(MSVC)
    SET (DEFAULT_BIN_SUBDIR .)
  ENDIF(MSVC)
ELSE (WIN32)

  IF (APPLE)
    # for Mac OS X, everything is put inside an application bundle
    # save the root install prefix for the app later
    SET (QGIS_MOBILE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
    SET (QGIS_MOBILE_MACAPP_PREFIX ${CMAKE_INSTALL_PREFIX}/${QGIS_MOBILE_APP_NAME}.app/Contents)
    # common prefix for components, let cmake handle it
    SET (CMAKE_INSTALL_PREFIX ${QGIS_MOBILE_MACAPP_PREFIX}/MacOS)
    # 4 bundling levels, each includes previous
    #   0  nothing
    #   1  Qt frameworks
    #   2  non-system libraries, "standard"
    #   3  non-system frameworks, "standalone"
    SET (QGIS_MOBILE_MACAPP_BUNDLE 1 CACHE STRING "What to bundle into app package")
    SET (QGIS_MOBILE_MACAPP_BUNDLE_USER "" CACHE STRING "Path to user bundling script")
    SET (QGIS_MOBILE_MACAPP_INSTALL_DEV FALSE CACHE BOOL "Install developer frameworks")
    SET (QGIS_MOBILE_MACAPP_DEV_PREFIX "/Library/Frameworks" CACHE STRING "Path to install developer frameworks")
    # if we have @loader_path, >=OSX 10.5 (darwin 9+)
    IF (CMAKE_SYSTEM_VERSION VERSION_GREATER 9.0.0 OR CMAKE_SYSTEM_VERSION VERSION_EQUAL 9.0.0)
        SET (OSX_HAVE_LOADERPATH 1)
    ELSE ()
        SET (OSX_HAVE_LOADERPATH 0)
    ENDIF ()
    #this will define ${APP_SERVICES_LIBRARY}
    FIND_LIBRARY(APP_SERVICES_LIBRARY ApplicationServices )

    SET (DEFAULT_BIN_SUBDIR      bin)
    SET (QGIS_MOBILE_BIN_SUBDIR_REV     ..)
    SET (DEFAULT_LIB_SUBDIR      lib)
    SET (QGIS_MOBILE_LIB_SUBDIR_REV     ..)
    SET (QGIS_MOBILE_FW_SUBDIR          ../Frameworks)
    SET (QGIS_MOBILE_FW_SUBDIR_REV      ../MacOS)
    SET (DEFAULT_DATA_SUBDIR     ../Resources)
    SET (QGIS_MOBILE_DATA_SUBDIR_REV    ../MacOS)
    SET (DEFAULT_LIBEXEC_SUBDIR  lib/qgis)
    SET (QGIS_MOBILE_LIBEXEC_SUBDIR_REV ../..)
    SET (DEFAULT_PLUGIN_SUBDIR   ../PlugIns/qgis)
    SET (QGIS_MOBILE_PLUGIN_SUBDIR_REV  ../../MacOS)
    SET (DEFAULT_INCLUDE_SUBDIR  include/qgis)
    # path for framework references when running from build directory
    # changed later to reference in-app resources upon install
    SET (CMAKE_INSTALL_NAME_DIR ${CMAKE_BINARY_DIR}/output/lib)
    IF (WITH_GLOBE)
      SET (OSG_PLUGINS_PATH "" CACHE PATH "Path to OSG plugins for bundling")
    ENDIF (WITH_GLOBE)
    # recent cmakes force SDKs, recent SDKs don't have user symlinks
    # need to find non-system frameworks
    # cmake bug #0007250 - CMAKE_SHARED_LINKER_FLAGS ignored when creating
    # a framework, so these need to be manually handled with LINK_FLAGS options
    SET (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -F/Library/Frameworks")
    SET (CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -F/Library/Frameworks")
    SET (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -F/Library/Frameworks")

  ELSE (APPLE)
    # UNIX
    SET (DEFAULT_BIN_SUBDIR     bin)
    SET (DEFAULT_LIB_SUBDIR     lib${LIB_SUFFIX})
    SET (DEFAULT_DATA_SUBDIR    share/qgis)
    SET (DEFAULT_LIBEXEC_SUBDIR lib${LIB_SUFFIX}/qgis)
    SET (DEFAULT_PLUGIN_SUBDIR  lib${LIB_SUFFIX}/qgis/plugins)
    SET (DEFAULT_INCLUDE_SUBDIR include/qgis)
  ENDIF (APPLE)

ENDIF (WIN32)

IF (ANDROID)
    SET (DEFAULT_PLUGIN_SUBDIR  lib)
    SET (DEFAULT_DATA_SUBDIR    files/share)
ENDIF (ANDROID)

#############################################################
# user-changeable settings which can be used to customize
# layout of QGIS installation
# (default values are platform-specific)

SET (QGIS_MOBILE_BIN_SUBDIR     ${DEFAULT_BIN_SUBDIR}     CACHE STRING "Subdirectory where executables will be installed")
SET (QGIS_MOBILE_LIB_SUBDIR     ${DEFAULT_LIB_SUBDIR}     CACHE STRING "Subdirectory where libraries will be installed")
SET (QGIS_MOBILE_LIBEXEC_SUBDIR ${DEFAULT_LIBEXEC_SUBDIR} CACHE STRING "Subdirectory where private executables will be installed")
SET (QGIS_MOBILE_DATA_SUBDIR    ${DEFAULT_DATA_SUBDIR}    CACHE STRING "Subdirectory where QGIS data will be installed")
SET (QGIS_MOBILE_PLUGIN_SUBDIR  ${DEFAULT_PLUGIN_SUBDIR}  CACHE STRING "Subdirectory where plugins will be installed")
SET (QGIS_MOBILE_INCLUDE_SUBDIR ${DEFAULT_INCLUDE_SUBDIR} CACHE STRING "Subdirectory where header files will be installed")

# mark *_SUBDIR variables as advanced as this is not something
# that an average user would use
MARK_AS_ADVANCED (QGIS_MOBILE_BIN_SUBDIR QGIS_MOBILE_LIB_SUBDIR QGIS_MOBILE_LIBEXEC_SUBDIR QGIS_MOBILE_DATA_SUBDIR QGIS_MOBILE_PLUGIN_SUBDIR QGIS_MOBILE_INCLUDE_SUBDIR)

# full paths for the installation
SET (QGIS_MOBILE_BIN_DIR     ${QGIS_MOBILE_BIN_SUBDIR})
SET (QGIS_MOBILE_LIB_DIR     ${QGIS_MOBILE_LIB_SUBDIR})
SET (QGIS_MOBILE_LIBEXEC_DIR ${QGIS_MOBILE_LIBEXEC_SUBDIR})
SET (QGIS_MOBILE_DATA_DIR    ${QGIS_MOBILE_DATA_SUBDIR})
SET (QGIS_MOBILE_PLUGIN_DIR  ${QGIS_MOBILE_PLUGIN_SUBDIR})
SET (QGIS_MOBILE_INCLUDE_DIR ${QGIS_MOBILE_INCLUDE_SUBDIR})

# set the default locations where the targets (executables, libraries) will land when compiled
# this is to allow running qgis from the source tree without having to actually do a "make install"
SET (QGIS_MOBILE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${QGIS_MOBILE_OUTPUT_DIRECTORY}/${QGIS_MOBILE_BIN_SUBDIR})
SET (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${QGIS_MOBILE_OUTPUT_DIRECTORY}/${QGIS_MOBILE_LIB_SUBDIR})

# Eat QGIS Windows DLL Export stuff (will need to be adjusted for win compat)
ADD_DEFINITIONS( "-DCORE_EXPORT=" )
ADD_DEFINITIONS( "-DGUI_EXPORT=" )

# Process subdirs

IF(ENABLE_TESTS)
  ENABLE_TESTING()
  ADD_SUBDIRECTORY(test)
ENDIF(ENABLE_TESTS)
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(images)

ADD_CUSTOM_TARGET(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")
