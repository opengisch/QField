import QtQuick 2.11
import org.qgis 1.0
import org.qfield 1.0

Repeater {
  id: featureListSelectionHighlight
  property FeatureListModelSelection selectionModel
  property MapSettings mapSettings
  property color color: "yellow"
  property color selectionColor: "red"

  model: selectionModel.model

  delegate: GeometryRenderer {
    mapSettings: featureListSelectionHighlight.mapSettings
    geometryWrapper.qgsGeometry: model.geometry
    geometryWrapper.crs: model.crs

    color: selectionModel && model.index === selectionModel.selection ? "red" : "yellow"
  }

}
