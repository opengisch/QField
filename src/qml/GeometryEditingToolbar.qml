import QtQuick 2.6
import org.qgis 1.0
import "js/style.js" as Style

Row {
  id: geometryEditingToolbbar
  property RubberbandModel rubberbandModel
  property bool isDigitizing: rubberbandModel.vertexCount > 1 //!< Readonly
  visible: rubberbandModel.vectorLayer !== null

  spacing: 4 * dp
  padding: 4 * dp

  signal vertexAdded
  signal vertexRemoved
  signal cancel
  signal confirm

  Button {
    id: removeVertexButton
    iconSource: Style.getThemeIcon( "ic_remove_white_24dp" )
    visible: rubberbandModel.vertexCount > 1
    round: true
    bgcolor: "#616161"

    onClicked: {
      vertexRemoved()
    }
  }

  Button {
    id: cancelButton
    iconSource: Style.getThemeIcon( "ic_clear_white_24dp" )
    visible: rubberbandModel.vertexCount > 1
    round: true
    bgcolor: "#616161"

    onClicked: {
      cancel()
    }
  }

  Button {
    id: confirmButton
    iconSource: {
      Style.getThemeIcon( "ic_save_white_24dp" )
    }
    visible: {
      if ( Number( rubberbandModel.geometryType ) === 0 )
      {
        false
      }
      else if  ( Number( rubberbandModel.geometryType ) === 1 )
      {
        // Line: at least 2 points (last point not saved)
        rubberbandModel.vertexCount > 2
      }
      else if  ( Number( rubberbandModel.geometryType ) === 2 )
      {
        // Polygon: at least 3 points (last point not saved)
        rubberbandModel.vertexCount > 3
      }
    }
    round: true
    bgcolor: "#FFD600"

    onClicked: {
      // remove editing vertex for lines and polygons
      vertexRemoved()
      confirm()
    }
  }

  Button {
    id: addVertexButton
    iconSource: {
        Style.getThemeIcon( "ic_add_white_24dp" )
    }
    round: true
    bgcolor: "#2E7D32"

    onClicked: {
      if ( Number( rubberbandModel.geometryType ) === 0 )
        confirm()
      else
        vertexAdded()
    }
  }
}
