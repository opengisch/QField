set(QFIELD_3D_SRCS
    maptoview3d.cpp
    qfield3dqmlregistration.cpp
    quick3dgeometry.cpp
    quick3dgeometryconfiguration.cpp
    quick3dgeometryutils.cpp
    quick3dmaptexturedata.cpp
    quick3drubberbandgeometry.cpp
    quick3dterraingeometry.cpp
    quick3dterrainprovider.cpp)

set(QFIELD_3D_HDRS
    maptoview3d.h
    qfield3dqmlregistration.h
    quick3dgeometry.h
    quick3dgeometryconfiguration.h
    quick3dgeometryutils.h
    quick3dmaptexturedata.h
    quick3drubberbandgeometry.h
    quick3dterraingeometry.h
    quick3dterrainprovider.h)

add_library(qfield_3d STATIC ${QFIELD_3D_SRCS} ${QFIELD_3D_HDRS})

include(GenerateExportHeader)
generate_export_header(qfield_3d)

target_include_directories(qfield_3d SYSTEM PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

target_include_directories(qfield_3d PUBLIC ${CMAKE_SOURCE_DIR}/src/3d)

target_compile_features(qfield_3d PUBLIC cxx_std_20)

set_target_properties(qfield_3d PROPERTIES AUTOMOC TRUE)

target_link_libraries(qfield_3d PUBLIC qfield_core Qt::Core Qt::Gui Qt::Quick
                                       Qt::Quick3D)

set(QFIELD_3D_QML_FILES
    qml/QfFeatureListSelectionHighlight3D.qml qml/QfMapCanvas3D.qml
    qml/QfRubberband3D.qml qml/QfTerrainMesh.qml
    qml/QfTouchCameraController.qml)

# NO_GENERATE_QMLTYPES keeps type registration where it is, in
# QField3D::registerQmlTypes(). Without it the QML_ELEMENT classes here would
# also register themselves and end up registered twice.
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
  QML_FILES
  ${QFIELD_3D_QML_FILES})

qt_import_qml_plugins(qfield_3d)

# Absolute, because src/app and test copy these from another directory.
set(QFIELD_3D_QML_FILES_ABS "")
foreach(qml_file ${QFIELD_3D_QML_FILES})
  list(APPEND QFIELD_3D_QML_FILES_ABS "${CMAKE_CURRENT_SOURCE_DIR}/${qml_file}")
endforeach()
set_property(TARGET qfield_3d PROPERTY QML_FILES ${QFIELD_3D_QML_FILES_ABS})
