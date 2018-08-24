import QtQuick 2.6
import org.qgis 1.0
import org.qfield 1.0
import "js/style.js" as Style

VisibiltyFadingRow {
  id: geometryEditingToolbar

  property FeatureModel featureModel
  property MapSettings mapSettings

  spacing: 4 * dp
  padding: 4 * dp

  Button {
    id: cancelButton
    iconSource: Style.getThemeIcon( "ic_clear_white_24dp" )
    round: true
    bgcolor: "#FFD600"

    onClicked: {
      featureModel.vertexModel.clear()
    }
  }

  Button {
    id: applyButton
    iconSource: Style.getThemeIcon( "ic_save_white_24dp" )
    round: true
    bgcolor: featureModel.vertexModel.dirty ? "#FFD600" : "#616161"

    onClicked: {
      if (featureModel.vertexModel.dirty){
        featureModel.applyVertexModelToGeometry()
        featureModel.save()
        featureModel.vertexModel.clear()
      }
    }
  }


  Button {
    id: previousVertexButton
    iconSource: Style.getThemeIcon( "ic_chevron_left_white_24dp" )
    round: true
    bgcolor: "#FFD600"

    onClicked: {
      featureModel.vertexModel.previousVertex()
    }
  }

  Button {
    id: removeVertexButton
    iconSource: Style.getThemeIcon( "ic_remove_white_24dp" )
    round: true
    bgcolor: featureModel.vertexModel.canRemoveVertex ? "#FFD600" : "#616161"

    onClicked: {
      if (featureModel.vertexModel.canRemoveVertex){
        featureModel.vertexModel.removeCurrentVertex()
      }
    }
  }

  Button {
    id: nextVertexButton
    iconSource: Style.getThemeIcon( "ic_chevron_right_white_24dp" )
    round: true
    bgcolor: "#FFD600"

    onClicked: {
      featureModel.vertexModel.nextVertex()
    }
  }

  Connections {
    target: vertexModel
    onCurrentPointChanged:
    {
      if (featureModel.vertexModel.editingMode == VertexModel.EditVertex)
        mapSettings.setCenter(featureModel.vertexModel.currentPoint)
    }
  }
}

