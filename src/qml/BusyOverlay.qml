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
      }
      PropertyChanges {
        target: busyOverlay
        visible: false
      }
    },
    State {
      name: "visible"
      PropertyChanges {
        target: busyOverlay
        visible: true
      }
      PropertyChanges {
        target: busyOverlay
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
          script: busyProgress.value = 0.0
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

  BusyIndicator {
    id: busyIndicator
    anchors.centerIn: parent
    visible: !busyProgress.visible
    running: busyOverlay.visible
    width: 100
    height: 100
  }

  ProgressBar {
    id: busyProgress
    anchors.centerIn: busyIndicator
    visible: value != 0.0
    width: 160
    height: 14
    indeterminate: false
    value: 0.0
    to: 1.0
  }

  Rectangle {
    id: busyMessageShield
    anchors.top: busyIndicator.bottom
    anchors.topMargin: 10
    anchors.horizontalCenter: parent.horizontalCenter
    color: Theme.toolButtonBackgroundSemiOpaqueColor
    radius: 4

    width: busyMessage.width
    height: busyMessage.contentHeight + 5

    Text {
      id: busyMessage

      property int absoluteWidth: busyMessageFontMetrics.boundingRect(text).width + 10
      width: Math.min(mainWindow.width - 20, absoluteWidth)

      anchors.centerIn: parent
      horizontalAlignment: Text.AlignHCenter
      font: Theme.tipFont
      color: Theme.toolButtonColor
      text: ''
      wrapMode: Text.WordWrap
    }
  }

  FontMetrics {
    id: busyMessageFontMetrics
    font: busyMessage.font
  }
}
