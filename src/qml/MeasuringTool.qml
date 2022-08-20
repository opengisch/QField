import QtQuick 2.14

import org.qgis 1.0
import org.qfield 1.0

import Theme 1.0

Item {
  id: measuringTool

  property alias measuringRubberband: rubberband
  property bool isClosingArea: rubberband.model.vertexCount > 2
                               && vertexFirstLastDistance.screenDistance < 10
  property bool isArea: false

  MapToScreen {
    id: vertexFirstLastDistance
    mapSettings: rubberband.mapSettings
    mapDistance: GeometryUtils.distanceBetweenPoints(rubberband.model.firstCoordinate,
                                                     rubberband.model.currentCoordinate)
  }

  Repeater {
    id: vertices
    model: rubberband.model.vertices
    delegate: Rectangle {
      MapToScreen {
        id: vertexToScreen
        mapSettings: rubberband.mapSettings
        mapPoint: modelData
      }

      visible: rubberband.model.vertexCount > 1

      x: vertexToScreen.screenPoint.x - width/2
      y: vertexToScreen.screenPoint.y - width/2

      width: isClosingArea
             && (index === 0 || index === rubberband.model.vertexCount - 1)
             ? 20
             : 10
      height: width
      radius: width / 2
      color: "#20000000"
      border.color: '#80000000'
      border.width: 2
    }
  }

  Rubberband {
    id: rubberband
    lineWidth: 2.5
    color: '#80000000'

    model: RubberbandModel {
      frozen: false
      geometryType: isClosingArea || isArea
                    ? QgsWkbTypes.PolygonGeometry
                    : QgsWkbTypes.LineGeometry
      crs: rubberband.mapSettings.destinationCrs
    }

    anchors.fill: parent
  }

  Connections {
    target: rubberband.model

    function onVertexCountChanged() {
      if (rubberband.model.vertexCount > 2
          && vertexFirstLastDistance.screenDistance < 10) {
        isArea = true;
      } else if (rubberband.model.vertexCount <= 1) {
        isArea = false;
      }
    }
  }
}
