

# File QFieldGuide.qml

[**File List**](files.md) **>** [**qml**](dir_1d81f7f4362e4398c0a695c4e466fcb0.md) **>** [**QFieldGuide.qml**](QFieldGuide_8qml.md)

[Go to the documentation of this file](QFieldGuide_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Shapes
import QtQuick.Window
import QtQuick.Controls.Material
import org.qfield
import Theme

Popup {
  id: guide
  required property var baseRoot
  property bool enablePanelAnimation: false
  property bool allowedToShow: true
  property var steps: []
  property int targetMargins: 5
  property int index: 0
  property string finishText: qsTr("Finish")
  property string nextText: qsTr("Next")
  property string previousText: qsTr("Previous")

  signal guideFinished

  padding: 0
  parent: Overlay.overlay
  width: internalObject.parentWidth
  height: internalObject.parentHeight
  background: Item {}
  contentItem: Item {}
  clip: true
  onVisibleChanged: {
    if (visible) {
      guide.index = 0;
      enablePanelAnimation = false;
    }
  }
  onIndexChanged: {
    canvas.requestPaint();
    if (index == 1) {
      enablePanelAnimation = true;
    }
    if (steps[index] && steps[index].type === "action") {
      steps[index].forwardAction();
      delayedPainter.restart();
    }
  }

  function blockGuide() {
    allowedToShow = false;
    if (opened) {
      close();
    }
  }

  function runTour() {
    if (allowedToShow)
      open();
  }

  Item {
    id: internalObject
    property var window: Window.window
    property point pos: Qt.point(0, 0)
    property var step: steps[index]
    property var target: {
      if (steps[index]) {
        if (steps[index].type === "information") {
          return steps[index].target();
        } else if (steps[index].type === "action") {
          let objectAfterAction = undefined;
          if (index + 1 < steps.length) {
            objectAfterAction = steps[index + 1].target();
          } else {
            objectAfterAction = steps[index].target();
          }
          return objectAfterAction;
        }
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
      var ctx = canvas.getContext("2d");
      ctx.clearRect(0, 0, canvasSize.width, canvasSize.height);
      ctx.save();
      ctx.fillStyle = "#88000000";
      ctx.fillRect(0, 0, canvasSize.width, canvasSize.height);
      ctx.globalCompositeOperation = 'destination-out';
      ctx.fillStyle = 'black';
      for (let i = 0; i < internalObject.target.length; ++i) {
        internalObject.pos = internalObject.target[i].mapToGlobal(0, 0);
        internalObject.pos = Qt.point(internalObject.pos.x - internalObject.window.x, internalObject.pos.y - internalObject.window.y);
        var rect = Qt.rect(internalObject.pos.x - guide.targetMargins, internalObject.pos.y - guide.targetMargins, internalObject.target[i].width + guide.targetMargins * 2, internalObject.target[i].height + guide.targetMargins * 2);
        drawRoundedRect(rect, 8, ctx);
      }
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

    MouseArea {
      anchors.fill: parent
    }
  }

  Rectangle {
    id: hintPanel
    property int dir: {
      if (y < internalObject.pos.y)
        return 1;
      return 0;
    }

    color: Theme.mainBackgroundColor
    objectName: "hintPanel"
    width: Math.max(250, baseRoot.width / 2)
    height: 88 + description.height + (animatedHint.visible ? animatedHint.height + 8 : 20)
    radius: 4
    clip: true

    Behavior on x {
      enabled: enablePanelAnimation

      NumberAnimation {
        id: panelXAnimation
        duration: 150
      }
    }
    Behavior on y {
      enabled: enablePanelAnimation

      NumberAnimation {
        id: panelYAnimation
        duration: 150
      }
    }

    x: {
      if (internalObject.target && internalObject.target[0]) {
        return Math.min(Math.max(8, internalObject.pos.x + internalObject.target[0].width / 2 - width / 2), guide.width - width - 10);
      }
      return 0;
    }
    y: {
      if (internalObject.target && internalObject.target[0]) {
        var ty = internalObject.pos.y + internalObject.target[0].height + guide.targetMargins + 15;
        if ((ty + height) >= guide.height) {
          if (internalObject.pos.y - height - guide.targetMargins - 8 >= 0) {
            return internalObject.pos.y - height - guide.targetMargins - 8;
          } else {
            return ty - height - 28 - mainWindow.sceneBottomMargin;
          }
        }
        return ty - 8;
      }
      return 0;
    }

    QfProgrerssRing {
      id: progerss
      value: (guide.index + 1) / steps.length
      size: nextButton.height
      anchors {
        verticalCenter: nextButton.verticalCenter
        left: parent.left
        leftMargin: 15
      }
    }

    Text {
      id: title
      objectName: "guideInternalTitle"
      font: Theme.strongFont
      elide: Text.ElideRight
      text: {
        if (internalObject.step) {
          return internalObject.step.title;
        }
        return "";
      }
      color: Theme.mainColor
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
      objectName: "guideInternalDescription"
      font: Theme.tipFont
      wrapMode: Text.WordWrap
      color: Theme.mainTextColor
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
      visible: internalObject.step ? internalObject.step.animatedGuide !== undefined : false
      source: visible ? internalObject.step ? internalObject.step.animatedGuide : "" : ""
      anchors {
        bottom: parent.bottom
        left: parent.left
        right: hintPanel.right
        rightMargin: 15
        bottomMargin: 15
      }
      fillMode: AnimatedImage.PreserveAspectFit
    }

    QfButton {
      id: nextButton
      objectName: "nextButton"

      property bool isLast: guide.index === steps.length - 1

      text: isLast ? guide.finishText : guide.nextText
      anchors {
        bottom: parent.bottom
        right: parent.right
        rightMargin: 15
        bottomMargin: 15
      }
      bgcolor: Theme.mainColor
      height: 32
      radius: 5
      onClicked: {
        if (isLast) {
          guide.close();
          guideFinished();
        } else {
          guide.index = guide.index + 1;
        }
      }
    }

    QfButton {
      id: previousButton
      objectName: "previousButton"
      text: guide.previousText
      bgcolor: "transparent"
      color: Theme.mainColor
      height: 32
      radius: 5
      visible: guide.index !== 0
      anchors {
        right: nextButton.left
        bottom: parent.bottom
        bottomMargin: 15
        rightMargin: 15
      }
      onClicked: {
        if (steps[index - 1].type === "action") {
          steps[index - 1].backwardAction();
        } else {
          guide.index -= 1;
        }
      }
    }

    QfToolButton {
      anchors {
        right: parent.right
        top: parent.top
        topMargin: 8
        rightMargin: 8
      }
      width: 30
      height: width
      icon.color: Theme.mainTextColor
      icon.source: Theme.getThemeVectorIcon('ic_close_white_24dp')
      padding: 0
      bgcolor: "transparent"

      onClicked: {
        guide.close();
      }
    }
  }

  Shape {
    id: pointerToItem
    rotation: hintPanel.dir ? 0 : 180
    visible: panelXAnimation.running || panelYAnimation.running ? 0 : 1

    x: {
      if (internalObject.target && internalObject.target[0]) {
        return internalObject.pos.x + internalObject.target[0].width / 2 - pointerToItem.width / 2;
      }
      return 0;
    }
    y: {
      if (internalObject.target && internalObject.target[0]) {
        return internalObject.pos.y + (hintPanel.dir ? -(height + 4) : internalObject.target[0].height + 4);
      }
      return 0;
    }

    ShapePath {
      fillColor: Theme.mainBackgroundColor
      strokeWidth: 1
      strokeColor: Theme.mainBackgroundColor
      PathSvg {
        path: "M 0 0 L 20 0 L 10 10 Z"
      }
    }
  }
}
```


