set(QFIELD_3D_SRCS
    qfmaptoview3d.cpp
    qfield3dqmlregistration.cpp
    qf3dgeometry.cpp
    qf3dgeometryconfiguration.cpp
    qf3dgeometryutils.cpp
    qf3dmaptexturedata.cpp
    qf3drubberbandgeometry.cpp
    qf3dterraingeometry.cpp
    qf3dterrainprovider.cpp)

set(QFIELD_3D_HDRS
    qfmaptoview3d.h
    qfield3dqmlregistration.h
    qf3dgeometry.h
    qf3dgeometryconfiguration.h
    qf3dgeometryutils.h
    qf3dmaptexturedata.h
    qf3drubberbandgeometry.h
    qf3dterraingeometry.h
    qf3dterrainprovider.h)

add_library(qfield_3d STATIC ${QFIELD_3D_SRCS} ${QFIELD_3D_HDRS})

include(GenerateExportHeader)
generate_export_header(qfield_3d)

target_include_directories(
  qfield_3d SYSTEM PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)

target_include_directories(
  qfield_3d PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/3d>
                   $<INSTALL_INTERFACE:${QFIELD_INCLUDE_SUBDIR}>)

target_compile_features(qfield_3d PUBLIC cxx_std_20)

set_target_properties(qfield_3d PROPERTIES AUTOMOC TRUE)

target_link_libraries(qfield_3d PUBLIC qfield_core Qt::Core Qt::Gui Qt::Quick
                                       Qt::Quick3D)

install(FILES ${QFIELD_3D_HDRS} "${CMAKE_CURRENT_BINARY_DIR}/qfield_3d_export.h"
        DESTINATION ${QFIELD_INCLUDE_DIR})

add_subdirectory(qml)

if(CMAKE_GENERATOR STREQUAL "Xcode")
  set(QFIELD_3D_QML_OUTPUT_TARGETS "")
endif()

install(
  TARGETS qfield_3d ${QFIELD_3D_QML_OUTPUT_TARGETS}
  EXPORT qfield-targets
  RUNTIME DESTINATION ${QFIELD_BIN_DIR}
  LIBRARY DESTINATION ${QFIELD_LIB_DIR}
  ARCHIVE DESTINATION ${QFIELD_LIB_DIR}
  OBJECTS DESTINATION ${QFIELD_LIB_DIR})
