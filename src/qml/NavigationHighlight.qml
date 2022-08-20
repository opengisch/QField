import QtQuick 2.14

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
      crs: navigation.mapSettings.crs ? navigation.mapSettings.crs : CoordinateReferenceSystemUtils.invalidCrs()
    }
    color: Theme.navigationColorSemiOpaque
    lineWidth: positionSource.active && positionSource.positionInformation && positionSource.positionInformation.latitudeValid ? 5 : 1
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
