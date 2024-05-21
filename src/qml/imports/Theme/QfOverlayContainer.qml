import QtQuick 2.14
import Theme 1.0

Rectangle {
  id: container

  default property alias contents: containerLayout.children
  property alias title: title.text

  width: parent.width
  height: childrenRect.height
  color: Theme.mainBackgroundColorSemiOpaque
  radius: 8
  clip: true

  Column {
    id: containerLayout
    width: parent.width - 10
    anchors.horizontalCenter: parent.horizontalCenter
    topPadding: 5
    bottomPadding: 5
    spacing: 4

    Text {
      id: title
      text: "Sensors"
      font: Theme.strongTipFont
      color: Theme.mainTextColor
      leftPadding: 6
    }
  }
}
