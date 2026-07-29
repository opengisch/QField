import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import org.qfield
import Theme

/**
 * \ingroup qml
 */
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

    property int contentPadding: 20
    property int topPadding: toastLayout.columns === 1 ? 8 : 10
    property int bottomPadding: toastLayout.columns === 1 ? 12 : 10
    property int actionWidth: toastAction.visible ? toastAction.width + 10 : 0
    property int absoluteMessageWidth: toastFontMetrics.boundingRect(toastMessage.text).width + actionWidth + 10
    property int unrestrainedWidth: contentPadding * 2 + toastFontMetrics.boundingRect(toastMessage.text).width + actionWidth + 10

    z: 1
    width: Math.min(unrestrainedWidth, toast.width - 20)
    height: toastLayout.height + topPadding + bottomPadding
    anchors.centerIn: parent

    color: "#CC202020"
    border.color: "#CC404040"
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
      z: toastLayout.z - 1
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

    Rectangle {
      id: toastIndicator
      anchors.left: parent.left
      anchors.top: parent.top
      anchors.bottom: parent.bottom
      width: 5
      topLeftRadius: toastContent.radius
      bottomLeftRadius: toastContent.radius
      topRightRadius: 0
      bottomRightRadius: 0
      color: toast.type === 'error' ? Theme.errorColor : Theme.warningColor
      visible: toast.type != 'info'
    }

    GridLayout {
      id: toastLayout
      width: parent.width - toastContent.contentPadding * 2
      anchors.top: parent.top
      anchors.left: parent.left
      anchors.topMargin: toastContent.topPadding
      anchors.leftMargin: toastContent.contentPadding
      columnSpacing: 10
      rowSpacing: 12
      columns: toastContent.absoluteMessageWidth > mainWindow.width * 1.75 ? 1 : 2

      Text {
        id: toastMessage
        Layout.fillWidth: true
        wrapMode: Text.Wrap
        color: Theme.light

        font: Theme.defaultFont
        horizontalAlignment: Text.AlignLeft
      }

      QfButton {
        id: toastAction
        Layout.alignment: (toastLayout.columns === 1 ? Qt.AlignLeft : Qt.AlignHCenter) | Qt.AlignVCenter
        visible: text != ''
        radius: 4
        bgcolor: "#00000000"
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
