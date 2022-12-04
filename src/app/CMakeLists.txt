find_package(${QT_PKG}QuickCompiler QUIET)

if(MSVC)
  configure_file("${CMAKE_SOURCE_DIR}/platform/windows/appicon.rc.in"
                 "${CMAKE_CURRENT_BINARY_DIR}/appicon.rc")
  set(ICON_SRC "${CMAKE_CURRENT_BINARY_DIR}/appicon.rc")
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  set(qfield_ICON ${CMAKE_SOURCE_DIR}/platform/macos/qfield_logo.icns)
  set_source_files_properties(${qfield_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION
                                                        "Resources")
  list(APPEND RESOURCES ${qfield_ICON})
endif()

# lrelease .ts files
find_package(${QT_PKG} COMPONENTS LinguistTools)

file(GLOB TS_FILES ${CMAKE_SOURCE_DIR}/i18n/*.ts)
set_source_files_properties(${TS_FILES} PROPERTIES OUTPUT_LOCATION
                                                   "${CMAKE_BINARY_DIR}")
if(BUILD_WITH_QT6)
  qt_add_translation(QM_FILES ${TS_FILES})
else()
  qt5_add_translation(QM_FILES ${TS_FILES})
endif()

# include manually copied .qm files
file(GLOB QM_FILES_MANUAL ${CMAKE_SOURCE_DIR}/i18n/*.qm)
list(APPEND QM_FILES ${QM_FILES_MANUAL})

# create translations QRC file
set(TRANSLATIONS_QRC "${CMAKE_BINARY_DIR}/translations.qrc")
file(WRITE ${TRANSLATIONS_QRC} "<RCC>\n    <qresource prefix=\"/\">")
foreach(QM_FILE ${QM_FILES})
  get_filename_component(QM_FILE_NAME ${QM_FILE} NAME)
  file(APPEND ${TRANSLATIONS_QRC}
       "\n        <file alias=\"i18n/${QM_FILE_NAME}\">${QM_FILE}</file>")
endforeach()
file(APPEND ${TRANSLATIONS_QRC} "\n    </qresource>\n</RCC>")

configure_file(${CMAKE_SOURCE_DIR}/images/logo.qrc.in
               ${CMAKE_BINARY_DIR}/images/logo.qrc @ONLY)

function(create_executable)
  cmake_parse_arguments(exe "" "TARGET" "EXTRA_ARGS" "" ${ARGN})

  set(QFIELD_SOURCES
      main.cpp
      ${RESOURCES}
      ${CMAKE_SOURCE_DIR}/images/images.qrc
      ${CMAKE_BINARY_DIR}/images/logo.qrc
      ${CMAKE_SOURCE_DIR}/resources/resources.qrc
      ${ICON_SRC}
      ${TRANSLATIONS_QRC})

  if(ANDROID)
    add_library(${exe_TARGET} SHARED ${QFIELD_SOURCES})
  else()
    add_executable(${exe_TARGET} ${exe_EXTRA_ARGS} ${QFIELD_SOURCES})
  endif()

  target_link_libraries(${exe_TARGET} PRIVATE qfield_core qfield_qml)

  if(ANDROID)
    target_link_libraries(${exe_TARGET} PRIVATE qfield_core qfield_service)
  endif()

  if(WITH_SENTRY)
    target_link_libraries(${exe_TARGET} PRIVATE qfield_sentry)
  endif()

  get_target_property(QT_TARGET_TYPE ${QT_PKG}::Core TYPE)
  # This should be in src/qml/CMakeLists.txt but that's not possible with Qt5.
  # Retry with Qt6 Same is in tests/CMakeLists.txt
  # https://bugreports.qt.io/browse/QTBUG-80847
  if(${QT_TARGET_TYPE} STREQUAL "STATIC_LIBRARY")
    find_package(${QT_PKG} COMPONENTS QmlImportScanner)

    get_target_property(qfield_qmlfiles qfield_qml QML_FILES)
    file(COPY ${qfield_qmlfiles}
         DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/qmldrop/app")

    if(NOT WITH_BLUETOOTH)
      file(REMOVE
           "${CMAKE_CURRENT_BINARY_DIR}/qmldrop/app/BluetoothDeviceChooser.qml")
    endif()

    # Will be no longer needed for Qt6
    include("${CMAKE_SOURCE_DIR}/cmake/QtQmlImportScanner.cmake")
    qt_import_qml_plugins(${exe_TARGET} PATH_TO_SCAN
                          "${CMAKE_CURRENT_BINARY_DIR}/qmldrop/app")
  endif()
  if(BUILD_WITH_QT6)
    qt_import_plugins(${exe_TARGET} INCLUDE ${QT_PKG}::QSvgPlugin)
  else()
    qt5_import_plugins(${exe_TARGET} INCLUDE ${QT_PKG}::QSvgPlugin)
  endif()

  if(WITH_VCPKG) # this triggers the initialisation of static qca ossl plugin.
    target_compile_definitions(${exe_TARGET} PUBLIC HAVE_STATIC_QCA_PLUGINS)
  endif()

  set_target_properties(${exe_TARGET} PROPERTIES AUTORCC TRUE)
  if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set_target_properties(
      ${exe_TARGET}
      PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "${PROJECT_NAME}"
                 MACOSX_BUNDLE_INFO_STRING "${PROJECT_NAME} OPENGIS.ch"
                 MACOSX_BUNDLE_ICON_FILE "qfield_logo.icns"
                 MACOSX_BUNDLE_GUI_IDENTIFIER "${PROJECT_NAME}"
                 MACOSX_BUNDLE_LONG_VERSION_STRING "${PROJECT_VERSION}"
                 MACOSX_BUNDLE_SHORT_VERSION_STRING
                 "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
                 MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}"
                 MACOSX_BUNDLE_COPYRIGHT "Copyright (c) OPENGIS.ch")
  endif()
endfunction()

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  set(EXTRA_ARGS MACOSX_BUNDLE)
endif()
create_executable(TARGET qfield EXTRA_ARGS ${EXTRA_ARGS})

if(WITH_SPIX)
  create_executable(TARGET qfield_spix)
  target_link_libraries(qfield_spix PRIVATE Spix::Spix)
  target_compile_definitions(qfield_spix PUBLIC WITH_SPIX)
endif()

install(
  TARGETS qfield
  BUNDLE DESTINATION ${QFIELD_BIN_DIR}
  RUNTIME DESTINATION ${QFIELD_BIN_DIR}
  LIBRARY DESTINATION ${QFIELD_LIB_DIR}
  ARCHIVE DESTINATION ${QFIELD_LIB_DIR}
  FRAMEWORK DESTINATION ${QFIELD_FW_SUBDIR}
  PUBLIC_HEADER DESTINATION ${QFIELD_INCLUDE_DIR})

if(IOS)
  # QFIELD_APP_ICON_SUFFIX should be set from command line to either _pr or
  # _beta

  include("${CMAKE_SOURCE_DIR}/cmake/AddQtIosApp.cmake")
  add_qt_ios_app(
    qfield
    NAME "QField"
    VERSION "${APK_VERSION_CODE}"
    SHORT_VERSION "${APK_VERSION_CODE}"
    LONG_VERSION "${APK_VERSION_CODE}"
    ASSET_DIR "${PROJECT_SOURCE_DIR}/platform/ios/Assets.xcassets"
    CATALOG_APPICON "AppIcon${QFIELD_APP_ICON_SUFFIX}"
    LAUNCHSCREEN_STORYBOARD
      "${PROJECT_SOURCE_DIR}/platform/ios/LaunchScreen.storyboard"
    RESOURCE_FILES "${SHARE_DIR}"
    CUSTOM_PLIST "${PROJECT_SOURCE_DIR}/platform/ios/Info.plist.in"
    BUNDLE_IDENTIFIER "ch.opengis.qfield"
    ORIENTATION_PORTRAIT
    ORIENTATION_PORTRAIT_UPDOWN
    ORIENTATION_LANDSCAPE_LEFT
    ORIENTATION_LANDSCAPE_RIGHT
    REQUIRES_FULL_SCREEN
    HIDDEN_STATUS_BAR
    IPA
    UPLOAD_SYMBOL
    VERBOSE)
endif()
