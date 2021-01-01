import QtQuick 2.12

import org.qgis 1.0
import org.qfield 1.0
import Theme 1.0
import ".."

VisibilityFadingRow {
    id: reshapeToolbar

    signal finished()

    property FeatureModel featureModel
    property bool screenHovering: false //<! if the stylus pen is used, one should not use the add button

    readonly property bool blocking: drawPolygonToolbar.isDigitizing

    spacing: 4

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

    QfToolButton {
      id: stopToolButton
      iconSource: Theme.getThemeIcon( "ic_chevron_left_white_24dp" )
      round: true
      visible: !drawPolygonToolbar.rubberbandModel || drawPolygonToolbar.rubberbandModel.vertexCount < 2
      bgcolor: Theme.darkGray

      onClicked: {
          cancel()
          finished()
      }
    }

    DigitizingToolbar {
        id: drawPolygonToolbar
        showConfirmButton: true
        screenHovering: reshapeToolbar.screenHovering

        onConfirm: {
            rubberbandModel.frozen = true
            rubberbandModel.removeVertex()
            if (!featureModel.currentLayer.editBuffer())
                featureModel.currentLayer.startEditing()
            var result = GeometryUtils.reshapeFromRubberband(featureModel.currentLayer, featureModel.feature.id, rubberbandModel)
            if ( result !== QgsGeometryStatic.Success )
            {
                displayToast( qsTr( 'An error when reshaping the geometry' ) );
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
        }
    }

    function init(featureModel, mapSettings, editorRubberbandModel)
    {
        reshapeToolbar.featureModel = featureModel
        drawPolygonToolbar.rubberbandModel = editorRubberbandModel
        drawPolygonToolbar.rubberbandModel.geometryType = QgsWkbTypes.PolygonGeometry
        drawPolygonToolbar.mapSettings = mapSettings
        drawPolygonToolbar.stateVisible = true
    }

    function cancel()
    {
        drawPolygonToolbar.cancel()
    }
}
