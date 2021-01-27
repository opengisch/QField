import QtQuick 2.12
import QtQuick.Controls 2.12

import org.qgis 1.0
import Theme 1.0

VisibilityFadingRow {
  id: digitizingToolbar
  property RubberbandModel rubberbandModel
  property CoordinateLocator coordinateLocator // optional coordinateLocator to flash
  property MapSettings mapSettings
  property bool showConfirmButton: true //<! if the geometry type is point, it will never be shown
  property bool screenHovering: false //<! if the stylus pen is used, one should not use the add button

  property bool geometryRequested: false
  property var geometryRequestedItem
  property VectorLayer geometryRequestedLayer

  readonly property bool isDigitizing: rubberbandModel ? rubberbandModel.vertexCount > 1 : false //!< Readonly

  property bool geometryValid: false

  spacing: 4

  /* This signal is emitted when the user confirms the digitized geometry.
   * The correspoding handler is \c onConfirm.
   */
  signal confirm
  /* This signal is emitted when the user cancels geometry digitizing.
   * The correspoding handler is \c onCancel.
   */
  signal cancel
  signal vertexCountChanged

  Connections {
      target: rubberbandModel

      function onVertexCountChanged() {
          // set geometry valid
          if ( Number( rubberbandModel ? rubberbandModel.geometryType : 0 ) === 0 )
          {
            geometryValid = false
          }
          else if  ( Number( rubberbandModel.geometryType ) === 1 )
          {
            // Line: at least 2 points (last point not saved)
            geometryValid = rubberbandModel.vertexCount > 2
          }
          else if  ( Number( rubberbandModel.geometryType ) === 2 )
          {
            // Polygon: at least 3 points (last point not saved)
            geometryValid = rubberbandModel.vertexCount > 3
          }
          else
          {
            geometryValid = false
          }

          // emit the signal of digitizingToolbar
          vertexCountChanged()
      }
  }

  QfToolButton {
    id: cancelButton
    iconSource: Theme.getThemeIcon( "ic_clear_white_24dp" )
    visible: rubberbandModel && rubberbandModel.vertexCount > 1
    round: true
    bgcolor: Theme.darkRed

    onClicked: {
      cancelDialog.open();
    }
  }

  QfToolButton {
    id: confirmButton
    iconSource: {
      Theme.getThemeIcon( "ic_check_white_48dp" )
    }
    visible: {
      if ( !showConfirmButton )
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
      if ( !rubberbandModel || rubberbandModel.vertexCount == 0)
        stop();

      removeVertex()
      if ( interval > 100 ) interval = interval * 0.8;
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

    onClicked: {
        if ( coordinateLocator && coordinateLocator.overrideLocation !== undefined && coordinateLocator.accuracyRequirementFail ) {
            displayToast( qsTr( "Position accuracy doesn't meet the minimum requirement, vertex not added" ) )
            return;
        }

        if ( Number( rubberbandModel.geometryType ) === QgsWkbTypes.PointGeometry ||
                Number( rubberbandModel.geometryType ) === QgsWkbTypes.NullGeometry )
            confirm()
        else
            addVertex()
    }
  }

  Dialog {
    id: cancelDialog
    parent: mainWindow.contentItem

    visible: false
    modal: true

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
      rubberbandModel.reset()
      cancel();
      visible = false;
    }
    onRejected: {
      visible = false;
    }
  }

  function addVertex()
  {
    if (coordinateLocator)
      coordinateLocator.flash()
    rubberbandModel.addVertex()
  }

  function removeVertex()
  {
    rubberbandModel.removeVertex()
    mapSettings.setCenter( rubberbandModel.currentCoordinate )
  }
}
