

# File QfCompassDial.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfCompassDial.qml**](QfCompassDial_8qml.md)

[Go to the documentation of this file](QfCompassDial_8qml.md)


```C++
import QtQuick
import QtQuick.Shapes
import org.qfield.gui

Item {
  id: compassDial

  property real mapRotation: 0

  property color northColor: QfTheme.mainColor

  property color southColor: QfTheme.toolButtonColor

  property color tickColor: QfTheme.gray

  property color backgroundColor: QfTheme.toolButtonBackgroundColor

  readonly property real centerX: width / 2
  readonly property real centerY: height / 2
  readonly property real needleHalfLength: height * 0.26
  readonly property real needleHalfWidth: width * 0.09
  readonly property real hubRadius: height * 0.085
  readonly property real tickRadius: height * 0.37

  Rectangle {
    anchors.fill: parent
    radius: width / 2
    color: compassDial.backgroundColor
  }

  Repeater {
    model: 8

    delegate: Rectangle {
      readonly property real angleDegrees: index * 45
      readonly property real angleRadians: angleDegrees * Math.PI / 180

      x: compassDial.centerX + compassDial.tickRadius * Math.sin(angleRadians) - width / 2
      y: compassDial.centerY - compassDial.tickRadius * Math.cos(angleRadians) - height / 2
      width: Math.max(1.5, compassDial.width * 0.032)
      height: compassDial.height * 0.055
      radius: width / 2
      rotation: angleDegrees
      color: compassDial.tickColor
    }
  }

  Shape {
    anchors.fill: parent
    rotation: compassDial.mapRotation
    preferredRendererType: Shape.CurveRenderer

    ShapePath {
      strokeColor: "transparent"
      fillColor: compassDial.northColor
      startX: compassDial.centerX
      startY: compassDial.centerY - compassDial.needleHalfLength

      PathLine {
        x: compassDial.centerX + compassDial.needleHalfWidth
        y: compassDial.centerY
      }
      PathLine {
        x: compassDial.centerX - compassDial.needleHalfWidth
        y: compassDial.centerY
      }
    }

    ShapePath {
      strokeColor: "transparent"
      fillColor: compassDial.southColor
      startX: compassDial.centerX
      startY: compassDial.centerY + compassDial.needleHalfLength

      PathLine {
        x: compassDial.centerX + compassDial.needleHalfWidth
        y: compassDial.centerY
      }
      PathLine {
        x: compassDial.centerX - compassDial.needleHalfWidth
        y: compassDial.centerY
      }
    }
  }
}
```


