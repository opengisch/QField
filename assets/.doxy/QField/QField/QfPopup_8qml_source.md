

# File QfPopup.qml

[**File List**](files.md) **>** [**imports**](dir_3be62dd4600925273911e91e0c7964f3.md) **>** [**Theme**](dir_1633596792308d5fdfbf00fb99c556ce.md) **>** [**QfPopup.qml**](QfPopup_8qml.md)

[Go to the documentation of this file](QfPopup_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Controls.Material.impl
import org.qfield

Popup {
  id: control
  padding: 5
  modal: true

  background: Rectangle {
    radius: control.Material.roundedScale
    color: Theme.mainBackgroundColor

    layer.enabled: control.Material.elevation > 0
    layer.effect: RoundedElevationEffect {
      elevation: control.Material.elevation
      roundedScale: control.Material.roundedScale
    }
  }
}
```


