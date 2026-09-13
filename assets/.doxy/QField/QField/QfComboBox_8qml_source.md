

# File QfComboBox.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfComboBox.qml**](QfComboBox_8qml.md)

[Go to the documentation of this file](QfComboBox_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Templates as T
import QtQuick.Controls.impl
import QtQuick.Controls.Material
import QtQuick.Controls.Material.impl
import org.qfield.core

ComboBox {
  id: comboBox

  property alias text: contentText

  Material.accent: Material.primaryTextColor

  contentItem: T.TextField {
    id: contentText
    leftPadding: comboBox.background.visible ? Material.textFieldHorizontalPadding : 0
    topPadding: Material.textFieldVerticalPadding
    bottomPadding: Material.textFieldVerticalPadding

    text: comboBox.editable ? comboBox.editText : comboBox.displayText

    enabled: comboBox.editable
    autoScroll: comboBox.editable
    readOnly: comboBox.down
    inputMethodHints: comboBox.inputMethodHints
    validator: comboBox.validator
    selectByMouse: comboBox.selectTextByMouse

    color: comboBox.enabled ? comboBox.Material.foreground : comboBox.Material.hintTextColor
    selectionColor: comboBox.Material.accentColor
    selectedTextColor: comboBox.Material.primaryHighlightedTextColor
    verticalAlignment: Text.AlignVCenter

    cursorDelegate: CursorDelegate {}
  }
}
```


