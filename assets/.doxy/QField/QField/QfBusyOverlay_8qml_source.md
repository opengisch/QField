

# File QfBusyOverlay.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfBusyOverlay.qml**](QfBusyOverlay_8qml.md)

[Go to the documentation of this file](QfBusyOverlay_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield.core
import org.qfield.gui

Rectangle {
  id: busyOverlay

  property alias text: busyMessage.text
  property alias progress: busyProgress.value
  property bool showProgress: false
  property string actionText: ""  // Empty = no actionText, set text to show actionText and activate if needed

  signal actionClicked

  anchors.fill: parent
  color: QfTheme.darkGraySemiOpaque
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
    width: Math.min(parent.width * 0.8, 420)
    height: contentColumn.height + 24
    color: QfTheme.mainBackgroundColor
    radius: 12
    z: 2

    ColumnLayout {
      id: contentColumn
      anchors.centerIn: parent
      width: parent.width - 32
      spacing: 12

      Text {
        id: busyMessage
        Layout.fillWidth: true
        horizontalAlignment: Text.AlignLeft
        font: QfTheme.defaultFont
        color: QfTheme.mainTextColor
        text: ''
        wrapMode: Text.Wrap
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
            color: QfTheme.controlBackgroundAlternateColor
          }

          contentItem: Item {
            implicitHeight: 6

            Rectangle {
              width: busyProgress.visualPosition * parent.width
              height: parent.height
              radius: 3
              color: QfTheme.mainColor

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
          font: QfTheme.tipFont
          color: QfTheme.secondaryTextColor
        }
      }

      Text {
        id: actionLink
        Layout.fillWidth: true
        Layout.topMargin: 4
        horizontalAlignment: Text.AlignRight
        visible: busyOverlay.actionText !== ""
        text: busyOverlay.actionText
        font: QfTheme.defaultFont
        color: QfTheme.mainColor

        MouseArea {
          anchors.fill: parent
          anchors.margins: -8  // increase clickable area
          z: 10
          cursorShape: Qt.PointingHandCursor
          onClicked: {
            busyOverlay.actionClicked();
          }
        }
      }
    }
  }

  MouseArea {
    id: busyOverlayCatcher
    anchors.fill: parent
    enabled: busyOverlay.visible
    z: 1

    acceptedButtons: Qt.AllButtons

    // Block all events when overlay is present
    onClicked: mouse => {
      mouse.accepted = true;
    }
    onPressed: mouse => {
      mouse.accepted = true;
    }
    onReleased: mouse => {
      mouse.accepted = true;
    }
    onPressAndHold: mouse => {
      mouse.accepted = true;
    }
  }
}
```


