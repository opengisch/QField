

# File QfComboBox.qml

[**File List**](files.md) **>** [**imports**](dir_3be62dd4600925273911e91e0c7964f3.md) **>** [**Theme**](dir_1633596792308d5fdfbf00fb99c556ce.md) **>** [**QfComboBox.qml**](QfComboBox_8qml.md)

[Go to the documentation of this file](QfComboBox_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Controls.impl
import QtQuick.Controls.Material
import QtQuick.Controls.Material.impl

ComboBox {
  id: comboBox

  property alias text: contentText

  contentItem: Text {
    id: contentText
    leftPadding: enabled ? Material.textFieldHorizontalPadding : 0

    text: comboBox.displayText
    font: comboBox.font
    color: Theme.mainTextColor
    verticalAlignment: Text.AlignVCenter
    horizontalAlignment: Text.AlignLeft
    elide: Text.ElideRight
  }
}
```


