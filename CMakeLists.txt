#############################################################
# CMake settings
cmake_minimum_required(VERSION 3.19)

set(VCPKG_TAG 604697b05b6bf8c75518d2c133a149ff7ecb3c8b)

set(QML_IMPORT_PATH ${CMAKE_SOURCE_DIR}/src/qml/imports CACHE PATH "QML import path for Qt Creator to detect custom modules properly")

set(CMAKE_COLOR_MAKEFILE ON)
set(APP_NAME "QField" CACHE STRING "Application Name")
set(APP_ICON "qfield_logo" CACHE STRING "Application Icon (must exist as images/icons/{APP_ICON}.svg and platform/android/res/drawable/{APP_ICON}_vector.xml)")
set(APK_VERSION_CODE "1" CACHE STRING "Apk Version Code (Example: 1)")
set(APP_VERSION "0" CACHE STRING "Application Version (Example: v1.0.0)")
set(APP_VERSION_STR "local - dev" CACHE STRING "Application Version Name (Example: 1.0.0 - Homerun)")
set(APP_PACKAGE_NAME "qfield" CACHE STRING "Package name suffix. E.g. qfield --> ch.opengis.qfield")

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
set(WITH_SERIALPORT
    ON
    CACHE BOOL "Enable serial port support")
set(WITH_NFC
    ON
    CACHE BOOL "Enable NFC support")

set(SENTRY_DSN "" CACHE STRING "The sentry dsn for logging purpose. If not set, sentry will be disabled")
set(SENTRY_ENV "" CACHE STRING "The sentry environment for logging purpose.")
mark_as_advanced(SENTRY_DSN SENTRY_ENV)
if(SENTRY_DSN)
  set(WITH_SENTRY 1)
else()
  set(WITH_SENTRY 0)
endif()

set(QT_MIN_VERSION 6.6.0)

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

  set(ANDROID_DRAWABLE_DENSITIES mdpi hdpi xhdpi xxhdpi xxxhdpi)

  foreach(DENSITY ${ANDROID_DRAWABLE_DENSITIES})
    set(DST_DIR "${ANDROID_PACKAGE_SOURCE_DIR}/res/drawable-${DENSITY}")
    file(MAKE_DIRECTORY "${DST_DIR}")

    configure_file(
      "${CMAKE_SOURCE_DIR}/platform/android/res/drawable-${DENSITY}/${APP_ICON}.png"
      "${DST_DIR}/${APP_ICON}.png"
      COPYONLY
    )
  endforeach()

  set(DST_DRAWABLE_DIR "${ANDROID_PACKAGE_SOURCE_DIR}/res/drawable")
  file(MAKE_DIRECTORY "${DST_DRAWABLE_DIR}")

  configure_file(
    "${CMAKE_SOURCE_DIR}/platform/android/res/drawable/${APP_ICON}_vector.xml"
    "${DST_DRAWABLE_DIR}/${APP_ICON}_vector.xml"
    COPYONLY
  )

endif()

find_package(Qt6 COMPONENTS Concurrent Core Qml Gui Xml Positioning Widgets Network Quick Svg Sql Sensors WebView Multimedia Bluetooth Nfc RemoteObjects WebSockets Quick3D REQUIRED)

if(NOT CMAKE_SYSTEM_NAME STREQUAL "iOS")
  find_package(Qt6 COMPONENTS PrintSupport REQUIRED)
endif()

find_package(QGIS COMPONENTS Core Analysis REQUIRED)
find_package(PROJ REQUIRED)
find_package(GDAL REQUIRED)
find_package(Qca REQUIRED)
find_package(LibZip REQUIRED)

if(ANDROID)
  # Qt by default assumes that cmake files live in `lib/cmake`. Point to `share` instead.
  # See https://github.com/qt/qtbase/blob/39290c508e2c7fa816c6f79b3eb284adc0cad3c0/src/corelib/Qt6AndroidMacros.cmake#L1016C16-L1026
  get_filename_component(QT_ANDROID_PATH_CMAKE_DIR_${ANDROID_ABI} "${Qt6_DIR}/.." ABSOLUTE)
  message(STATUS "QT_ANDROID_PATH_CMAKE_DIR_${ANDROID_ABI}: ${QT_ANDROID_PATH_CMAKE_DIR_${ANDROID_ABI}}")
elseif(CMAKE_SYSTEM_NAME STREQUAL "iOS")
  set(QT_HOST_PATH "${VCPKG_INSTALLED_DIR}/${VCPKG_HOST_TRIPLET}")
  set(QT_HOST_PATH_CMAKE_DIR "${QT_HOST_PATH}/share")
  # The QmlImportScanner is not discovered on the host path, it should probably be caught by
  # https://github.com/qt/qtbase/blob/8c14b0c/cmake/QtConfig.cmake.in#L104-L122
  # at least we know where to find it:
  set(Qt6QmlImportScanner_DIR "${VCPKG_INSTALLED_DIR}/share/Qt6QmlImportScanner")
  message(STATUS "QT_HOST_PATH: ${QT_HOST_PATH}")
endif()

set(ENABLE_TESTS CACHE BOOL "Enable tests")
set(WITH_UNITTESTS TRUE CACHE BOOL "Enable unit tests")
set(WITH_SPIX FALSE CACHE BOOL "Compile with Spix for testing")

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
  set(SHARE_DIR "${ANDROID_PACKAGE_SOURCE_DIR}/assets")
else()
  set(SHARE_DIR "${CMAKE_BINARY_DIR}/output/share")
endif()
file(MAKE_DIRECTORY "${SHARE_DIR}")

if(WITH_SENTRY)
  add_subdirectory(src/sentry)
endif()

if (WITH_SPIX)
  find_package(Spix REQUIRED)
endif ()

add_subdirectory(src/core)
add_subdirectory(src/qml)
if (ANDROID)
  add_subdirectory(src/service)
endif()
add_subdirectory(src/app)

if (ENABLE_TESTS)
  set (TEST_DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test/testdata")

  find_package(Qt6 COMPONENTS Test QuickTest)
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

  # This is needed for gdal to successfully access remote datasets through encrypted connections
  file(DOWNLOAD https://curl.se/ca/cacert.pem "${CMAKE_BINARY_DIR}/cacert.pem" TLS_VERIFY ON STATUS DOWNLOAD_STATUS)
  list(GET DOWNLOAD_STATUS 0 STATUS_CODE)
  list(GET DOWNLOAD_STATUS 1 ERROR_MESSAGE)
  if(NOT ${STATUS_CODE} EQUAL 0)
      message(FATAL_ERROR "Error occurred during download: ${ERROR_MESSAGE}")
  endif()

  add_custom_command(TARGET deploy
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_BINARY_DIR}/cacert.pem" "${SHARE_DIR}/cacert.pem"
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
  install(FILES "${CMAKE_BINARY_DIR}/cacert.pem" DESTINATION ${CMAKE_INSTALL_DATADIR}/)

  add_dependencies(qfield deploy)
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  install(FILES ${CMAKE_SOURCE_DIR}/images/icons/qfield_logo.svg DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/128x128/apps RENAME qfield.svg)
  install(FILES ${CMAKE_SOURCE_DIR}/platform/linux/qfield.desktop DESTINATION ${CMAKE_INSTALL_DATADIR}/qfield)
  install(FILES ${CMAKE_SOURCE_DIR}/platform/linux/qfield.metainfo.xml DESTINATION ${CMAKE_INSTALL_DATADIR}/metainfo)
endif()

include(Package)
