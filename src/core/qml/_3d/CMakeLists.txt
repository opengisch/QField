set(QFIELD_CORE_3D_QML_FILES
    QfFeatureListSelectionHighlight3D.qml QfMapCanvas3D.qml QfRubberband3D.qml
    QfTerrainMesh.qml QfTouchCameraController.qml)

qt_add_library(qfield_core_qml_3d STATIC)

# Required by qt_add_qml_module, even without C++ sources here.
set_target_properties(qfield_core_qml_3d PROPERTIES AUTOMOC TRUE)

qt_add_qml_module(
  qfield_core_qml_3d
  URI
  org.qfield.core._3d
  VERSION
  1.0
  NO_PLUGIN
  RESOURCE_PREFIX
  "/qt/qml"
  QML_FILES
  ${QFIELD_CORE_3D_QML_FILES})

qt_import_qml_plugins(qfield_core_qml_3d)

# Absolute, because src/app and test copy these from another directory.
set(QFIELD_CORE_3D_QML_FILES_ABS "")
foreach(qml_file ${QFIELD_CORE_3D_QML_FILES})
  list(APPEND QFIELD_CORE_3D_QML_FILES_ABS
       "${CMAKE_CURRENT_SOURCE_DIR}/${qml_file}")
endforeach()
set_property(TARGET qfield_core_qml_3d PROPERTY QML_FILES
                                                ${QFIELD_CORE_3D_QML_FILES_ABS})
