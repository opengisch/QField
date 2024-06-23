import QtQuick 2.14

import org.qgis 1.0
import org.qfield 1.0

Repeater {
  id: processingAlgorithmPreview

  property ProcessingAlgorithm algorithm
  property MapSettings mapSettings
  property color color: "#99000000"

  model: algorithm.previewGeometries

  delegate: GeometryRenderer {
    mapSettings: processingAlgorithmPreview.mapSettings
    geometryWrapper.qgsGeometry: modelData
    geometryWrapper.crs: processingAlgorithmPreview.algorithm.inPlaceLayer.crs

    color: processingAlgorithmPreview.color
    z: 1
  }
}
