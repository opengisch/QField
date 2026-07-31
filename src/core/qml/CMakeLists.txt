add_subdirectory(_3d)

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

qt_add_library(qfield_core_qml STATIC)

# Required by qt_add_qml_module, even without C++ sources here.
set_target_properties(qfield_core_qml PROPERTIES AUTOMOC TRUE)

qt_add_qml_module(
  qfield_core_qml
  URI
  org.qfield.core
  VERSION
  1.0
  NO_PLUGIN
  RESOURCE_PREFIX
  "/qt/qml"
  QML_FILES
  ${QFIELD_CORE_QML_FILES})

qt_import_qml_plugins(qfield_core_qml)

# Absolute, because src/app and test copy these from another directory.
set(QFIELD_CORE_QML_FILES_ABS "")
foreach(qml_file ${QFIELD_CORE_QML_FILES})
  list(APPEND QFIELD_CORE_QML_FILES_ABS
       "${CMAKE_CURRENT_SOURCE_DIR}/${qml_file}")
endforeach()
set_property(TARGET qfield_core_qml PROPERTY QML_FILES
                                             ${QFIELD_CORE_QML_FILES_ABS})
