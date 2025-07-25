import QtQuick
import QtQuick.Controls

Rectangle {
  id: badge

  enum Alignment {
    TopLeft,
    TopRight,
    BottomLeft,
    BottomRight
  }

  property alias badgeText: inText
  property int alignment: QfBadge.Alignment.TopRight

  property int topMargin: 2
  property int bottomMargin: 2
  property int leftMargin: 2
  property int rightMargin: 2

  width: 12
  height: 12
  radius: width / 2

  border.width: 1.5
  border.color: Theme.mainBackgroundColor

  anchors {
    top: (alignment === QfBadge.Alignment.TopLeft || alignment === QfBadge.Alignment.TopRight) ? parent.top : undefined
    bottom: (alignment === QfBadge.Alignment.BottomLeft || alignment === QfBadge.Alignment.BottomRight) ? parent.bottom : undefined
    left: (alignment === QfBadge.Alignment.TopLeft || alignment === QfBadge.Alignment.BottomLeft) ? parent.left : undefined
    right: (alignment === QfBadge.Alignment.TopRight || alignment === QfBadge.Alignment.BottomRight) ? parent.right : undefined

    topMargin: badge.topMargin
    bottomMargin: badge.bottomMargin
    leftMargin: badge.leftMargin
    rightMargin: badge.rightMargin
  }

  Text {
    id: inText
    anchors.fill: parent
    font.pointSize: Math.max(8, height > 0 ? (text.length === 1 ? height / 2 : height / 2.2) : 8)
    horizontalAlignment: Qt.AlignHCenter
    verticalAlignment: Qt.AlignVCenter
  }
}
