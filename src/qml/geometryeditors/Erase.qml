import QtQuick 2.14

import org.qgis 1.0
import org.qfield 1.0
import Theme 1.0

import ".."

VisibilityFadingRow {
  id: eraseToolbar

  signal finished()

  property FeatureModel featureModel
  property GeometryRenderer editorRenderer
  property bool screenHovering: false //<! if the stylus pen is used, one should not use the add button

  readonly property bool blocking: drawPolygonToolbar.isDigitizing

  spacing: 4

  function canvasClicked(point)
  {
    drawPolygonToolbar.addVertex()
    return true // handled
  }

  function canvasLongPressed(point)
  {
    drawPolygonToolbar.confirm()
    return true // handled
  }

  function canvasFreehandBegin()
  {
    geometryEditorsRubberband.visible = false
    drawPolygonToolbar.addVertex()
    drawPolygonToolbar.rubberbandModel.measureValue = 20.0
    return true // handled
  }

  function canvasFreehandEnd()
  {
    drawPolygonToolbar.rubberbandModel.measureValue = 0.0
    drawPolygonToolbar.addVertex();
    return true // handled
  }

  DigitizingToolbar {
    id: drawPolygonToolbar
    showConfirmButton: true
    screenHovering: eraseToolbar.screenHovering

    digitizingLogger.type: 'edit_erase'
    digitizingLogger.digitizingLayer: featureModel.currentLayer

    function addVertex() {
      digitizingLogger.addCoordinate( coordinateLocator.currentCoordinate )
      coordinateLocator.flash()

      rubberbandModel.addVertex()
    }

    onConfirmed: {
      digitizingLogger.writeCoordinates()

      rubberbandModel.frozen = true
      if (!featureModel.currentLayer.editBuffer())
          featureModel.currentLayer.startEditing()
      var result = GeometryUtils.eraseFromRubberband(featureModel.currentLayer, featureModel.feature.id, rubberbandModel)
      if ( result !== GeometryUtils.Success )
      {
          displayToast( qsTr( 'The geometry could not be erased' ), 'error' );
          featureModel.currentLayer.rollBack()
          rubberbandModel.reset()
      }
      else
      {
          featureModel.currentLayer.commitChanges()
          rubberbandModel.reset()
          featureModel.refresh()
          featureModel.applyGeometryToVertexModel()
      }
      rubberbandModel.reset()
      eraseToolbar.editorRenderer.geometryWrapper.clear()
    }

    onCancel: {
      rubberbandModel.reset()
      eraseToolbar.editorRenderer.geometryWrapper.clear()
    }

    onVertexCountChanged: {
      editorRenderer.geometryWrapper.crs = featureModel.currentLayer.crs
      editorRenderer.geometryWrapper.qgsGeometry = GeometryUtils.variableWidthBufferByMFromRubberband(drawPolygonToolbar.rubberbandModel, featureModel.currentLayer.crs)
    }
  }

  function init(featureModel, mapSettings, editorRubberbandModel, editorRenderer)
  {
    eraseToolbar.featureModel = featureModel
    eraseToolbar.editorRenderer = editorRenderer
    eraseToolbar.editorRenderer.mapSettings = mapSettings
    drawPolygonToolbar.rubberbandModel = editorRubberbandModel
    drawPolygonToolbar.rubberbandModel.geometryType = Qgis.GeometryType.Line
    drawPolygonToolbar.rubberbandModel.measureValue = 20.0
    drawPolygonToolbar.mapSettings = mapSettings
    drawPolygonToolbar.stateVisible = true
  }

  function cancel()
  {
    drawPolygonToolbar.cancel()
    geometryEditorsRubberband.visible = true
  }
}
