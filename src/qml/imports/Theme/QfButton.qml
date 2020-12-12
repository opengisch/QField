import QtQuick.Controls 2.12
import QtQuick 2.12

import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Material.impl 2.12

Button {
  id: button
  padding: 8
  topInset: 2
  bottomInset: 2

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
  contentItem: Text {
      text: parent.text
      font: Theme.tipFont
      color: "white"
      horizontalAlignment: Text.AlignHCenter
      verticalAlignment: Text.AlignVCenter
      elide: Text.ElideRight
  }
}

