import QtQuick 2.14

import org.qgis 1.0
import org.qfield 1.0

Repeater {
  id: featureListSelectionHighlight
  property FeatureListModelSelection selectionModel
  property MapSettings mapSettings
  property double translateX: 0.0
  property double translateY: 0.0
  property color color: "yellow"
  property color focusedColor: "red"
  property color selectedColor: Theme.mainColor
  property bool showSelectedOnly: false

  model: selectionModel.model

  delegate: GeometryRenderer {
    mapSettings: featureListSelectionHighlight.mapSettings
    geometryWrapper.qgsGeometry: model.geometry
    geometryWrapper.crs: model.crs

    visible: featureListSelectionHighlight.visible && ( !showSelectedOnly || model.featureSelected )
    color: model.featureSelected ? featureListSelectionHighlight.selectedColor : selectionModel.model.selectedCount === 0 && selectionModel && model.index === selectionModel.focusedItem ? featureListSelectionHighlight.focusedColor : featureListSelectionHighlight.color
    borderColor: "white"

    transform: Translate {
        x: featureListSelectionHighlight.translateX
        y: -featureListSelectionHighlight.translateY
    }
  }
}
