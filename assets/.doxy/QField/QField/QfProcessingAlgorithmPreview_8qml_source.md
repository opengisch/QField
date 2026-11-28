

# File QfProcessingAlgorithmPreview.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfProcessingAlgorithmPreview.qml**](QfProcessingAlgorithmPreview_8qml.md)

[Go to the documentation of this file](QfProcessingAlgorithmPreview_8qml.md)


```C++
import QtQuick
import org.qgis
import org.qfield.core
import org.qfield.gui

Repeater {
  id: processingAlgorithmPreview

  property QfProcessingAlgorithm algorithm
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
```


