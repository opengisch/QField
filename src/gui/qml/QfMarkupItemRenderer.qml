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

  /// type:QfMarkupItem
  property var markupItem: undefined

  /// type:QgsQuickMapSettings
  property MapSettings mapSettings

  property double lineWidth: 3
  property double pointSize: 20

  QfGeometryWrapper {
    id: geometryWrapper
    crs: QfCoordinateReferenceSystemUtils.wgs84Crs()
    qgsGeometry: markupItem.geometry
  }

  Component {
    id: linePolygonComponent

    QfLinePolygon {
      mapSettings: markupItemRenderer.mapSettings

      geometry: geometryWrapper
      color: markupItem.color
      lineWidth: markupItemRenderer.lineWidth
    }
  }

  Component {
    id: pointComponent

    Repeater {
      model: geometryWrapper.pointList()

      Rectangle {
        property CoordinateTransformer ct: CoordinateTransformer {
          id: _ct
          sourceCrs: geometryWrapper.crs
          sourcePosition: modelData
          destinationCrs: markupItemRenderer.mapSettings.destinationCrs
          transformContext: qgisProject ? qgisProject.transformContext : QfCoordinateReferenceSystemUtils.emptyTransformContext()
        }

        QfMapToScreen {
          id: mapToScreenPosition
          mapSettings: markupItemRenderer.mapSettings
          mapPoint: _ct.projectedPosition
        }

        x: mapToScreenPosition.screenPoint.x - width / 2
        y: mapToScreenPosition.screenPoint.y - width / 2

        color: Qt.hsla(markupItem.color.hslHue, markupItem.color.hslSaturation, markupItem.color.hslLightness, 0.5)
        width: markupItemRenderer.pointSize
        height: markupItemRenderer.pointSize
        radius: markupItemRenderer.pointSize / 2

        border.color: markupItem.color
        border.width: markupItemRenderer.lineWidth
      }
    }
  }

  Loader {
    sourceComponent: geometryWrapper.qgsGeometry.type === Qgis.GeometryType.Point ? pointComponent : linePolygonComponent
  }
}
