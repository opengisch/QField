import QtQuick
import QtQuick.Controls
import org.qgis
import org.qfield
import Theme
import ".."

QfVisibilityFadingRow {
  id: fillRingToolbar

  signal finished

  property FeatureModel featureModel
  property bool screenHovering: false //<! if the stylus pen is used, one should not use the add button

  readonly property bool blocking: drawPolygonToolbar.isDigitizing

  spacing: 4

  function canvasClicked(point, type) {
    if (type === "stylus") {
      if (LayerUtils.isCurvedGeometry(featureModel.currentLayer) == true && settings.valueBool("/QField/Digitizing/CurveEdition", false) == true) {
        if (drawPolygonToolbar.rubberbandModel.isDuringCurveDrawing() == true || drawPolygonToolbar.rubberbandModel.vertexCount == 1) {
          if (drawPolygonToolbar.rubberbandModel.vertexCount != 1) {
            drawPolygonToolbar.addCurve();
          } else {
            drawPolygonToolbar.addVertex();
          }
        } else {
          drawPolygonToolbar.addMiddlePointCurve();
        }
      } else {
        drawPolygonToolbar.addVertex();
      }
      return true;
    }
    return false;
  }

  function canvasLongPressed(point, type) {
    if (type === "stylus") {
      drawPolygonToolbar.confirm();
      return true;
    }
    return false;
  }

  DigitizingToolbar {
    id: drawPolygonToolbar
    showConfirmButton: true
    screenHovering: fillRingToolbar.screenHovering

    digitizingLogger.type: 'edit_fillring'

    EmbeddedFeatureForm {
      id: formPopupLoader
      state: 'Add'
      currentLayer: featureModel && featureModel.currentLayer
    }

    onConfirmed: {
      digitizingLogger.writeCoordinates();
      rubberbandModel.frozen = true;
      if (!featureModel.currentLayer.editBuffer())
        featureModel.currentLayer.startEditing();
      var result = GeometryUtils.addRingFromRubberband(featureModel.currentLayer, featureModel.feature.id, rubberbandModel);
      if (result !== GeometryUtils.Success) {
        if (result === GeometryUtils.AddRingNotClosed)
          displayToast(qsTr('The ring is not closed'), 'error');
        else if (result === GeometryUtils.AddRingNotValid)
          displayToast(qsTr('The ring is not valid'), 'error');
        else if (result === GeometryUtils.AddRingCrossesExistingRings)
          displayToast(qsTr('The ring crosses existing rings (it is not disjoint)'), 'error');
        else if (result === GeometryUtils.AddRingNotInExistingFeature)
          displayToast(qsTr('The ring doesn\'t have any existing ring to fit into'), 'error');
        else
          displayToast(qsTr('Unknown error when creating the ring'), 'error');
        featureModel.currentLayer.rollBack();
        drawPolygonToolbar.rubberbandModel.reset();
      } else {
        addPolygonDialog.show();
      }
    }

    onCancel: {
      rubberbandModel.reset();
    }
  }

  QfDialog {
    id: addPolygonDialog
    parent: mainWindow.contentItem
    title: qsTr("Fill ring")
    Label {
      width: parent.width
      wrapMode: Text.WordWrap
      text: qsTr("Would you like to fill the ring with a new polygon?")
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

  function init(featureModel, mapSettings, editorRubberbandModel, editorRenderer) {
    fillRingToolbar.featureModel = featureModel;
    drawPolygonToolbar.digitizingLogger.digitizingLayer = featureModel.currentLayer;
    drawPolygonToolbar.rubberbandModel = editorRubberbandModel;
    drawPolygonToolbar.rubberbandModel.geometryType = Qgis.GeometryType.Polygon;
    drawPolygonToolbar.mapSettings = mapSettings;
    drawPolygonToolbar.stateVisible = true;
  }

  function cancel() {
    drawPolygonToolbar.cancel();
  }

  function commitRing() {
    featureModel.currentLayer.commitChanges();
    drawPolygonToolbar.rubberbandModel.reset();
  }

  function cancelRing() {
    featureModel.currentLayer.rollBack();
    drawPolygonToolbar.rubberbandModel.reset();
  }

  function fillWithPolygon() {
    var polygonGeometry = GeometryUtils.polygonFromRubberband(drawPolygonToolbar.rubberbandModel, featureModel.currentLayer.crs, featureModel.currentLayer.wkbType());
    var feature = FeatureUtils.createBlankFeature(featureModel.currentLayer.fields, polygonGeometry);

    // Show form
    formPopupLoader.onFeatureSaved.connect(commitRing);
    formPopupLoader.onFeatureCancelled.connect(cancelRing);
    formPopupLoader.feature = feature;
    formPopupLoader.open();
  }
}
