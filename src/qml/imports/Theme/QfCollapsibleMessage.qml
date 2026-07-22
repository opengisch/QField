import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Controls.Material.impl
import org.qfield

/**
 * \ingroup qml
 */
Item {
  id: collapsibleMessage

  property bool collapsed: true
  property string externalLink: ""
  property color color: Theme.mainTextColor
  property alias iconSource: messageIcon.iconSource
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
    color: Qt.rgba(collapsibleMessage.color.r, collapsibleMessage.color.g, collapsibleMessage.color.b, 0.1)
    border.color: collapsibleMessage.color
    border.width: 1
    radius: 8
  }

  QfToolButton {
    id: messageIcon
    visible: iconSource !== ''
    anchors.left: parent.left
    anchors.verticalCenter: titleText.verticalCenter
    enabled: false
    bgcolor: "transparent"
    iconColor: collapsibleMessage.color
  }

  Text {
    id: titleText
    width: parent.width - 5
    anchors.top: parent.top
    anchors.left: parent.left
    leftPadding: messageIcon.visible ? messageIcon.width + 2 : 8
    topPadding: 10
    bottomPadding: 10
    clip: true
    font: Theme.defaultFont
    color: Theme.mainTextColor
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
      color: collapsibleMessage.color
      radius: background.radius
    }
  }

  Rectangle {
    id: separator
    width: parent.width - 36
    anchors.top: titleText.bottom
    anchors.horizontalCenter: parent.horizontalCenter
    height: 1
    visible: detailsText.text !== ''
    color: collapsibleMessage.color
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
      if (detailsText.text !== '') {
        parent.collapsed = !parent.collapsed;
      }
    }
  }
}
