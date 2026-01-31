

# File LinePolygon.qml

[**File List**](files.md) **>** [**qml**](dir_1d81f7f4362e4398c0a695c4e466fcb0.md) **>** [**LinePolygon.qml**](LinePolygon_8qml.md)

[Go to the documentation of this file](LinePolygon_8qml.md)


```C++
import QtQuick
import QtQuick.Shapes
import org.qfield
import org.qgis
import Theme

LinePolygonShape {
  id: linePolygonShape

  onPolylinesChanged: {
    if (polylines.length > 0) {
      const pathElements = [];
      for (const polyline of polylines) {
        var pathPolyline = componentPathPolyline.createObject(shapePath);
        pathPolyline.path = polyline;
        pathElements.push(pathPolyline);
      }
      shapePath.pathElements = pathElements;
    } else {
      shapePath.pathElements = [componentPathPolyline.createObject(shapePath)];
    }
  }

  Component {
    id: componentPathPolyline

    PathPolyline {}
  }

  Shape {
    id: shape
    anchors.fill: parent

    ShapePath {
      id: shapePath
      strokeColor: linePolygonShape.color
      strokeWidth: linePolygonShape.lineWidth / linePolygonShape.scale
      strokeStyle: ShapePath.SolidLine
      fillColor: linePolygonShape.polylinesType === Qgis.GeometryType.Polygon ? Qt.hsla(strokeColor.hslHue, strokeColor.hslSaturation, strokeColor.hslLightness, 0.25) : "transparent"
      joinStyle: ShapePath.RoundJoin
      capStyle: ShapePath.RoundCap
    }
  }
}
```


