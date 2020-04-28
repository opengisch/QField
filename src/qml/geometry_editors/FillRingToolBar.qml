import ".."
import QtQuick 2.12
import org.qgis 1.0
import org.qfield 1.0
import Theme 1.0

VisibilityFadingRow {
  id: fillRingToolbar

  signal finished()

  property FeatureModel featureModel
  property bool screenHovering: false //<! if the stylus pen is used, one should not use the add button

  readonly property bool blocking: drawPolygonToolbar.isDigitizing

  spacing: 4 * dp

  function canvasClicked(point)
  {
    var mapPoint = drawPolygonToolbar.mapSettings.screenToCoordinate(point)
    drawPolygonToolbar.rubberbandModel.addVertexFromPoint(mapPoint)
    return true // handled
  }

  function canvasLongPressed(point)
  {
    var mapPoint = drawPolygonToolbar.mapSettings.screenToCoordinate(point)
    drawPolygonToolbar.rubberbandModel.addVertexFromPoint(mapPoint)
    drawPolygonToolbar.confirm()
    return true // handled
  }

  DigitizingToolbar {
    id: drawPolygonToolbar
    showConfirmButton: true
    screenHovering: fillRingToolbar.screenHovering

    EmbeddedFeatureForm {
      id: formPopupLoader
      state: 'Add'
      currentLayer: featureModel && featureModel.currentLayer
    }

    onConfirm: {
      rubberbandModel.frozen = true
      if (!featureModel.currentLayer.editBuffer())
        featureModel.currentLayer.startEditing()
      var result = GeometryUtils.addRingFromRubberBand(featureModel.currentLayer, featureModel.feature.id, rubberbandModel)
      if ( result !== QgsGeometryStatic.Success )
      {
        if ( result === QgsGeometryStatic.AddRingNotClosed )
          displayToast( qsTr( 'Error: the input ring is not closed' ) );
        else if ( result === QgsGeometryStatic.AddRingNotValid )
          displayToast( qsTr( 'Error: the input ring is not valid' ) );
        else if ( result === QgsGeometryStatic.AddRingCrossesExistingRings )
          displayToast( qsTr( 'Error: the input ring crosses existing rings (it is not disjoint)' ) );
        else if ( result === QgsGeometryStatic.AddRingNotInExistingFeature )
          displayToast( qsTr( 'Error: the input ring doesn\'t have any existing ring to fit into' ) );
        else
          displayToast( qsTr( 'Unknown error when creating the ring' ) );

        featureModel.currentLayer.rollBack()
        cancel()
        finished()
      }
      else
      {
        questionDialog.questionText = qsTr("Would you like to add a ring or rather fill a new polygon in the ring?")
        questionDialog.nButtons = 3
        questionDialog.button1Text = qsTr("Cancel")
        questionDialog.button2Text = qsTr("Add ring")
        questionDialog.button3Text = qsTr("Fill poylgon")

        questionDialog.button1Clicked.connect(rollbackRingAndCancel)
        questionDialog.button2Clicked.connect(commitAndFinish)
        questionDialog.button3Clicked.connect(fillPolygon)
        questionDialog.open()
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

  function commitAndFinish(){
    featureModel.currentLayer.commitChanges()
    cancel()
    finished()
  }

  function rollbackRingAndCancel()
  {
    featureModel.currentLayer.rollBack()
    cancel()
    finished()
  }

  function fillPolygon()
  {
    var polygonGeometry = GeometryUtils.polygonFromRubberband(drawPolygonToolbar.rubberbandModel, featureModel.currentLayer.crs)
    var feature = FeatureUtils.initFeature(featureModel.currentLayer, polygonGeometry)

    // Show form
    formPopupLoader.onFeatureSaved.connect(commitAndFinish)
    formPopupLoader.onFeatureCancelled.connect(rollbackRingAndCancel)

    formPopupLoader.feature = feature
    formPopupLoader.open()
  }
}
