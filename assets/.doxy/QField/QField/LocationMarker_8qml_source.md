

# File LocationMarker.qml

[**File List**](files.md) **>** [**qml**](dir_1d81f7f4362e4398c0a695c4e466fcb0.md) **>** [**LocationMarker.qml**](LocationMarker_8qml.md)

[Go to the documentation of this file](LocationMarker_8qml.md)


```C++
import QtQuick
import QtQuick.Shapes
import QtQuick.Window
import org.qgis
import org.qfield
import Theme

Item {
  id: locationMarker

  property variant location

  property point screenLocation
  property real screenAccuracy
  readonly property bool isOnMapCanvas: screenLocation.x > 0 && screenLocation.x < mapCanvas.width && screenLocation.y > 0 && screenLocation.y < mapCanvas.height

  property bool bubbleVisible: false
  property string bubbleText: ""
  property color bubbleTextColor: Theme.mainTextColor
  property color bubbleColor: Theme.mainBackgroundColorSemiOpaque
  property var bubbleAction: undefined

  property real accuracy: 0
  property real direction: -1 // A -1 value indicates absence of movement direction information
  property real speed: -1 // A -1 value indicates absence of speed information
  property real orientation: -1 // A -1 value indicates absence of compass orientation

  property color strokeColor: "white"
  property color color: Qt.darker(Theme.positionColor, 1.25)
  property color semiOpaqueColor: Qt.hsla(color.hslHue, color.hslSaturation, color.hslLightness, 0.1)

  property MapSettings mapSettings

  function updateScreenLocation() {
    screenLocation = mapSettings.coordinateToScreen(location);
    screenAccuracy = accuracy / mapSettings.mapUnitsPerPoint;
  }

  Rectangle {
    id: accuracyMarker
    visible: screenAccuracy > 0.0
    width: screenAccuracy * 2
    height: screenAccuracy * 2

    x: screenLocation.x - width / 2
    y: screenLocation.y - height / 2

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

    x: screenLocation.x - width / 2
    y: screenLocation.y - height

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

  Item {
    id: locationMarkerBubbleMessage
    width: Math.min(bubbleMessage.implicitWidth + 14, locationMarker.parent.width - 80)
    height: bubbleMessage.height + pointerToLocationMarker.height + 10
    visible: locationMarker.bubbleVisible
    anchors.horizontalCenter: movementMarker.horizontalCenter
    anchors.top: movementMarker.bottom
    anchors.topMargin: -10

    Shape {
      id: pointerToLocationMarker
      anchors.top: parent.top
      anchors.horizontalCenter: parent.horizontalCenter
      rotation: 180

      ShapePath {
        fillColor: locationMarker.bubbleColor
        strokeWidth: 0
        strokeColor: locationMarker.bubbleColor
        PathSvg {
          path: "M 0 0 L 20 0 L 10 10 Z"
        }
      }
    }

    Rectangle {
      width: parent.width
      height: parent.height - pointerToLocationMarker.height
      anchors.top: pointerToLocationMarker.bottom

      color: locationMarker.bubbleColor
      radius: 4
      clip: true

      Text {
        id: bubbleMessage
        font: Theme.tipFont
        wrapMode: Text.WordWrap
        color: locationMarker.bubbleTextColor
        text: locationMarker.bubbleText

        anchors.centerIn: parent
        horizontalAlignment: Text.AlignHCenter
      }

      MouseArea {
        anchors.fill: parent
        onPressed: {
          locationMarker.bubbleAction();
        }
      }
    }
  }

  Shape {
    id: movementMarker
    visible: speed > 0 && isOnMapCanvas
    width: 26
    height: 26

    x: screenLocation.x - width / 2
    y: screenLocation.y - height / 2

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
    visible: !movementMarker.visible && isOnMapCanvas

    width: 14
    height: 14

    x: screenLocation.x - width / 2
    y: screenLocation.y - height / 2

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
    visible: !isOnMapCanvas
    width: 20
    height: 24

    x: Math.min(mapCanvas.width - width, Math.max(0, screenLocation.x - width / 2))
    y: Math.min(mapCanvas.height - width, Math.max(0, screenLocation.y - width / 2))

    transform: Rotation {
      origin.x: edgeMarker.width / 2
      origin.y: edgeMarker.width / 2
      angle: -(Math.atan2(mapCanvas.width / 2 - screenLocation.x, mapCanvas.height / 2 - screenLocation.y) / Math.PI) * 180
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
```


