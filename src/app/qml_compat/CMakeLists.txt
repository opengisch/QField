set(QFIELD_COMPAT_QML_FILES
    BookmarkHighlight.qml
    BookmarkRenderer.qml
    CoordinateLocator.qml
    FeatureListSelectionHighlight.qml
    GeometryHighlighter.qml
    GeometryRenderer.qml
    GridRenderer.qml
    KineticHandler.qml
    LinePolygon.qml
    LocationMarker.qml
    MapCanvas.qml
    MapCanvasPointHandler.qml
    NavigationHighlight.qml
    NavigationRenderer.qml
    Rubberband.qml
    ScaleBar.qml
    TrackingSession.qml
    VertexRubberband.qml)

qt_add_library(qfield_qml_compat STATIC)

set_target_properties(qfield_qml_compat PROPERTIES AUTOMOC TRUE)

qt_add_qml_module(
  qfield_qml_compat
  URI
  org.qfield
  VERSION
  1.0
  NO_PLUGIN
  NO_GENERATE_QMLTYPES
  RESOURCE_PREFIX
  "/qt/qml"
  QML_FILES
  ${QFIELD_COMPAT_QML_FILES})
