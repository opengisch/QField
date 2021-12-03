#############################################################
# CMake settings
cmake_minimum_required(VERSION 3.15)

set(QML_IMPORT_PATH ${CMAKE_SOURCE_DIR}/src/qml/imports CACHE PATH "QML import path for Qt Creator to detect custom modules properly")

set(CMAKE_COLOR_MAKEFILE ON)
set(APP_NAME "QField" CACHE STRING "Application Name")
set(APP_ICON "qfield_logo" CACHE STRING "Application Icon")
set(APK_VERSION_CODE "1" CACHE STRING "Apk Version Code (Example: 1)")
set(APP_VERSION "" CACHE STRING "Application Version (Example: 1.0.0)")
set(APP_VERSION_STR "local - dev" CACHE STRING "Application Version Name (Example: 1.0.0 - Homerun)")
set(APP_PACKAGE_NAME "qfield" CACHE STRING "Package name suffix. E.g. qfield --> ch.opengis.qfield")

string(REGEX REPLACE "v" "" CLEAN_APP_VERSION "${APP_VERSION}")

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

include(GetGitRevisionDescription)

set(USE_MAC_BUNDLING ON) # Will be overwritten by the VcpkgToolchain for vcpkg builds
include(VcpkgToolchain)


project(${APP_NAME}
  VERSION ${CLEAN_APP_VERSION}
)


GET_GIT_HEAD_REVISION(GIT_REFSPEC GIT_REV)

message(STATUS "Building for git rev ${GIT_REV}")

set(DEFAULT_BIN_SUBDIR     bin)
set(DEFAULT_CGIBIN_SUBDIR  bin)
set(DEFAULT_LIB_SUBDIR     lib${LIB_SUFFIX})
set(DEFAULT_INCLUDE_SUBDIR include/qfield)

set(QFIELD_BIN_SUBDIR     ${DEFAULT_BIN_SUBDIR}     CACHE STRING "Subdirectory where executables will be installed")
set(QFIELD_LIB_SUBDIR     ${DEFAULT_LIB_SUBDIR}     CACHE STRING "Subdirectory where libraries will be installed")
set(QFIELD_INCLUDE_SUBDIR ${DEFAULT_INCLUDE_SUBDIR} CACHE STRING "Subdirectory where header files will be installed")


set(RELATIVE_PREFIX_PATH ${WITH_VCPKG} CACHE BOOL "Use a prefix path relative to the application itself rather than hard coding the path while compiling")
if(RELATIVE_PREFIX_PATH)
  add_definitions(-DRELATIVE_PREFIX_PATH)
endif()

mark_as_advanced (QFIELD_INCLUDE_SUBDIR QFIELD_BIN_SUBDIR QFIELD_LIB_SUBDIR)
set(QFIELD_BIN_DIR ${QFIELD_BIN_SUBDIR})
set(QFIELD_LIB_DIR ${QFIELD_LIB_SUBDIR})
set(QFIELD_INCLUDE_DIR ${QFIELD_INCLUDE_SUBDIR})
set(QT_ANDROID_APPLICATION_BINARY "qfield")

# set the default locations where the targets (executables, libraries) will land when compiled
# this is to allow running QField directly from the build directory.
if (ANDROID)
  set(QFIELD_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/android-build)
else()
  set(QFIELD_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${QFIELD_OUTPUT_DIRECTORY}/${QFIELD_BIN_SUBDIR}/$<0:>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${QFIELD_OUTPUT_DIRECTORY}/${QFIELD_LIB_SUBDIR}/$<0:>)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/resources DESTINATION share/qfield)

set(WITH_SPIX FALSE CACHE STRING "Compile with Spix for testing")

if (WITH_SPIX)
  find_package(Spix)
endif ()

if (ANDROID)
  # This is necessary in order for CMake to be able to detect libraries in OSGeo4A
  list(APPEND CMAKE_FIND_ROOT_PATH /)
  SET(VCPKG_BASE_DIR "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}")

  add_definitions(-DQGIS_INSTALL_DIR="") # TODO: do we need osgeo4a/[lib]/files here? see qgis.pri

  # Android SDK stuff
  set(ANDROID_TARGET_PLATFORM CACHE INT "Target Android platform SDK version")
  set(ANDROID_PLATFORM 21 CACHE INT "Minimum Android platform SDK version")
  set(ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_BINARY_DIR}/android-template)

  include(platform/android/openssl/CMakeLists.txt)

  find_package(Qt5 COMPONENTS AndroidExtras REQUIRED)

  set(SHARE_PATH "${CMAKE_CURRENT_BINARY_DIR}/android-build/assets/share")
  file(COPY "${VCPKG_BASE_DIR}/share/proj4/" DESTINATION "${SHARE_PATH}/proj/")
  file(MAKE_DIRECTORY "${SHARE_PATH}/qgis/")
  file(COPY "${VCPKG_BASE_DIR}/files/share/svg/" DESTINATION "${SHARE_PATH}/qgis/svg/")
  file(COPY "${VCPKG_BASE_DIR}/files/share/resources/" DESTINATION "${SHARE_PATH}/qgis/resources/")

  # Remove world map to keep apk size a bit smaller
  file(REMOVE "${SHARE_PATH}/qgis/resources/data/world_map.gpkg")
  file(COPY resources/ DESTINATION "${SHARE_PATH}/qfield/")

  # Don't do tests when building for Android
  set(ENABLE_TESTS FALSE)
endif()

set(QT_MIN_VERSION 5.14.0)
find_package(Qt5 COMPONENTS Concurrent Core Qml Gui Xml Positioning Widgets Network Quick Svg OpenGL Sql Sensors WebView Bluetooth MultimediaQuick REQUIRED)

# PrintSupport isn't required, because it doesn't exist for ios
# qgis will deal with it an define a public 'QT_NO_PRINTER'
find_package(Qt5 COMPONENTS PrintSupport QUIET)

find_package(QGIS REQUIRED)
if(NOT ${CMAKE_SYSTEM_NAME} STREQUAL "iOS")
if(WITH_VCPKG)
  find_package(PROJ)
else()
  find_package(Proj)
endif()
  find_package(GDAL)
endif()
find_package(Qca REQUIRED)
find_package(QtKeychain)

if (${CMAKE_SYSTEM_NAME} STREQUAL "iOS")
  add_definitions(-DQT_NO_PRINTER)

  set(SHARE_PATH "${CMAKE_CURRENT_BINARY_DIR}/share")

  # keep trailing slash for QField to rename the directory (instead of copying in subdir)
  file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources/ DESTINATION ${SHARE_PATH}/qfield)
  file(COPY ${QGIS_APPLE_RESOURCES_DIR}/resources DESTINATION ${SHARE_PATH}/qgis)
  file(COPY ${QGIS_APPLE_RESOURCES_DIR}/svg DESTINATION ${SHARE_PATH}/qgis)
  file(COPY ${QFIELD_PROJ_DIR} DESTINATION ${SHARE_PATH})
  # Remove world map to keep apk size a bit smaller
  file(REMOVE "${SHARE_PATH}/qgis/resources/data/world_map.gpkg")

  set(QFIELD_RESOURCE_FILES ${SHARE_PATH})
endif()

set(ENABLE_TESTS CACHE BOOL "Build unit tests")

if(MSVC)
  find_package(Qt5 COMPONENTS Charts REQUIRED) # vcpkg doesn't include QtCharts.dll as dep of the qml module otherwise
  add_definitions(-D_USE_MATH_DEFINES)
  add_definitions(-DNOMINMAX)
  add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
endif()

add_subdirectory(src/core)
add_subdirectory(src/app)

if (ENABLE_TESTS)
  find_package(Qt5 COMPONENTS Test QuickTest)
  enable_testing()
  add_subdirectory(test)
endif()

if(WITH_VCPKG AND NOT ANDROID)
  SET(VCPKG_BASE_DIR "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}")
  if(MSVC)
    SET(QGIS_PLUGIN_DIR "${VCPKG_BASE_DIR}/tools/qgis/plugins")
    file(GLOB PROVIDER_LIBS
      "${QGIS_PLUGIN_DIR}/*provider*.dll"
    )
    file(GLOB AUTHMETHODS_LIBS
      "${QGIS_PLUGIN_DIR}/*authmethod*.dll"
    )
    # From QGIS CMakeLists.txt
    set(QGIS_PLUGIN_INSTALL_PREFIX "plugins")
  else()
    SET(QGIS_PLUGIN_DIR "${VCPKG_BASE_DIR}/lib/qgis/plugins")
    file(GLOB PROVIDER_LIBS
      "${QGIS_PLUGIN_DIR}/*provider*.so"
    )
    file(GLOB AUTHMETHODS_LIBS
      "${QGIS_PLUGIN_DIR}/*authmethod*.so"
    )
    # From QGIS CMakeLists.txt
    set(QGIS_PLUGIN_INSTALL_PREFIX "lib${LIB_SUFFIX}/qgis/plugins")
  endif()
  add_custom_target(deploy)
  add_custom_command(TARGET deploy
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/output/bin/qgis/plugins"
  )
  foreach(LIB ${PROVIDER_LIBS})
    add_custom_command(TARGET deploy
                      POST_BUILD
                      COMMAND ${CMAKE_COMMAND} -E copy_if_different "${LIB}" "${CMAKE_BINARY_DIR}/output/bin/qgis/plugins"
    )
    install(FILES "${LIB}" DESTINATION "${QGIS_PLUGIN_INSTALL_PREFIX}")
  endforeach()
  foreach(LIB ${AUTHMETHODS_LIBS})
    add_custom_command(TARGET deploy
                      POST_BUILD
                      COMMAND ${CMAKE_COMMAND} -E copy_if_different "${LIB}" "${CMAKE_BINARY_DIR}/output/bin/qgis/plugins"
    )
  endforeach()
  add_custom_command(TARGET deploy
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${VCPKG_BASE_DIR}/share/qgis/resources" "${CMAKE_BINARY_DIR}/output/bin/qgis/resources"
  )
  add_custom_command(TARGET deploy
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${VCPKG_BASE_DIR}/share/proj4" "${CMAKE_BINARY_DIR}/output/bin/proj"
  )
  add_custom_command(TARGET deploy
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${VCPKG_BASE_DIR}/share/gdal" "${CMAKE_BINARY_DIR}/output/bin/gdal"
  )
  install(DIRECTORY "${VCPKG_BASE_DIR}/share/qgis/resources/" DESTINATION "share/qgis/resources")
  install(DIRECTORY "${VCPKG_BASE_DIR}/share/proj4/" DESTINATION "share/proj")
  install(DIRECTORY "${VCPKG_BASE_DIR}/share/gdal/" DESTINATION "share/gdal")

  if(EXISTS ${VCPKG_BASE_DIR}/plugins/mediaservice)
    add_custom_command(TARGET deploy
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_directory "${VCPKG_BASE_DIR}/plugins/mediaservice" "${CMAKE_BINARY_DIR}/output/bin/plugins/mediaservice"
    )
    install(DIRECTORY "${VCPKG_BASE_DIR}/plugins/mediaservice/" DESTINATION "bin/plugins/mediaservice")
  endif()

  add_dependencies(qfield deploy)
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  install(FILES ${CMAKE_SOURCE_DIR}/images/icons/qfield_logo.svg DESTINATION share/icons RENAME qfield.svg)
  install(FILES ${CMAKE_SOURCE_DIR}/platform/linux/qfield.desktop DESTINATION share/qfield)
endif()

include(Package)
