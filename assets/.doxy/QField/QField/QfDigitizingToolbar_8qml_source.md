

# File QfDigitizingToolbar.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfDigitizingToolbar.qml**](QfDigitizingToolbar_8qml.md)

[Go to the documentation of this file](QfDigitizingToolbar_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import org.qgis
import org.qfield.core
import org.qfield.gui

QfVisibilityFadingRow {
  id: digitizingToolbar

  property QfRubberbandModel rubberbandModel
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

    property bool deferredVertexCountChanged: false

    function onCurrentCoordinateChanged() {
      if (deferredVertexCountChanged) {
        const vertices = rubberbandModel.vertices;
        if (vertices[rubberbandModel.vertexCount - 1] !== vertices[rubberbandModel.vertexCount - 2]) {
          processVertexCountChanged();
          deferredVertexCountChanged = false;
        }
      }
    }

    function onVertexCountChanged() {
      if (rubberbandModel.geometryType === Qgis.GeometryType.Line && rubberbandModel.vertexCount === 2) {
        deferredVertexCountChanged = true;
      } else if (rubberbandModel.geometryType === Qgis.GeometryType.Polygon && rubberbandModel.vertexCount === 3) {
        deferredVertexCountChanged = true;
      } else {
        processVertexCountChanged();
      }
    }
  }

  function processVertexCountChanged() {
    // check whether geometry is valid and can be confirmed
    checkGeometryValidity();

    // emit the signal of digitizingToolbar
    vertexCountChanged();
  }

  QfDigitizingLogger {
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
    iconSource: QfTheme.getThemeVectorIcon("ic_clear_white_24dp")
    visible: rubberbandModel && rubberbandModel.vertexCount > 1
    round: true
    bgcolor: QfTheme.darkRed

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
    iconSource: QfTheme.getThemeVectorIcon("ic_check_white_24dp")
    visible: {
      if (!showConfirmButton) {
        false;
      } else {
        geometryValid;
      }
    }
    round: true
    bgcolor: !enabled ? QfTheme.toolButtonBackgroundSemiOpaqueColor : QfTheme.mainColor

    onClicked: {
      dashBoard.shouldReturnHome = false;
      confirm();
    }
  }

  QfCogoOperations {
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

    QfCogoExecutor {
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
    iconSource: QfTheme.getThemeVectorIcon("ic_remove_white_24dp")
    iconColor: QfTheme.toolButtonColor
    visible: rubberbandModel && rubberbandModel.vertexCount > 1
    round: true
    bgcolor: QfTheme.toolButtonBackgroundColor

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
        QfTheme.toolButtonBackgroundSemiOpaqueColor;
      } else if (!showConfirmButton) {
        QfTheme.toolButtonBackgroundColor;
      } else if (!rubberbandModel || rubberbandModel.geometryType === Qgis.GeometryType.Point || rubberbandModel.geometryType === Qgis.GeometryType.Null) {
        QfTheme.mainColor;
      } else {
        QfTheme.toolButtonBackgroundColor;
      }
    }
    iconSource: QfTheme.getThemeVectorIcon("ic_add_white_24dp")
    iconColor: enabled ? QfTheme.toolButtonColor : QfTheme.toolButtonBackgroundSemiOpaqueColor

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
        if (rubberbandModel.geometryType === Qgis.GeometryType.Point || rubberbandModel.geometryType === Qgis.GeometryType.Null) {
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
      if (rubberbandModel.geometryType === Qgis.GeometryType.Point || rubberbandModel.geometryType === Qgis.GeometryType.Null) {
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
    const extraVertexNeed = cogoEnabled || (coordinateLocator && coordinateLocator.positionLocked && positioningSettings.averagedPositioning && positioningSettings.averagedPositioningMinimumCount > 1) ? 1 : 0;

    // set geometry valid
    if (!rubberbandModel || rubberbandModel.geometryType === Qgis.GeometryType.Point) {
      geometryValid = false;
    } else if (rubberbandModel.geometryType === Qgis.GeometryType.Line) {
      // Line: at least 2 points
      geometryValid = rubberbandModel.vertexCount > 1 + extraVertexNeed;
    } else if (rubberbandModel.geometryType === Qgis.GeometryType.Polygon) {
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


