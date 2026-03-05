

# File Toast.qml

[**File List**](files.md) **>** [**qml**](dir_1d81f7f4362e4398c0a695c4e466fcb0.md) **>** [**Toast.qml**](Toast_8qml.md)

[Go to the documentation of this file](Toast_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import org.qfield
import Theme

Popup {
  id: toast

  property string type: 'info'
  property double edgeSpacing: 54
  property double bottomSpacing: 0
  property var act: undefined
  property var timeoutAct: undefined
  property bool timeoutFeedback: false

  property real virtualKeyboardHeight: {
    if (Qt.platform.os === "android") {
      const top = Qt.inputMethod.keyboardRectangle.top / Screen.devicePixelRatio;
      if (top > 0) {
        const height = Qt.inputMethod.keyboardRectangle.height / Screen.devicePixelRatio;
        return height - (top + height - mainWindow.height);
      }
    }
    return 0;
  }

  x: edgeSpacing
  y: mainWindow.height
  z: 10001

  width: mainWindow.width - edgeSpacing * 2
  height: toastMessage.contentHeight + 20
  topMargin: 0
  leftMargin: 0
  rightMargin: 0
  bottomMargin: 60 + Math.max(bottomSpacing, virtualKeyboardHeight)
  padding: 0
  closePolicy: Popup.NoAutoClose

  background: Item {}

  onClosed: {
    toastTimer.stop();
    animationTimer.stop();
    animationTimer.reset();
    toast.timeoutAct = undefined;
    timeoutFeedback = false;
  }

  Rectangle {
    id: toastContent
    z: 1
    width: toastRow.width + 20
    height: toastMessage.contentHeight + 10
    anchors.centerIn: parent

    color: "#66212121"
    radius: 4
    opacity: 0

    Behavior on opacity {
      NumberAnimation {
        duration: 250
      }
    }

    onOpacityChanged: {
      if (opacity === 0.0) {
        toast.close();
      }
    }

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

        visible: text != ''
        height: toastMessage.height

        radius: 4
        bgcolor: "#99000000"
        color: Theme.mainColor
        font.pointSize: Theme.tipFont.pointSize

        onClicked: {
          if (toast.act !== undefined) {
            toast.act();
          }
          toast.close();
          toastContent.opacity = 0;
        }
      }
    }
  }

  MouseArea {
    anchors.fill: parent
    onPressed: {
      if (toast.timeoutAct !== undefined) {
        toast.timeoutAct();
      }
      toast.close();
      toastContent.opacity = 0;
    }
  }

  FontMetrics {
    id: toastFontMetrics
    font: toastMessage.font
  }

  Timer {
    id: animationTimer
    interval: 50
    repeat: true

    property real position: 0

    onTriggered: {
      position += interval;
      if (animationProgressBar.value === 1) {
        animationTimer.stop();
      }
    }

    function reset() {
      position = 0;
    }
  }

  Timer {
    id: toastTimer
    interval: 3000
    repeat: false
    onTriggered: {
      if (toast.timeoutAct !== undefined) {
        toast.timeoutAct();
      }
      toastContent.opacity = 0;
    }
  }

  function show(text, type, action_text, action_function, timeout_feedback, timeout_function) {
    if (toastTimer.running) {
      if (toastMessage.text === text) {
        if (animationTimer.running) {
          animationTimer.reset();
          animationTimer.restart();
        }
        toastTimer.restart();
        return;
      } else {
        if (toast.timeoutAct !== undefined) {
          toast.timeoutAct();
        }
      }
    }
    toastMessage.text = text;
    toast.type = type || 'info';
    if (timeout_feedback !== undefined) {
      toast.timeoutFeedback = timeout_feedback;
    } else {
      toast.timeoutFeedback = false;
    }
    if (timeout_function !== undefined) {
      toast.timeoutAct = timeout_function;
    } else {
      toast.timeoutAct = undefined;
    }
    if (action_text !== undefined && action_function !== undefined) {
      toastAction.text = action_text;
      toast.act = action_function;
      toastTimer.interval = 5000;
    } else {
      toastAction.text = '';
      toast.act = undefined;
      toastTimer.interval = 3000;
    }
    toastContent.opacity = 1;
    toast.open();
    toastTimer.restart();
    if (toast.timeoutFeedback) {
      animationTimer.reset();
      animationTimer.restart();
    }
  }
}
```


