add_subdirectory(cogoparameterwidgets)
add_subdirectory(editorwidgets)
add_subdirectory(geometryeditors)
add_subdirectory(processingparameterwidgets)

set(QFIELD_GUI_QML_FILES
    QfBluetoothDeviceChooser.qml
    QfBookmarkList.qml
    QfBookmarkProperties.qml
    QfBrowserPanel.qml
    QfBusyOverlay.qml
    QfCodeReader.qml
    QfCogoOperationSettings.qml
    QfCogoOperations.qml
    QfConfirmationToolbar.qml
    QfDigitizingToolbar.qml
    QfEgenioussDeviceChooser.qml
    QfElevationProfile.qml
    QfEmbeddedFeatureForm.qml
    QfFeatureForm.qml
    QfFeatureListForm.qml
    QfFileDeviceChooser.qml
    QfGeometryEditorsToolbar.qml
    QfImageDial.qml
    QfInformationDrawer.qml
    QfInformationPopup.qml
    QfLayerLoginDialog.qml
    QfLayerTreeItemProperties.qml
    QfLegend.qml
    QfLocatorItem.qml
    QfLocatorSettings.qml
    QfMessageLog.qml
    QfNavigationBar.qml
    QfNavigationInformationView.qml
    QfOverlayFeatureFormDrawer.qml
    QfPositioningDeviceSettings.qml
    QfPositioningInformationView.qml
    QfPositioningNtripSettings.qml
    QfPositioningPreciseView.qml
    QfProcessingAlgorithmForm.qml
    QfProcessingAlgorithmsList.qml
    QfProjectCreationScreen.qml
    QfQFieldAudioRecorder.qml
    QfQFieldCamera.qml
    QfRelationCombobox.qml
    QfSensorInformationView.qml
    QfSerialPortDeviceChooser.qml
    QfTcpDeviceChooser.qml
    QfTemporalProperties.qml
    QfToast.qml
    QfTrackerFeatureForm.qml
    QfTrackerSettings.qml
    QfUdpDeviceChooser.qml
    QfVariableEditor.qml
    QfWelcomeScreen.qml)

qt_add_library(qfield_gui_qml STATIC)

# Required by qt_add_qml_module, even without C++ sources here.
set_target_properties(qfield_gui_qml PROPERTIES AUTOMOC TRUE)

qt_add_qml_module(
  qfield_gui_qml
  URI
  org.qfield.gui
  VERSION
  1.0
  NO_PLUGIN
  RESOURCE_PREFIX
  "/qt/qml"
  QML_FILES
  ${QFIELD_GUI_QML_FILES})

qt_import_qml_plugins(qfield_gui_qml)

# Absolute, because src/app and test copy these from another directory.
set(QFIELD_GUI_QML_FILES_ABS "")
foreach(qml_file ${QFIELD_GUI_QML_FILES})
  list(APPEND QFIELD_GUI_QML_FILES_ABS
       "${CMAKE_CURRENT_SOURCE_DIR}/${qml_file}")
endforeach()
set_property(TARGET qfield_gui_qml PROPERTY QML_FILES
                                            ${QFIELD_GUI_QML_FILES_ABS})
