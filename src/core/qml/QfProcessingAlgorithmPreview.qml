import QtQuick
import org.qgis
import org.qfield.core

/**
 * \ingroup qml
 */
Repeater {
  id: processingAlgorithmPreview

  property ProcessingAlgorithm algorithm
  /// type:QgsQuickMapSettings
  property MapSettings mapSettings
  property color color: Theme.processingPreview

  model: algorithm.previewGeometries

  delegate: QfGeometryRenderer {
    mapSettings: processingAlgorithmPreview.mapSettings
    geometryWrapper.qgsGeometry: modelData
    geometryWrapper.crs: processingAlgorithmPreview.algorithm.inPlaceLayer.crs

    color: processingAlgorithmPreview.color
    z: 1
  }
}
