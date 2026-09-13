

# File QfActionButton.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfActionButton.qml**](QfActionButton_8qml.md)

[Go to the documentation of this file](QfActionButton_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Controls.Material.impl
import org.qfield.core

ToolButton {
  id: button

  property string toolImage: ''
  property string toolText: qsTr("close")
  property bool backgroundless: false
  property alias innerActionIcon: innerAction

  height: QfTheme.toolButtonSize
  width: height + (buttonText.visible ? buttonText.width + 24 : 0) + (innerAction.visible ? 32 : 0)
  clip: true

  Behavior on width {
    NumberAnimation {
      duration: 200
    }
  }

  background: Rectangle {
    width: parent.width
    height: QfTheme.toolButtonSize
    color: backgroundless ? "transparent" : QfTheme.toolButtonBackgroundSemiOpaqueColor
    radius: height / 2

    QfToolButton {
      anchors.left: parent.left
      anchors.top: parent.top
      width: QfTheme.toolButtonSize
      height: QfTheme.toolButtonSize
      enabled: false
      round: true
      iconSource: button.toolImage
      iconColor: button.backgroundless ? QfTheme.mainTextColor : QfTheme.toolButtonColor
      bgcolor: button.backgroundless ? "transparent" : QfTheme.toolButtonBackgroundColor
    }

    Ripple {
      id: ripple
      clip: true
      width: parent.width
      height: QfTheme.toolButtonSize
      clipRadius: 4
      pressed: button.down
      anchor: parent
      active: button.down
      color: "#22aaaaaa"
    }
  }

  contentItem: Row {
    anchors.left: parent.left
    anchors.leftMargin: QfTheme.toolButtonSize + 8
    spacing: 8
    padding: 0
    visible: button.toolText !== ""

    Text {
      id: buttonText
      anchors.verticalCenter: parent.verticalCenter
      verticalAlignment: Text.AlignVCenter
      text: button.toolText
      color: button.backgroundless ? QfTheme.mainTextColor : QfTheme.toolButtonColor
      font: QfTheme.strongFont
    }

    Image {
      id: innerAction
      anchors.verticalCenter: parent.verticalCenter
      width: 24
      height: 24
      fillMode: Image.PreserveAspectFit
      horizontalAlignment: Image.AlignHCenter
      verticalAlignment: Image.AlignVCenter
      source: QfTheme.getThemeVectorIcon("ic_close_white_24dp")
      sourceSize.width: 24 * Screen.devicePixelRatio
      sourceSize.height: 24 * Screen.devicePixelRatio
      visible: true
    }
  }
}
```


