import QtQuick 2.12
import QtQuick.Controls 2.12

import org.qgis 1.0
import org.qfield 1.0
import Theme 1.0
import ".."

VisibilityFadingRow {
  id: fillRingToolbar

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
    screenHovering: fillRingToolbar.screenHovering

    EmbeddedFeatureForm {
      id: formPopupLoader
      state: 'Add'
      currentLayer: featureModel && featureModel.currentLayer
    }

    onConfirm: {
      rubberbandModel.frozen = true
      rubberbandModel.removeVertex()
      if (!featureModel.currentLayer.editBuffer())
        featureModel.currentLayer.startEditing()
      var result = GeometryUtils.addRingFromRubberband(featureModel.currentLayer, featureModel.feature.id, rubberbandModel)
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
      }
      else
      {
        addPolygonDialog.show();
      }
    }
  }

  Dialog {
    id: addPolygonDialog
    parent: mainWindow.contentItem

    visible: false
    modal: true

    x: ( mainWindow.width - width ) / 2
    y: ( mainWindow.height - height ) / 2

    title: qsTr( "Fill ring" )
    Label {
      width: parent.width
      wrapMode: Text.WordWrap
      text: qsTr( "Would you like to fill the ring with a new polygon?" )
    }

    standardButtons: Dialog.Yes | Dialog.No

    onAccepted: {
        fillWithPolygon();
    }

    onRejected: {
        commitRing();
    }

    function show() {
        this.open();
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

  function commitRing() {
    featureModel.currentLayer.commitChanges()
    drawPolygonToolbar.rubberbandModel.reset()
  }

  function cancelRing() {
    featureModel.currentLayer.rollBack()
    drawPolygonToolbar.rubberbandModel.reset()
  }

  function fillWithPolygon()
  {
    var polygonGeometry = GeometryUtils.polygonFromRubberband(drawPolygonToolbar.rubberbandModel, featureModel.currentLayer.crs)
    var feature = FeatureUtils.initFeature(featureModel.currentLayer, polygonGeometry)

    // Show form
    formPopupLoader.onFeatureSaved.connect(commitRing)
    formPopupLoader.onFeatureCancelled.connect(cancelRing)

    formPopupLoader.feature = feature
    formPopupLoader.open()
  }
}
