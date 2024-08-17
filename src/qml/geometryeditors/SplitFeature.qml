import QtQuick
import org.qgis
import org.qfield
import Theme
import ".."

QfVisibilityFadingRow {
  id: splitFeatureToolbar

  signal finished

  property FeatureModel featureModel
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

  DigitizingToolbar {
    id: drawLineToolbar
    showConfirmButton: true
    screenHovering: splitFeatureToolbar.screenHovering

    digitizingLogger.type: 'edit_split'

    onConfirmed: {
      digitizingLogger.writeCoordinates();
      rubberbandModel.frozen = true;
      // TODO: featureModel.currentLayer.selectByIds([featureModel.feature.id], VectorLayerStatic.SetSelection)
      LayerUtils.selectFeaturesInLayer(featureModel.currentLayer, [featureModel.feature.id], VectorLayerStatic.SetSelection);
      if (!featureModel.currentLayer.editBuffer())
        featureModel.currentLayer.startEditing();
      var result = GeometryUtils.splitFeatureFromRubberband(featureModel.currentLayer, drawLineToolbar.rubberbandModel);
      if (result !== GeometryUtils.Success) {
        displayToast(qsTr('Feature could not be split'), 'error');
        featureModel.currentLayer.rollBack();
        rubberbandModel.reset();
        cancel();
        finished();
      } else {
        featureModel.currentLayer.commitChanges();
        rubberbandModel.reset();
        cancel();
        finished();
      }
      featureModel.currentLayer.removeSelection();
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
