import QtQuick 2.6
import org.qgis 1.0
import Theme 1.0

VisibilityFadingRow {
  id: digitizingToolbar
  property RubberbandModel rubberbandModel
  property string mode: "digitize"  // digitize or measure
  property CoordinateLocator coordinateLocator // optional coordinateLocator to flash
  property MapSettings mapSettings

  property bool isDigitizing: rubberbandModel ? rubberbandModel.vertexCount > 1 : false //!< Readonly


  spacing: 4 * dp
  padding: 4 * dp

  signal confirm

  Button {
    id: cancelButton
    iconSource: Theme.getThemeIcon( "ic_clear_white_24dp" )
    visible: rubberbandModel && rubberbandModel.vertexCount > 1
    round: true
    bgcolor: "#900000"

    onClicked: {
      cancel()
    }
  }

  Button {
    id: confirmButton
    iconSource: {
      Theme.getThemeIcon( "ic_check_white_48dp" )
    }
    visible: {
      if ( Number( rubberbandModel ? rubberbandModel.geometryType : 0 ) === 0 || mode === 'measure' )
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
      removeVertex()
      confirm()
    }
  }

  Button {
    id: removeVertexButton
    iconSource: Theme.getThemeIcon( "ic_remove_white_24dp" )
    visible: rubberbandModel && rubberbandModel.vertexCount > 1
    round: true
    bgcolor: Theme.darkGray

    onClicked: {
      removeVertex()
    }
  }

  Button {
    id: addVertexButton
    iconSource: {
        Theme.getThemeIcon( "ic_add_white_24dp" )
    }
    round: true
    bgcolor: mode === 'measure' ? Theme.darkGray : Number( rubberbandModel ? rubberbandModel.geometryType : 0 ) === QgsWkbTypes.PointGeometry ? Theme.mainColor : Theme.darkGray

    onClicked: {
      if ( Number( rubberbandModel.geometryType ) === QgsWkbTypes.PointGeometry ||
           Number( rubberbandModel.geometryType ) === QgsWkbTypes.NullGeometry )
        confirm()
      else
        addVertex()
    }
  }

  function addVertex()
  {
    if (coordinateLocator)
      coordinateLocator.flash()
    rubberbandModel.addVertex()
  }

  function removeVertex()
  {
    rubberbandModel.removeVertex()
    mapSettings.setCenter( rubberbandModel.currentCoordinate )
  }

  function cancel()
  {
    rubberbandModel.reset()
  }
}
