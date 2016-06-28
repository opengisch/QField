import QtQuick 2.0
import org.qgis 1.0
import "js/style.js" as Style

Row {
  id: digitizingToolbar
  property RubberbandModel rubberbandModel
  property bool isDigitizing //!< Readonly

  signal vertexAdded
  signal vertexRemoved
  signal cancel
  signal confirm

  Button {
    id: addVertexButton
    iconSource: {
      if( digitizingToolbar.rubberbandModel.vertexCount === 1 )
        Style.getThemeIcon( "start-new-line" )
      else
        Style.getThemeIcon( "add-line-vertex" )
    }

    visible: rubberbandModel.geometryType == 1 || rubberbandModel.geometryType == 2

    onClicked: {
      vertexAdded()
    }
  }

  Button {
    id: removeVertexButton
    iconSource: Style.getThemeIcon( "remove-line-vertex" )
    visible: rubberbandModel.vertexCount > 1

    onClicked: {
      vertexRemoved()
    }
  }

  Button {
    id: cancelButton
    iconSource: Style.getThemeIcon( "cancel-line" )
    visible: rubberbandModel.vertexCount > 1

    onClicked: {
      cancel()
    }
  }

  Button {
    id: confirmButton
    iconSource: {
      if ( rubberbandModel.geometryType == 0 )
      {
        Style.getThemeIcon( "add-point" )
      }
      else
      {
        Style.getThemeIcon( "confirm-line" )
      }
    }
    visible: {
      if ( rubberbandModel.geometryType == 0 )
      {
        true
      }
      else
      {
        rubberbandModel.vertexCount > 1
      }
    }

    onClicked: {
      confirm()
    }
  }
}
