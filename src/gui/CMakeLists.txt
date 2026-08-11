set(QFIELD_GUI_SRCS
    qfattributeformmodel.cpp
    qfattributeformmodelbase.cpp
    qfbarcodeimageprovider.cpp
    qfdrawingtemplatemodel.cpp
    qfexpressionevaluator.cpp
    qfexpressionvariablemodel.cpp
    qffeaturechecklistmodel.cpp
    qffocusstack.cpp
    qfgeometryeditorsmodel.cpp
    qflegendimageprovider.cpp
    qflocalfilesimageprovider.cpp
    qflocalfilesmodel.cpp
    qfmessagelogmodel.cpp
    qforderedrelationmodel.cpp
    qfparameterizedimage.cpp
    qfpermissions.cpp
    qfprintlayoutlistmodel.cpp
    qfprojectsimageprovider.cpp
    qfieldguiqmlregistration.cpp
    qfrecentprojectlistmodel.cpp
    qfreferencingfeaturelistmodel.cpp
    qfsensorlistmodel.cpp
    qftheme.cpp
    qfvaluemapmodel.cpp
    qfvaluemapmodelbase.cpp)

set(QFIELD_GUI_HDRS
    qfattributeformmodel.h
    qfattributeformmodelbase.h
    qfbarcodeimageprovider.h
    qfdrawingtemplatemodel.h
    qfexpressionevaluator.h
    qfexpressionvariablemodel.h
    qffeaturechecklistmodel.h
    qffocusstack.h
    qfgeometryeditorsmodel.h
    qflegendimageprovider.h
    qflocalfilesimageprovider.h
    qflocalfilesmodel.h
    qfmessagelogmodel.h
    qforderedrelationmodel.h
    qfparameterizedimage.h
    qfpermissions.h
    qfprintlayoutlistmodel.h
    qfprojectsimageprovider.h
    qfieldguiqmlregistration.h
    qfrecentprojectlistmodel.h
    qfreferencingfeaturelistmodel.h
    qfsensorlistmodel.h
    qftheme.h
    qfvaluemapmodel.h
    qfvaluemapmodelbase.h)

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
