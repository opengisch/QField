

# File Binary.qml

[**File List**](files.md) **>** [**editorwidgets**](dir_7621275d8427768f32eeef555af1c8d1.md) **>** [**Binary.qml**](Binary_8qml.md)

[Go to the documentation of this file](Binary_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import org.qfield

EditorWidgetBase {
  id: binaryItem

  height: childrenRect.height

  Label {
    id: binaryValue
    topPadding: 10
    bottomPadding: 10
    anchors.left: parent.left
    anchors.right: parent.right
    font: Theme.defaultFont
    color: Theme.mainTextColor
    opacity: 0.45
    wrapMode: Text.Wrap

    text: qsTr('(Blob)')
  }
}
```


