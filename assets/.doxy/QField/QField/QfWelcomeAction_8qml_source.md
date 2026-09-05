

# File QfWelcomeAction.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfWelcomeAction.qml**](QfWelcomeAction_8qml.md)

[Go to the documentation of this file](QfWelcomeAction_8qml.md)


```C++
import QtQuick
import QtQuick.Layouts
import org.qfield.core
import org.qfield.gui

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
    Layout.minimumWidth: QfTheme.toolButtonSize
    Layout.minimumHeight: QfTheme.toolButtonSize
    Layout.preferredWidth: Math.min(Screen.height / 4, root.width / 1.5)
    Layout.preferredHeight: Layout.preferredWidth
    icon.width: width / 2.2
    icon.height: height / 2.2
    bgcolor: QfTheme.controlBackgroundAlternateColor
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
    color: QfTheme.mainTextColor
    font: QfTheme.tipFont
  }
}
```


