import QtQuick 2.12
import org.qgis 1.0
import org.qfield 1.0
import Theme 1.0

VisibilityFadingRow {
  id: splitFeatureToolbar

  property FeatureModel featureModel

  spacing: 4 * dp
  padding: 4 * dp

  DigitizingToolbar {
    id: drawLineToolbar
    mode: "measure"
  }

  function init(featureModel, mapSettings, editorRubberbandModel)
  {
    splitFeatureToolbar.featureModel = featureModel
    drawLineToolbar.rubberbandModel = editorRubberbandModel
    drawLineToolbar.mapSettings = mapSettings
    drawLineToolbar.stateVisible = true
  }



}
