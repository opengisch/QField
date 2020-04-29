import QtQuick 2.12

import org.qgis 1.0
import org.qfield 1.0


Item {
  id: geometryRenderer
  property MapSettings mapSettings
  property alias geometryWrapper: geometryWrapper
  property double lineWidth: 8 * dp
  property color color: "yellow"
  property double pointSize: 20 * dp
  property color borderColor: "blue"
  property double borderSize: 2 * dp

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

      geometry: geometryRenderer.geometryWrapper
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
    // the sourceComponent is updated with the connection on wrapper qgsGeometryChanged signal
    // but it needs to be ready on first used
    sourceComponent: geometryWrapper && geometryWrapper.qgsGeometry.type === QgsWkbTypes.PointGeometry ? pointHighlight : linePolygonHighlight
  }

}
