import QtQuick 2.12
import QtGraphicalEffects 1.12
import QtSensors 5.12

import org.qgis 1.0
import Theme 1.0

Item {
  id: item

  property variant location // QgsPoint
  property real accuracy
  property real direction // A -1 value indicates absence of direction information
  property MapSettings mapSettings

  Magnetometer {
    id: magnetometer
    active: true
    returnGeoValues: false

    property bool hasValue: true
    property real x: 0
    property real y: 0

    onReadingChanged: {
      magnetometer.x = reading.x
      magnetometer.y = reading.y
      magnetometer.hasValue = true
    }
  }

  Rectangle {
    id: accuracyMarker
    property point screenLocation
    property real accuracy
    visible: accuracy > 0.0
    width: accuracy * 2
    height: accuracy * 2

    x: screenLocation.x - width/2
    y: screenLocation.y - height/2

    radius: width/2

    color: Theme.positionColorSemiOpaque
    border.color: Theme.darkPositionColorSemiOpaque
    border.width: 0.7
  }

  Image {
    id: compassDirectionMarker
    property point screenLocation
    property real direction
    visible: magnetometer.hasValue
    width: 48
    height: 48
    opacity: 0.6

    x: screenLocation.x - width/2
    y: screenLocation.y - height

    source: Theme.getThemeVectorIcon( "ic_compass_direction" )
    fillMode: Image.PreserveAspectFit
    rotation: (Math.atan2(magnetometer.x, magnetometer.y) / Math.PI) * 180
    transformOrigin: Item.Bottom
    smooth: true
  }

  Image {
    id: movementDirectionMarker
    property point screenLocation
    property real direction
    visible: direction >= 0
    width: 48
    height: 48

    x: screenLocation.x - width/2
    y: screenLocation.y - height

    source: Theme.getThemeVectorIcon( "ic_movement_direction" )
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
    id: positionMarker
    property point screenLocation
    property bool isOnCanvas: screenLocation.x > 0
                              && screenLocation.x < mapCanvas.width
                              && screenLocation.y > 0
                              && screenLocation.y < mapCanvas.height

    width: isOnCanvas ? 12 : 10
    height: isOnCanvas ? 12 : 10

    x: Math.min(mapCanvas.width, Math.max(0, screenLocation.x)) - width / 2
    y: Math.min(mapCanvas.height, Math.max(0, screenLocation.y)) - height / 2

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
  }

  onLocationChanged: {
   updateScreenLocation()
  }

  function updateScreenLocation() {
    var point = mapSettings.coordinateToScreen( location )
    positionMarker.screenLocation = point
    compassDirectionMarker.screenLocation = point
    movementDirectionMarker.screenLocation = point
    accuracyMarker.screenLocation = point

    movementDirectionMarker.direction = direction
    accuracyMarker.accuracy = accuracy / mapSettings.mapUnitsPerPoint
  }

  Component.onCompleted: {
    magnetometer.start()
  }
}
