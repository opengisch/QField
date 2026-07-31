set(QFIELD_GUI_SRCS
    attributeformmodel.cpp
    attributeformmodelbase.cpp
    barcodeimageprovider.cpp
    drawingtemplatemodel.cpp
    expressionevaluator.cpp
    expressionvariablemodel.cpp
    featurechecklistmodel.cpp
    focusstack.cpp
    geometryeditorsmodel.cpp
    legendimageprovider.cpp
    localfilesimageprovider.cpp
    localfilesmodel.cpp
    messagelogmodel.cpp
    orderedrelationmodel.cpp
    parameterizedimage.cpp
    permissions.cpp
    printlayoutlistmodel.cpp
    projectsimageprovider.cpp
    qfieldguiqmlregistration.cpp
    recentprojectlistmodel.cpp
    referencingfeaturelistmodel.cpp
    sensorlistmodel.cpp
    valuemapmodel.cpp
    valuemapmodelbase.cpp)

set(QFIELD_GUI_HDRS
    attributeformmodel.h
    attributeformmodelbase.h
    barcodeimageprovider.h
    drawingtemplatemodel.h
    expressionevaluator.h
    expressionvariablemodel.h
    featurechecklistmodel.h
    focusstack.h
    geometryeditorsmodel.h
    legendimageprovider.h
    localfilesimageprovider.h
    localfilesmodel.h
    messagelogmodel.h
    orderedrelationmodel.h
    parameterizedimage.h
    permissions.h
    printlayoutlistmodel.h
    projectsimageprovider.h
    qfieldguiqmlregistration.h
    recentprojectlistmodel.h
    referencingfeaturelistmodel.h
    sensorlistmodel.h
    valuemapmodel.h
    valuemapmodelbase.h)

add_library(qfield_gui STATIC ${QFIELD_GUI_SRCS} ${QFIELD_GUI_HDRS})

include(GenerateExportHeader)
generate_export_header(qfield_gui)

target_include_directories(qfield_gui SYSTEM PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

target_include_directories(qfield_gui PUBLIC ${CMAKE_SOURCE_DIR}/src/gui)

target_compile_features(qfield_gui PUBLIC cxx_std_20)

set_target_properties(qfield_gui PROPERTIES AUTOMOC TRUE)

target_link_libraries(qfield_gui PUBLIC qfield_core Qt::Core Qt::Gui Qt::Quick
                                        Qt::Widgets)

# QML owned by this library. The /qml prefix keeps resource paths stable.
set(QFIELD_GUI_QML_FILES
    qml/QfBluetoothDeviceChooser.qml
    qml/QfBookmarkList.qml
    qml/QfBookmarkProperties.qml
    qml/QfBrowserPanel.qml
    qml/QfBusyOverlay.qml
    qml/QfCodeReader.qml
    qml/QfCogoOperationSettings.qml
    qml/QfCogoOperations.qml
    qml/QfConfirmationToolbar.qml
    qml/QfDigitizingToolbar.qml
    qml/QfEgenioussDeviceChooser.qml
    qml/QfElevationProfile.qml
    qml/QfEmbeddedFeatureForm.qml
    qml/QfFeatureForm.qml
    qml/QfFeatureListForm.qml
    qml/QfFileDeviceChooser.qml
    qml/QfGeometryEditorsToolbar.qml
    qml/QfImageDial.qml
    qml/QfInformationDrawer.qml
    qml/QfInformationPopup.qml
    qml/QfLayerLoginDialog.qml
    qml/QfLayerTreeItemProperties.qml
    qml/QfLegend.qml
    qml/QfLocatorItem.qml
    qml/QfLocatorSettings.qml
    qml/QfMessageLog.qml
    qml/QfNavigationBar.qml
    qml/QfNavigationInformationView.qml
    qml/QfOverlayFeatureFormDrawer.qml
    qml/QfPositioningDeviceSettings.qml
    qml/QfPositioningInformationView.qml
    qml/QfPositioningNtripSettings.qml
    qml/QfPositioningPreciseView.qml
    qml/QfProcessingAlgorithmForm.qml
    qml/QfProcessingAlgorithmsList.qml
    qml/QfProjectCreationScreen.qml
    qml/QfQFieldAudioRecorder.qml
    qml/QfQFieldCamera.qml
    qml/QfRelationCombobox.qml
    qml/QfSensorInformationView.qml
    qml/QfSerialPortDeviceChooser.qml
    qml/QfTcpDeviceChooser.qml
    qml/QfTemporalProperties.qml
    qml/QfToast.qml
    qml/QfTrackerFeatureForm.qml
    qml/QfTrackerSettings.qml
    qml/QfUdpDeviceChooser.qml
    qml/QfVariableEditor.qml
    qml/QfWelcomeScreen.qml
    qml/cogoparameterwidgets/QfCogoParameterWidgetBase.qml
    qml/cogoparameterwidgets/angle.qml
    qml/cogoparameterwidgets/distance.qml
    qml/cogoparameterwidgets/elevation.qml
    qml/cogoparameterwidgets/enum.qml
    qml/cogoparameterwidgets/point.qml
    qml/editorwidgets/ExternalResourceUtils.js
    qml/editorwidgets/QfBinary.qml
    qml/editorwidgets/QfCheckBox.qml
    qml/editorwidgets/QfColor.qml
    qml/editorwidgets/QfDateTime.qml
    qml/editorwidgets/QfEditorWidgetBase.qml
    qml/editorwidgets/QfExternalResource.qml
    qml/editorwidgets/QfRange.qml
    qml/editorwidgets/QfRelationEditorBase.qml
    qml/editorwidgets/QfRelationReference.qml
    qml/editorwidgets/QfTextEdit.qml
    qml/editorwidgets/QfUuidGenerator.qml
    qml/editorwidgets/QfValueMap.qml
    qml/editorwidgets/QfValueRelation.qml
    qml/editorwidgets/relationeditors/gallery_relation_editor.qml
    qml/editorwidgets/relationeditors/ordered_relation_editor.qml
    qml/editorwidgets/relationeditors/relation_editor.qml
    qml/geometryeditors/QfErase.qml
    qml/geometryeditors/QfFillRing.qml
    qml/geometryeditors/QfGeometryEditorBase.qml
    qml/geometryeditors/QfReshape.qml
    qml/geometryeditors/QfSplitFeature.qml
    qml/geometryeditors/QfVertexEditor.qml
    qml/processingparameterwidgets/QfProcessingParameterWidgetBase.qml
    qml/processingparameterwidgets/area.qml
    qml/processingparameterwidgets/boolean.qml
    qml/processingparameterwidgets/distance.qml
    qml/processingparameterwidgets/enum.qml
    qml/processingparameterwidgets/number.qml
    qml/processingparameterwidgets/source.qml)

qt_add_resources(
  qfield_gui
  "gui_qml"
  PREFIX
  "/qml"
  BASE
  "${CMAKE_CURRENT_SOURCE_DIR}/qml"
  FILES
  ${QFIELD_GUI_QML_FILES})

# The import scanner needs absolute paths (QTBUG-80847).
set(QFIELD_GUI_QML_FILES_ABS "")
foreach(qml_file ${QFIELD_GUI_QML_FILES})
  list(APPEND QFIELD_GUI_QML_FILES_ABS
       "${CMAKE_CURRENT_SOURCE_DIR}/${qml_file}")
endforeach()
set_property(TARGET qfield_gui PROPERTY QML_FILES ${QFIELD_GUI_QML_FILES_ABS})

qt_import_qml_plugins(qfield_gui)
