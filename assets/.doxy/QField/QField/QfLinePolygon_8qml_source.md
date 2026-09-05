

# File QfLinePolygon.qml

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qml**](dir_5e9804d2322dd99f421cb2c64b2c04c3.md) **>** [**QfLinePolygon.qml**](QfLinePolygon_8qml.md)

[Go to the documentation of this file](QfLinePolygon_8qml.md)


```C++
import QtQuick
import QtQuick.Shapes
import org.qfield.core
import org.qgis

QfLinePolygonShape {
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


