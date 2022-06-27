import QtQuick.Controls 2.14
import QtQuick.Controls.impl 2.14
import QtQuick 2.14

import QtQuick.Controls.Material 2.14
import QtQuick.Controls.Material.impl 2.14

Button {
  id: button
  property color bgcolor: Theme.mainColor
  property alias color: label.color

  padding: 8
  leftPadding: 8
  rightPadding: 8
  leftInset: 4
  rightInset: 4
  topInset: 2
  bottomInset: 2

  icon.color: button.color

  background: Rectangle {
      anchors.fill: parent
      color: !parent.enabled ? Theme.lightGray : button.bgcolor
      radius: 12
      border.width: 1
      border.color: !parent.enabled
                    ? Theme.lightGray
                    : button.bgcolor != "#00000000" ? button.bgcolor : button.color

      Ripple {
          clip: true
          width: parent.width
          height: parent.height
          pressed: button.down
          anchor: parent
          active: button.down
          color: Material.rippleColor
      }
  }

  contentItem: IconLabel {
    id: label
    spacing: parent.spacing
    mirrored: parent.mirrored
    display: parent.display

    icon: parent.icon
    color: "white"
    font: Theme.tipFont
    alignment: Qt.AlignCenter | Qt.AlignVCenter
    text: parent.text
  }
}

