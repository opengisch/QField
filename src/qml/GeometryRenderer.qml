import QtQuick 2.11
import org.qgis 1.0
import org.qfield 1.0


Item {
  id: geometryRenderer
  //signal qgsGeometryChangedv2
  property alias geometryWrapper: geometryWrapper
  property double lineWidth: 8 * dp
  property color color: "yellow"
  property double pointSize: 20 * dp
  property color borderColor: "blue"
  property double borderSize: 2 * dp
  property MapSettings mapSettings: mapCanvas.mapSettings

  QgsGeometryWrapper {
    id: geometryWrapper
  }

  Connections {
    target: geometryWrapper

    onQgsGeometryChanged: {
      geometryComponent.sourceComponent = undefined
      if (geometryWrapper && geometryWrapper.qgsGeometry.type === QgsWkbTypes.PointGeometry) {
        geometryComponent.sourceComponent = pointHighlight
      }
      else
      {
        geometryComponent.sourceComponent = linePolygonHighlight
      }
      //qgsGeometryChangedv2()
    }
  }

  Component {
    id: linePolygonHighlight

    LinePolygonHighlight {
      id: linePolygonHighlightItem
      mapSettings: geometryRenderer.mapSettings

      transform: MapTransform {
        mapSettings: geometryRenderer.mapSettings
      }

      geometry: geometryRenderer.geometry
      color: geometryRenderer.color
      width: geometryRenderer.lineWidth
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
          id: mapToScreen
          mapSettings: mapCanvas.mapSettings
          mapPoint: _ct.projectedPosition
        }

        x: mapToScreen.screenPoint.x - width/2
        y: mapToScreen.screenPoint.y - width/2

        color: geometryRenderer.color
        width: geometryRenderer.pointSize
        height: geometryRenderer.pointSize
        radius: geometryRenderer.pointSize / 2

        border.color: geometryRenderer.borderColor
        border.width: geometryRenderer.borderSize
      }
    }
  }

  Loader {
    id: geometryComponent
    sourceComponent: geometryWrapper && geometryWrapper.qgsGeometry.type === QgsWkbTypes.PointGeometry ? pointHighlight : linePolygonHighlight
  }

}
