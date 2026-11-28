

# File QfEditorWidgetBinary.qml

[**File List**](files.md) **>** [**editorwidgets**](dir_aa4aab3cdee284f0e217d9df55b13787.md) **>** [**QfEditorWidgetBinary.qml**](QfEditorWidgetBinary_8qml.md)

[Go to the documentation of this file](QfEditorWidgetBinary_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import org.qfield.core
import org.qfield.gui

QfEditorWidgetBase {
  id: binaryItem

  height: childrenRect.height

  Label {
    id: binaryValue
    topPadding: 10
    bottomPadding: 10
    anchors.left: parent.left
    anchors.right: parent.right
    font: QfTheme.defaultFont
    color: QfTheme.mainTextColor
    opacity: 0.45
    wrapMode: Text.Wrap

    text: qsTr('(Blob)')
  }
}
```


