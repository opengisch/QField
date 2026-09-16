

# File QfDropShadow.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfDropShadow.qml**](QfDropShadow_8qml.md)

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


