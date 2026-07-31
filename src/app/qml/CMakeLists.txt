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
    ScreenLocker.qml
    QgisMobileapp.qml)

qt_add_library(qfield_app_qml STATIC)

# Required by qt_add_qml_module, even without C++ sources here.
set_target_properties(qfield_app_qml PROPERTIES AUTOMOC TRUE)

qt_add_qml_module(
  qfield_app_qml
  URI
  org.qfield.app
  VERSION
  1.0
  NO_PLUGIN
  RESOURCE_PREFIX
  "/qt/qml"
  QML_FILES
  ${QFIELD_APP_QML_FILES})

qt_import_qml_plugins(qfield_app_qml)

# Absolute, because src/app and test copy these from another directory.
set(QFIELD_APP_QML_FILES_ABS "")
foreach(qml_file ${QFIELD_APP_QML_FILES})
  list(APPEND QFIELD_APP_QML_FILES_ABS
       "${CMAKE_CURRENT_SOURCE_DIR}/${qml_file}")
endforeach()
set_property(TARGET qfield_app_qml PROPERTY QML_FILES
                                            ${QFIELD_APP_QML_FILES_ABS})
