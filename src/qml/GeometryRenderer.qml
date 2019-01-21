import QtQuick 2.11
import org.qgis 1.0
import org.qfield 1.0


Item {
  id: geometryRenderer
  property QgsGeometryWrapper geometry

  Component {
    id: linePolygonHighlight

    LinePolygonHighlight {
      id: linePolygonHighlightItem
      mapSettings: mapCanvas.mapSettings

      transform: MapTransform {
        mapSettings: mapCanvas.mapSettings
      }

      geometry: geometryRenderer.geometry
      color: "yellow"
      width: 15 * dp
    }
  }

  Component {
    id: pointHighlight

    Repeater {
      model: geometry.pointList()

      Rectangle {
        property CoordinateTransformer ct: CoordinateTransformer {
          id: _ct
          sourceCrs: geometry.crs
          sourcePosition: modelData
          destinationCrs: mapCanvas.mapSettings.destinationCrs
          transformContext: qgisProject.transformContext
        }

        MapToScreen {
          id: mapToScreen
          mapSettings: mapCanvas.mapSettings
          mapPoint: _ct.projectedPosition
        }

        x: mapToScreen.screenPoint.x - width/2
        y: mapToScreen.screenPoint.y - width/2

        color: "yellow"
        width: 20*dp
        height: 20*dp
        radius: width / 2

        border.color: "red"
        border.width: 2*dp
      }
    }
  }

  Loader {
    sourceComponent: geometry.qgsGeometry.type === QgsWkbTypes.PointGeometry ? pointHighlight : linePolygonHighlight
  }

}
