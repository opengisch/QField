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

  ListModel {
    id: geometryEditorsModel

    ListElement{
      name: QT_TR_NOOP("Vertex tool")
      icon: "ray-vertex"
      mapToolbar: "VertexEditorToolbar"
    }
    ListElement{
      name: QT_TR_NOOP("Split tool")
      icon: "content-cut"
      mapToolbar: "VertexEditorToolbar"
      supportsPoint: false
    }
  }

  VisibilityFadingRow {
    id: selectorRow
    stateVisible: false

    Repeater {
      model: geometryEditorsModel
      delegate: Button {
        round: true
        bgcolor: "#FFD600"
        iconSource: Theme.getThemeIcon(icon)
        visible: vertexModel.geometry.type() in unsupportedGeometries
        onClicked: {
          selectorRow.stateVisible = false
          toolbarRow.source = mapToolbar +'.qml'
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
