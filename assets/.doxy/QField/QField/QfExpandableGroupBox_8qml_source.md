

# File QfExpandableGroupBox.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfExpandableGroupBox.qml**](QfExpandableGroupBox_8qml.md)

[Go to the documentation of this file](QfExpandableGroupBox_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield.core
import org.qfield.gui

Rectangle {
  id: expandableGroupBox
  implicitHeight: checked ? header.height + body.childrenRect.height + 30 : 60

  radius: 12
  color: QfTheme.groupBoxBackgroundColor
  border.width: 1
  border.color: QfTheme.controlBorderColor
  clip: true

  property alias title: headerText.text
  property alias checked: enabledSwitch.checked
  property alias interactive: enabledSwitch.visible
  property alias icon: headerIcon.iconSource
  property alias iconColor: headerIcon.iconColor
  default property alias content: body.children

  signal clicked

  Behavior on implicitHeight {
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

    MouseArea {
      id: headerMouseArea
      enabled: true
      anchors.verticalCenter: parent.verticalCenter
      anchors.left: parent.left
      anchors.right: enabledSwitch.left
      height: childrenRect.height

      QfToolButton {
        id: headerIcon
        width: 24
        height: 24
        padding: 0
        icon.width: 24
        icon.height: 24
        anchors.verticalCenter: headerText.verticalCenter
        anchors.left: parent.left
        enabled: false

        visible: iconSource !== ""
      }

      Label {
        id: headerText
        font: QfTheme.strongTitleFont
        color: QfTheme.mainTextColor
        wrapMode: Text.WordWrap
        anchors.left: headerIcon.right
        anchors.margins: headerIcon.visible ? 8 : 0
        anchors.right: parent.right
      }

      onClicked: mouse => {
        if (enabledSwitch.visible) {
          enabledSwitch.toggle();
          expandableGroupBox.clicked();
        }
      }
    }

    QfSwitch {
      id: enabledSwitch
      anchors.right: parent.right
      anchors.verticalCenter: parent.verticalCenter

      onClicked: {
        expandableGroupBox.clicked();
      }
    }
  }

  Rectangle {
    id: splitter
    width: parent.width
    height: 1
    anchors.top: header.bottom
    color: QfTheme.controlBorderColor
  }

  Item {
    id: body
    anchors.top: splitter.bottom
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.leftMargin: 20
    anchors.rightMargin: 20
    anchors.topMargin: 10
  }
}
```


