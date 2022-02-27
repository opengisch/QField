import QtQuick 2.12
import QtGraphicalEffects 1.12

import org.qgis 1.0
import Theme 1.0

Item {
  id: item

  property variant location // QgsPoint
  property real accuracy
  property real direction // A -1 value indicates absence of direction information
  property MapSettings mapSettings

  Rectangle {
    id: accuracyMarker
    property point location
    property real accuracy
    visible: accuracy > 0.0
    width: accuracy * 2
    height: accuracy * 2

    x: location.x - width/2
    y: location.y - height/2

    radius: width/2

    color: Theme.positionColorSemiOpaque
    border.color: Theme.darkPositionColorSemiOpaque
    border.width: 0.7
  }

  Image {
    id: directionMarker
    property point location
    property real direction
    visible: direction >= 0
    width: 48
    height: 48

    x: location.x - width/2
    y: location.y - height

    source: Theme.getThemeVectorIcon( "ic_gps_direction" )
    fillMode: Image.PreserveAspectFit
    rotation: direction
    transformOrigin: Item.Bottom
    smooth: true

    layer.enabled: true
    layer.effect: DropShadow {
        transparentBorder: true
        radius: 8
        samples: 25
        color: "#99000000"
        horizontalOffset: 0
        verticalOffset: 0
    }
  }

  Rectangle {
    id: marker

    property point location
    property bool isOnCanvas: location.x > 0
                              && location.x < mapCanvas.width
                              && location.y > 0
                              && location.y < mapCanvas.height

    width: isOnCanvas ? 12 : 10
    height: isOnCanvas ? 12 : 10

    x: Math.min(mapCanvas.width, Math.max(0, location.x)) - width/2
    y: Math.min(mapCanvas.height, Math.max(0, location.y)) - height/2

    radius: width/2

    color: Theme.positionColor
    border.color: "white"
    border.width: isOnCanvas ? 3 : 2

    SequentialAnimation on color  {
      loops: Animation.Infinite
      ColorAnimation  { from: Theme.positionColor; to: Theme.darkPositionColor; duration: 2000; easing.type: Easing.InOutQuad }
      ColorAnimation  { from: "#2374b5"; to: Theme.positionColor; duration: 1000; easing.type: Easing.InOutQuad }
    }

    layer.enabled: true
    layer.effect: DropShadow {
        transparentBorder: true
        radius: 8
        samples: 25
        color: "#99000000"
        horizontalOffset: 0
        verticalOffset: 0
    }
  }

  Connections {
    target: mapSettings


    function onExtentChanged() {
      updateScreenLocation()
    }
    function onOutputSizeChanged() {
      updateScreenLocation()
    }

    function updateScreenLocation() {
      marker.location = mapSettings.coordinateToScreen( location )
      directionMarker.location = mapSettings.coordinateToScreen( location )
      directionMarker.direction = direction
      accuracyMarker.location = mapSettings.coordinateToScreen( location )
      accuracyMarker.accuracy = accuracy / mapSettings.mapUnitsPerPoint
    }
  }

  onLocationChanged: {
    marker.location = mapSettings.coordinateToScreen( location );
    directionMarker.location = mapSettings.coordinateToScreen( location )
    directionMarker.direction = direction
    accuracyMarker.location = mapSettings.coordinateToScreen( location )
    accuracyMarker.accuracy = accuracy / mapSettings.mapUnitsPerPoint
  }
}
