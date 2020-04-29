import QtQuick 2.12
import QtGraphicalEffects 1.12

import org.qgis 1.0
import Theme 1.0

Item {
  id: item

  property variant location // QgsPointV2
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

    color: "#3364b5f6"
    border.color: "#992374b5"
    border.width: 0.7 * dp
  }

  Image {
    id: directionMarker
    property point location
    property real direction
    visible: direction >= 0
    width: 48 * dp
    height: 48 * dp

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
    width: 12*dp
    height: 12*dp

    x: location.x - width/2
    y: location.y - height/2

    radius: width/2

    color: "#64b5f6"
    border.color: "#FFFFFF"
    border.width: 3 * dp

    SequentialAnimation on color  {
      loops: Animation.Infinite
      ColorAnimation  { from: "#64b5f6"; to: "#2374b5"; duration: 2000; easing.type: Easing.InOutQuad }
      ColorAnimation  { from: "#2374b5"; to: "#64b5f6"; duration: 1000; easing.type: Easing.InOutQuad }
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
    onExtentChanged: {
      marker.location = mapSettings.coordinateToScreen( location )
      directionMarker.location = mapSettings.coordinateToScreen( location )
      directionMarker.direction = direction
      accuracyMarker.location = mapSettings.coordinateToScreen( location )
      accuracyMarker.accuracy = accuracy / mapSettings.mapUnitsPerPixel
    }
  }

  onLocationChanged: {
    marker.location = mapSettings.coordinateToScreen( location );
    directionMarker.location = mapSettings.coordinateToScreen( location )
    directionMarker.direction = direction
    accuracyMarker.location = mapSettings.coordinateToScreen( location )
    accuracyMarker.accuracy = accuracy / mapSettings.mapUnitsPerPixel
  }
}
