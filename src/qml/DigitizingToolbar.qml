import QtQuick 2.14
import QtQuick.Controls 2.14

import org.qgis 1.0
import org.qfield 1.0
import Theme 1.0

VisibilityFadingRow {
  id: digitizingToolbar

  property RubberbandModel rubberbandModel
  property MapSettings mapSettings

  property bool showConfirmButton: true //<! if the geometry type is point, it will never be shown
  property bool screenHovering: false //<! if the stylus pen is used, one should not use the add button

  property bool geometryRequested: false
  property var geometryRequestedItem
  property VectorLayer geometryRequestedLayer

  property alias digitizingLogger: digitizingLogger

  readonly property bool isDigitizing: rubberbandModel ? rubberbandModel.vertexCount > 1 : false //!< Readonly

  property bool geometryValid: false

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
          // set geometry valid
          if (Number( rubberbandModel ? rubberbandModel.geometryType : 0 ) === 0)
          {
            geometryValid = false
          }
          else if (Number( rubberbandModel.geometryType ) === 1)
          {
            // Line: at least 2 points
            geometryValid = rubberbandModel.vertexCount > 1
          }
          else if (Number( rubberbandModel.geometryType ) === 2)
          {
            // Polygon: at least 3 points
            geometryValid = rubberbandModel.vertexCount > 2
          }
          else
          {
            geometryValid = false
          }

          // emit the signal of digitizingToolbar
          vertexCountChanged()
      }
  }

  DigitizingLogger {
    id: digitizingLogger

    project: qgisProject
    mapSettings: digitizingToolbar.mapSettings
    digitizingLayer: rubberbandModel ? rubberbandModel.vectorLayer : null

    positionInformation: positionSource.positionInformation
    positionLocked: gnssLockButton.checked
    topSnappingResult: coordinateLocator.topSnappingResult
    cloudUserInformation: cloudConnection.userInformation
  }

  QfToolButton {
    id: cancelButton
    iconSource: Theme.getThemeIcon( "ic_clear_white_24dp" )
    visible: rubberbandModel && rubberbandModel.vertexCount > 1
    round: true
    bgcolor: Theme.darkRed

    onClicked: {
      if (stateMachine.state !== "measure") {
        cancelDialog.open();
      } else {
        digitizingLogger.clearCoordinates();
        rubberbandModel.reset()
        cancel();
      }
    }
  }

  QfToolButton {
    id: confirmButton
    iconSource: {
      Theme.getThemeIcon( "ic_check_white_48dp" )
    }
    visible: {
      if (!showConfirmButton)
      {
        false
      }
      else
      {
        geometryValid
      }
    }
    round: true
    bgcolor: Theme.mainColor

    onClicked: {
      rubberbandModel.frozen = true
      confirm()
    }
  }

  Timer {
    id: removeVertexTimer
    interval: 700
    repeat: true

    onTriggered: {
      if (!rubberbandModel || rubberbandModel.vertexCount == 0)
        stop();

      removeVertex()
      if (interval > 100) interval = interval * 0.8;
    }
  }

  QfToolButton {
    id: removeVertexButton
    iconSource: Theme.getThemeIcon( "ic_remove_vertex_white_24dp" )
    visible: rubberbandModel && rubberbandModel.vertexCount > 1
    round: true
    bgcolor: Theme.darkGray

    onPressed: {
      removeVertex()
      removeVertexTimer.interval = 700
      removeVertexTimer.restart()
    }
    onReleased: {
      removeVertexTimer.stop()
    }
    onCanceled: {
      removeVertexTimer.stop()
    }
  }

  QfToolButton {
    id: addVertexButton
    iconSource: Theme.getThemeIcon( "ic_add_vertex_white_24dp" )
    round: true
    enabled: !screenHovering
    bgcolor: {
        if (!showConfirmButton)
          Theme.darkGray
        else if (Number( rubberbandModel ? rubberbandModel.geometryType : 0 ) === QgsWkbTypes.PointGeometry ||
                 Number( rubberbandModel.geometryType ) === QgsWkbTypes.NullGeometry)
          Theme.mainColor
        else
          Theme.darkGray
    }

    states: [
        State { when: addVertexButton.enabled;
            PropertyChanges {   target: addVertexButton; opacity: 1.0    }
        },
        State { when: !addVertexButton.enabled;
            PropertyChanges {   target: addVertexButton; opacity: 0.0    }
        }
    ]
    transitions: [ Transition { NumberAnimation { property: "opacity"; duration: 200 } } ]

    property bool averagedPositionPressAndHeld: false
    property bool averagedPositionAutoRelease: false
    Connections {
      target: positionSource

      function onAveragedPositionCountChanged() {
        if (addVertexButton.averagedPositionAutoRelease && positionSource.averagedPosition
            && positionSource.averagedPositionCount >= positioningSettings.averagedPositioningMinimumCount
            && positioningSettings.averagedPositioningAutomaticStop) {
          addVertexButton.averagedPositionPressAndHeld = true;
          addVertexButton.released()
        }
      }
    }

    onPressAndHold: {
      if (coordinateLocator && coordinateLocator.positionLocked) {
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
        if (positioningSettings.averagedPositioning &&
            positioningSettings.averagedPositioningMinimumCount > positionSource.averagedPositionCount) {
          displayToast( qsTr( "The collected positions count does not meet the requirement" ), 'warning' )
          positionSource.averagedPosition = false;
          return;
        }

        if (!checkAccuracyRequirement()) {
          positionSource.averagedPosition = false;
          return;
        }
        if (Number(rubberbandModel.geometryType) === QgsWkbTypes.PointGeometry ||
            Number(rubberbandModel.geometryType) === QgsWkbTypes.NullGeometry) {
            confirm()
        } else {
            addVertex()
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
        if (!checkAccuracyRequirement()) {
          return;
        }

        if (coordinateLocator && coordinateLocator.positionLocked &&
            positioningSettings.averagedPositioning &&
            (positioningSettings.averagedPositioningMinimumCount > 1
             || !positioningSettings.averagedPositioningAutomaticStop)) {
          if (!positionSource.averagedPosition) {
            averagedPositionAutoRelease = true;
            positionSource.averagedPosition = true;
          } else {
            addVertexButton.averagedPositionPressAndHeld = true;
            addVertexButton.released()
          }
          return;
        }

        if (Number(rubberbandModel.geometryType) === QgsWkbTypes.PointGeometry ||
            Number(rubberbandModel.geometryType) === QgsWkbTypes.NullGeometry) {
            confirm()
        } else {
            addVertex()
        }
    }
  }

  Dialog {
    id: cancelDialog
    parent: mainWindow.contentItem

    visible: false
    modal: true
    font: Theme.defaultFont

    x: ( mainWindow.width - width ) / 2
    y: ( mainWindow.height - height ) / 2

    title: qsTr( "Cancel digitizing" )
    Label {
      width: parent.width
      wrapMode: Text.WordWrap
      text: qsTr( "Should the digitized geometry be discarded?" )
    }

    standardButtons: Dialog.Ok | Dialog.Cancel
    onAccepted: {
      digitizingLogger.clearCoordinates();
      rubberbandModel.reset()
      cancel();
      visible = false;
    }
    onRejected: {
      visible = false;
    }
  }

  function checkAccuracyRequirement() {
    if (coordinateLocator && coordinateLocator.positionLocked &&
         positioningSettings.accuracyIndicator && positioningSettings.accuracyRequirement) {
      if (positioningSettings.accuracyBad > 0 &&
           (!coordinateLocator.positionInformation ||
             !coordinateLocator.positionInformation.haccValid ||
              coordinateLocator.positionInformation.hacc >= positioningSettings.accuracyBad)) {
        displayToast( qsTr( "Position accuracy doesn't meet the minimum requirement, vertex not added" ), 'warning' )
        return false;
      }
    }
    return true;
  }

  function addVertex() {
    digitizingLogger.addCoordinate( coordinateLocator.currentCoordinate )
    coordinateLocator.flash()

    rubberbandModel.addVertex()
  }

  function removeVertex() {
    digitizingLogger.removeLastCoordinate();
    rubberbandModel.removeVertex()
    mapSettings.setCenter( rubberbandModel.currentCoordinate )
  }

  function confirm() {
      digitizingLogger.addCoordinate( coordinateLocator.currentCoordinate )
      confirmed()
  }
}
