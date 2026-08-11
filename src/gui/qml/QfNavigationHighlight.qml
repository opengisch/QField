import QtQuick
import org.qgis
import org.qfield.core
import org.qfield.gui

/**
 * \ingroup qml
 */
Item {
  id: navigationHighlight
  property QfNavigation navigation

  QfLinePolygon {
    visible: positionSource.active
    mapSettings: navigation.mapSettings
    geometry: QfGeometryWrapper {
      qgsGeometry: navigation.path
      crs: navigation.mapSettings.crs ? navigation.mapSettings.crs : QfCoordinateReferenceSystemUtils.invalidCrs()
    }
    color: QfTheme.navigationColorSemiOpaque
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
