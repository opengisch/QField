

# File QfGeometryEditorSplitFeature.qml

[**File List**](files.md) **>** [**geometryeditors**](dir_bf8ee2ec6aa9a141ef86ab67bdbc4be6.md) **>** [**QfGeometryEditorSplitFeature.qml**](QfGeometryEditorSplitFeature_8qml.md)

[Go to the documentation of this file](QfGeometryEditorSplitFeature_8qml.md)


```C++
import QtQuick
import org.qgis
import org.qfield.core
import org.qfield.gui

QfGeometryEditorBase {
  id: splitFeatureToolbar

  property bool screenHovering: false //<! if the stylus pen is used, one should not use the add button
  readonly property bool blocking: drawLineToolbar.isDigitizing

  spacing: 4

  function canvasClicked(point, type) {
    if (type === "stylus") {
      drawLineToolbar.addVertex();
      return true;
    }
    return false;
  }

  function canvasLongPressed(point, type) {
    if (type === "stylus") {
      drawLineToolbar.confirm();
      return true;
    }
    return false;
  }

  QfDigitizingToolbar {
    id: drawLineToolbar
    objectName: "splitDigitizingToolbar"
    showConfirmButton: true
    screenHovering: splitFeatureToolbar.screenHovering

    digitizingLogger.type: 'edit_split'

    onConfirmed: {
      digitizingLogger.writeCoordinates();
      rubberbandModel.frozen = true;
      const result = QfGeometryUtils.splitFeatureFromRubberband(featureModel.currentLayer, featureModel.feature.id, drawLineToolbar.rubberbandModel);
      if (result !== QfGeometryUtils.Success) {
        displayToast(qsTr('Feature could not be split'), 'error');
      }
      rubberbandModel.reset();
      cancel();
      finished();
    }

    onCancel: {
      rubberbandModel.reset();
    }
  }

  function init(featureModel, mapSettings, editorRubberbandModel, editorRenderer) {
    splitFeatureToolbar.featureModel = featureModel;
    drawLineToolbar.digitizingLogger.digitizingLayer = featureModel.currentLayer;
    drawLineToolbar.rubberbandModel = editorRubberbandModel;
    drawLineToolbar.rubberbandModel.geometryType = Qgis.GeometryType.Line;
    drawLineToolbar.mapSettings = mapSettings;
    drawLineToolbar.stateVisible = true;
  }

  function cancel() {
    drawLineToolbar.cancel();
  }
}
```


