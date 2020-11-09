import QtQuick.Controls 2.12
import QtQuick.Controls.impl 2.12
import QtQuick 2.12

Button {
  padding: 8
  topInset: 2
  bottomInset: 2

  background: Rectangle {
      anchors.fill: parent
      color: !parent.enabled ? Theme.lightGray : parent.down ? "#5a8725" : Theme.mainColor
      radius: 12
      Behavior on color {
        PropertyAnimation {
          duration: 25
          easing.type: Easing.InQuart
        }
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

