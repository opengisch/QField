import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
  implicitHeight: checked ? header.height + body.childrenRect.height + 20 : 60

  radius: 8
  color: Theme.groupBoxBackgroundColor
  clip: true

  property alias title: headerText.text
  property alias checked: enabledSwitch.checked
  property alias interactive: enabledSwitch.visible
  property alias icon: headerIcon.source
  default property alias content: body.children

  Behavior on implicitHeight  {
    NumberAnimation {
      duration: 200
    }
  }

  Item {
    id: header
    anchors.left: parent.left
    anchors.right: parent.right
    height: 60
    anchors.margins: 20

    Image {
      id: headerIcon
      anchors.verticalCenter: parent.verticalCenter
      anchors.left: parent.left
      fillMode: Image.PreserveAspectFit
      smooth: true
      sourceSize.width: 25
      sourceSize.height: 25
      visible: source != ""
    }

    Label {
      id: headerText
      font: Theme.strongTitleFont
      color: Theme.mainTextColor
      wrapMode: Text.WordWrap
      anchors.left: headerIcon.right
      anchors.margins: headerIcon.visible ? 8 : 0
      anchors.right: enabledSwitch.left
      anchors.verticalCenter: parent.verticalCenter
    }

    QfSwitch {
      id: enabledSwitch
      anchors.right: parent.right
      anchors.verticalCenter: parent.verticalCenter
    }
  }

  Rectangle {
    id: splitter
    width: parent.width
    height: 1
    anchors.top: header.bottom
    color: Theme.groupBoxSurfaceColor
  }

  Item {
    id: body
    anchors.top: splitter.bottom
    anchors.bottom: parent.bottom
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.leftMargin: 20
    anchors.rightMargin: 20
    anchors.topMargin: 10
    anchors.bottomMargin: 10
  }
}
