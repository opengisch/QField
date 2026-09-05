

# File QfRubberband.qml

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qml**](dir_5e9804d2322dd99f421cb2c64b2c04c3.md) **>** [**QfRubberband.qml**](QfRubberband_8qml.md)

[Go to the documentation of this file](QfRubberband_8qml.md)


```C++
import QtQuick
import QtQuick.Shapes
import org.qfield.core
import org.qgis

QfRubberbandShape {
  id: rubberbandShape

  property bool showVertices: false
  property var activePolyline: rubberbandShape.polylines.length > 0 ? rubberbandShape.polylines[0] : []

  Shape {
    anchors.fill: parent
    ShapePath {
      strokeColor: rubberbandShape.outlineColor
      strokeWidth: rubberbandShape.lineWidth / rubberbandShape.scale + 2
      strokeStyle: ShapePath.SolidLine
      fillColor: "transparent"
      joinStyle: ShapePath.RoundJoin
      capStyle: ShapePath.RoundCap

      PathPolyline {
        path: activePolyline
      }
    }
    ShapePath {
      strokeColor: rubberbandShape.color
      strokeWidth: rubberbandShape.lineWidth / rubberbandShape.scale
      strokeStyle: ShapePath.SolidLine
      fillColor: rubberbandShape.polylinesType === Qgis.GeometryType.Polygon ? Qt.hsla(strokeColor.hslHue, strokeColor.hslSaturation, strokeColor.hslLightness, 0.25) : "transparent"
      joinStyle: ShapePath.RoundJoin
      capStyle: ShapePath.RoundCap

      PathPolyline {
        path: activePolyline
      }
    }

    Repeater {
      id: rubberbandVertices
      model: showVertices && activePolyline.length > 1 ? activePolyline : []

      Rectangle {
        width: rubberbandShape.lineWidth / rubberbandShape.scale * 2
        height: width

        x: modelData.x - width / 2
        y: modelData.y - width / 2

        color: rubberbandShape.color
        border.width: 1
        border.color: rubberbandShape.outlineColor
      }
    }
  }
}
```


