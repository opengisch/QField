

# File QfOverlayContainer.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfOverlayContainer.qml**](QfOverlayContainer_8qml.md)

[Go to the documentation of this file](QfOverlayContainer_8qml.md)


```C++
import QtQuick
import QtQuick.Layouts
import org.qfield.core
import org.qfield.gui

Rectangle {
  id: container

  default property alias contents: containerLayout.children
  property alias layout: containerLayout
  property alias title: title.text
  property alias header: headerLayout.children

  width: parent.width
  height: childrenRect.height
  color: QfTheme.mainBackgroundColorSemiOpaque
  radius: 8
  clip: true

  Column {
    id: containerLayout
    width: parent.width - 10
    anchors.horizontalCenter: parent.horizontalCenter
    topPadding: 5
    bottomPadding: 5
    spacing: 4

    RowLayout {
      width: parent.width

      Text {
        id: title
        Layout.fillWidth: true
        Layout.leftMargin: 6
        Layout.rightMargin: 6
        text: qsTr("Positioning")
        font: QfTheme.strongTipFont
        color: QfTheme.mainTextColor
      }

      RowLayout {
        id: headerLayout
        Layout.rightMargin: 6
      }
    }
  }
}
```


