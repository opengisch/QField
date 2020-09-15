FIND_PACKAGE(Qt5QuickCompiler QUIET)
IF(Qt5QuickCompiler_FOUND AND NOT CMAKE_BUILD_TYPE MATCHES Debug AND NOT CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
  QTQUICK_COMPILER_ADD_RESOURCES(RESOURCES ${CMAKE_SOURCE_DIR}/src/qml/qml.qrc)
ELSE()
  SET(RESOURCES ${CMAKE_SOURCE_DIR}/src/qml/qml.qrc)
ENDIF()

SET(QML_IMPORT_PATH "${CMAKE_SOURCE_DIR}/src/qml/imports")

FILE(GLOB_RECURSE QML_SOURCES "${CMAKE_SOURCE_DIR}/src/qml/*.qml")
ADD_CUSTOM_TARGET(qfield_qml SOURCES ${QML_SOURCES})

IF (ANDROID)
  ADD_LIBRARY(qfield SHARED
    main.cpp
    ${RESOURCES}
    ${CMAKE_SOURCE_DIR}/images/images.qrc
    ${CMAKE_SOURCE_DIR}/resources/resources.qrc
    ${CMAKE_SOURCE_DIR}/i18n/translations.qrc
  )
#  ADD_DEPENDENCIES(qfield
#    assets
#  )
ELSE ()
  ADD_EXECUTABLE(qfield
    main.cpp
    ${RESOURCES}
    ${CMAKE_SOURCE_DIR}/images/images.qrc
    ${CMAKE_SOURCE_DIR}/resources/resources.qrc
    ${CMAKE_SOURCE_DIR}/i18n/translations.qrc
  )
ENDIF ()

TARGET_COMPILE_DEFINITIONS(qfield
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
IF (ANDROID)
    TARGET_LINK_LIBRARIES(qfield PRIVATE
      android
      log
    )
ENDIF(ANDROID)

TARGET_LINK_LIBRARIES(qfield PRIVATE
  qfield_core
  ${QGIS_CORE_LIBRARY}
)

SET_TARGET_PROPERTIES(qfield PROPERTIES AUTORCC TRUE)
INSTALL(TARGETS qfield
  BUNDLE DESTINATION ${QFIELD_BIN_DIR}
  RUNTIME DESTINATION ${QFIELD_BIN_DIR}
  LIBRARY DESTINATION ${QFIELD_LIB_DIR}
  ARCHIVE DESTINATION ${QFIELD_LIB_DIR}
  FRAMEWORK DESTINATION ${QFIELD_FW_SUBDIR}
  PUBLIC_HEADER DESTINATION ${QFIELD_INCLUDE_DIR}
)

IF(${CMAKE_SYSTEM_NAME} STREQUAL "iOS")

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
    LONG_VERSION "1.2.3.456"
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

  #get_target_property(QField_LOCATION qfield LOCATION)
  #add_custom_command(TARGET qfield
  #                   POST_BUILD
  #                   COMMAND mkdir -p ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release/qfield.app/Contents/Resources/
  #                   COMMAND cp -r ${QFIELD_PROJ_DIR} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release/qfield.app/Contents/Resources
  #                  )

ENDIF()
