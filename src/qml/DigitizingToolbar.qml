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
        Style.getThemeIcon( "ic_create_white_24dp" )
      else
        Style.getThemeIcon( "ic_add_white_24dp" )
    }

    visible: rubberbandModel.geometryType == 1 || rubberbandModel.geometryType == 2

    onClicked: {
      vertexAdded()
    }
  }

  Button {
    id: removeVertexButton
    iconSource: Style.getThemeIcon( "ic_remove_white_24dp" )
    visible: rubberbandModel.vertexCount > 1

    onClicked: {
      vertexRemoved()
    }
  }

  Button {
    id: cancelButton
    iconSource: Style.getThemeIcon( "ic_clear_white_24dp" )
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
        Style.getThemeIcon( "ic_create_white_24dp" )
      }
      else
      {
        Style.getThemeIcon( "ic_save_white_24dp" )
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
