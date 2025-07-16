import QtQuick
import QtQuick.Controls

Rectangle {
  property alias badgeText: inText

  anchors {
    top: parent.top
    right: parent.right
    rightMargin: 2
    topMargin: 2
  }

  width: 12
  height: 12
  radius: width / 2

  border.width: 1.5
  border.color: "white"

  Text {
    id: inText
    anchors.fill: parent
    font.pixelSize: text.length === 1 ? height / 1.6 : height / 1.8
    horizontalAlignment: Qt.AlignHCenter
    verticalAlignment: Qt.AlignVCenter
  }
}
