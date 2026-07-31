set(QFIELD_GUI_COGOPARAMETERWIDGETS_QML_FILES
    QfCogoParameterWidgetBase.qml angle.qml distance.qml elevation.qml enum.qml
    point.qml)

qt_add_library(qfield_gui_qml_cogoparameterwidgets STATIC)

# Required by qt_add_qml_module, even without C++ sources here.
set_target_properties(qfield_gui_qml_cogoparameterwidgets PROPERTIES AUTOMOC
                                                                     TRUE)

qt_add_qml_module(
  qfield_gui_qml_cogoparameterwidgets
  URI
  org.qfield.gui.cogoparameterwidgets
  VERSION
  1.0
  NO_PLUGIN
  RESOURCE_PREFIX
  "/qt/qml"
  QML_FILES
  ${QFIELD_GUI_COGOPARAMETERWIDGETS_QML_FILES})

qt_import_qml_plugins(qfield_gui_qml_cogoparameterwidgets)

# Absolute, because src/app and test copy these from another directory.
set(QFIELD_GUI_COGOPARAMETERWIDGETS_QML_FILES_ABS "")
foreach(qml_file ${QFIELD_GUI_COGOPARAMETERWIDGETS_QML_FILES})
  list(APPEND QFIELD_GUI_COGOPARAMETERWIDGETS_QML_FILES_ABS
       "${CMAKE_CURRENT_SOURCE_DIR}/${qml_file}")
endforeach()
set_property(
  TARGET qfield_gui_qml_cogoparameterwidgets
  PROPERTY QML_FILES ${QFIELD_GUI_COGOPARAMETERWIDGETS_QML_FILES_ABS})
