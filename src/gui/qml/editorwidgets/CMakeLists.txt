set(QFIELD_GUI_EDITORWIDGETS_QML_FILES
    ExternalResourceUtils.js
    QfBinary.qml
    QfCheckBox.qml
    QfColor.qml
    QfDateTime.qml
    QfEditorWidgetBase.qml
    QfExternalResource.qml
    QfRange.qml
    QfRelationEditorBase.qml
    QfRelationReference.qml
    QfTextEdit.qml
    QfUuidGenerator.qml
    QfValueMap.qml
    QfValueRelation.qml
    relationeditors/gallery_relation_editor.qml
    relationeditors/ordered_relation_editor.qml
    relationeditors/relation_editor.qml)

# A plain script, not a component to expose.
set_source_files_properties(ExternalResourceUtils.js PROPERTIES
                            QT_QML_SKIP_QMLDIR_ENTRY TRUE)

qt_add_library(qfield_gui_qml_editorwidgets STATIC)

# Required by qt_add_qml_module, even without C++ sources here.
set_target_properties(qfield_gui_qml_editorwidgets PROPERTIES AUTOMOC TRUE)

# Generate a qmldir for the relation editors subdirectory.
qt_policy(SET QTP0004 NEW)

qt_add_qml_module(
  qfield_gui_qml_editorwidgets
  URI
  org.qfield.gui.editorwidgets
  VERSION
  1.0
  NO_PLUGIN
  RESOURCE_PREFIX
  "/qt/qml"
  QML_FILES
  ${QFIELD_GUI_EDITORWIDGETS_QML_FILES})

qt_import_qml_plugins(qfield_gui_qml_editorwidgets)

# Absolute, because src/app and test copy these from another directory.
set(QFIELD_GUI_EDITORWIDGETS_QML_FILES_ABS "")
foreach(qml_file ${QFIELD_GUI_EDITORWIDGETS_QML_FILES})
  list(APPEND QFIELD_GUI_EDITORWIDGETS_QML_FILES_ABS
       "${CMAKE_CURRENT_SOURCE_DIR}/${qml_file}")
endforeach()
set_property(TARGET qfield_gui_qml_editorwidgets
             PROPERTY QML_FILES ${QFIELD_GUI_EDITORWIDGETS_QML_FILES_ABS})
