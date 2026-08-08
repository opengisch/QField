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
    theme.cpp
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
    theme.h
    valuemapmodel.h
    valuemapmodelbase.h)

add_library(qfield_gui STATIC ${QFIELD_GUI_SRCS} ${QFIELD_GUI_HDRS})

include(GenerateExportHeader)
generate_export_header(qfield_gui)

target_include_directories(
  qfield_gui SYSTEM PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)

target_include_directories(
  qfield_gui PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/gui>
                    $<INSTALL_INTERFACE:${QFIELD_INCLUDE_SUBDIR}>)

target_compile_features(qfield_gui PUBLIC cxx_std_20)

set_target_properties(qfield_gui PROPERTIES AUTOMOC TRUE)

target_link_libraries(qfield_gui PUBLIC qfield_core Qt::Core Qt::Gui Qt::Quick
                                        Qt::WebView Qt::Widgets)

install(FILES ${QFIELD_GUI_HDRS}
              "${CMAKE_CURRENT_BINARY_DIR}/qfield_gui_export.h"
        DESTINATION ${QFIELD_INCLUDE_DIR})

add_subdirectory(qml)

if(CMAKE_GENERATOR STREQUAL "Xcode")
  set(QFIELD_GUI_QML_OUTPUT_TARGETS "")
endif()

install(
  TARGETS qfield_gui ${QFIELD_GUI_QML_OUTPUT_TARGETS}
  EXPORT qfield-targets
  RUNTIME DESTINATION ${QFIELD_BIN_DIR}
  LIBRARY DESTINATION ${QFIELD_LIB_DIR}
  ARCHIVE DESTINATION ${QFIELD_LIB_DIR}
  OBJECTS DESTINATION ${QFIELD_LIB_DIR})
