import QtQuick 2.6
import org.qgis 1.0
import Theme 1.0

VisibilityFadingRow {
  id: digitizingToolbar
  property RubberbandModel rubberbandModel
  property bool isDigitizing: rubberbandModel ? rubberbandModel.vertexCount > 1 : false //!< Readonly

  spacing: 4 * dp
  padding: 4 * dp

  signal vertexAdded
  signal vertexRemoved
  signal cancel
  signal confirm

  Button {
    id: cancelButton
    iconSource: Theme.getThemeIcon( "ic_clear_white_24dp" )
    visible: rubberbandModel.vertexCount > 1
    round: true
    bgcolor: "#900000"

    onClicked: {
      cancel()
    }
  }

  Button {
    id: confirmButton
    iconSource: {
      Theme.getThemeIcon( "ic_save_white_24dp" )
    }
    visible: {
      if ( Number( rubberbandModel ? rubberbandModel.geometryType : 0 ) === 0 || stateMachine.state === 'measure' )
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
    bgcolor: Theme.mainColor

    onClicked: {
      // remove editing vertex for lines and polygons
      vertexRemoved()
      confirm()
    }
  }

  Button {
    id: removeVertexButton
    iconSource: Theme.getThemeIcon( "ic_remove_white_24dp" )
    visible: rubberbandModel.vertexCount > 1
    round: true
    bgcolor: Theme.darkGray

    onClicked: {
      vertexRemoved()
    }
  }

  Button {
    id: addVertexButton
    iconSource: {
        Theme.getThemeIcon( "ic_add_white_24dp" )
    }
    round: true
    bgcolor: stateMachine.state === 'measure' ? Theme.darkGray : Number( rubberbandModel ? rubberbandModel.geometryType : 0 ) === QgsWkbTypes.PointGeometry ? Theme.mainColor : Theme.darkGray

    onClicked: {
      if ( Number( rubberbandModel.geometryType ) === QgsWkbTypes.PointGeometry ||
           Number( rubberbandModel.geometryType ) === QgsWkbTypes.NullGeometry )
        confirm()
      else
        vertexAdded()
    }
  }
}
