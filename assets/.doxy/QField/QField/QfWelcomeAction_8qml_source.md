

# File QfWelcomeAction.qml

[**File List**](files.md) **>** [**imports**](dir_3be62dd4600925273911e91e0c7964f3.md) **>** [**Theme**](dir_1633596792308d5fdfbf00fb99c556ce.md) **>** [**QfWelcomeAction.qml**](QfWelcomeAction_8qml.md)

[Go to the documentation of this file](QfWelcomeAction_8qml.md)


```C++
import QtQuick
import QtQuick.Layouts
import org.qfield
import Theme

ColumnLayout {
  id: root

  property url iconSource
  property color iconColor
  property string label
  signal clicked

  spacing: 4

  QfToolButton {
    id: actionButton
    Layout.alignment: Qt.AlignHCenter
    Layout.minimumWidth: Theme.toolButtonSize
    Layout.minimumHeight: Theme.toolButtonSize
    Layout.preferredWidth: Math.min(Screen.height / 4, root.width / 1.5)
    Layout.preferredHeight: Layout.preferredWidth
    icon.width: width / 2.2
    icon.height: height / 2.2
    bgcolor: Theme.controlBackgroundAlternateColor
    round: false
    roundborder: true
    iconSource: root.iconSource
    iconColor: root.iconColor
    smooth: true
    onClicked: root.clicked()
  }

  Text {
    Layout.fillWidth: true
    visible: root.width / 1.5 > actionButton.Layout.minimumWidth
    text: root.label
    horizontalAlignment: Text.AlignHCenter
    wrapMode: Text.WordWrap
    color: Theme.mainTextColor
    font: Theme.tipFont
  }
}
```


