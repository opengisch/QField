set(QFIELD_APP_QML_FILES
    About.qml
    BadLayerItem.qml
    Changelog.qml
    DashBoard.qml
    Dummy.qml
    Nyuki.qml
    PluginItem.qml
    PluginManagerSettings.qml
    QFieldCloudDangerZone.qml
    QFieldCloudDeltaHistory.qml
    QFieldCloudLogin.qml
    QFieldCloudPackageLayersFeedback.qml
    QFieldCloudPopup.qml
    QFieldCloudProjectDetails.qml
    QFieldCloudProjectFilter.qml
    QFieldCloudScreen.qml
    QFieldCloudStatusBanner.qml
    QFieldGuide.qml
    QFieldLocalDataPickerScreen.qml
    QFieldSettings.qml
    QFieldSketcher.qml
    QgisMobileapp.qml
    ScreenLocker.qml)

# Type registration stays imperative, in QFieldApp::registerQmlTypes().
qt_add_qml_module(
  qfield_app
  URI
  org.qfield.app
  VERSION
  1.0
  NO_PLUGIN
  NO_GENERATE_QMLTYPES
  RESOURCE_PREFIX
  "/qt/qml"
  QML_FILES
  ${QFIELD_APP_QML_FILES})

qt_import_qml_plugins(qfield_app)

# Absolute, because src/app and test copy these from another directory.
set(QFIELD_APP_QML_FILES_ABS "")
foreach(qml_file ${QFIELD_APP_QML_FILES})
  list(APPEND QFIELD_APP_QML_FILES_ABS
       "${CMAKE_CURRENT_SOURCE_DIR}/${qml_file}")
endforeach()
set_property(TARGET qfield_app PROPERTY QML_FILES ${QFIELD_APP_QML_FILES_ABS})
