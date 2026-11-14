

# File QfFeatureListSelectionHighlight.qml

[**File List**](files.md) **>** [**app**](dir_5194e5fea318fda12687127c23f8aba1.md) **>** [**qml**](dir_408f7dc2ccecf14179dc3d37678eaa5b.md) **>** [**QfFeatureListSelectionHighlight.qml**](QfFeatureListSelectionHighlight_8qml.md)

[Go to the documentation of this file](QfFeatureListSelectionHighlight_8qml.md)


```C++
import QtQuick
import org.qgis
import org.qfield.core
import org.qfield.gui

Repeater {
  id: featureListSelectionHighlight

  property QfFeatureListModelSelection selectionModel
  property MapSettings mapSettings
  property double translateX: 0.0
  property double translateY: 0.0
  property double rotationDegrees: 0.0
  property color color: "yellow"
  property color focusedColor: "red"
  property color selectedColor: QfTheme.mainColor
  property bool showSelectedOnly: false
  property double originX: 0.0
  property double originY: 0.0

  model: selectionModel.model

  delegate: QfGeometryRenderer {
    mapSettings: featureListSelectionHighlight.mapSettings
    geometryWrapper.crs: model.crs
    geometryWrapper.qgsGeometry: model.geometry

    visible: featureListSelectionHighlight.visible && (!showSelectedOnly || model.featureSelected)
    color: model.featureSelected ? featureListSelectionHighlight.selectedColor : selectionModel.model.selectedCount === 0 && selectionModel && model.index === selectionModel.focusedItem ? featureListSelectionHighlight.focusedColor : featureListSelectionHighlight.color
    z: model.index === selectionModel.focusedItem ? 1 : 0

    transform: [
      Translate {
        x: featureListSelectionHighlight.translateX
        y: -featureListSelectionHighlight.translateY
      },
      Rotation {
        origin.x: featureListSelectionHighlight.originX
        origin.y: featureListSelectionHighlight.originY
        angle: featureListSelectionHighlight.rotationDegrees
      }
    ]
  }
}
```


