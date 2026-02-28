

# File QfToolButton.qml

[**File List**](files.md) **>** [**imports**](dir_3be62dd4600925273911e91e0c7964f3.md) **>** [**Theme**](dir_1633596792308d5fdfbf00fb99c556ce.md) **>** [**QfToolButton.qml**](QfToolButton_8qml.md)

[Go to the documentation of this file](QfToolButton_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Controls.Material.impl
import org.qfield

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

  width: 48
  height: 48
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
    border.width: 2

    badgeText.color: bottomRightIndicatorFgColor
    badgeText.text: bottomRightIndicatorText

    bottomMargin: 7
    rightMargin: 5
  }
}
```


