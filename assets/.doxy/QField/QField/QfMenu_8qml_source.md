

# File QfMenu.qml

[**File List**](files.md) **>** [**imports**](dir_3be62dd4600925273911e91e0c7964f3.md) **>** [**Theme**](dir_1633596792308d5fdfbf00fb99c556ce.md) **>** [**QfMenu.qml**](QfMenu_8qml.md)

[Go to the documentation of this file](QfMenu_8qml.md)


```C++
import QtQuick
import QtQuick.Controls

Menu {
  id: control

  property int paddingMultiplier: 1
  property bool skipFirstRow: false
  property real minimumRowWidth: 50

  width: {
    let result = minimumRowWidth;
    let padding = 0;
    const initialRow = skipFirstRow ? 1 : 0;
    for (let i = initialRow; i < control.count; ++i) {
      const item = control.itemAt(i);
      if (item) {
        if (item.contentItem) {
          result = Math.max(item.contentItem.implicitWidth, result);
        }
        padding = Math.max(item.leftPadding + item.rightPadding, padding);
      }
    }
    return mainWindow.width > 0 ? Math.min(result + padding * paddingMultiplier, mainWindow.width - 20) : result + padding;
  }
}
```


