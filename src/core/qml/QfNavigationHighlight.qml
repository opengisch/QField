import QtQuick
import org.qgis
import org.qfield.core
import Theme

/**
 * \ingroup qml
 */
Item {
  id: navigationHighlight
  property Navigation navigation

  QfLinePolygon {
    visible: positionSource.active
    mapSettings: navigation.mapSettings
    geometry: QgsGeometryWrapper {
      qgsGeometry: navigation.path
      crs: navigation.mapSettings.crs ? navigation.mapSettings.crs : CoordinateReferenceSystemUtils.invalidCrs()
    }
    color: Theme.navigationColorSemiOpaque
    lineWidth: positionSource.active && positionSource.positionInformation && positionSource.positionInformation.latitudeValid ? 5 : 1
  }

  Repeater {
    model: navigation.model

    delegate: QfNavigationRenderer {
      mapSettings: navigation.mapSettings
      geometryWrapper.qgsGeometry: model.Point
      geometryWrapper.crs: navigation.mapSettings.destinationCrs
      pointIndex: model.index
      pointType: model.PointType
    }
  }
}
