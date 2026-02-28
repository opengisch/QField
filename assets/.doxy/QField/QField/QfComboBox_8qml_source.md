

# File QfComboBox.qml

[**File List**](files.md) **>** [**imports**](dir_3be62dd4600925273911e91e0c7964f3.md) **>** [**Theme**](dir_1633596792308d5fdfbf00fb99c556ce.md) **>** [**QfComboBox.qml**](QfComboBox_8qml.md)

[Go to the documentation of this file](QfComboBox_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Templates as T
import QtQuick.Controls.impl
import QtQuick.Controls.Material
import QtQuick.Controls.Material.impl
import org.qfield

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


