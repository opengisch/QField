import QtQuick 2.12
import QtQml.Models 2.12
import org.qgis 1.0
import org.qfield 1.0
import Theme 1.0


/**
This contains several geometry editing tools
A tool must subclass VisibilityFadingRow
And contains following functions:
  * function init(featureModel, mapSettings, editorRubberbandModel)
  * function close()
*/

VisibilityFadingRow {
  id: geometryEditorsToolbar

  property FeatureModel featureModel
  property MapSettings mapSettings
  property RubberbandModel editorRubberbandModel

  spacing: 4 * dp
  padding: 4 * dp

  GeometryEditorsModel {
    id: editors
  }
  Component.onCompleted: {
    editors.addEditor("Vertex tool", "ray-vertex", "VertexEditorToolbar.qml")
    editors.addEditor("Split tool", "content-cut", "SplitFeatureToolbar.qml", GeometryEditorsModelSingleton.Line | GeometryEditorsModelSingleton.Polygon)
    var toolbarQml = editors.data(editors.index(0,0), GeometryEditorsModelSingleton.ToolbarRole)
    var iconPath = editors.data(editors.index(0,0), GeometryEditorsModelSingleton.IconPathRole)
    toolbarRow.load(toolbarQml, iconPath)
  }

  VisibilityFadingRow {
    id: selectorRow
    stateVisible: false

    spacing: 4 * dp
    padding: 4 * dp

    Repeater {
      model: editors
      delegate: Button {
        round: true
        bgcolor: "#FFD600"
        iconSource: Theme.getThemeIcon(iconPath)
        visible: GeometryEditorsModelSingleton.supportsGeometry(featureModel.vertexModel.geometry, supportedGeometries)
        onClicked: {
          // close current tool if any
          if (toolbarRow.item)
            toolbarRow.item.close()
          selectorRow.stateVisible = false
          toolbarRow.load(toolbar, iconPath)
        }
      }
    }
  }

  Loader {
    id: toolbarRow

    function load(qmlSource, iconPath){
      source = qmlSource
      item.init(geometryEditorsToolbar.featureModel, geometryEditorsToolbar.mapSettings, geometryEditorsToolbar.editorRubberbandModel)
      toolbarRow.item.stateVisible = true
      activeToolButton.iconSource = Theme.getThemeIcon(iconPath)
    }

    function hide() {
      if(item)
        item.stateVisible = false
    }
  }

  function cancelEditors() {
    if (toolbarRow.item)
      toolbarRow.item.close()
    featureModel.vertexModel.clear()
  }

  Button {
    id: activeToolButton
    round: true
    bgcolor: "#FFD600"
    onClicked: {
      toolbarRow.hide()
      selectorRow.stateVisible = true
    }
  }

}
