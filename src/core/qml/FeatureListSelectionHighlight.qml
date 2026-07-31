import QtQuick
import org.qgis
import org.qfield

/**
 * \ingroup qml
 */
Repeater {
  id: featureListSelectionHighlight

  property FeatureListModelSelection selectionModel
  /// type:QgsQuickMapSettings
  property MapSettings mapSettings
  property double translateX: 0.0
  property double translateY: 0.0
  property double rotationDegrees: 0.0
  property color color: "yellow"
  property color focusedColor: "red"
  property color selectedColor: Theme.mainColor
  property bool showSelectedOnly: false
  property double originX: 0.0
  property double originY: 0.0

  model: selectionModel.model

  delegate: GeometryRenderer {
    mapSettings: featureListSelectionHighlight.mapSettings
    geometryWrapper.qgsGeometry: model.geometry
    geometryWrapper.crs: model.crs

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
