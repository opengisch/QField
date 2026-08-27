set(QFIELD_APP_QML_FILES
    QfAbout.qml
    QfAppSettings.qml
    QfBadLayerItem.qml
    QfChangelog.qml
    QfCoordinateLocator.qml
    QfCloudDangerZone.qml
    QfCloudDeltaHistory.qml
    QfCloudLogin.qml
    QfCloudPackageLayersFeedback.qml
    QfCloudPendingChanges.qml
    QfCloudPopup.qml
    QfCloudProjectDetails.qml
    QfCloudProjectFilter.qml
    QfCloudScreen.qml
    QfCloudStatusBanner.qml
    QfDashBoard.qml
    QfDummy.qml
    QfFeatureListSelectionHighlight.qml
    QfGuide.qml
    QfLocalDataPickerScreen.qml
    QfMeasuringTool.qml
    QfNyuki.qml
    QfPluginItem.qml
    QfPluginManagerSettings.qml
    QfPositioningSettings.qml
    QfScreenLocker.qml
    QfSketcher.qml
    QfTrackerSettings.qml
    QfWelcomeScreen.qml
    QgisMobileapp.qml)

# Type registration stays imperative, in QfApp::registerQmlTypes().
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
