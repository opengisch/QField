

# File QfDialog.qml

[**File List**](files.md) **>** [**imports**](dir_3be62dd4600925273911e91e0c7964f3.md) **>** [**Theme**](dir_1633596792308d5fdfbf00fb99c556ce.md) **>** [**QfDialog.qml**](QfDialog_8qml.md)

[Go to the documentation of this file](QfDialog_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import org.qfield

Dialog {
  visible: false
  modal: true
  font: Theme.defaultFont
  standardButtons: Dialog.Ok | Dialog.Cancel

  x: (mainWindow.width - width) / 2
  y: (mainWindow.height - height) / 2

  onClosed: {
    focusstack.forceActiveFocusOnLastTaker();
  }
}
```


