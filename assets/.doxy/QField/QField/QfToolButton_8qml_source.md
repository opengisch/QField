

# File QfToolButton.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfToolButton.qml**](QfToolButton_8qml.md)

[Go to the documentation of this file](QfToolButton_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Controls.Material.impl
import org.qfield.core
import org.qfield.gui

RoundButton {
  id: button

  property alias iconSource: button.icon.source
  property alias iconColor: button.icon.color
  property alias statusBadge: badge
  property string bottomRightIndicatorText: ''
  property string bottomRightIndicatorBgColor: Theme.cloudColor
  property string bottomRightIndicatorFgColor: Theme.light
  property bool round: false
  property bool roundborder: false
  property alias bgcolor: backgroundRectangle.color
  property color borderColor: 'transparent'

  width: Theme.toolButtonSize
  height: Theme.toolButtonSize
  implicitWidth: width
  implicitHeight: height
  focusPolicy: Qt.NoFocus

  topInset: 0
  bottomInset: 0
  leftInset: 0
  rightInset: 0
  padding: 10

  background: Rectangle {
    id: backgroundRectangle
    implicitWidth: 100
    implicitHeight: 25
    border.width: round && roundborder ? height / 6 : 1
    border.color: button.borderColor
    color: 'transparent'
    radius: round ? height / 2 : roundborder ? 12 : 0
    clip: true

    Behavior on color {
      ColorAnimation {
        duration: 200
      }
    }

    Ripple {
      id: ripple
      clip: true
      width: parent.width
      height: parent.height
      clipRadius: 4
      pressed: button.down
      anchor: parent
      active: button.down
      color: Theme.darkTheme ? bgcolor == "#ffffff" ? "#10000000" : "#10ffffff" : bgcolor == "#ffffff" || bgcolor == "#00000000" ? "#10000000" : "#22ffffff"
    }
  }

  icon.source: ""
  icon.color: "transparent" // setting the color to transparent tells Qt to draw the icon using the original source color(s)
  icon.width: Math.min(width, height) / 2
  icon.height: Math.min(width, height) / 2

  Material.foreground: icon.color
  font: Theme.tipFont

  QfBadge {
    id: badge
    alignment: QfBadge.Alignment.BottomRight

    z: 2
    width: button.width / 2.5
    height: width
    visible: bottomRightIndicatorText
    color: bottomRightIndicatorBgColor
    border.color: Theme.mainBackgroundColor
    border.width: Math.max(1, button.width / 24)

    badgeText.color: bottomRightIndicatorFgColor
    badgeText.text: bottomRightIndicatorText

    bottomMargin: button.width * 0.15
    rightMargin: button.width * 0.1
  }
}
```


