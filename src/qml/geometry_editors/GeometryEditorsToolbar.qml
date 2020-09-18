import QtQuick 2.12
import QtQml.Models 2.12

import org.qgis 1.0
import org.qfield 1.0
import Theme 1.0
import ".."

/**
This contains several geometry editing tools
A tool must subclass VisibilityFadingRow
And contains following functions:
  * function init(featureModel, mapSettings, editorRubberbandModel)
  * function cancel()
The following signal:
  * signal finished()
It can optionally implement properties:
  * blocking (bool) which prevents from switching tools
  * vertexRubberbandVisible (bool) to show the vertex rubberband (false by default)
It can optionally implement properties which will be bind:
  * screenHovering determines if a pen is currently hovering the screen
It can optionally implement the functions:
  * canvasClicked(point)
  * canvasLongPressed(point)
  These functions must return true if they catch the event
*/

VisibilityFadingRow {
  id: geometryEditorsToolbar

  property FeatureModel featureModel //<! the feature which has its geometry being edited
  property MapSettings mapSettings
  property RubberbandModel editorRubberbandModel //<! an additional Rubberband model for the tools (when drawing lines in split or addRing tools)
  property bool screenHovering: false //<! if the stylus pen is used, one should not use the add button

  spacing: 4

  signal editorChanged

  GeometryEditorsModel {
    id: editors
  }
  Component.onCompleted: {
    editors.addEditor(qsTr("Vertex Tool"), "ic_vertex_tool_white_24dp", "VertexEditorToolbar.qml")
    editors.addEditor(qsTr("Split Tool"), "ic_split_tool_white_24dp", "SplitFeatureToolbar.qml", GeometryEditorsModelSingleton.Line | GeometryEditorsModelSingleton.Polygon)
    editors.addEditor(qsTr("Fill Ring Tool"), "ic_ring_tool_white_24dp", "FillRingToolBar.qml", GeometryEditorsModelSingleton.Polygon)
  }

  function init() {
    var lastUsed = settings.value( "/QField/GeometryEditorLastUsed", -1 )
    if (lastUsed >= 0)
    {
      selectorRow.stateVisible = false
      var toolbarQml = editors.data(editors.index(lastUsed, 0), GeometryEditorsModelSingleton.ToolbarRole)
      var iconPath = editors.data(editors.index(lastUsed, 0), GeometryEditorsModelSingleton.IconPathRole)
      var name = editors.data(editors.index(lastUsed, 0), GeometryEditorsModelSingleton.NameRole)
      toolbarRow.load(toolbarQml, iconPath, name)
    }
  }

  function cancelEditors() {
    if ( toolbarRow.item )
      toolbarRow.item.cancel()
    featureModel.vertexModel.clear()
  }

  // returns true if handled
  function canvasClicked(point) {
    if ( toolbarRow.item && toolbarRow.visible )
      return toolbarRow.item.canvasClicked(point)
    else
      return false
  }

  // returns true if handled
  function canvasLongPressed(point) {
    if ( toolbarRow.item && toolbarRow.visible )
      return toolbarRow.item.canvasLongPressed(point)
    else
      return false
  }

  VisibilityFadingRow {
    id: selectorRow
    stateVisible: true

    spacing: 4

    Repeater {
      model: editors
      delegate: QfToolButton {
        round: true
        bgcolor: Theme.mainColor
        iconSource: Theme.getThemeIcon(iconPath)
        visible: GeometryEditorsModelSingleton.supportsGeometry(featureModel.vertexModel.geometry, supportedGeometries)
        onClicked: {
          // close current tool if any
          if (toolbarRow.item)
            toolbarRow.item.cancel()
          selectorRow.stateVisible = false
          toolbarRow.load(toolbar, iconPath, name)
          settings.setValue( "/QField/GeometryEditorLastUsed", index )
        }
      }
    }
  }

  Loader {
    id: toolbarRow

    width: item && item.stateVisible ? item.implicitWidth : 0   

    function load(qmlSource, iconPath, name){
      source = qmlSource
      item.init(geometryEditorsToolbar.featureModel, geometryEditorsToolbar.mapSettings, geometryEditorsToolbar.editorRubberbandModel)
        if (toolbarRow.item.screenHovering !== undefined)
          toolbarRow.item.screenHovering = geometryEditorsToolbar.screenHovering
        if (toolbarRow.item.vertexRubberbandVisible !== undefined)
          vertexRubberband.isVisible = toolbarRow.item.vertexRubberbandVisible
        else
          vertexRubberband.isVisible = false

        toolbarRow.item.stateVisible = true
      displayToast(name)
    }

    onSourceChanged: {
      geometryEditorsToolbar.editorChanged()
    }
  }

  onScreenHoveringChanged: {
    if (toolbarRow.item && toolbarRow.item.screenHovering !== undefined)
     toolbarRow.item.screenHovering = geometryEditorsToolbar.screenHovering
  }

  Connections {
      target: toolbarRow.item
      onFinished: {
        featureModel.vertexModel.clear()
        toolbarRow.source = ''
      }
  }

  QfToolButton {
    id: activeToolButton
    iconSource: Theme.getThemeIcon("more_horiz")
    round: true
    visible: !selectorRow.stateVisible && !( toolbarRow.item && toolbarRow.item.stateVisible && toolbarRow.item.blocking )
    bgcolor: Theme.mainColor
    onClicked: {
      toolbarRow.item.cancel()
      toolbarRow.source = ''
      selectorRow.stateVisible = true
    }
  }
}
