

# File Erase.qml

[**File List**](files.md) **>** [**geometryeditors**](dir_a044c600d9d985eeadf51620d76e3ed0.md) **>** [**Erase.qml**](Erase_8qml.md)

[Go to the documentation of this file](Erase_8qml.md)


```C++
import QtQuick
import org.qgis
import org.qfield
import Theme
import "."
import ".."

GeometryEditorBase {
  id: eraseToolbar

  property bool screenHovering: false //<! if the stylus pen is used, one should not use the add button

  property int size: sizeButton.sizeMedium

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

  function canvasFreehandBegin() {
    geometryEditorsRubberband.visible = false;
    drawPolygonToolbar.rubberbandModel.measureValue = 0.0;
    drawPolygonToolbar.addVertex();
    adjustSize();
    return true; // handled
  }

  function canvasFreehandEnd() {
    drawPolygonToolbar.rubberbandModel.measureValue = 0.0;
    drawPolygonToolbar.addVertex();
    return true; // handled
  }

  DigitizingToolbar {
    id: drawPolygonToolbar
    showConfirmButton: true
    screenHovering: eraseToolbar.screenHovering

    digitizingLogger.type: 'edit_erase'

    function addVertex() {
      digitizingLogger.addCoordinate(coordinateLocator.currentCoordinate);
      coordinateLocator.flash();
      rubberbandModel.addVertex();
    }

    onConfirmed: {
      digitizingLogger.writeCoordinates();
      rubberbandModel.frozen = true;
      if (!featureModel.currentLayer.editBuffer()) {
        featureModel.currentLayer.startEditing();
      }
      var result = GeometryUtils.eraseFromRubberband(featureModel.currentLayer, featureModel.feature.id, rubberbandModel);
      if (result !== GeometryUtils.Success) {
        if (result === GeometryUtils.AddPartNotMultiGeometry) {
          displayToast(qsTr('The geometry could not be modified into multiple parts'), 'error');
        } else {
          displayToast(qsTr('The geometry could not be modified'), 'error');
        }
        featureModel.currentLayer.rollBack();
        rubberbandModel.reset();
      } else {
        featureModel.currentLayer.commitChanges();
        rubberbandModel.reset();
        featureModel.refresh();
        featureModel.applyGeometryToVertexModel();
      }
      rubberbandModel.reset();
      eraseToolbar.editorRenderer.geometryWrapper.clear();
    }

    onCancel: {
      rubberbandModel.reset();
      eraseToolbar.editorRenderer.geometryWrapper.clear();
    }

    onVertexCountChanged: {
      editorRenderer.geometryWrapper.crs = featureModel.currentLayer.crs;
      editorRenderer.geometryWrapper.qgsGeometry = GeometryUtils.variableWidthBufferByMFromRubberband(drawPolygonToolbar.rubberbandModel, featureModel.currentLayer.crs);
    }
  }

  Connections {
    target: drawPolygonToolbar.mapSettings

    function onExtentChanged() {
      adjustSize();
    }
  }

  QfToolButton {
    id: sizeButton

    property int sizeSmall: 2
    property int sizeMedium: 6
    property int sizeLarge: 12

    iconSource: eraseToolbar.size == sizeSmall ? Theme.getThemeVectorIcon("ic_size_small_white_24dp") : eraseToolbar.size == sizeMedium ? Theme.getThemeVectorIcon("ic_size_medium_white_24dp") : Theme.getThemeVectorIcon("ic_size_large_white_24dp")
    iconColor: Theme.toolButtonColor
    round: true
    visible: true
    bgcolor: Theme.toolButtonBackgroundColor

    onClicked: {
      if (eraseToolbar.size == sizeSmall) {
        eraseToolbar.size = sizeMedium;
      } else if (eraseToolbar.size == sizeMedium) {
        eraseToolbar.size = sizeLarge;
      } else {
        eraseToolbar.size = sizeSmall;
      }
      adjustSize();
    }
  }

  function adjustSize() {
    drawPolygonToolbar.rubberbandModel.measureValue = drawPolygonToolbar.mapSettings.mapUnitsPerPoint * 5 * eraseToolbar.size;
  }

  function init(featureModel, mapSettings, editorRubberbandModel, editorRenderer) {
    eraseToolbar.featureModel = featureModel;
    eraseToolbar.editorRenderer = editorRenderer;
    eraseToolbar.editorRenderer.mapSettings = mapSettings;
    drawPolygonToolbar.digitizingLogger.digitizingLayer = featureModel.currentLayer;
    drawPolygonToolbar.mapSettings = mapSettings;
    drawPolygonToolbar.rubberbandModel = editorRubberbandModel;
    drawPolygonToolbar.rubberbandModel.geometryType = Qgis.GeometryType.Line;
    adjustSize();
    drawPolygonToolbar.stateVisible = true;
  }

  function cancel() {
    drawPolygonToolbar.cancel();
    geometryEditorsRubberband.visible = true;
  }
}
```


