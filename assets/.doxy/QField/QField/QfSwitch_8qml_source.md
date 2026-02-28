

# File QfSwitch.qml

[**File List**](files.md) **>** [**imports**](dir_3be62dd4600925273911e91e0c7964f3.md) **>** [**Theme**](dir_1633596792308d5fdfbf00fb99c556ce.md) **>** [**QfSwitch.qml**](QfSwitch_8qml.md)

[Go to the documentation of this file](QfSwitch_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Controls.Material.impl
import org.qfield

SwitchDelegate {
  id: control

  indicator: SwitchIndicator {
    control: control
    x: control.text ? (control.mirrored ? control.leftPadding : control.width - width - control.rightPadding) : control.leftPadding + (control.availableWidth - width) / 2
    y: control.topPadding + (control.availableHeight - height) / 2

    color: control.enabled ? (control.checked ? Theme.mainColor : Theme.controlBorderColor) : (control.checked ? Theme.mainTextDisabledColor : Theme.controlBorderColor)
    border.color: control.enabled ? (control.checked ? Theme.mainColor : control.Material.switchUncheckedHandleColor) : (control.checked ? control.Material.switchDisabledCheckedTrackColor : control.Material.switchDisabledUncheckedTrackBorderColor)
  }
}
```


