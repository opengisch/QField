import QtQuick 2.12

import org.qgis 1.0
import Theme 1.0

VisibilityFadingRow {
  id: digitizingToolbar
  property RubberbandModel rubberbandModel
  property CoordinateLocator coordinateLocator // optional coordinateLocator to flash
  property MapSettings mapSettings
  property bool showConfirmButton: true //<! if the geometry type is point, it will never be shown
  property bool screenHovering: false //<! if the stylus pen is used, one should not use the add button

  readonly property bool isDigitizing: rubberbandModel ? rubberbandModel.vertexCount > 1 : false //!< Readonly

  property bool geometryValid: false

  spacing: 4

  signal confirm
  signal vertexCountChanged

  Connections {
      target: rubberbandModel

      function onVertexCountChanged() {
          // set geometry valid
          if ( Number( rubberbandModel ? rubberbandModel.geometryType : 0 ) === 0 )
          {
            geometryValid = false
          }
          else if  ( Number( rubberbandModel.geometryType ) === 1 )
          {
            // Line: at least 2 points (last point not saved)
            geometryValid = rubberbandModel.vertexCount > 2
          }
          else if  ( Number( rubberbandModel.geometryType ) === 2 )
          {
            // Polygon: at least 3 points (last point not saved)
            geometryValid = rubberbandModel.vertexCount > 3
          }

          // emit the signal of digitizingToolbar
          vertexCountChanged()
      }
  }

  QfToolButton {
    id: cancelButton
    iconSource: Theme.getThemeIcon( "ic_clear_white_24dp" )
    visible: rubberbandModel && rubberbandModel.vertexCount > 1
    round: true
    bgcolor: Theme.darkRed

    onClicked: {
      cancel()
    }
  }

  QfToolButton {
    id: confirmButton
    iconSource: {
      Theme.getThemeIcon( "ic_check_white_48dp" )
    }
    visible: {
      if ( !showConfirmButton )
      {
        false
      }
      else
      {
        geometryValid
      }
    }
    round: true
    bgcolor: Theme.mainColor

    onClicked: {
      // remove editing vertex for lines and polygons
      rubberbandModel.frozen = true
      rubberbandModel.removeVertex()
      confirm()
    }
  }

  QfToolButton {
    id: removeVertexButton
    iconSource: Theme.getThemeIcon( "ic_remove_white_24dp" )
    visible: rubberbandModel && rubberbandModel.vertexCount > 1
    round: true
    bgcolor: Theme.darkGray

    onClicked: {
      removeVertex()
    }
  }

  QfToolButton {
    id: addVertexButton
    iconSource: Theme.getThemeIcon( "ic_add_white_24dp" )
    round: true
    enabled: !screenHovering
    bgcolor: {
        if (!showConfirmButton)
          Theme.darkGray
        else if (Number( rubberbandModel ? rubberbandModel.geometryType : 0 ) === QgsWkbTypes.PointGeometry)
          Theme.mainColor
        else
          Theme.darkGray
    }

    states: [
        State { when: addVertexButton.enabled;
            PropertyChanges {   target: addVertexButton; opacity: 1.0    }
        },
        State { when: !addVertexButton.enabled;
            PropertyChanges {   target: addVertexButton; opacity: 0.0    }
        }
    ]
    transitions: [ Transition { NumberAnimation { property: "opacity"; duration: 200 } } ]

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
