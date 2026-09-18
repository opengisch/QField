import QtQuick
import QtQuick.Shapes
import org.qgis
import org.qfield.core
import org.qfield.gui

/**
 * \ingroup qml_gui
 */
Item {
  id: markupItemRenderer

  /// type:QgsQuickMapSettings
  property MapSettings mapSettings
  property QfMarkupItem markupItem

  QfGeometryRenderer {
    mapSettings: markupItemRenderer.mapSettings
    geometryWrapper.crs: QfCoordinateReferenceSystemUtils.wgs84Crs()
    geometryWrapper.qgsGeometry: markupItem.geometry
    color: markupItem.color
    lineWidth: 3
    pointSize: 12
  }
}
