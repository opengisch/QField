

# File Rubberband.qml

[**File List**](files.md) **>** [**qml**](dir_1d81f7f4362e4398c0a695c4e466fcb0.md) **>** [**Rubberband.qml**](Rubberband_8qml.md)

[Go to the documentation of this file](Rubberband_8qml.md)


```C++
import QtQuick
import QtQuick.Shapes
import org.qfield
import org.qgis
import Theme

RubberbandShape {
  id: rubberbandShape

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
        path: rubberbandShape.polylines[0]
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
        path: rubberbandShape.polylines[0]
      }
    }
  }
}
```


