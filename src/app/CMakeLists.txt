find_package(Qt5QuickCompiler QUIET)
if(Qt5QuickCompiler_FOUND AND NOT CMAKE_BUILD_TYPE MATCHES Debug AND NOT CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
  QTQUICK_COMPILER_ADD_RESOURCES(RESOURCES ${CMAKE_SOURCE_DIR}/src/qml/qml.qrc)
else()
  set(RESOURCES ${CMAKE_SOURCE_DIR}/src/qml/qml.qrc)
endif()

set(QML_IMPORT_PATH "${CMAKE_SOURCE_DIR}/src/qml/imports")

file(GLOB_RECURSE QML_SOURCES "${CMAKE_SOURCE_DIR}/src/qml/*.qml")
add_custom_target(qfield_qml SOURCES ${QML_SOURCES})

if (ANDROID)
  add_library(qfield SHARED
    main.cpp
    ${RESOURCES}
    ${CMAKE_SOURCE_DIR}/images/images.qrc
    ${CMAKE_SOURCE_DIR}/resources/resources.qrc
    ${CMAKE_SOURCE_DIR}/i18n/translations.qrc
  )
#  add_dependencies(qfield
#    assets
#  )
else()
  add_executable(qfield
    main.cpp
    ${RESOURCES}
    ${CMAKE_SOURCE_DIR}/images/images.qrc
    ${CMAKE_SOURCE_DIR}/resources/resources.qrc
    ${CMAKE_SOURCE_DIR}/i18n/translations.qrc
  )
endif()

target_compile_definitions(qfield
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
if (ANDROID)
    target_link_libraries(qfield PRIVATE
      android
      log
    )
endif()

target_link_libraries(qfield PRIVATE
  qfield_core
  ${QGIS_CORE_LIBRARY}
)

if (WITH_SPIX)
  target_link_libraries(qfield PRIVATE
    Spix::Spix
  )
  target_compile_definitions(qfield PUBLIC WITH_SPIX)
endif ()

set_target_properties(qfield PROPERTIES AUTORCC TRUE)
install(TARGETS qfield
  BUNDLE DESTINATION ${QFIELD_BIN_DIR}
  RUNTIME DESTINATION ${QFIELD_BIN_DIR}
  LIBRARY DESTINATION ${QFIELD_LIB_DIR}
  ARCHIVE DESTINATION ${QFIELD_LIB_DIR}
  FRAMEWORK DESTINATION ${QFIELD_FW_SUBDIR}
  PUBLIC_HEADER DESTINATION ${QFIELD_INCLUDE_DIR}
)

if(${CMAKE_SYSTEM_NAME} STREQUAL "iOS")

  include("${CMAKE_SOURCE_DIR}/cmake/QtStaticCMake.cmake")
  qt_generate_plugin_import(qfield)
  qt_generate_qml_plugin_import(qfield QML_SRC "${CMAKE_SOURCE_DIR}/src/qml")

  # QFIELD_APP_ICON_SUFFIX should be set from command line to either _pr or _beta

  include("${CMAKE_SOURCE_DIR}/cmake/AddQtIosApp.cmake")
  add_qt_ios_app(qfield
    NAME "QField"
    ASSET_DIR "${PROJECT_SOURCE_DIR}/ios/Assets.xcassets"
    CATALOG_APPICON "AppIcon${QFIELD_APP_ICON_SUFFIX}"
    LAUNCHSCREEN_STORYBOARD "${PROJECT_SOURCE_DIR}/ios/LaunchScreen.storyboard"
    RESOURCE_FILES "${QFIELD_RESOURCE_FILES}"
    CUSTOM_PLIST "${PROJECT_SOURCE_DIR}/ios/Info.plist.in"
    BUNDLE_IDENTIFIER "ch.opengis.qfield"
    VERSION "${CLEAN_APP_VERSION}"
    SHORT_VERSION "${CLEAN_APP_VERSION}"
    LONG_VERSION "${CLEAN_APP_VERSION}.${GIT_REV}"
    ORIENTATION_PORTRAIT
    ORIENTATION_PORTRAIT_UPDOWN
    ORIENTATION_LANDSCAPE_LEFT
    ORIENTATION_LANDSCAPE_RIGHT
    REQUIRES_FULL_SCREEN
    HIDDEN_STATUS_BAR
    IPA
    UPLOAD_SYMBOL
    VERBOSE
  )

endif()
