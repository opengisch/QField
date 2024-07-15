import QtQuick
import QtQuick.Controls
import QtQuick.Shapes
import QtQuick.Window
import QtQuick.Controls.Material
import Theme

Popup {
  id: guide
  padding: 0
  parent: Overlay.overlay
  width: internalObject.parentWidth
  height: internalObject.parentHeight
  background: Item {
  }
  contentItem: Item {
  }
  clip: true
  onVisibleChanged: {
    if (visible) {
      guide.index = 0;
      enablePannelAnimation = false;
    }
  }
  onIndexChanged: {
    canvas.requestPaint();
    if (index == 1)
      enablePannelAnimation = true;
  }

  property bool enablePannelAnimation: false
  property var steps: []
  property int targetMargins: 5
  property Component nextButton: nextButton
  property Component prevButton: prevButton
  property int index: 0
  property string finishText: qsTr("Finish")
  property string nextText: qsTr("Next")
  property string previousText: qsTr("Previous")

  Component {
    id: nextButton
    QfButton {
      text: isEnd ? guide.finishText : guide.nextText
      verticalPadding: 0
      horizontalPadding: 12
      bgcolor: Theme.mainColor
      color: "white"
      height: 32
      radius: 5
      onClicked: {
        if (isEnd) {
          guide.close();
        } else {
          guide.index = guide.index + 1;
        }
      }
    }
  }

  Component {
    id: prevButton
    QfButton {
      text: guide.previousText
      verticalPadding: 0
      horizontalPadding: 12
      bgcolor: "#00000000"
      color: Theme.gray
      height: 32
      radius: 5
      onClicked: {
        guide.index -= 1;
      }
    }
  }

  Item {
    id: internalObject
    property var window: Window.window
    property point pos: Qt.point(0, 0)
    property var step: steps[index]
    property var target: {
      if (steps[index]) {
        return steps[index].target();
      }
      return undefined;
    }
    property int parentHeight: {
      if (guide.parent) {
        return guide.parent.height;
      }
      return guide.height;
    }
    property int parentWidth: {
      if (guide.parent) {
        return guide.parent.width;
      }
      return guide.width;
    }
  }

  Connections {
    target: internalObject.window
    function onWidthChanged() {
      canvas.requestPaint();
      delayedPainter.restart();
    }
    function onHeightChanged() {
      canvas.requestPaint();
      delayedPainter.restart();
    }
  }

  // not to draw for every frame of resizing
  Timer {
    id: delayedPainter
    interval: 200
    onTriggered: {
      canvas.requestPaint();
    }
  }

  Canvas {
    id: canvas
    anchors.fill: parent
    onPaint: {
      internalObject.pos = internalObject.target.mapToGlobal(0, 0);
      internalObject.pos = Qt.point(internalObject.pos.x - internalObject.window.x, internalObject.pos.y - internalObject.window.y);
      var ctx = canvas.getContext("2d");
      ctx.clearRect(0, 0, canvasSize.width, canvasSize.height);
      ctx.save();
      ctx.fillStyle = "#88000000";
      ctx.fillRect(0, 0, canvasSize.width, canvasSize.height);
      ctx.globalCompositeOperation = 'destination-out';
      ctx.fillStyle = 'black';
      var rect = Qt.rect(internalObject.pos.x - guide.targetMargins, internalObject.pos.y - guide.targetMargins, internalObject.target.width + guide.targetMargins * 2, internalObject.target.height + guide.targetMargins * 2);
      drawRoundedRect(rect, 8, ctx);
      ctx.restore();
    }

    function drawRoundedRect(rect, r, ctx) {
      ctx.beginPath();
      ctx.moveTo(rect.x + r, rect.y);
      ctx.lineTo(rect.x + rect.width - r, rect.y);
      ctx.arcTo(rect.x + rect.width, rect.y, rect.x + rect.width, rect.y + r, r);
      ctx.lineTo(rect.x + rect.width, rect.y + rect.height - r);
      ctx.arcTo(rect.x + rect.width, rect.y + rect.height, rect.x + rect.width - r, rect.y + rect.height, r);
      ctx.lineTo(rect.x + r, rect.y + rect.height);
      ctx.arcTo(rect.x, rect.y + rect.height, rect.x, rect.y + rect.height - r, r);
      ctx.lineTo(rect.x, rect.y + r);
      ctx.arcTo(rect.x, rect.y, rect.x + r, rect.y, r);
      ctx.closePath();
      ctx.fill();
    }
  }

  Rectangle {
    id: hintPannel
    width: Math.max(250, mainWindow.width / 2)
    height: 88 + description.height + (animatedHint.visible ? animatedHint.height + 8 : 20)
    radius: 4
    clip: true

    Behavior on x  {
      enabled: enablePannelAnimation

      NumberAnimation {
        duration: 150
      }
    }
    Behavior on y  {
      enabled: enablePannelAnimation

      NumberAnimation {
        duration: 150
      }
    }

    property color color: Theme.mainBackgroundColor
    property int dir: {
      if (y < internalObject.pos.y)
        return 1;
      return 0;
    }
    x: {
      if (internalObject.target) {
        return Math.min(Math.max(8, internalObject.pos.x + internalObject.target.width / 2 - width / 2), guide.width - width - 10);
      }
      return 0;
    }
    y: {
      if (internalObject.target) {
        var ty = internalObject.pos.y + internalObject.target.height + guide.targetMargins + 15;
        if ((ty + height) >= guide.height) {
          return internalObject.pos.y - height - guide.targetMargins - 8;
        }
        return ty - 8;
      }
      return 0;
    }

    Text {
      font.bold: true
      elide: Text.ElideRight
      text: {
        if (internalObject.step) {
          return internalObject.step.title;
        }
        return "";
      }
      anchors {
        top: parent.top
        left: parent.left
        topMargin: 15
        leftMargin: 15
        right: parent.right
        rightMargin: 32
      }
    }

    Text {
      id: description
      wrapMode: Text.WrapAnywhere
      maximumLineCount: 4
      elide: Text.ElideRight
      text: {
        if (internalObject.step) {
          return internalObject.step.description;
        }
        return "";
      }
      anchors {
        top: parent.top
        left: parent.left
        right: parent.right
        rightMargin: 15
        topMargin: 42
        leftMargin: 15
      }
    }

    AnimatedImage {
      id: animatedHint
      width: 133
      height: 100
      visible: internalObject.step.animatedGuide !== undefined
      source: visible ? internalObject.step.animatedGuide : ""
      anchors.top: description.bottom
      anchors.left: parent.left
      fillMode: AnimatedImage.PreserveAspectFit
      anchors.leftMargin: 14
      anchors.topMargin: 8
    }

    Loader {
      id: nextButtonLoader
      property bool isEnd: guide.index === steps.length - 1
      sourceComponent: nextButton
      anchors {
        bottom: parent.bottom
        bottomMargin: 8
        right: parent.right
        rightMargin: 15
      }
    }
    Loader {
      id: prevButtonLoader
      visible: guide.index !== 0
      sourceComponent: prevButton
      anchors {
        right: nextButtonLoader.left
        bottom: parent.bottom
        bottomMargin: 8
        rightMargin: 14
      }
    }

    QfButton {
      anchors {
        right: parent.right
        top: parent.top
        margins: 10
      }
      width: 20
      height: width
      bgcolor: Theme.darkRed
      onClicked: {
        guide.close();
      }
    }
  }

  Image {
    id: pointerToItem
    width: 48
    height: width
    source: hintPannel.dir ? Theme.getThemeVectorIcon("ic_arrow_drop_down_48dp") : Theme.getThemeVectorIcon("ic_arrow_drop_up_48dp")

    Behavior on x  {
      enabled: enablePannelAnimation

      NumberAnimation {
        duration: 150
      }
    }
    Behavior on y  {
      enabled: enablePannelAnimation

      NumberAnimation {
        duration: 150
      }
    }

    x: {
      if (internalObject.target) {
        return internalObject.pos.x < 10 ? 16 : internalObject.pos.x - 12;
      }
      return 0;
    }
    y: {
      if (internalObject.target) {
        return internalObject.pos.y + (hintPannel.dir ? -(height - 14) : internalObject.target.height - 14);
      }
      return 0;
    }
  }
}
