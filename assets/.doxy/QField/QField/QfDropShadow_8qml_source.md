

# File QfDropShadow.qml

[**File List**](files.md) **>** [**imports**](dir_3be62dd4600925273911e91e0c7964f3.md) **>** [**Theme**](dir_1633596792308d5fdfbf00fb99c556ce.md) **>** [**QfDropShadow.qml**](QfDropShadow_8qml.md)

[Go to the documentation of this file](QfDropShadow_8qml.md)


```C++
import QtQuick
import QtQuick.Effects

MultiEffect {
  id: effect

  // dummy properties to support Qt5 and Qt6
  property int samples
  property int radius
  property bool transparentBorder

  // property aliases
  property alias color: effect.shadowColor
  property alias horizontalOffset: effect.shadowHorizontalOffset
  property alias verticalOffset: effect.shadowVerticalOffset

  shadowEnabled: true
}
```


