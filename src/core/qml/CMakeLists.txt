set(QFIELD_CORE_QML_FILES
    QfGridRenderer.qml
    QfKineticHandler.qml
    QfLinePolygon.qml
    QfMapCanvas.qml
    QfMapCanvasPointHandler.qml
    QfRubberband.qml
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
  QML_FILES
  ${QFIELD_CORE_QML_FILES})

qt_import_qml_plugins(qfield_core)

# Absolute, because src/app and test copy these from another directory.
set(QFIELD_CORE_QML_FILES_ABS "")
foreach(qml_file ${QFIELD_CORE_QML_FILES})
  list(APPEND QFIELD_CORE_QML_FILES_ABS
       "${CMAKE_CURRENT_SOURCE_DIR}/${qml_file}")
endforeach()
set_property(TARGET qfield_core PROPERTY QML_FILES ${QFIELD_CORE_QML_FILES_ABS})
