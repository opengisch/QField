#############################################################
# CMake settings
cmake_minimum_required(VERSION 3.19)

set(VCPKG_TAG 06b5f4a769d848d1a20fa0acd556019728b56273)

set(QML_IMPORT_PATH ${CMAKE_SOURCE_DIR}/src/qml/imports CACHE PATH "QML import path for Qt Creator to detect custom modules properly")

set(CMAKE_COLOR_MAKEFILE ON)
set(APP_NAME "QField" CACHE STRING "Application Name")
set(APP_ICON "qfield_logo" CACHE STRING "Application Icon")
set(APK_VERSION_CODE "1" CACHE STRING "Apk Version Code (Example: 1)")
set(APP_VERSION "0" CACHE STRING "Application Version (Example: v1.0.0)")
set(APP_VERSION_STR "local - dev" CACHE STRING "Application Version Name (Example: 1.0.0 - Homerun)")
set(APP_PACKAGE_NAME "qfield" CACHE STRING "Package name suffix. E.g. qfield --> ch.opengis.qfield")

set(BUILD_WITH_QT6 OFF CACHE BOOL "Build with Qt6")

string(REGEX REPLACE "v" "" CLEAN_APP_VERSION "${APP_VERSION}")

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

# Platform specific fixes (android, macos, ...)
include(Platform)
# Obtain git revision
include(GetGitRevisionDescription)
# The vcpkg toolchain to compile dependencies
include(VcpkgToolchain)
# The default installation paths
include(GNUInstallDirs)

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

set(WITH_CCACHE OFF CACHE BOOL "Build with ccache")
if(WITH_CCACHE)
  include(ccache)
endif()

install(DIRECTORY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${CMAKE_INSTALL_DATADIR}/qfield)

set(WITH_BLUETOOTH
    ON
    CACHE BOOL "Enable bluetooth support")

set(WITH_SPIX FALSE CACHE BOOL "Compile with Spix for testing")

if (WITH_SPIX)
  find_package(Spix REQUIRED)
endif ()

set(SENTRY_DSN "" CACHE STRING "The sentry dsn for logging purpose. If not set, sentry will be disabled")
set(SENTRY_ENV "" CACHE STRING "The sentry environment for logging purpose.")
mark_as_advanced(SENTRY_DSN SENTRY_ENV)
if(SENTRY_DSN)
  set(WITH_SENTRY 1)
else()
  set(WITH_SENTRY 0)
endif()

if(BUILD_WITH_QT6)
  set(QT_PKG Qt6)
else()
  set(QT_PKG Qt5)
endif()

if (ANDROID)
  if(VCPKG_TARGET_TRIPLET STREQUAL "arm-android" OR VCPKG_TARGET_TRIPLET STREQUAL "arm-neon-android")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -fstack-protector-strong -lunwind -Wl,--exclude-libs=libunwind.a")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -fstack-protector-strong -lunwind -Wl,--exclude-libs=libunwind.a")
  else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -fstack-protector-strong")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -fstack-protector-strong")
  endif()

  link_libraries("-ljnigraphics")

  # Make sure the generator for android_deployment_settings.json can figure out the architecture
  list(APPEND CMAKE_FIND_ROOT_PATH /)

  set(ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_BINARY_DIR}/android-template)

  include(FetchContent)
  FetchContent_Declare(android_openssl
      GIT_REPOSITORY https://github.com/KDAB/android_openssl.git
      GIT_TAG aa3c67c59160715216e89ae2c48291d6021601fa
  )
  FetchContent_MakeAvailable(android_openssl)
  include(${FETCHCONTENT_BASE_DIR}/android_openssl-src/CMakeLists.txt)

  if(NOT BUILD_WITH_QT6)
    find_package(Qt5 COMPONENTS AndroidExtras REQUIRED)
  endif()
endif()

set(QT_MIN_VERSION 5.14.0)
find_package(${QT_PKG} COMPONENTS Concurrent Core Qml Gui Xml Positioning Widgets Network Quick Svg OpenGL Sql Sensors WebView Multimedia REQUIRED)

# PrintSupport isn't required, because it doesn't exist for ios
# qgis will deal with it an define a public 'QT_NO_PRINTER'
find_package(${QT_PKG} COMPONENTS PrintSupport QUIET)

find_package(QGIS COMPONENTS Core Analysis REQUIRED)
find_package(PROJ REQUIRED)
find_package(GDAL REQUIRED)
find_package(Qca REQUIRED)
find_package(QtKeychain REQUIRED)

set(ENABLE_TESTS CACHE BOOL "Build unit tests")

if(MSVC)
  add_definitions(-D_USE_MATH_DEFINES)
  add_definitions(-DNOMINMAX)
  add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)

  # Generate pdb files
  add_compile_options("$<$<NOT:$<CONFIG:Debug>>:/Zi>")
  add_link_options("$<$<NOT:$<CONFIG:Debug>>:/DEBUG>")
  add_link_options("$<$<NOT:$<CONFIG:Debug>>:/OPT:REF>")
  add_link_options("$<$<NOT:$<CONFIG:Debug>>:/OPT:ICF>")
elseif(CMAKE_SYSTEM_NAME STREQUAL "iOS")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
  set(CMAKE_XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT "dwarf-with-dsym")
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  set(SHARE_DIR "${CMAKE_BINARY_DIR}/output/bin/qfield.app/Contents/share")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Android")
  set(SHARE_DIR "${CMAKE_BINARY_DIR}/android-build/assets/share")
else()
  set(SHARE_DIR "${CMAKE_BINARY_DIR}/output/share")
endif()
file(MAKE_DIRECTORY "${SHARE_DIR}")

add_subdirectory(src/core)
add_subdirectory(src/qml)
if (ANDROID)
  add_subdirectory(src/service)
endif()
add_subdirectory(src/app)

if(WITH_SENTRY)
  add_subdirectory(src/sentry)
endif()

if (ENABLE_TESTS)
  find_package(${QT_PKG} COMPONENTS Test QuickTest)
  enable_testing()
  add_subdirectory(test)
endif()

if(WITH_VCPKG)
  # Copy files from the install dir to where it
  # will be bundled
  add_custom_target(deploy)

  function(copy_resource source target)
    add_custom_command(TARGET deploy
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E make_directory "${SHARE_DIR}/${target}"
      COMMAND ${CMAKE_COMMAND} -E copy_directory "${source}" "${SHARE_DIR}/${target}"
    )
  endfunction()

  set(VCPKG_BASE_DIR "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}")
  if(MSVC)
    set(QGIS_PLUGIN_DIR "${VCPKG_BASE_DIR}/tools/qgis/plugins")
    file(GLOB PROVIDER_LIBS
      "${QGIS_PLUGIN_DIR}/*provider*.dll"
    )
    file(GLOB AUTHMETHODS_LIBS
      "${QGIS_PLUGIN_DIR}/*authmethod*.dll"
    )
    # From QGIS CMakeLists.txt
    set(QGIS_PLUGIN_INSTALL_PREFIX "plugins")
  else()
    set(QGIS_PLUGIN_DIR "${VCPKG_BASE_DIR}/lib/qgis/plugins")
    file(GLOB PROVIDER_LIBS
      "${QGIS_PLUGIN_DIR}/*provider*.so"
    )
    file(GLOB AUTHMETHODS_LIBS
      "${QGIS_PLUGIN_DIR}/*authmethod*.so"
    )
    # From QGIS CMakeLists.txt
    set(QGIS_PLUGIN_INSTALL_PREFIX "lib${LIB_SUFFIX}/qgis/plugins")
  endif()
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
    COMMAND ${CMAKE_COMMAND} -E make_directory "${SHARE_DIR}/qgis/resources"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${SHARE_DIR}/qgis/svg"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${VCPKG_BASE_DIR}/share/qgis/resources" "${SHARE_DIR}/qgis/resources"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${VCPKG_BASE_DIR}/share/qgis/svg" "${SHARE_DIR}/qgis/svg"
    COMMAND ${CMAKE_COMMAND} -E rm -- "${SHARE_DIR}/qgis/resources/data/world_map.gpkg"
    COMMAND ${CMAKE_COMMAND} -E rm -R -- "${SHARE_DIR}/qgis/resources/cpt-city-qgis-min"
  )
  set(PROJ_DATA_PATH "${VCPKG_BASE_DIR}/share/proj")

  if(NOT EXISTS "${PROJ_DATA_PATH}/proj.db")
    message(FATAL_ERROR "proj.db not found at ${PROJ_DATA_PATH}/proj.db")
  endif()

  copy_resource("${PROJ_DATA_PATH}" "proj")
  copy_resource("${VCPKG_BASE_DIR}/share/gdal" "gdal")
  copy_resource("${CMAKE_SOURCE_DIR}/resources/" "qfield")
  install(DIRECTORY "${VCPKG_BASE_DIR}/share/qgis/resources/" DESTINATION "${CMAKE_INSTALL_DATADIR}/qgis/resources")
  install(DIRECTORY "${VCPKG_BASE_DIR}/share/qgis/svg/" DESTINATION "${CMAKE_INSTALL_DATADIR}/qgis/svg")
  install(DIRECTORY "${PROJ_DATA_PATH}/" DESTINATION "${CMAKE_INSTALL_DATADIR}/proj/")
  install(DIRECTORY "${VCPKG_BASE_DIR}/share/gdal/" DESTINATION "${CMAKE_INSTALL_DATADIR}/gdal")

  add_dependencies(qfield deploy)
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  install(FILES ${CMAKE_SOURCE_DIR}/images/icons/qfield_logo.svg DESTINATION ${CMAKE_INSTALL_DATADIR}/icons RENAME qfield.svg)
  install(FILES ${CMAKE_SOURCE_DIR}/platform/linux/qfield.desktop DESTINATION ${CMAKE_INSTALL_DATADIR}/qfield)
  install(FILES ${CMAKE_SOURCE_DIR}/platform/linux/qfield.metainfo.xml DESTINATION ${CMAKE_INSTALL_DATADIR}/metainfo)
endif()

include(Package)
