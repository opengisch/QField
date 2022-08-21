import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Controls.Material 2.14
import QtQuick.Controls.Material.impl 2.14

import Theme 1.0

RoundButton {
  id: button

  property string iconSource: ''
  property string bottomRightIndicatorText: ''
  property string bottomRightIndicatorBgColor: Theme.cloudColor
  property string bottomRightIndicatorFgColor: Theme.light
  property bool round: false
  property bool roundborder: false
  property color bgcolor: 'transparent'
  property color borderColor: bgcolor

  width: 48
  height: 48
  implicitWidth: width
  implicitHeight: height

  focusPolicy: Qt.NoFocus
  topInset:0
  bottomInset:0
  leftInset:0
  rightInset:0
  padding:10
  background: Rectangle {
    implicitWidth: 100
    implicitHeight: 25
    border.width: round && roundborder ? height / 6 : !round
    border.color: borderColor
    color: bgcolor
    radius: round ? height / 2 : 0

    Behavior on color {
      ColorAnimation {
        duration: 200
      }
    }

    Ripple {
      id: ripple
      clip: true
      width: parent.width
      height: parent.height
      clipRadius: 4
      pressed: button.down
      anchor: parent
      active: button.down
      color: bgcolor == "#ffffff" || bgcolor == "#00000000" ? "#10000000" : "#22ffffff"

      layer.enabled: true
      layer.effect: QfOpacityMask {
        maskSource: Rectangle
        {
          width: ripple.width
          height: ripple.height
          radius: ripple.height / 2
        }
      }
    }
  }

  contentItem: Rectangle {
    implicitWidth: 100
    implicitHeight: 25
    color: "transparent"
    z: 1

    Image {
      anchors.fill: parent
      fillMode: Image.PreserveAspectFit
      horizontalAlignment: Image.AlignHCenter
      verticalAlignment: Image.AlignVCenter
      source: iconSource
      sourceSize.width: 24 * screen.devicePixelRatio
      sourceSize.height: 24 * screen.devicePixelRatio
    }
  }

  Rectangle {
    id: bottomRightIndicator
    color: bottomRightIndicatorBgColor
    anchors.bottom: button.bottom
    anchors.right: button.right
    anchors.bottomMargin: 7
    anchors.rightMargin: 5
    width: button.width / 2.5
    height: width
    visible: bottomRightIndicatorText
    radius: width
    border.width: 2
    border.color: Theme.mainColor
    z: 2

    Text {
      anchors.fill: parent
      color: bottomRightIndicatorFgColor
      text: bottomRightIndicatorText
      font.pixelSize: bottomRightIndicatorText.length == 1
                      ? height / 1.6
                      : height / 1.8
      horizontalAlignment: Qt.AlignHCenter
      verticalAlignment: Qt.AlignVCenter
    }
  }
}
