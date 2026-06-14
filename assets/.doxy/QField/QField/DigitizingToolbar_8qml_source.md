

# File DigitizingToolbar.qml

[**File List**](files.md) **>** [**qml**](dir_1d81f7f4362e4398c0a695c4e466fcb0.md) **>** [**DigitizingToolbar.qml**](DigitizingToolbar_8qml.md)

[Go to the documentation of this file](DigitizingToolbar_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import org.qgis
import org.qfield
import Theme

QfVisibilityFadingRow {
  id: digitizingToolbar

  property RubberbandModel rubberbandModel
  property MapSettings mapSettings

  property bool cogoEnabled: false
  property var cogoOperationSettings: undefined
  property alias cogoExecutor: cogoExecutor

  property bool showConfirmButton: true //<! if the geometry type is point, it will never be shown
  property bool screenHovering: false //<! if the stylus pen is used, one should not use the add button

  property bool geometryRequested: false
  property var geometryRequestedItem
  property VectorLayer geometryRequestedLayer

  property alias digitizingLogger: digitizingLogger
  property alias cancelDialog: cancelDialog

  readonly property bool isDigitizing: rubberbandModel ? rubberbandModel.vertexCount > 1 : false 

  property bool geometryValid: false

  signal requestJumpToPoint(var center, real scale, bool handleMargins)
  signal requestPosition(var item, bool fromCoordinateLocator)

  spacing: 4

  /* This signal is emitted when the digitized geometry has been confirmed.
   * The correspoding handler is \c onConfirmed.
   */
  signal confirmed
  /* This signal is emitted when the user cancels geometry digitizing.
   * The correspoding handler is \c onCancel.
   */
  signal cancel
  signal vertexCountChanged

  Connections {
    target: rubberbandModel

    function onVertexCountChanged() {
      // check whether geometry is valid and can be confirmed
      checkGeometryValidity();

      // emit the signal of digitizingToolbar
      vertexCountChanged();
    }
  }

  DigitizingLogger {
    id: digitizingLogger

    project: qgisProject
    mapSettings: digitizingToolbar.mapSettings
    digitizingLayer: rubberbandModel ? rubberbandModel.vectorLayer : null

    positionInformation: positionSource.positionInformation
    positionLocked: coordinateLocator.positionLocked
    topSnappingResult: coordinateLocator.topSnappingResult
    cloudUserInformation: projectInfo.cloudUserInformation
  }

  QfToolButton {
    id: cancelButton
    iconSource: Theme.getThemeVectorIcon("ic_clear_white_24dp")
    visible: rubberbandModel && rubberbandModel.vertexCount > 1
    round: true
    bgcolor: Theme.darkRed

    onClicked: {
      dashBoard.shouldReturnHome = false;
      if (stateMachine.state !== "measure") {
        cancelDialog.open();
      } else {
        digitizingLogger.clearCoordinates();
        rubberbandModel.reset();
        cancel();
      }
    }
  }

  QfToolButton {
    id: confirmButton
    iconSource: Theme.getThemeVectorIcon("ic_check_white_24dp")
    visible: {
      if (!showConfirmButton) {
        false;
      } else {
        geometryValid;
      }
    }
    round: true
    bgcolor: !enabled ? Theme.toolButtonBackgroundSemiOpaqueColor : Theme.mainColor

    onClicked: {
      dashBoard.shouldReturnHome = false;
      confirm();
    }
  }

  CogoOperations {
    id: cogoOperations
    enabled: digitizingToolbar.cogoEnabled

    mapSettings: digitizingToolbar.mapSettings
    cogoOperationSettings: digitizingToolbar.cogoOperationSettings

    onEnabledChanged: {
      digitizingToolbar.cogoOperationSettings.visible = enabled;
      checkGeometryValidity();
    }

    onRequestJumpToPoint: function (center, scale, handleMargins) {
      digitizingToolbar.requestJumpToPoint(center, scale, handleMargins);
    }

    onRequestPosition: function (item, fromCoordinateLocator) {
      digitizingToolbar.requestPosition(item, fromCoordinateLocator);
    }

    CogoExecutor {
      id: cogoExecutor

      name: digitizingToolbar.cogoOperationSettings ? digitizingToolbar.cogoOperationSettings.name : ""
      parameterValues: digitizingToolbar.cogoOperationSettings ? digitizingToolbar.cogoOperationSettings.parameterValues : {}

      mapSettings: digitizingToolbar.mapSettings
      rubberbandModel: digitizingToolbar.rubberbandModel

      onParametersChanged: {
        cogoOperationSettings.parameters = parameters;
        cogoOperationSettings.clear();
      }
    }
  }

  Timer {
    id: removeVertexTimer
    interval: 700
    repeat: true

    onTriggered: {
      if (!rubberbandModel || rubberbandModel.vertexCount === 0) {
        stop();
      }
      removeVertex();
      if (interval > 100)
        interval = interval * 0.8;
    }
  }

  QfToolButton {
    id: removeVertexButton
    iconSource: Theme.getThemeVectorIcon("ic_remove_white_24dp")
    iconColor: Theme.toolButtonColor
    visible: rubberbandModel && rubberbandModel.vertexCount > 1
    round: true
    bgcolor: Theme.toolButtonBackgroundColor

    onPressed: {
      removeVertex();
      removeVertexTimer.interval = 700;
      removeVertexTimer.restart();
    }
    onReleased: {
      removeVertexTimer.stop();
    }
    onCanceled: {
      removeVertexTimer.stop();
    }
  }

  QfToolButton {
    id: addVertexButton
    round: true
    enabled: (cogoEnabled && cogoExecutor.isReady) || (!cogoEnabled && !screenHovering)
    bgcolor: {
      if (!enabled) {
        Theme.toolButtonBackgroundSemiOpaqueColor;
      } else if (!showConfirmButton) {
        Theme.toolButtonBackgroundColor;
      } else if (Number(rubberbandModel ? rubberbandModel.geometryType : 0) === Qgis.GeometryType.Point || Number(rubberbandModel.geometryType) === Qgis.GeometryType.Null) {
        Theme.mainColor;
      } else {
        Theme.toolButtonBackgroundColor;
      }
    }
    iconSource: Theme.getThemeVectorIcon("ic_add_white_24dp")
    iconColor: enabled ? Theme.toolButtonColor : Theme.toolButtonBackgroundSemiOpaqueColor

    property bool lastAdditionAveraged: false
    property bool averagedPositionPressAndHeld: false
    property bool averagedPositionAutoRelease: false

    Connections {
      target: positionSource

      function onAveragedPositionCountChanged() {
        if (addVertexButton.averagedPositionAutoRelease && positionSource.averagedPosition && positionSource.averagedPositionCount >= positioningSettings.averagedPositioningMinimumCount && positioningSettings.averagedPositioningAutomaticStop) {
          addVertexButton.averagedPositionPressAndHeld = true;
          addVertexButton.released();
        }
      }
    }

    onPressAndHold: {
      if (!cogoEnabled && coordinateLocator && coordinateLocator.positionLocked) {
        if (!checkAccuracyRequirement()) {
          return;
        }
        averagedPositionPressAndHeld = true;
        positionSource.averagedPosition = true;
      }
    }

    onReleased: {
      if (!averagedPositionPressAndHeld) {
        return;
      }
      averagedPositionPressAndHeld = false;
      averagedPositionAutoRelease = false;
      if (coordinateLocator && coordinateLocator.positionLocked) {
        if (positioningSettings.averagedPositioning && positioningSettings.averagedPositioningMinimumCount > positionSource.averagedPositionCount) {
          displayToast(qsTr("The collected positions count does not meet the requirement"), 'warning');
          positionSource.averagedPosition = false;
          return;
        }
        if (!checkAccuracyRequirement()) {
          positionSource.averagedPosition = false;
          return;
        }
        lastAdditionAveraged = true;
        addVertex();
        if (Number(rubberbandModel.geometryType) === Qgis.GeometryType.Point || Number(rubberbandModel.geometryType) === Qgis.GeometryType.Null) {
          confirm();
        }
        positionSource.averagedPosition = false;
      }
    }

    onCanceled: {
      if (coordinateLocator.positionLocked) {
        positionSource.averagedPosition = false;
      }
    }

    onClicked: {
      if (cogoEnabled) {
        if (cogoExecutor.isReady) {
          const success = cogoExecutor.execute();
          if (success) {
            cogoOperationSettings.clear();
            // Recenter to last added vertex
            mapSettings.setCenter(rubberbandModel.lastCoordinate, true);
            if (Number(rubberbandModel.geometryType) === Qgis.GeometryType.Point) {
              confirm();
            }
          }
        }
        return;
      }
      if (!checkAccuracyRequirement()) {
        return;
      }
      if (coordinateLocator && coordinateLocator.positionLocked && positioningSettings.averagedPositioning && (positioningSettings.averagedPositioningMinimumCount > 1 || !positioningSettings.averagedPositioningAutomaticStop)) {
        if (!positionSource.averagedPosition) {
          averagedPositionAutoRelease = true;
          positionSource.averagedPosition = true;
        } else {
          addVertexButton.averagedPositionPressAndHeld = true;
          addVertexButton.released();
        }
        return;
      }
      lastAdditionAveraged = false;
      if (Number(rubberbandModel.geometryType) === Qgis.GeometryType.Point || Number(rubberbandModel.geometryType) === Qgis.GeometryType.Null) {
        confirm();
      } else {
        addVertex();
      }
    }
  }

  QfDialog {
    id: cancelDialog
    parent: mainWindow.contentItem
    title: qsTr("Cancel digitizing")
    Label {
      width: parent.width
      wrapMode: Text.WordWrap
      text: qsTr("Should the digitized geometry be discarded?")
    }
    onAccepted: {
      digitizingLogger.clearCoordinates();
      rubberbandModel.reset();
      cancel();
      visible = false;
    }
    onRejected: {
      visible = false;
    }
  }

  function checkAccuracyRequirement() {
    if (coordinateLocator && coordinateLocator.positionLocked && positioningSettings.accuracyIndicator && positioningSettings.accuracyRequirement) {
      if (positioningSettings.accuracyBad > 0 && (!coordinateLocator.positionInformation || !coordinateLocator.positionInformation.haccValid || coordinateLocator.positionInformation.hacc >= positioningSettings.accuracyBad)) {
        displayToast(qsTr("Position accuracy doesn't meet the minimum requirement, vertex not added"), 'warning');
        return false;
      }
    }
    return true;
  }

  function checkGeometryValidity() {
    var extraVertexNeed = cogoEnabled || (coordinateLocator && coordinateLocator.positionLocked && positioningSettings.averagedPositioning && positioningSettings.averagedPositioningMinimumCount > 1) ? 1 : 0;

    // set geometry valid
    if (Number(rubberbandModel ? rubberbandModel.geometryType : 0) === 0) {
      geometryValid = false;
    } else if (Number(rubberbandModel.geometryType) === 1) {
      // Line: at least 2 points
      geometryValid = rubberbandModel.vertexCount > 1 + extraVertexNeed;
    } else if (Number(rubberbandModel.geometryType) === 2) {
      // Polygon: at least 3 points
      geometryValid = rubberbandModel.vertexCount > 2 + extraVertexNeed;
    } else {
      geometryValid = false;
    }
  }

  function triggerAddVertex() {
    addVertexButton.clicked();
  }

  function addVertex() {
    digitizingLogger.addCoordinate(coordinateLocator.currentCoordinate);
    coordinateLocator.flash();
    rubberbandModel.addVertex();
  }

  function removeVertex() {
    digitizingLogger.removeLastCoordinate();
    rubberbandModel.removeVertex();
    mapSettings.setCenter(rubberbandModel.currentCoordinate, true);
  }

  function confirm() {
    rubberbandModel.frozen = true;
    if (addVertexButton.lastAdditionAveraged) {
      rubberbandModel.removeVertex();
    } else {
      digitizingLogger.addCoordinate(coordinateLocator.currentCoordinate);
    }
    confirmed();
  }
}
```


