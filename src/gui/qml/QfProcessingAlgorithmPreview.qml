import QtQuick
import org.qgis
import org.qfield.core
import org.qfield.gui

/**
 * \ingroup qml_gui
 */
Repeater {
  id: processingAlgorithmPreview

  property QfProcessingAlgorithm algorithm
  /// type:QgsQuickMapSettings
  property MapSettings mapSettings
  property color color: QfTheme.processingPreview

  model: algorithm.previewGeometries

  delegate: QfGeometryRenderer {
    mapSettings: processingAlgorithmPreview.mapSettings
    geometryWrapper.crs: processingAlgorithmPreview.algorithm.inPlaceLayer.crs
    geometryWrapper.qgsGeometry: modelData

    color: processingAlgorithmPreview.color
    z: 1
  }
}
