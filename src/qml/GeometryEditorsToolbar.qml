import QtQuick 2.12
import QtQml.Models 2.12
import org.qgis 1.0
import org.qfield 1.0
import Theme 1.0


VisibilityFadingRow {
  id: geometryEditorsToolbar

  property FeatureModel featureModel
  property MapSettings mapSettings

  spacing: 4 * dp
  padding: 4 * dp

  GeometryEditorsModel {
    id: editors
  }
  Component.onCompleted: {
    editors.addEditor("Vertex tool", "ray-vertex", "VertexEditorToolbar")
    editors.addEditor("Split tool", "content-cut", "VertexEditorToolbar", GeometryEditorsModelSingleton.Line | GeometryEditorsModelSingleton.Polygon)
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
          toolbarRow.source = toolbar +'.qml'
        }
      }
    }
  }

  Loader {
    id: toolbarRow
    onStatusChanged: {
      if (toolbarRow.status === Loader.Ready){
        item.featureModel = geometryEditorsToolbar.featureModel
        item.mapSettings = geometryEditorsToolbar.mapSettings
        item.stateVisible = true
      }
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
