

# File QfCollapsibleMessage.qml

[**File List**](files.md) **>** [**imports**](dir_3be62dd4600925273911e91e0c7964f3.md) **>** [**Theme**](dir_1633596792308d5fdfbf00fb99c556ce.md) **>** [**QfCollapsibleMessage.qml**](QfCollapsibleMessage_8qml.md)

[Go to the documentation of this file](QfCollapsibleMessage_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Controls.Material.impl
import org.qfield

Item {
  property bool collapsed: true
  property string externalLink: ""
  property alias color: titleText.color
  property alias detailsColor: detailsText.color
  property alias font: titleText.font
  property alias titleText: titleText.text
  property alias detailsText: detailsText.text

  clip: true
  height: collapsed ? titleText.height + 1 : titleText.height + detailsText.height + 1
  implicitHeight: height

  Behavior on height {
    NumberAnimation {
      duration: 100
      easing.type: Easing.InQuad
    }
  }

  Rectangle {
    id: background
    anchors.fill: parent
    color: "transparent"
    border.color: titleText.color
    border.width: 1
    opacity: 0.25
    radius: 12
  }

  Text {
    id: titleText
    width: parent.width - 5
    anchors.top: parent.top
    anchors.left: parent.left
    leftPadding: 8
    topPadding: 10
    bottomPadding: 10
    clip: true
    font: Theme.defaultFont
    color: "black"
    horizontalAlignment: Text.AlignLeft
    wrapMode: Text.WordWrap
  }

  ToolButton {
    id: externalLinkButton
    z: mainMouseArea.z + 1
    visible: externalLink !== ''
    flat: false
    text: "?"
    anchors.verticalCenter: titleText.verticalCenter
    anchors.right: parent.right
    anchors.rightMargin: 4
    highlighted: true
    Material.accent: Theme.mainBackgroundColor
    font.bold: true
    onClicked: {
      Qt.openUrlExternally(externalLink);
    }
    background: Rectangle {
      implicitWidth: 30
      implicitHeight: 30
      color: titleText.color
      radius: background.radius
    }
  }

  Rectangle {
    id: separator
    width: parent.width - 36
    anchors.top: titleText.bottom
    anchors.horizontalCenter: parent.horizontalCenter
    height: 1
    color: titleText.color
    opacity: 0.25
  }

  Text {
    id: detailsText
    width: parent.width - 5
    anchors.top: separator.bottom
    anchors.right: externalLinkButton.left
    anchors.left: titleText.left
    leftPadding: 8
    topPadding: 10
    bottomPadding: 10
    clip: true
    font.pointSize: titleText.font.pointSize / 1.5
    font.weight: titleText.font.weight
    font.italic: titleText.font.italic
    font.family: titleText.font.family
    color: titleText.color
    wrapMode: Text.WordWrap
  }

  MouseArea {
    id: mainMouseArea
    anchors.fill: parent
    onClicked: {
      parent.collapsed = !parent.collapsed;
    }
  }
}
```


