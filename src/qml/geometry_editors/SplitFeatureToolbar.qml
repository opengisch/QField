import QtQuick 2.14

import org.qgis 1.0
import org.qfield 1.0
import Theme 1.0
import ".."


VisibilityFadingRow {
  id: splitFeatureToolbar

  signal finished()

  property FeatureModel featureModel
  property bool screenHovering: false //<! if the stylus pen is used, one should not use the add button
  readonly property bool blocking: drawLineToolbar.isDigitizing

  spacing: 4


  function canvasClicked(point)
  {
    drawLineToolbar.addVertex();
    return true; // handled
  }

  function canvasLongPressed(point)
  {
    drawLineToolbar.confirm();
    return true; // handled
  }

  DigitizingToolbar {
    id: drawLineToolbar
    showConfirmButton: true
    screenHovering: splitFeatureToolbar.screenHovering

    digitizingLogger.type: 'edit_split'
    digitizingLogger.digitizingLayer: featureModel.currentLayer

    onConfirmed: {
      digitizingLogger.writeCoordinates()

      rubberbandModel.frozen = true
      // TODO: featureModel.currentLayer.selectByIds([featureModel.feature.id], VectorLayerStatic.SetSelection)
      LayerUtils.selectFeaturesInLayer(featureModel.currentLayer, [featureModel.feature.id], VectorLayerStatic.SetSelection)
      if (!featureModel.currentLayer.editBuffer())
        featureModel.currentLayer.startEditing()

      var result = GeometryUtils.splitFeatureFromRubberband(featureModel.currentLayer, drawLineToolbar.rubberbandModel)
      if ( result !== GeometryUtils.Success )
      {
        displayToast( qsTr( 'Feature could not be split' ), 'error' );
        featureModel.currentLayer.rollBack()
        rubberbandModel.reset()
        cancel()
        finished()
      }
      else
      {
        featureModel.currentLayer.commitChanges()
        rubberbandModel.reset()
        cancel()
        finished()
      }

      featureModel.currentLayer.removeSelection()
    }
  }

  function init(featureModel, mapSettings, editorRubberbandModel)
  {
    splitFeatureToolbar.featureModel = featureModel
    drawLineToolbar.rubberbandModel = editorRubberbandModel
    drawLineToolbar.rubberbandModel.geometryType = QgsWkbTypes.LineGeometry
    drawLineToolbar.mapSettings = mapSettings
    drawLineToolbar.stateVisible = true
  }

  function cancel()
  {
    drawLineToolbar.cancel()
  }

}
