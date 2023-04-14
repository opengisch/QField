import QtQuick 2.14
import QtQuick.Controls 2.14

import Theme 1.0

Popup {
  id: toast

  property string type: 'info'

  y: parent.height - 112
  z: 10001

  width: parent.width
  height: 40
  margins: 0
  closePolicy: Popup.NoAutoClose

  opacity: 0
  Behavior on opacity {
    NumberAnimation { duration: 250 }
  }

  background: Rectangle {
    color: "transparent"
  }

  Rectangle {
    id: toastContent
    z: 1
    width: toastMessage.width + (toastAction.width > 0 ? toastAction.width + 40 : 0)
    height: toastMessage.height + 10
    anchors.centerIn: parent

    color: "#66212121"
    radius: 4

    Rectangle {
      id: toastIndicator
      anchors.left: parent.left
      anchors.leftMargin: 10
      anchors.verticalCenter: parent.verticalCenter
      width:  10
      height: 10
      radius: 5
      color: toast.type === 'error' ? Theme.errorColor : Theme.warningColor
      visible: toast.type != 'info'
    }

    Row {
      anchors.centerIn: parent
      spacing: 10

      Text {
        id: toastMessage

        property int absoluteWidth: toastFontMetrics.boundingRect(text).width + 20

        width: ( 40 + absoluteWidth + toastAction.width ) > mainWindow.width
               ? mainWindow.width - toastAction.width - 40
               : absoluteWidth
        leftPadding: toastAction.width > 0 && toast.type != 'info' ? 10 : 0
        wrapMode: Text.Wrap
        topPadding: 3
        bottomPadding: 3
        color: Theme.light

        font: Theme.secondaryTitleFont
        horizontalAlignment: Text.AlignHCenter
      }

      QfButton {
        id: toastAction

        property var act: undefined

        visible: text != ''
        height: toastMessage.height

        radius: 4
        bgcolor: "#99000000"
        color: Theme.mainColor
        font.pointSize: Theme.defaultFont.pointSize
        font.weight: Font.Bold

        onClicked: {
          if (act !== undefined) {
            act();
          }
        }
      }
    }
  }

  FontMetrics {
    id: toastFontMetrics
    font: toastMessage.font
  }

  Timer {
    id: toastTimer
    interval: 3000
    onTriggered: {
      toast.opacity = 0
    }
  }

  onOpacityChanged: {
    if ( opacity == 0 ) {
      toastContent.visible = false
      toast.close()
    }
  }

  MouseArea {
    anchors.fill: parent
    onClicked: {
      toast.close()
      toast.opacity = 0
    }
  }

  function show(text, type, action_text, action_function) {
    toastMessage.text = text
    toast.type = type || 'info'

    if (action_text !== undefined && action_function !== undefined) {
      toastAction.text = action_text.toUpperCase()
      toastAction.act = action_function
      toastTimer.interval = 5000
    } else {
      toastAction.text = ''
      toastAction.act = undefined
      toastTimer.interval = 3000
    }

    toastContent.visible = true
    toast.open()
    toast.opacity = 1
    toastTimer.restart()
  }
}
