import ".."
import QtQuick 2.12
import org.qgis 1.0
import org.qfield 1.0
import Theme 1.0
import Utils 1.0


VisibilityFadingRow {
  id: fillRingToolbar

  signal finished()

  property FeatureModel featureModel
  readonly property bool blocking: drawPolygonToolbar.isDigitizing

  spacing: 4 * dp

  DigitizingToolbar {
    id: drawPolygonToolbar
    showConfirmButton: true

    onConfirm: {
      if (!featureModel.currentLayer.editBuffer())
        featureModel.currentLayer.startEditing()
      var result = QFieldUtils.addRingFromRubberBand(featureModel.currentLayer, featureModel.feature.id, rubberbandModel)
      if ( result !== QgsGeometryStatic.Success )
      {
        // TODO WARN
        /*
      AddRingNotClosed, //!< The input ring is not closed
      AddRingNotValid, //!< The input ring is not valid
      AddRingCrossesExistingRings, //!< The input ring crosses existing rings (it is not disjoint)
      AddRingNotInExistingFeature
      */
        featureModel.currentLayer.rollBack()
        cancel()
        finished()
      }
      else
      {
        var polygonGeometry = QFieldUtils.polygonFromRubberband(rubberbandModel, featureModel.currentLayer.crs)
        var feature = QFieldUtils.initFeature(featureModel.currentLayer, polygonGeometry)

        // Show form
        var popupComponent = Qt.createComponent("qrc:/qml/EmbeddedFeatureForm.qml")
        var embeddedPopup = popupComponent.createObject(mainWindow, {"parent" : mainWindow})

        embeddedPopup.onFeatureSaved.connect(newFeatureSaved)
        embeddedPopup.onFeatureCancelled.connect(newFeatureDiscarded)

        embeddedPopup.state = 'Add'
        embeddedPopup.attributeFormModel.featureModel.currentLayer = featureModel.currentLayer
        embeddedPopup.attributeFormModel.featureModel.resetAttributes()
        embeddedPopup.attributeFormModel.featureModel.feature = feature
        embeddedPopup.open()
      }
    }
  }

  function init(featureModel, mapSettings, editorRubberbandModel)
  {
    fillRingToolbar.featureModel = featureModel
    drawPolygonToolbar.rubberbandModel = editorRubberbandModel
    drawPolygonToolbar.rubberbandModel.geometryType = QgsWkbTypes.PolygonGeometry
    drawPolygonToolbar.mapSettings = mapSettings
    drawPolygonToolbar.stateVisible = true
  }

  function cancel()
  {
    drawPolygonToolbar.cancel()
  }

  function newFeatureSaved(){
    featureModel.currentLayer.commitChanges()
    cancel()
    finished()
  }

  function newFeatureDiscarded()
  {
    featureModel.currentLayer.rollBack()
    cancel()
    finished()
  }

}
