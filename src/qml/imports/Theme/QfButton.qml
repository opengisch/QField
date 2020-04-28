import QtQuick.Controls 2.12
import QtQuick 2.12

Button {
  padding: 8 * dp
  topInset: 2 * dp
  bottomInset: 2 * dp

  background: Rectangle {
      anchors.fill: parent
      color: !parent.enabled ? Theme.lightGray : parent.down ? "#5a8725" : Theme.mainColor
      radius: 12 * dp
      Behavior on color {
        PropertyAnimation {
          duration: 25
          easing.type: Easing.InQuart
        }
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

