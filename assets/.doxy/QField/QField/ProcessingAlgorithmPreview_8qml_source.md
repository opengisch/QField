

# File ProcessingAlgorithmPreview.qml

[**File List**](files.md) **>** [**qml**](dir_1d81f7f4362e4398c0a695c4e466fcb0.md) **>** [**ProcessingAlgorithmPreview.qml**](ProcessingAlgorithmPreview_8qml.md)

[Go to the documentation of this file](ProcessingAlgorithmPreview_8qml.md)


```C++
import QtQuick
import org.qgis
import org.qfield
import Theme

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
```


