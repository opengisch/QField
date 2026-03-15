

# File NavigationRenderer.qml

[**File List**](files.md) **>** [**qml**](dir_1d81f7f4362e4398c0a695c4e466fcb0.md) **>** [**NavigationRenderer.qml**](NavigationRenderer_8qml.md)

[Go to the documentation of this file](NavigationRenderer_8qml.md)


```C++
import QtQuick
import QtQuick.Shapes
import org.qgis
import org.qfield
import Theme

Item {
  id: navigationRenderer

  property var pointIndex: undefined
  property int pointType: NavigationModel.Destination

  property MapSettings mapSettings
  property alias geometryWrapper: geometryWrapper

  QgsGeometryWrapper {
    id: geometryWrapper
  }

  Connections {
    target: geometryWrapper

    function onQgsGeometryChanged() {
      geometryComponent.sourceComponent = undefined;
      if (geometryWrapper && geometryWrapper.qgsGeometry.type === Qgis.GeometryType.Point) {
        geometryComponent.sourceComponent = pointHighlight;
      }
    }
  }

  Component {
    id: pointHighlight

    Repeater {
      model: geometryWrapper.pointList()

      Item {
        property CoordinateTransformer ct: CoordinateTransformer {
          id: _ct
          sourceCrs: geometryWrapper.crs
          sourcePosition: modelData
          destinationCrs: mapCanvas.mapSettings.destinationCrs
          transformContext: qgisProject ? qgisProject.transformContext : CoordinateReferenceSystemUtils.emptyTransformContext()
        }

        MapToScreen {
          id: mapToScreenPosition
          mapSettings: mapCanvas.mapSettings
          mapPoint: _ct.projectedPosition
        }

        property bool isOnMapCanvas: mapToScreenPosition.screenPoint.x > 0 && mapToScreenPosition.screenPoint.x < mapCanvas.width && mapToScreenPosition.screenPoint.y > 0 && mapToScreenPosition.screenPoint.y < mapCanvas.height
        Rectangle {
          id: point
          visible: isOnMapCanvas
          x: mapToScreenPosition.screenPoint.x - width / 2
          y: mapToScreenPosition.screenPoint.y - height / 2

          width: 16
          height: 16
          color: Theme.navigationColor
          border.color: "white"
          border.width: 3
          transform: Rotation {
            origin.x: point.width / 2
            origin.y: point.width / 2
            angle: 45
          }

          layer.enabled: true
          layer.samples: 4
          layer.effect: QfDropShadow {
            transparentBorder: true
            radius: 8
            color: "#99000000"
            horizontalOffset: 0
            verticalOffset: 0
          }
        }

        Shape {
          id: edgePoint
          visible: !isOnMapCanvas
          width: 20
          height: 24

          x: Math.min(mapCanvas.width - width, Math.max(0, mapToScreenPosition.screenPoint.x - width / 2))
          y: Math.min(mapCanvas.height - width, Math.max(0, mapToScreenPosition.screenPoint.y - width / 2))

          transform: Rotation {
            origin.x: edgePoint.width / 2
            origin.y: edgePoint.width / 2
            angle: -(Math.atan2(mapCanvas.width / 2 - mapToScreenPosition.screenPoint.x, mapCanvas.height / 2 - mapToScreenPosition.screenPoint.y) / Math.PI) * 180
          }

          ShapePath {
            strokeWidth: 3
            strokeColor: "white"
            strokeStyle: ShapePath.SolidLine
            fillColor: Theme.navigationColor
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
          layer.samples: 4
          layer.effect: QfDropShadow {
            transparentBorder: true
            radius: 8
            color: "#99000000"
            horizontalOffset: 0
            verticalOffset: 0
          }
        }
      }
    }
  }

  Loader {
    id: geometryComponent
    // the sourceComponent is updated with the connection on wrapper qgsGeometryChanged signal
    // but it needs to be ready on first used
    sourceComponent: geometryWrapper && geometryWrapper.qgsGeometry.type === Qgis.GeometryType.Point ? pointHighlight : undefined
  }
}
```


