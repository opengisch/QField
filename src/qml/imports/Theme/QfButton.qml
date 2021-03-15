import QtQuick.Controls 2.12
import QtQuick.Controls.impl 2.12
import QtQuick 2.12

import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Material.impl 2.12

Button {
  id: button
  property color bgcolor: Theme.mainColor
  padding: 8
  topInset: 2
  bottomInset: 2
  icon.color: 'white'

  background: Rectangle {
      anchors.fill: parent
      color: !parent.enabled ? Theme.lightGray : Theme.mainColor
      radius: 12

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
    spacing: parent.spacing
    mirrored: parent.mirrored
    display: parent.display

    icon: parent.icon
    color: 'white'
    font: Theme.tipFont
    alignment: Qt.AlignCenter | Qt.AlignVCenter
    text: parent.text
  }
}

