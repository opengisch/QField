

# File QfPopup.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfPopup.qml**](QfPopup_8qml.md)

[Go to the documentation of this file](QfPopup_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Controls.Material.impl
import org.qfield.core
import org.qfield.gui

Popup {
  id: control
  padding: 5
  modal: true

  background: Rectangle {
    radius: control.Material.roundedScale
    color: QfTheme.mainBackgroundColor

    layer.enabled: control.Material.elevation > 0
    layer.effect: RoundedElevationEffect {
      elevation: control.Material.elevation
      roundedScale: control.Material.roundedScale
    }
  }
}
```


