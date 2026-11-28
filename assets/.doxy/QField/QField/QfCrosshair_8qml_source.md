

# File QfCrosshair.qml

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qml**](dir_5e9804d2322dd99f421cb2c64b2c04c3.md) **>** [**QfCrosshair.qml**](QfCrosshair_8qml.md)

[Go to the documentation of this file](QfCrosshair_8qml.md)


```C++
import QtQuick
import QtQuick.Shapes

Shape {
  id: crosshair

  property color fillColor: "#000000"
  property color outlineColor: "#FFFFFF"
  property real sizeScale: 1.0
  property bool isSnapped: false
  readonly property real halfWidth: width / 2
  readonly property real arcSpacing: isSnapped ? 0 : 20
  readonly property real crossHalfLength: sizeScale >= 2.0 ? 16 : sizeScale >= 1.5 ? 14 : 8

  width: (isSnapped ? 32 : 48) * sizeScale
  height: width
  preferredRendererType: Shape.CurveRenderer

  Behavior on width {
    SmoothedAnimation {
      duration: 2000
    }
  }

  ShapePath {
    id: crosshairPathBuffer
    strokeColor: crosshair.outlineColor
    strokeWidth: crosshairPath.strokeWidth + 2
    fillColor: "transparent"
  }

  ShapePath {
    id: crosshairPath
    strokeColor: crosshair.fillColor
    strokeWidth: 2
    fillColor: "transparent"

    PathAngleArc {
      centerX: crosshair.halfWidth
      centerY: crosshair.halfWidth
      radiusX: crosshair.halfWidth
      radiusY: crosshair.halfWidth
      startAngle: 0 + crosshair.arcSpacing
      sweepAngle: 90 - crosshair.arcSpacing * 2
    }
    PathAngleArc {
      centerX: crosshair.halfWidth
      centerY: crosshair.halfWidth
      radiusX: crosshair.halfWidth
      radiusY: crosshair.halfWidth
      startAngle: 90 + crosshair.arcSpacing
      sweepAngle: 90 - crosshair.arcSpacing * 2
    }
    PathAngleArc {
      centerX: crosshair.halfWidth
      centerY: crosshair.halfWidth
      radiusX: crosshair.halfWidth
      radiusY: crosshair.halfWidth
      startAngle: 180 + crosshair.arcSpacing
      sweepAngle: 90 - crosshair.arcSpacing * 2
    }
    PathAngleArc {
      centerX: crosshair.halfWidth
      centerY: crosshair.halfWidth
      radiusX: crosshair.halfWidth
      radiusY: crosshair.halfWidth
      startAngle: 270 + crosshair.arcSpacing
      sweepAngle: 90 - crosshair.arcSpacing * 2
    }
    PathMove {
      x: crosshair.halfWidth
      y: crosshair.halfWidth - crosshair.crossHalfLength
    }
    PathLine {
      x: crosshair.halfWidth
      y: crosshair.halfWidth + crosshair.crossHalfLength
    }
    PathMove {
      x: crosshair.halfWidth - crosshair.crossHalfLength
      y: crosshair.halfWidth
    }
    PathLine {
      x: crosshair.halfWidth + crosshair.crossHalfLength
      y: crosshair.halfWidth
    }
  }

  Component.onCompleted: {
    crosshairPathBuffer.pathElements = crosshairPath.pathElements;
  }
}
```


