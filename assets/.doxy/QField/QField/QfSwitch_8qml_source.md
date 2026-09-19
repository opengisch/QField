

# File QfSwitch.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfSwitch.qml**](QfSwitch_8qml.md)

[Go to the documentation of this file](QfSwitch_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Controls.Material.impl
import org.qfield.core
import org.qfield.gui

SwitchDelegate {
  id: control

  indicator: SwitchIndicator {
    control: control
    x: control.text ? (control.mirrored ? control.leftPadding : control.width - width - control.rightPadding) : control.leftPadding + (control.availableWidth - width) / 2
    y: control.topPadding + (control.availableHeight - height) / 2

    color: control.enabled ? (control.checked ? QfTheme.mainColor : QfTheme.controlBorderColor) : (control.checked ? QfTheme.mainTextDisabledColor : QfTheme.controlBorderColor)
    border.color: control.enabled ? (control.checked ? QfTheme.mainColor : control.Material.switchUncheckedHandleColor) : (control.checked ? control.Material.switchDisabledCheckedTrackColor : control.Material.switchDisabledUncheckedTrackBorderColor)
  }
}
```


