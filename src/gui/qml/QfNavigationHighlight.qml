import QtQuick
import org.qgis
import org.qfield.core
import org.qfield.gui

/**
 * \ingroup org.qfield.gui
 */
Item {
  id: navigationHighlight
  property QfNavigation navigation

  QfLinePolygon {
    visible: positionSource.active
    mapSettings: navigation.mapSettings
    geometry: QfGeometryWrapper {
      crs: navigation.mapSettings.crs ? navigation.mapSettings.crs : QfCoordinateReferenceSystemUtils.invalidCrs()
      qgsGeometry: navigation.path
    }
    color: QfTheme.navigationColorSemiOpaque
    lineWidth: positionSource.active && positionSource.positionInformation && positionSource.positionInformation.latitudeValid ? 5 : 1
  }

  Repeater {
    model: navigation.model

    delegate: QfNavigationRenderer {
      mapSettings: navigation.mapSettings
      geometryWrapper.crs: navigation.mapSettings.destinationCrs
      geometryWrapper.qgsGeometry: model.Point
      pointIndex: model.index
      pointType: model.PointType
    }
  }
}
