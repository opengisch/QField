import QtQuick 2.14

import Theme 1.0
import org.qgis 1.0
import org.qfield 1.0

Repeater {
  id: processingAlgorithmPreview

  property ProcessingAlgorithm algorithm
  property MapSettings mapSettings
  property color color: Theme.processingPreview

  model: algorithm.previewGeometries

  delegate: GeometryRenderer {
    mapSettings: processingAlgorithmPreview.mapSettings
    geometryWrapper.qgsGeometry: modelData
    geometryWrapper.crs: processingAlgorithmPreview.algorithm.inPlaceLayer.crs

    color: processingAlgorithmPreview.color
    z: 1
  }
}
