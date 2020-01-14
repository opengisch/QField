import QtQuick 2.12
import org.qgis 1.0
import org.qfield 1.0
import Theme 1.0
import Utils 1.0


VisibilityFadingRow {
  id: splitFeatureToolbar

  signal finished()

  property FeatureModel featureModel
  readonly property bool blocking: drawLineToolbar.isDigitizing

  spacing: 4 * dp

  DigitizingToolbar {
    id: drawLineToolbar
    showConfirmButton: true

    onConfirm: {
      // TODO: featureModel.currentLayer.selectByIds([featureModel.feature.id], VectorLayerStatic.SetSelection)
      Utils.selectFeaturesInLayer(featureModel.currentLayer, [featureModel.feature.id], VectorLayerStatic.SetSelection)
      var line = drawLineToolbar.rubberbandModel.pointSequence(featureModel.currentLayer.crs)
      if (!featureModel.currentLayer.editBuffer())
        featureModel.currentLayer.startEditing()
      featureModel.currentLayer.splitFeatures(line) // TODO: add option for topological mode
      featureModel.currentLayer.commitChanges()
      cancel()
      finished()
    }
  }

  function init(featureModel, mapSettings, editorRubberbandModel)
  {
    splitFeatureToolbar.featureModel = featureModel
    drawLineToolbar.rubberbandModel = editorRubberbandModel
    drawLineToolbar.mapSettings = mapSettings
    drawLineToolbar.stateVisible = true
  }

  function cancel()
  {
    drawLineToolbar.cancel()
  }

}
