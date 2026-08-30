import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Controls.Material.impl
import org.qfield.core
import org.qfield.gui

/**
 * \ingroup qml_gui
 */
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
