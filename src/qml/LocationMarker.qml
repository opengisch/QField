import QtQuick
import QtQuick.Shapes
import QtQuick.Window
import org.qgis
import Theme

/**
 * \ingroup qml
 */
Item {
  id: locationMarker

  property variant location // QgsPoint

  property real accuracy: 0
  property real direction: -1 // A -1 value indicates absence of movement direction information
  property real speed: -1 // A -1 value indicates absence of speed information
  property real orientation: -1 // A -1 value indicates absence of compass orientation

  property color strokeColor: "white"
  property color color: Qt.darker(Theme.positionColor, 1.25)
  property color semiOpaqueColor: Qt.hsla(color.hslHue, color.hslSaturation, color.hslLightness, 0.1)

  property MapSettings mapSettings

  QtObject {
    id: props

    property point screenLocation
    property real screenAccuracy

    property bool isOnMapCanvas: screenLocation.x > 0 && screenLocation.x < mapCanvas.width && screenLocation.y > 0 && screenLocation.y < mapCanvas.height
  }
  function updateScreenLocation() {
    props.screenLocation = mapSettings.coordinateToScreen(location);
    props.screenAccuracy = accuracy / mapSettings.mapUnitsPerPoint;
  }

  Rectangle {
    id: accuracyMarker
    visible: props.screenAccuracy > 0.0
    width: props.screenAccuracy * 2
    height: props.screenAccuracy * 2

    x: props.screenLocation.x - width / 2
    y: props.screenLocation.y - height / 2

    radius: width / 2

    color: locationMarker.semiOpaqueColor
    border.color: locationMarker.color
    border.width: 0.7
  }

  Item {
    id: compassDirectionMarker
    visible: orientation > -1
    width: 48
    height: 48
    opacity: 0.6

    x: props.screenLocation.x - width / 2
    y: props.screenLocation.y - height

    rotation: orientation + mapSettings.rotation
    transformOrigin: Item.Bottom

    property real wideness: 58.0

    Shape {
      width: 48
      height: 48

      ShapePath {
        strokeWidth: 0
        strokeColor: "transparent"
        fillGradient: LinearGradient {
          x1: 24
          y1: 48
          x2: 24
          y2: 0
          GradientStop {
            position: 0.0
            color: locationMarker.color
          }
          GradientStop {
            position: 1.0
            color: locationMarker.semiOpaqueColor
          }
        }
        joinStyle: ShapePath.MiterJoin

        PathAngleArc {
          centerX: 24
          centerY: 48
          radiusX: 48
          radiusY: 48
          startAngle: -90 - (compassDirectionMarker.wideness / 2)
          sweepAngle: compassDirectionMarker.wideness
        }
        PathLine {
          x: 24
          y: 48
        }
      }
    }

    Shape {
      width: 48
      height: 48

      ShapePath {
        strokeWidth: 0
        strokeColor: "transparent"
        fillGradient: LinearGradient {
          x1: 24
          y1: 48
          x2: 24
          y2: 0
          GradientStop {
            position: 0.0
            color: locationMarker.color
          }
          GradientStop {
            position: 1.0
            color: locationMarker.semiOpaqueColor
          }
        }
        joinStyle: ShapePath.MiterJoin
        startX: 24
        startY: 48

        PathLine {
          x: 48 * Math.sin((180 - (compassDirectionMarker.wideness / 2)) * Math.PI / 180) + 24
          y: 48 * Math.cos((180 - (compassDirectionMarker.wideness / 2)) * Math.PI / 180) + 48
        }
        PathLine {
          x: 24
          y: 40
        }
        PathLine {
          x: 48 * Math.sin((180 + (compassDirectionMarker.wideness / 2)) * Math.PI / 180) + 24
          y: 48 * Math.cos((180 + -(compassDirectionMarker.wideness / 2)) * Math.PI / 180) + 48
        }
        PathLine {
          x: 24
          y: 48
        }
      }
    }
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
      strokeColor: locationMarker.strokeColor
      strokeStyle: ShapePath.SolidLine
      fillColor: locationMarker.color
      joinStyle: ShapePath.MiterJoin
      startX: 13
      startY: 2
      PathLine {
        x: 21
        y: 22
      }
      PathLine {
        x: 13
        y: 16
      }
      PathLine {
        x: 5
        y: 22
      }
      PathLine {
        x: 13
        y: 2
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

    width: 13
    height: 13

    x: props.screenLocation.x - width / 2
    y: props.screenLocation.y - height / 2

    radius: width / 2

    color: locationMarker.color
    border.color: "white"
    border.width: 2

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
      origin.x: edgeMarker.width / 2
      origin.y: edgeMarker.width / 2
      angle: -(Math.atan2(mapCanvas.width / 2 - props.screenLocation.x, mapCanvas.height / 2 - props.screenLocation.y) / Math.PI) * 180
    }

    ShapePath {
      strokeWidth: 3
      strokeColor: locationMarker.strokeColor
      strokeStyle: ShapePath.SolidLine
      fillColor: locationMarker.color
      joinStyle: ShapePath.MiterJoin
      startX: 10
      startY: 0
      PathLine {
        x: 18
        y: 20
      }
      PathLine {
        x: 2
        y: 20
      }
      PathLine {
        x: 10
        y: 0
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
      updateScreenLocation();
    }
    function onRotationChanged() {
      updateScreenLocation();
    }
    function onOutputSizeChanged() {
      updateScreenLocation();
    }
  }

  onLocationChanged: {
    updateScreenLocation();
  }
}
