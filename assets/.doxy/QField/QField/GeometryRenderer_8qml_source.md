

# File GeometryRenderer.qml

[**File List**](files.md) **>** [**qml**](dir_1d81f7f4362e4398c0a695c4e466fcb0.md) **>** [**GeometryRenderer.qml**](GeometryRenderer_8qml.md)

[Go to the documentation of this file](GeometryRenderer_8qml.md)


```C++
import QtQuick
import org.qgis
import org.qfield

Item {
  id: geometryRenderer

  property MapSettings mapSettings
  property alias geometryWrapper: geometryWrapper
  property double lineWidth: 3.5
  property color color: "#ff0000"
  property double pointSize: 20
  property double borderSize: 3

  QgsGeometryWrapper {
    id: geometryWrapper
  }

  Connections {
    target: geometryWrapper

    function onQgsGeometryChanged() {
      geometryComponent.sourceComponent = undefined;
      if (geometryWrapper && geometryWrapper.qgsGeometry.type === Qgis.GeometryType.Point) {
        geometryComponent.sourceComponent = pointHighlight;
      } else {
        geometryComponent.sourceComponent = linePolygonHighlight;
      }
    }
  }

  Component {
    id: linePolygonHighlight

    LinePolygon {
      id: linePolygonItem
      mapSettings: geometryRenderer.mapSettings

      geometry: geometryRenderer.geometryWrapper
      color: geometryRenderer.color
      lineWidth: geometryRenderer.lineWidth
    }
  }

  Component {
    id: pointHighlight

    Repeater {
      model: geometryWrapper.pointList()

      Rectangle {
        property CoordinateTransformer ct: CoordinateTransformer {
          id: _ct
          sourceCrs: geometryWrapper.crs
          sourcePosition: modelData
          destinationCrs: mapCanvas.mapSettings.destinationCrs
          transformContext: qgisProject.transformContext
        }

        MapToScreen {
          id: mapToScreenPosition
          mapSettings: mapCanvas.mapSettings
          mapPoint: _ct.projectedPosition
        }

        x: mapToScreenPosition.screenPoint.x - width / 2
        y: mapToScreenPosition.screenPoint.y - width / 2

        color: Qt.hsla(geometryRenderer.color.hslHue, geometryRenderer.color.hslSaturation, geometryRenderer.color.hslLightness, 0.5)
        width: geometryRenderer.pointSize
        height: geometryRenderer.pointSize
        radius: geometryRenderer.pointSize / 2

        border.color: geometryRenderer.color
        border.width: geometryRenderer.borderSize
      }
    }
  }

  Loader {
    id: geometryComponent
    // the sourceComponent is updated with the connection on wrapper qgsGeometryChanged signal
    // but it needs to be ready on first used
    sourceComponent: geometryWrapper && geometryWrapper.qgsGeometry.type === Qgis.GeometryType.Point ? pointHighlight : linePolygonHighlight
  }
}
```


