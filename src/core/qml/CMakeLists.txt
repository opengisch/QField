set(QFIELD_CORE_QML_FILES
    QfBookmarkHighlight.qml
    QfBookmarkRenderer.qml
    QfCogoOperationPreview.qml
    QfCoordinateLocator.qml
    QfDropShadow.qml
    QfFeatureListSelectionHighlight.qml
    QfGeometryHighlighter.qml
    QfGeometryRenderer.qml
    QfGridRenderer.qml
    QfKineticHandler.qml
    QfLinePolygon.qml
    QfLocationMarker.qml
    QfMapCanvas.qml
    QfMapCanvasPointHandler.qml
    QfMeasuringTool.qml
    QfNavigationHighlight.qml
    QfNavigationRenderer.qml
    QfPositioningSettings.qml
    QfProcessingAlgorithmPreview.qml
    QfRubberband.qml
    QfScaleBar.qml
    QfTrackingSession.qml
    QfVertexRubberband.qml)

# Type registration stays imperative, in QFieldCore::registerQmlTypes().
qt_add_qml_module(
  qfield_core
  URI
  org.qfield.core
  VERSION
  1.0
  NO_PLUGIN
  NO_GENERATE_QMLTYPES
  RESOURCE_PREFIX
  "/qt/qml"
  OUTPUT_TARGETS
  qfield_core_qml_output_targets
  QML_FILES
  ${QFIELD_CORE_QML_FILES})

set(QFIELD_CORE_QML_OUTPUT_TARGETS
    ${qfield_core_qml_output_targets}
    PARENT_SCOPE)

qt_import_qml_plugins(qfield_core)

# Absolute, because src/app and test copy these from another directory.
set(QFIELD_CORE_QML_FILES_ABS "")
foreach(qml_file ${QFIELD_CORE_QML_FILES})
  list(APPEND QFIELD_CORE_QML_FILES_ABS
       "${CMAKE_CURRENT_SOURCE_DIR}/${qml_file}")
endforeach()
set_property(TARGET qfield_core PROPERTY QML_FILES ${QFIELD_CORE_QML_FILES_ABS})
