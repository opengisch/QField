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
    qml/BluetoothDeviceChooser.qml
    qml/BookmarkList.qml
    qml/BookmarkProperties.qml
    qml/BrowserPanel.qml
    qml/BusyOverlay.qml
    qml/CodeReader.qml
    qml/CogoOperationSettings.qml
    qml/CogoOperations.qml
    qml/ConfirmationToolbar.qml
    qml/DigitizingToolbar.qml
    qml/EgenioussDeviceChooser.qml
    qml/ElevationProfile.qml
    qml/EmbeddedFeatureForm.qml
    qml/FeatureForm.qml
    qml/FeatureListForm.qml
    qml/FileDeviceChooser.qml
    qml/GeometryEditorsToolbar.qml
    qml/ImageDial.qml
    qml/InformationDrawer.qml
    qml/InformationPopup.qml
    qml/LayerLoginDialog.qml
    qml/LayerTreeItemProperties.qml
    qml/Legend.qml
    qml/LocatorItem.qml
    qml/LocatorSettings.qml
    qml/MessageLog.qml
    qml/NavigationBar.qml
    qml/NavigationInformationView.qml
    qml/OverlayFeatureFormDrawer.qml
    qml/PositioningDeviceSettings.qml
    qml/PositioningInformationView.qml
    qml/PositioningNtripSettings.qml
    qml/PositioningPreciseView.qml
    qml/ProcessingAlgorithmForm.qml
    qml/ProcessingAlgorithmsList.qml
    qml/ProjectCreationScreen.qml
    qml/QFieldAudioRecorder.qml
    qml/QFieldCamera.qml
    qml/RelationCombobox.qml
    qml/SensorInformationView.qml
    qml/SerialPortDeviceChooser.qml
    qml/TcpDeviceChooser.qml
    qml/TemporalProperties.qml
    qml/Toast.qml
    qml/TrackerFeatureForm.qml
    qml/TrackerSettings.qml
    qml/UdpDeviceChooser.qml
    qml/VariableEditor.qml
    qml/WelcomeScreen.qml
    qml/cogoparameterwidgets/CogoParameterWidgetBase.qml
    qml/cogoparameterwidgets/angle.qml
    qml/cogoparameterwidgets/distance.qml
    qml/cogoparameterwidgets/elevation.qml
    qml/cogoparameterwidgets/enum.qml
    qml/cogoparameterwidgets/point.qml
    qml/editorwidgets/Binary.qml
    qml/editorwidgets/CheckBox.qml
    qml/editorwidgets/Color.qml
    qml/editorwidgets/DateTime.qml
    qml/editorwidgets/EditorWidgetBase.qml
    qml/editorwidgets/ExternalResource.qml
    qml/editorwidgets/ExternalResourceUtils.js
    qml/editorwidgets/Range.qml
    qml/editorwidgets/RelationEditorBase.qml
    qml/editorwidgets/RelationReference.qml
    qml/editorwidgets/TextEdit.qml
    qml/editorwidgets/UuidGenerator.qml
    qml/editorwidgets/ValueMap.qml
    qml/editorwidgets/ValueRelation.qml
    qml/editorwidgets/relationeditors/gallery_relation_editor.qml
    qml/editorwidgets/relationeditors/ordered_relation_editor.qml
    qml/editorwidgets/relationeditors/relation_editor.qml
    qml/geometryeditors/Erase.qml
    qml/geometryeditors/FillRing.qml
    qml/geometryeditors/GeometryEditorBase.qml
    qml/geometryeditors/Reshape.qml
    qml/geometryeditors/SplitFeature.qml
    qml/geometryeditors/VertexEditor.qml
    qml/processingparameterwidgets/ProcessingParameterWidgetBase.qml
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
