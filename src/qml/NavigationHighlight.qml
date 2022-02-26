import QtQuick 2.12

import org.qgis 1.0
import org.qfield 1.0

import Theme 1.0

Item {
  id: navigationHighlight
  property Navigation navigation

  LinePolygonHighlight {
    visible: positionSource.active
    mapSettings: navigation.mapSettings
    geometry:   QgsGeometryWrapper {
      qgsGeometry: navigation.path
      crs: navigation.mapSettings.crs
    }
    color: "#77FF0000"
    width: positionSource.active && positionSource.positionInfo && positionSource.positionInfo.latitudeValid ? 3 : 1
  }

  Repeater {
    model: navigation.model

    delegate: NavigationRenderer {
      mapSettings: navigation.mapSettings
      geometryWrapper.qgsGeometry: model.Point
      geometryWrapper.crs: navigation.mapSettings.destinationCrs
      pointIndex: model.index
      pointType: model.PointType
    }
  }
}
