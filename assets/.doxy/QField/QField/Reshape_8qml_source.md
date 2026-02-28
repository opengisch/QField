

# File Reshape.qml

[**File List**](files.md) **>** [**geometryeditors**](dir_a044c600d9d985eeadf51620d76e3ed0.md) **>** [**Reshape.qml**](Reshape_8qml.md)

[Go to the documentation of this file](Reshape_8qml.md)


```C++
import QtQuick
import org.qgis
import org.qfield
import Theme
import "."
import ".."

GeometryEditorBase {
  id: reshapeToolbar

  property bool screenHovering: false //<! if the stylus pen is used, one should not use the add button

  readonly property bool blocking: drawPolygonToolbar.isDigitizing

  spacing: 4

  function canvasClicked(point, type) {
    if (type === "stylus") {
      drawPolygonToolbar.addVertex();
      return true;
    }
    return false;
  }

  function canvasLongPressed(point, type) {
    if (type === "stylus") {
      drawPolygonToolbar.confirm();
      return true;
    }
    return false;
  }

  DigitizingToolbar {
    id: drawPolygonToolbar
    showConfirmButton: true
    screenHovering: reshapeToolbar.screenHovering

    digitizingLogger.type: 'edit_reshape'

    onConfirmed: {
      digitizingLogger.writeCoordinates();
      rubberbandModel.frozen = true;
      if (!featureModel.currentLayer.editBuffer())
        featureModel.currentLayer.startEditing();
      var result = GeometryUtils.reshapeFromRubberband(featureModel.currentLayer, featureModel.feature.id, rubberbandModel);
      if (result !== GeometryUtils.Success) {
        displayToast(qsTr('The geometry could not be reshaped'), 'error');
        featureModel.currentLayer.rollBack();
        rubberbandModel.reset();
      } else {
        featureModel.currentLayer.commitChanges();
        rubberbandModel.reset();
        featureModel.refresh();
        featureModel.applyGeometryToVertexModel();
      }
    }

    onCancel: {
      rubberbandModel.reset();
    }
  }

  function init(featureModel, mapSettings, editorRubberbandModel, editorRenderer) {
    reshapeToolbar.featureModel = featureModel;
    drawPolygonToolbar.digitizingLogger.digitizingLayer = featureModel.currentLayer;
    drawPolygonToolbar.rubberbandModel = editorRubberbandModel;
    drawPolygonToolbar.rubberbandModel.geometryType = Qgis.GeometryType.Polygon;
    drawPolygonToolbar.mapSettings = mapSettings;
    drawPolygonToolbar.stateVisible = true;
  }

  function cancel() {
    drawPolygonToolbar.cancel();
  }
}
```


