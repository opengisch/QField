

# File QfGeometryEditorsToolbar.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfGeometryEditorsToolbar.qml**](QfGeometryEditorsToolbar_8qml.md)

[Go to the documentation of this file](QfGeometryEditorsToolbar_8qml.md)


```C++
import QtQuick
import QtQml.Models
import org.qgis
import org.qfield.core
import org.qfield.gui

QfVisibilityFadingRow {
  id: geometryEditorsToolbar

  property QfFeatureModel featureModel //<! the feature which has its geometry being edited
  property MapSettings mapSettings
  property QfRubberbandModel editorRubberbandModel //<! an additional QfRubberband model for the tools (when drawing lines in split or add ring tools)
  property QfGeometryRenderer editorRenderer
  property bool screenHovering: false //<! if the stylus pen is used, one should not use the add button

  property string image: QfTheme.getThemeVectorIcon("ic_edit_geometry_white_24dp")

  signal editorChanged
  signal requestJumpToPoint(var center, real scale, bool handleMargins)

  QfGeometryEditorsModel {
    id: editors
  }

  Component.onCompleted: {
    editors.addEditor(qsTr("Vertex Tool"), "ic_vertex_tool_white_24dp", "geometryeditors/QfGeometryEditorVertexEditor.qml");
    editors.addEditor(qsTr("Split Tool"), "ic_split_tool_white_24dp", "geometryeditors/QfGeometryEditorSplitFeature.qml", QfGeometryEditorsModelSingleton.Line | QfGeometryEditorsModelSingleton.Polygon);
    editors.addEditor(qsTr("Reshape Tool"), "ic_reshape_tool_white_24dp", "geometryeditors/QfGeometryEditorReshape.qml", QfGeometryEditorsModelSingleton.Line | QfGeometryEditorsModelSingleton.Polygon);
    editors.addEditor(qsTr("Erase Tool"), "ic_erase_tool_white_24dp", "geometryeditors/QfGeometryEditorErase.qml", QfGeometryEditorsModelSingleton.Line | QfGeometryEditorsModelSingleton.Polygon);
    editors.addEditor(qsTr("Fill Ring Tool"), "ic_ring_tool_white_24dp", "geometryeditors/QfGeometryEditorFillRing.qml", QfGeometryEditorsModelSingleton.Polygon);
  }

  function init() {
    var lastUsed = settings.value("/QField/GeometryEditorLastUsed", -1);
    if (lastUsed >= 0 && lastUsed < editors.rowCount()) {
      selectorRow.stateVisible = false;
      var toolbarQml = editors.data(editors.index(lastUsed, 0), QfGeometryEditorsModelSingleton.ToolbarRole);
      var iconPath = editors.data(editors.index(lastUsed, 0), QfGeometryEditorsModelSingleton.IconPathRole);
      var name = editors.data(editors.index(lastUsed, 0), QfGeometryEditorsModelSingleton.NameRole);
      geometryEditorsToolbar.image = QfTheme.getThemeVectorIcon(iconPath);
      toolbarRow.load(toolbarQml, iconPath, name);
    }
  }

  function cancelEditors() {
    if (toolbarRow.item) {
      toolbarRow.item.cancel();
    }
    featureModel.vertexModel.clear();
  }

  // returns true if handled
  function canvasClicked(point, type) {
    if (toolbarRow.item && toolbarRow.visible)
      return toolbarRow.item.canvasClicked(point, type);
    else
      return false;
  }

  // returns true if handled
  function canvasLongPressed(point, type) {
    if (toolbarRow.item && toolbarRow.visible)
      return toolbarRow.item.canvasLongPressed(point, type);
    else
      return false;
  }

  // returns true if handled or not defined
  function canvasFreehandBegin() {
    if (toolbarRow.item && toolbarRow.visible)
      return toolbarRow.item.canvasFreehandBegin ? toolbarRow.item.canvasFreehandBegin() : true;
    else
      return false;
  }

  // returns true if handled or not defined
  function canvasFreehandEnd() {
    if (toolbarRow.item && toolbarRow.visible)
      return toolbarRow.item.canvasFreehandEnd ? toolbarRow.item.canvasFreehandEnd() : true;
    else
      return false;
  }

  QfVisibilityFadingRow {
    id: selectorRow
    stateVisible: true

    Repeater {
      model: editors
      delegate: QfToolButton {
        round: true
        bgcolor: QfTheme.mainColor
        iconSource: QfTheme.getThemeVectorIcon(iconPath)
        visible: QfGeometryEditorsModelSingleton.supportsGeometry(featureModel.vertexModel.geometry, supportedGeometries)
        onClicked: {
          // close current tool
          if (toolbarRow.item) {
            toolbarRow.item.cancel();
          }
          selectorRow.stateVisible = false;
          geometryEditorsToolbar.image = QfTheme.getThemeVectorIcon(iconPath);
          toolbarRow.load(toolbar, iconPath, name);
          settings.setValue("/QField/GeometryEditorLastUsed", index);
        }
      }
    }
  }

  Loader {
    id: toolbarRow

    width: item && item.stateVisible ? item.implicitWidth : 0

    function load(qmlSource, iconPath, name) {
      source = qmlSource;
      item.init(geometryEditorsToolbar.featureModel, geometryEditorsToolbar.mapSettings, geometryEditorsToolbar.editorRubberbandModel, geometryEditorsToolbar.editorRenderer);
      if (toolbarRow.item.screenHovering !== undefined) {
        toolbarRow.item.screenHovering = geometryEditorsToolbar.screenHovering;
      }
      if (toolbarRow.item.vertexRubberbandVisible !== undefined) {
        vertexRubberband.isVisible = toolbarRow.item.vertexRubberbandVisible;
      } else {
        vertexRubberband.isVisible = false;
      }
      toolbarRow.item.stateVisible = true;
      displayToast(name);
    }

    onSourceChanged: {
      geometryEditorsToolbar.editorChanged();
    }
  }

  onScreenHoveringChanged: {
    if (toolbarRow.item && toolbarRow.item.screenHovering !== undefined)
      toolbarRow.item.screenHovering = geometryEditorsToolbar.screenHovering;
  }

  Connections {
    target: toolbarRow.item

    function onFinished() {
      featureModel.vertexModel.clear();
    }

    function requestJumpToPoint(center, scale, handleMargins) {
      geometryEditorsToolbar.requestJumpToPoint(center, scale, handleMargins);
    }
  }

  QfToolButton {
    id: activeToolButton
    iconSource: QfTheme.getThemeVectorIcon("ic_ellipsis_black_24dp")
    iconColor: QfTheme.toolButtonColor
    round: true
    visible: !selectorRow.stateVisible && !(toolbarRow.item && toolbarRow.item.stateVisible && toolbarRow.item.blocking)
    bgcolor: QfTheme.mainColor
    onClicked: {
      toolbarRow.item.cancel();
      toolbarRow.source = '';
      vertexRubberband.isVisible = false;
      selectorRow.stateVisible = true;
      image = QfTheme.getThemeVectorIcon("ic_edit_geometry_white_24dp");
      settings.setValue("/QField/GeometryEditorLastUsed", -1);
    }
  }
}
```


