

# File QfGeometryEditorFillRing.qml

[**File List**](files.md) **>** [**geometryeditors**](dir_bf8ee2ec6aa9a141ef86ab67bdbc4be6.md) **>** [**QfGeometryEditorFillRing.qml**](QfGeometryEditorFillRing_8qml.md)

[Go to the documentation of this file](QfGeometryEditorFillRing_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import org.qgis
import org.qfield.core
import org.qfield.gui

QfGeometryEditorBase {
  id: fillRingToolbar

  property bool screenHovering: false //<! if the stylus pen is used, one should not use the add button

  readonly property bool blocking: drawPolygonToolbar.isDigitizing

  property alias addPolygonDialog: addPolygonDialog
  property alias formPopupLoader: formPopupLoader

  spacing: 4

  function canvasClicked(point, type) {
    if (type === "stylus") {
      drawPolygonToolbar.addVertex();
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

  QfDigitizingToolbar {
    id: drawPolygonToolbar
    objectName: "fillRingDigitizingToolbar"
    showConfirmButton: true
    screenHovering: fillRingToolbar.screenHovering

    digitizingLogger.type: 'edit_fillring'

    QfEmbeddedFeatureForm {
      id: formPopupLoader
      state: 'Add'
      onRequestJumpToPoint: function (center, scale, handleMargins) {
        fillRingToolbar.requestJumpToPoint(center, scale, handleMargins);
      }
    }

    onConfirmed: {
      digitizingLogger.writeCoordinates();
      rubberbandModel.frozen = true;
      var result = QfGeometryUtils.addRingFromRubberband(featureModel.currentLayer, featureModel.feature.id, rubberbandModel);
      if (result !== QfGeometryUtils.Success) {
        if (result === QfGeometryUtils.AddRingNotClosed)
          displayToast(qsTr('The ring is not closed'), 'error');
        else if (result === QfGeometryUtils.AddRingNotValid)
          displayToast(qsTr('The ring is not valid'), 'error');
        else if (result === QfGeometryUtils.AddRingCrossesExistingRings)
          displayToast(qsTr('The ring crosses existing rings (it is not disjoint)'), 'error');
        else if (result === QfGeometryUtils.AddRingNotInExistingFeature)
          displayToast(qsTr('The ring doesn\'t have any existing ring to fit into'), 'error');
        else
          displayToast(qsTr('Unknown error when creating the ring'), 'error');
        drawPolygonToolbar.rubberbandModel.reset();
      } else {
        addPolygonDialog.open();
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
      drawPolygonToolbar.rubberbandModel.reset();
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

  function commitRingFeature() {
    featureModel.currentLayer.commitChanges();
    drawPolygonToolbar.rubberbandModel.reset();
  }

  function cancelRingFeature() {
    featureModel.currentLayer.rollBack();
    drawPolygonToolbar.rubberbandModel.reset();
  }

  function fillWithPolygon() {
    var polygonGeometry = QfGeometryUtils.polygonFromRubberband(drawPolygonToolbar.rubberbandModel, featureModel.currentLayer.crs, featureModel.currentLayer.wkbType());
    var feature = QfFeatureUtils.createBlankFeature(featureModel.currentLayer.fields, polygonGeometry);

    formPopupLoader.onFeatureSaved.connect(commitRingFeature);
    formPopupLoader.onFeatureCancelled.connect(cancelRingFeature);

    formPopupLoader.currentLayer = fillRingToolbar.featureModel.currentLayer;
    formPopupLoader.feature = feature;
    formPopupLoader.open();
  }
}
```


