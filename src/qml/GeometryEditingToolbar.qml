import QtQuick 2.6
import org.qgis 1.0
import org.qfield 1.0
import "js/style.js" as Style

Row {
  id: geometryEditingToolbar

  property FeatureModel featureModel
  property MapSettings mapSettings


  spacing: 4 * dp
  padding: 4 * dp

  signal cancel

  Button {
    id: cancelButton
    iconSource: Style.getThemeIcon( "ic_clear_white_24dp" )
    round: true
    bgcolor: "#616161"

    onClicked: {
      cancel()
    }
  }

  Button {
    id: applyButton
    iconSource: Style.getThemeIcon( "ic_save_white_24dp" )
    round: true
    bgcolor: "#616161"

    // TODO only visible when changes

    onClicked: {
      featureModel.applyVertexModelToGeometry()
      featureModel.save()
    }
  }


  Button {
    id: previousVertexButton
    iconSource: Style.getThemeIcon( "ic_chevron_left_white_24dp" )
    round: true
    bgcolor: "#616161"

    onClicked: {
      featureModel.vertexModel.previousVertex()
    }
  }

  Button {
    id: removeVertexButton
    iconSource: Style.getThemeIcon( "ic_remove_white_24dp" )
    round: true
    bgcolor: "#616161"

    // TODO: visible when possible

    onClicked: {
      // TODO
    }
  }

  Button {
    id: nextVertexButton
    iconSource: Style.getThemeIcon( "ic_chevron_right_white_24dp" )
    round: true
    bgcolor: "#616161"

    onClicked: {
      featureModel.vertexModel.nextVertex()
    }
  }

  Connections {
      target: vertexModel
      onCurrentPointChanged:
      {
// TODO check mode
        mapSettings.setCenter(featureModel.vertexModel.currentPoint)
      }
  }
}
