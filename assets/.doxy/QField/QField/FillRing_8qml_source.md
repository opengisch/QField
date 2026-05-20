

# File FillRing.qml

[**File List**](files.md) **>** [**geometryeditors**](dir_a044c600d9d985eeadf51620d76e3ed0.md) **>** [**FillRing.qml**](FillRing_8qml.md)

[Go to the documentation of this file](FillRing_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import org.qgis
import org.qfield
import Theme
import "."
import ".."

GeometryEditorBase {
  id: fillRingToolbar

  property bool screenHovering: false //<! if the stylus pen is used, one should not use the add button

  readonly property bool blocking: drawPolygonToolbar.isDigitizing

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

  DigitizingToolbar {
    id: drawPolygonToolbar
    showConfirmButton: true
    screenHovering: fillRingToolbar.screenHovering

    digitizingLogger.type: 'edit_fillring'

    EmbeddedFeatureForm {
      id: formPopupLoader
      state: 'Add'
      currentLayer: featureModel && featureModel.currentLayer
      onRequestJumpToPoint: function (center, scale, handleMargins) {
        fillRingToolbar.requestJumpToPoint(center, scale, handleMargins);
      }
    }

    onConfirmed: {
      digitizingLogger.writeCoordinates();
      rubberbandModel.frozen = true;
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
    var polygonGeometry = GeometryUtils.polygonFromRubberband(drawPolygonToolbar.rubberbandModel, featureModel.currentLayer.crs, featureModel.currentLayer.wkbType());
    var feature = FeatureUtils.createBlankFeature(featureModel.currentLayer.fields, polygonGeometry);

    // Show form
    formPopupLoader.onFeatureSaved.connect(commitRingFeature);
    formPopupLoader.onFeatureCancelled.connect(cancelRingFeature);
    formPopupLoader.feature = feature;
    formPopupLoader.open();
  }
}
```


