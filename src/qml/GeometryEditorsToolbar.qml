import QtQuick 2.12
import QtQml.Models 2.12
import org.qgis 1.0
import org.qfield 1.0
import Theme 1.0


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
  }

  VisibilityFadingRow {
    id: selectorRow
    stateVisible: false

    Repeater {
      model: editors
      delegate: Button {
        round: true
        bgcolor: "#FFD600"
        iconSource: Theme.getThemeIcon(iconPath)
        visible: GeometryEditorsModelSingleton.supportsGeometry(featureModel.vertexModel.geometry, supportedGeometries)
        onClicked: {
          selectorRow.stateVisible = false
          toolbarRow.load(toolbar)
        }
      }
    }
  }

  Loader {
    id: toolbarRow
    function load(qml){
      source = qml
      item.init(geometryEditorsToolbar.featureModel, geometryEditorsToolbar.mapSettings, geometryEditorsToolbar.editorRubberbandModel)
      toolbarRow.item.stateVisible = true
    }

    function hide() {if(item) item.stateVisible = false}
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
