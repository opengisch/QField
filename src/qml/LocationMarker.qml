import QtQuick 2.14
import QtQuick.Shapes 1.14
import QtQuick.Window 2.14

import org.qgis 1.0
import Theme 1.0

import "."

Item {
  id: locationMarker

  property variant location // QgsPoint

  property real accuracy: 0
  property real direction: -1 // A -1 value indicates absence of movement direction information
  property real speed: -1 // A -1 value indicates absence of speed information
  property real orientation: -1 // A -1 value indicates absence of compass orientation

  property color color: Theme.positionColor
  property color darkerColor: Qt.darker(color, 1.75)
  property color semiOpaqueColor: Qt.hsla(color.hslHue, color.hslSaturation, color.hslLightness, 0.2)

  property MapSettings mapSettings

  QtObject {
    id: props

    property point screenLocation
    property real screenAccuracy

    property bool isOnMapCanvas: screenLocation.x > 0
                              && screenLocation.x < mapCanvas.width
                              && screenLocation.y > 0
                              && screenLocation.y < mapCanvas.height
  }
  function updateScreenLocation() {
    props.screenLocation = mapSettings.coordinateToScreen( location )
    props.screenAccuracy = accuracy / mapSettings.mapUnitsPerPoint
  }

  Rectangle {
    id: accuracyMarker
    visible: props.screenAccuracy > 0.0
    width: props.screenAccuracy * 2
    height: props.screenAccuracy * 2

    x: props.screenLocation.x - width / 2
    y: props.screenLocation.y - height / 2

    radius: width/2

    color: locationMarker.semiOpaqueColor
    border.color: locationMarker.darkerColor
    border.width: 0.7
  }

  Image {
    id: compassDirectionMarker
    visible: orientation > -1
    width: 48
    height: 48
    opacity: 0.6

    x: props.screenLocation.x - width / 2
    y: props.screenLocation.y - height

    source: Theme.getThemeVectorIcon( "ic_compass_direction" )
    fillMode: Image.PreserveAspectFit
    rotation: orientation + mapSettings.rotation
    transformOrigin: Item.Bottom
    smooth: true
  }

  Shape {
    id: movementMarker
    visible: speed > 0 && props.isOnMapCanvas
    width: 26
    height: 26

    x: props.screenLocation.x - width / 2
    y: props.screenLocation.y - height / 2

    rotation: direction + mapSettings.rotation
    transformOrigin: Item.Center

    ShapePath {
      strokeWidth: 3
      strokeColor: "white"
      strokeStyle: ShapePath.SolidLine
      fillColor: locationMarker.color
      joinStyle: ShapePath.MiterJoin
      startX: 13
      startY: 2
      PathLine { x: 21; y: 22 }
      PathLine { x: 13; y: 16 }
      PathLine { x: 5; y: 22 }
      PathLine { x: 13; y: 2 }

      SequentialAnimation on fillColor  {
        loops: Animation.Infinite
        ColorAnimation  { from: locationMarker.color; to: locationMarker.darkerColor; duration: 2000; easing.type: Easing.InOutQuad }
        ColorAnimation  { from: locationMarker.darkerColor; to: locationMarker.color; duration: 1000; easing.type: Easing.InOutQuad }
      }
    }

    layer.enabled: true
    layer.effect: QfDropShadow {
        transparentBorder: true
        samples: 16
        color: "#99000000"
        horizontalOffset: 0
        verticalOffset: 0
    }
  }

  Rectangle {
    id: positionMarker
    visible: !movementMarker.visible && props.isOnMapCanvas

    width: 12
    height: 12

    x: props.screenLocation.x - width / 2
    y: props.screenLocation.y - height / 2

    radius: width / 2

    color: locationMarker.color
    border.color: "white"
    border.width: 3

    SequentialAnimation on color  {
      loops: Animation.Infinite
      ColorAnimation  { from: locationMarker.color; to: locationMarker.darkerColor; duration: 2000; easing.type: Easing.InOutQuad }
      ColorAnimation  { from: locationMarker.darkerColor; to: locationMarker.color; duration: 1000; easing.type: Easing.InOutQuad }
    }

    layer.enabled: true
    layer.effect: QfDropShadow {
        transparentBorder: true
        samples: 16
        color: "#99000000"
        horizontalOffset: 0
        verticalOffset: 0
    }
  }

  Shape {
    id: edgeMarker
    visible: !props.isOnMapCanvas
    width: 20
    height: 24

    x: Math.min(mapCanvas.width - width, Math.max(0, props.screenLocation.x - width / 2))
    y: Math.min(mapCanvas.height - width, Math.max(0, props.screenLocation.y - width / 2))

    transform: Rotation {
        origin.x: edgeMarker.width / 2;
        origin.y: edgeMarker.width / 2;
        angle:-(Math.atan2(mapCanvas.width / 2 - props.screenLocation.x, mapCanvas.height / 2 - props.screenLocation.y) / Math.PI) * 180
    }

    ShapePath {
      strokeWidth: 3
      strokeColor: "white"
      strokeStyle: ShapePath.SolidLine
      fillColor: locationMarker.color
      joinStyle: ShapePath.MiterJoin
      startX: 10
      startY: 0
      PathLine { x: 18; y: 20 }
      PathLine { x: 2; y: 20 }
      PathLine { x: 10; y: 0 }

      SequentialAnimation on fillColor  {
        loops: Animation.Infinite
        ColorAnimation  { from: locationMarker.color; to: locationMarker.darkerColor; duration: 2000; easing.type: Easing.InOutQuad }
        ColorAnimation  { from: locationMarker.darkerColor; to: locationMarker.color; duration: 1000; easing.type: Easing.InOutQuad }
      }
    }

    layer.enabled: true
    layer.effect: QfDropShadow {
        transparentBorder: true
        samples: 16
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
    function onRotationChanged() {
      updateScreenLocation()
    }
    function onOutputSizeChanged() {
      updateScreenLocation()
    }
  }

  onLocationChanged: {
   updateScreenLocation()
  }
}
