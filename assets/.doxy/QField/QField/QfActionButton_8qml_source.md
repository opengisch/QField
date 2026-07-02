

# File QfActionButton.qml

[**File List**](files.md) **>** [**imports**](dir_3be62dd4600925273911e91e0c7964f3.md) **>** [**Theme**](dir_1633596792308d5fdfbf00fb99c556ce.md) **>** [**QfActionButton.qml**](QfActionButton_8qml.md)

[Go to the documentation of this file](QfActionButton_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Controls.Material.impl
import org.qfield

ToolButton {
  id: button

  property string toolImage: ''
  property string toolText: qsTr("close")
  property alias innerActionIcon: innerAction

  height: 48
  width: height + (buttonText.visible ? buttonText.width + 24 : 0) + (innerAction.visible ? 32 : 0)
  clip: true

  Behavior on width {
    NumberAnimation {
      duration: 200
    }
  }

  background: Rectangle {
    width: parent.width
    height: 48
    color: Theme.toolButtonBackgroundSemiOpaqueColor
    radius: height / 2

    QfToolButton {
      anchors.left: parent.left
      anchors.top: parent.top
      width: 48
      height: 48
      enabled: false
      round: true
      iconSource: button.toolImage
      iconColor: Theme.toolButtonColor
      bgcolor: Theme.toolButtonBackgroundColor
    }

    Ripple {
      id: ripple
      clip: true
      width: parent.width
      height: 48
      clipRadius: 4
      pressed: button.down
      anchor: parent
      active: button.down
      color: "#22aaaaaa"
    }
  }

  contentItem: Row {
    anchors.left: parent.left
    anchors.leftMargin: 48 + 8
    spacing: 8
    padding: 0
    visible: button.toolText !== ""

    Text {
      id: buttonText
      anchors.verticalCenter: parent.verticalCenter
      verticalAlignment: Text.AlignVCenter
      text: button.toolText
      color: Theme.toolButtonColor
      font: Theme.strongFont
    }

    Image {
      id: innerAction
      anchors.verticalCenter: parent.verticalCenter
      width: 24
      height: 24
      fillMode: Image.PreserveAspectFit
      horizontalAlignment: Image.AlignHCenter
      verticalAlignment: Image.AlignVCenter
      source: Theme.getThemeVectorIcon("ic_close_white_24dp")
      sourceSize.width: 24 * Screen.devicePixelRatio
      sourceSize.height: 24 * Screen.devicePixelRatio
      visible: true
    }
  }
}
```


