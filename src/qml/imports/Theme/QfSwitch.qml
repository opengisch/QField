import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Controls.Material.impl
import org.qfield

/**
 * \ingroup qml
 */
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
