import QtQuick
import QtQuick.Controls
import Theme

/**
 * \ingroup qml
 */
Popup {
  id: toast

  property string type: 'info'
  property int edgeSpacing: 52
  property bool timeoutFeedback: false
  property real virtualKeyboardHeight: {
    const top = Qt.inputMethod.keyboardRectangle.top / Screen.devicePixelRatio;
    if (top > 0) {
      const height = Qt.inputMethod.keyboardRectangle.height / Screen.devicePixelRatio;
      return height - (top + height - mainWindow.height);
    }
    return 0;
  }

  x: edgeSpacing
  y: mainWindow.height - 112 - virtualKeyboardHeight
  z: 10001

  width: mainWindow.width - edgeSpacing * 2
  height: toastMessage.contentHeight + 20
  margins: 0
  padding: 0
  closePolicy: Popup.NoAutoClose

  opacity: 0

  Behavior on opacity  {
    NumberAnimation {
      duration: 250
    }
  }

  background: Item {
  }

  Rectangle {
    id: toastContent
    z: 1
    width: toastRow.width + 20
    height: toastMessage.contentHeight + 10
    anchors.centerIn: parent

    color: "#66212121"
    radius: 4

    ProgressBar {
      id: animationProgressBar
      padding: 2
      anchors.fill: parent
      visible: timeoutFeedback
      z: toastRow.z - 1
      value: animationTimer.position / toastTimer.interval

      background: Item {
        anchors.fill: parent
      }

      contentItem: Item {
        anchors.fill: parent

        Rectangle {
          width: animationProgressBar.visualPosition * parent.width
          height: parent.height
          radius: 2
          color: Qt.hsla(Theme.mainColor.hslHue, Theme.mainColor.hslSaturation, Theme.mainColor.hslLightness, 0.5)
          visible: !animationProgressBar.indeterminate
        }
      }
    }

    Row {
      id: toastRow
      anchors.centerIn: parent
      spacing: 10

      Rectangle {
        id: toastIndicator
        anchors.verticalCenter: parent.verticalCenter
        width: 10
        height: 10
        radius: 5
        color: toast.type === 'error' ? Theme.errorColor : Theme.warningColor
        visible: toast.type != 'info'
      }

      Text {
        id: toastMessage

        property int absoluteWidth: toastFontMetrics.boundingRect(text).width + 10

        width: 40 + absoluteWidth + (toastIndicator.visible ? toastIndicator.width + 10 : 0) + (toastAction.visible ? toastAction.width + 10 : 0) > toast.width ? toast.width - (toastIndicator.visible ? toastIndicator.width + 10 : 0) - (toastAction.visible ? toastAction.width + 10 : 0) - 40 : absoluteWidth
        wrapMode: Text.Wrap
        topPadding: 3
        bottomPadding: 3
        color: Theme.light

        font: Theme.defaultFont
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
        font.pointSize: Theme.tipFont.pointSize

        onClicked: {
          if (act !== undefined) {
            act();
          }
          toast.close();
          animationTimer.stopAct = undefined;
        }
      }
    }
  }

  MouseArea {
    anchors.fill: parent
    onPressed: {
      toast.close();
      toast.opacity = 0;
    }
  }

  FontMetrics {
    id: toastFontMetrics
    font: toastMessage.font
  }

  Timer {
    id: animationTimer
    interval: 50
    repeat: timeoutFeedback

    property real position: 0
    property var stopAct: undefined

    onTriggered: {
      position += interval;
      if (animationProgressBar.value === 1) {
        animationTimer.stop();
        reset();
        if (stopAct !== undefined) {
          stopAct();
        }
        timeoutFeedback = false;
      }
    }

    function reset() {
      position = 0;
    }
  }

  Timer {
    id: toastTimer
    interval: 3000
    onTriggered: {
      toast.opacity = 0;
    }
  }

  onOpacityChanged: {
    if (opacity == 0) {
      toastContent.visible = false;
      toast.close();
      animationTimer.reset();
      timeoutFeedback = false;
    }
  }

  function show(text, type, action_text, action_function, timeout_feedback, timeout_function) {
    toastMessage.text = text;
    toast.type = type || 'info';
    if (timeout_feedback !== undefined) {
      toast.timeoutFeedback = timeout_feedback;
    } else {
      toast.timeoutFeedback = false;
    }
    if (timeout_function !== undefined) {
      animationTimer.stopAct = timeout_function;
    }
    if (action_text !== undefined && action_function !== undefined) {
      toastAction.text = action_text;
      toastAction.act = action_function;
      toastTimer.interval = 5000;
    } else {
      toastAction.text = '';
      toastAction.act = undefined;
      toastTimer.interval = 3000;
    }
    toastContent.visible = true;
    toast.open();
    toast.opacity = 1;
    toastTimer.restart();
    if (toast.timeoutFeedback) {
      animationTimer.reset();
      animationTimer.restart();
    }
  }
}
