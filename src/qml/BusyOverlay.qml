import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield
import Theme

/**
 * \ingroup qml
 */
Rectangle {
  id: busyOverlay

  property alias text: busyMessage.text
  property alias progress: busyProgress.value
  property bool showProgress: false
  property string buttonText: ""  // Empty = no button, set text to show button and activate if needed

  signal buttonClicked

  anchors.fill: parent
  color: Theme.darkGraySemiOpaque
  opacity: 0
  visible: false

  state: "hidden"

  states: [
    State {
      name: "hidden"
      PropertyChanges {
        target: busyOverlay
        opacity: 0
        visible: false
      }
    },
    State {
      name: "visible"
      PropertyChanges {
        target: busyOverlay
        visible: true
        opacity: 1
      }
    }
  ]

  transitions: [
    Transition {
      from: "hidden"
      to: "visible"
      SequentialAnimation {
        PropertyAnimation {
          target: busyOverlay
          property: "visible"
          duration: 0
        }
        ScriptAction {
          script: {
            busyProgress.value = 0.0;
            busyOverlay.showProgress = false;
          }
        }
        NumberAnimation {
          target: busyOverlay
          easing.type: Easing.InOutQuad
          properties: "opacity"
          duration: 250
        }
      }
    },
    Transition {
      from: "visible"
      to: "hidden"
      SequentialAnimation {
        PropertyAnimation {
          target: busyOverlay
          easing.type: Easing.InOutQuad
          property: "opacity"
          duration: 250
        }
        PropertyAnimation {
          target: busyOverlay
          property: "visible"
          duration: 0
        }
      }
    }
  ]

  // Auto-enable progress tracking when progress value is set > 0
  onProgressChanged: {
    if (progress > 0) {
      showProgress = true;
    }
  }

  Rectangle {
    id: busyCard
    anchors.centerIn: parent
    width: Math.min(parent.width * 0.8, 320)
    height: contentColumn.height + 24
    color: Theme.mainBackgroundColor
    radius: 12

    ColumnLayout {
      id: contentColumn
      anchors.centerIn: parent
      width: parent.width - 32
      spacing: 12

      Text {
        id: busyMessage
        Layout.fillWidth: true
        horizontalAlignment: Text.AlignLeft
        font: Theme.defaultFont
        color: Theme.mainTextColor
        text: ''
        wrapMode: Text.WordWrap
        visible: text.length > 0
      }

      BusyIndicator {
        id: busyIndicator
        Layout.alignment: Qt.AlignHCenter
        Layout.preferredWidth: 48
        Layout.preferredHeight: 48
        running: busyOverlay.visible
        visible: !busyOverlay.showProgress
      }

      ColumnLayout {
        Layout.fillWidth: true
        spacing: 6
        visible: busyOverlay.showProgress

        ProgressBar {
          id: busyProgress
          Layout.fillWidth: true
          Layout.preferredHeight: 6
          value: 0.0
          to: 1.0

          background: Rectangle {
            implicitHeight: 6
            radius: 3
            color: Theme.controlBackgroundAlternateColor
          }

          contentItem: Item {
            implicitHeight: 6

            Rectangle {
              width: busyProgress.visualPosition * parent.width
              height: parent.height
              radius: 3
              color: Theme.mainColor

              Behavior on width {
                NumberAnimation {
                  duration: 200
                  easing.type: Easing.OutCubic
                }
              }
            }
          }
        }

        Text {
          Layout.fillWidth: true
          horizontalAlignment: Text.AlignRight
          text: Math.round(busyProgress.value * 100) + "%"
          font: Theme.tipFont
          color: Theme.secondaryTextColor
        }
      }

      Text {
        id: actionButton
        Layout.fillWidth: true
        Layout.topMargin: 4
        horizontalAlignment: Text.AlignRight
        visible: busyOverlay.buttonText !== ""
        text: busyOverlay.buttonText
        font: Theme.defaultFont
        color: Theme.mainColor

        MouseArea {
          anchors.fill: parent
          anchors.margins: -8  // increase clickable area
          cursorShape: Qt.PointingHandCursor
          onClicked: {
            busyOverlay.buttonClicked();
          }
        }
      }
    }
  }

  MouseArea {
    id: busyOverlayCatcher
    anchors.fill: parent
    enabled: busyOverlay.visible

    onClicked: mouse => {
      // Needed to avoid people interacting with the UI while the busy overlay is visible
      // (e.g. while uploading to webDAV, users shouldn't be allowed to select other files or navigate to other pages)
      return;
    }
  }
}
