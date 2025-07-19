import QtQuick
import QtQuick.Controls

Rectangle {
  id: badge

  property alias badgeText: inText
  property int alignment: QfBadge.TopRight

  property int topMargin: 2
  property int bottomMargin: 2
  property int leftMargin: 2
  property int rightMargin: 2

  width: 12
  height: 12
  radius: width / 2

  border.width: 1.5
  border.color: Theme.darkTheme ? Theme.mainTextColor : Theme.mainBackgroundColor

  anchors {
    top: (alignment === QfBadge.TopLeft || alignment === QfBadge.TopRight) ? parent.top : undefined
    bottom: (alignment === QfBadge.BottomLeft || alignment === QfBadge.BottomRight) ? parent.bottom : undefined
    left: (alignment === QfBadge.TopLeft || alignment === QfBadge.BottomLeft) ? parent.left : undefined
    right: (alignment === QfBadge.TopRight || alignment === QfBadge.BottomRight) ? parent.right : undefined

    topMargin: badge.topMargin
    bottomMargin: badge.bottomMargin
    leftMargin: badge.leftMargin
    rightMargin: badge.rightMargin
  }

  Text {
    id: inText
    anchors.fill: parent
    font.pixelSize: text.length === 1 ? height / 1.6 : height / 1.8
    horizontalAlignment: Qt.AlignHCenter
    verticalAlignment: Qt.AlignVCenter
  }
}
