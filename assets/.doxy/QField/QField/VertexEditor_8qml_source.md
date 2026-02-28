

# File VertexEditor.qml

[**File List**](files.md) **>** [**geometryeditors**](dir_a044c600d9d985eeadf51620d76e3ed0.md) **>** [**VertexEditor.qml**](VertexEditor_8qml.md)

[Go to the documentation of this file](VertexEditor_8qml.md)


```C++
import QtQuick
import org.qgis
import org.qfield
import Theme
import "."
import ".."

GeometryEditorBase {
  id: vertexEditorToolbar

  property bool screenHovering: false //<! if the stylus pen is used, one should not use the add button
  property bool vertexRubberbandVisible: true
  property int currentVertexId: -1
  property bool currentVertexModified: false

  readonly property bool blocking: featureModel ? featureModel.vertexModel.dirty : false

  spacing: 4

  function init(featureModel, mapSettings, editorRubberbandModel, editorRenderer) {
    featureModel.vertexModel.currentVertexIndex = -1;
    vertexEditorToolbar.featureModel = featureModel;
    vertexEditorToolbar.mapSettings = mapSettings;
    digitizingLogger.digitizingLayer = featureModel.currentLayer;
  }

  function cancel() {
    featureModel.vertexModel.editingMode = VertexModel.NoEditing;
    featureModel.vertexModel.reset();
  }

  function applyChanges(apply) {
    if (apply && featureModel.vertexModel.dirty) {
      featureModel.applyGeometry(true);
      if (!featureModel.save()) {
        displayToast(qsTr("Failed to save feature!"), 'error');
      }

      //set the vertexModel original geometry to the one of the updated feature
      featureModel.vertexModel.updateGeometry(featureModel.feature.geometry);
    }
  }

  function canvasClicked(point, type) {
    if (type === "stylus") {
      if (featureModel.vertexModel.currentVertexIndex == -1)
        featureModel.vertexModel.selectVertexAtPosition(point, 14);
      else {
        digitizingLogger.addCoordinate(featureModel.vertexModel.currentPoint);
        featureModel.vertexModel.currentVertexIndex = -1;
        vertexEditorToolbar.currentVertexModified = false;
      }
    } else {
      featureModel.vertexModel.selectVertexAtPosition(point, 14, false);
    }
    return true;
  }

  QfToolButton {
    id: undoButton
    iconSource: Theme.getThemeVectorIcon("ic_undo_black_24dp")
    iconColor: Theme.toolButtonColor
    round: true
    visible: featureModel && featureModel.vertexModel.canUndo
    bgcolor: Theme.toolButtonBackgroundColor
    onClicked: {
      featureModel.vertexModel.undoHistory();
      mapSettings.setCenter(featureModel.vertexModel.currentPoint, true);
    }
  }

  QfToolButton {
    id: cancelButton
    iconSource: Theme.getThemeVectorIcon("ic_clear_white_24dp")
    round: true
    visible: featureModel && featureModel.vertexModel.dirty && !qfieldSettings.autoSave
    bgcolor: Theme.darkRed
    onClicked: {
      digitizingLogger.clearCoordinates();
      cancel();
      finished();
    }
  }

  QfToolButton {
    id: applyButton
    iconSource: Theme.getThemeVectorIcon("ic_check_white_24dp")
    iconColor: Theme.toolButtonColor
    round: true
    visible: featureModel && featureModel.vertexModel.dirty
    bgcolor: !qfieldSettings.autoSave ? Theme.mainColor : Theme.toolButtonBackgroundColor

    onClicked: {
      if (vertexEditorToolbar.currentVertexModified) {
        digitizingLogger.addCoordinate(featureModel.vertexModel.currentPoint);
      }
      digitizingLogger.writeCoordinates();
      applyChanges(true);
      finished();
    }
  }

  QfToolButton {
    id: removeVertexButton
    iconSource: Theme.getThemeVectorIcon("ic_remove_white_24dp")
    iconColor: Theme.toolButtonColor
    round: true
    visible: featureModel && featureModel.vertexModel.canRemoveVertex
    bgcolor: Theme.toolButtonBackgroundColor

    onClicked: {
      if (featureModel.vertexModel.canRemoveVertex) {
        featureModel.vertexModel.removeCurrentVertex();
        if (screenHovering) {
          featureModel.vertexModel.currentVertexIndex = -1;
        }
      }
      applyChanges(qfieldSettings.autoSave);
    }
  }

  QfToolButton {
    id: addVertexButton
    round: true
    enabled: !screenHovering && featureModel && featureModel.vertexModel.canAddVertex && featureModel.vertexModel.editingMode !== VertexModel.AddVertex
    bgcolor: enabled ? Theme.darkGray : Theme.darkGraySemiOpaque
    iconSource: Theme.getThemeVectorIcon("ic_add_white_24dp")
    iconColor: enabled ? Theme.toolButtonColor : Theme.toolButtonBackgroundSemiOpaqueColor

    onClicked: {
      applyChanges(qfieldSettings.autoSave);
      if (featureModel.vertexModel.currentVertexIndex != -1) {
        if (featureModel.vertexModel.editingMode === VertexModel.AddVertex) {
          featureModel.vertexModel.editingMode = VertexModel.EditVertex;
        } else {
          featureModel.vertexModel.editingMode = VertexModel.AddVertex;
        }
      } else {
        featureModel.vertexModel.addVertexNearestToPosition(coordinateLocator.currentCoordinate);
        applyChanges(qfieldSettings.autoSave);
      }
    }
  }

  QfToolButton {
    id: previousVertexButton
    round: true
    enabled: !screenHovering
    visible: featureModel && (featureModel.vertexModel.canAddVertex || featureModel.vertexModel.editingMode === VertexModel.AddVertex)
    bgcolor: enabled && featureModel && featureModel.vertexModel.canPreviousVertex ? Theme.toolButtonBackgroundColor : Theme.toolButtonBackgroundSemiOpaqueColor
    iconSource: Theme.getThemeVectorIcon("ic_chevron_left_white_24dp")
    iconColor: enabled && featureModel && featureModel.vertexModel.canPreviousVertex ? Theme.toolButtonColor : Theme.toolButtonBackgroundSemiOpaqueColor

    onClicked: {
      if (vertexEditorToolbar.currentVertexModified) {
        digitizingLogger.addCoordinate(featureModel.vertexModel.currentPoint);
      }
      applyChanges(qfieldSettings.autoSave);
      featureModel.vertexModel.previous();
    }
  }

  QfToolButton {
    id: nextVertexButton
    round: true
    enabled: !screenHovering
    visible: featureModel && (featureModel.vertexModel.canAddVertex || featureModel.vertexModel.editingMode === VertexModel.AddVertex)
    bgcolor: enabled && featureModel && featureModel.vertexModel.canNextVertex ? Theme.darkGray : Theme.darkGraySemiOpaque
    iconSource: Theme.getThemeVectorIcon("ic_chevron_right_white_24dp")
    iconColor: enabled && featureModel && featureModel.vertexModel.canNextVertex ? Theme.toolButtonColor : Theme.toolButtonBackgroundSemiOpaqueColor

    onClicked: {
      if (vertexEditorToolbar.currentVertexModified) {
        digitizingLogger.addCoordinate(featureModel.vertexModel.currentPoint);
      }
      applyChanges(qfieldSettings.autoSave);
      featureModel.vertexModel.next();
    }
  }

  DigitizingLogger {
    id: digitizingLogger
    type: 'edit_vertex'

    project: qgisProject
    mapSettings: mapSettings

    positionInformation: positionSource.positionInformation
    positionLocked: coordinateLocator.positionLocked
    topSnappingResult: coordinateLocator.topSnappingResult
    cloudUserInformation: projectInfo.cloudUserInformation
  }

  Connections {
    target: geometryEditingVertexModel

    function onCurrentVertexIndexChanged() {
      if (featureModel.vertexModel.currentVertexIndex != -1 && vertexEditorToolbar.currentVertexId !== featureModel.vertexModel.currentVertexIndex && !screenHovering && featureModel.vertexModel.editingMode !== VertexModel.NoEditing) {
        mapSettings.setCenter(featureModel.vertexModel.currentPoint, true);
        vertexEditorToolbar.currentVertexId = featureModel.vertexModel.currentVertexIndex;
        vertexEditorToolbar.currentVertexModified = false;
      }
    }

    function onCurrentPointChanged() {
      vertexEditorToolbar.currentVertexModified = true;
    }
  }
}
```


