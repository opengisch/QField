set(QFIELD_3D_QML_FILES
    QfFeatureListSelectionHighlight3D.qml QfMapCanvas3D.qml QfRubberband3D.qml
    QfTerrainMesh.qml QfTouchCameraController.qml)

# Type registration stays imperative, in QField3D::registerQmlTypes().
qt_add_qml_module(
  qfield_3d
  URI
  org.qfield._3d
  VERSION
  1.0
  NO_PLUGIN
  NO_GENERATE_QMLTYPES
  RESOURCE_PREFIX
  "/qt/qml"
  OUTPUT_TARGETS
  qfield_3d_qml_output_targets
  QML_FILES
  ${QFIELD_3D_QML_FILES})

set(QFIELD_3D_QML_OUTPUT_TARGETS
    ${qfield_3d_qml_output_targets}
    PARENT_SCOPE)

qt_import_qml_plugins(qfield_3d)

# Absolute, because src/app and test copy these from another directory.
set(QFIELD_3D_QML_FILES_ABS "")
foreach(qml_file ${QFIELD_3D_QML_FILES})
  list(APPEND QFIELD_3D_QML_FILES_ABS "${CMAKE_CURRENT_SOURCE_DIR}/${qml_file}")
endforeach()
set_property(TARGET qfield_3d PROPERTY QML_FILES ${QFIELD_3D_QML_FILES_ABS})
