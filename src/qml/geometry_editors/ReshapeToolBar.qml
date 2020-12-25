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
                cancel()
                finished()
            }
            else
            {
                featureModel.currentLayer.commitChanges()
                cancel()
                finished()
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
