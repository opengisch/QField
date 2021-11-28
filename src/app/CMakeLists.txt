find_package(Qt5QuickCompiler QUIET)
if(Qt5QuickCompiler_FOUND
   AND NOT CMAKE_BUILD_TYPE MATCHES Debug
   AND NOT CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
  qtquick_compiler_add_resources(RESOURCES ${CMAKE_SOURCE_DIR}/src/qml/qml.qrc)
else()
  set(RESOURCES ${CMAKE_SOURCE_DIR}/src/qml/qml.qrc)
endif()

if(MSVC)
  configure_file("${CMAKE_SOURCE_DIR}/platform/windows/appicon.rc.in"
                 "${CMAKE_CURRENT_BINARY_DIR}/appicon.rc")
  set(ICON_SRC "${CMAKE_CURRENT_BINARY_DIR}/appicon.rc")
endif()

set(QML_IMPORT_PATH "${CMAKE_SOURCE_DIR}/src/qml/imports")

file(GLOB_RECURSE QML_SOURCES "${CMAKE_SOURCE_DIR}/src/qml/*.qml")
add_custom_target(qfield_qml SOURCES ${QML_SOURCES})

# lrelease .ts files
find_package(Qt5 COMPONENTS LinguistTools)

file(GLOB TS_FILES ${CMAKE_SOURCE_DIR}/i18n/*.ts)
set_source_files_properties(${TS_FILES} PROPERTIES OUTPUT_LOCATION
                                                   "${CMAKE_BINARY_DIR}")
qt5_add_translation(QM_FILES ${TS_FILES})

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

if(ANDROID)
  add_library(
    qfield SHARED
    main.cpp ${RESOURCES} ${CMAKE_SOURCE_DIR}/images/images.qrc
    ${CMAKE_BINARY_DIR}/images/logo.qrc
    ${CMAKE_SOURCE_DIR}/resources/resources.qrc ${TRANSLATIONS_QRC})

  # Strip binary for release builds
  add_custom_command(
    TARGET qfield
    POST_BUILD
    COMMAND ${CMAKE_STRIP} $<TARGET_FILE:qfield>)
  # add_dependencies(qfield assets )
else()
  add_executable(
    qfield
    main.cpp
    ${RESOURCES}
    ${CMAKE_SOURCE_DIR}/images/images.qrc
    ${CMAKE_BINARY_DIR}/images/logo.qrc
    ${CMAKE_SOURCE_DIR}/resources/resources.qrc
    ${ICON_SRC}
    ${TRANSLATIONS_QRC})
endif()

target_compile_definitions(
  qfield PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

if(ANDROID)
  target_link_libraries(qfield PRIVATE android log)
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "iOS")
  # debug symbols https://stackoverflow.com/a/49714128/1548052
  target_compile_options(qfield PRIVATE $<$<CXX_COMPILER_ID:Clang>:-g>)
endif()

target_link_libraries(qfield PRIVATE qfield_core ${QGIS_CORE_LIBRARY})

target_link_libraries(qfield PRIVATE qfield_core)

if(WITH_SPIX)
  target_link_libraries(qfield PRIVATE Spix::Spix)
  target_compile_definitions(qfield PUBLIC WITH_SPIX)
endif()

set_target_properties(qfield PROPERTIES AUTORCC TRUE)
install(
  TARGETS qfield
  BUNDLE DESTINATION ${QFIELD_BIN_DIR}
  RUNTIME DESTINATION ${QFIELD_BIN_DIR}
  LIBRARY DESTINATION ${QFIELD_LIB_DIR}
  ARCHIVE DESTINATION ${QFIELD_LIB_DIR}
  FRAMEWORK DESTINATION ${QFIELD_FW_SUBDIR}
  PUBLIC_HEADER DESTINATION ${QFIELD_INCLUDE_DIR})

if(${CMAKE_SYSTEM_NAME} STREQUAL "iOS")

  include("${CMAKE_SOURCE_DIR}/cmake/QtStaticCMake.cmake")
  qt_generate_plugin_import(qfield)
  qt_generate_qml_plugin_import(qfield QML_SRC "${CMAKE_SOURCE_DIR}/src/qml")

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
    RESOURCE_FILES "${QFIELD_RESOURCE_FILES}"
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
