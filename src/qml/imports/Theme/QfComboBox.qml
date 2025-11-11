import QtQuick
import QtQuick.Controls
import QtQuick.Controls.impl
import QtQuick.Controls.Material
import QtQuick.Controls.Material.impl

/**
 * \ingroup qml
 */
ComboBox {
  id: comboBox

  property alias text: contentText

  contentItem: Text {
    id: contentText
    leftPadding: enabled ? Material.textFieldHorizontalPadding : 0

    text: comboBox.displayText
    font: comboBox.font
    color: Theme.mainTextColor
    verticalAlignment: Text.AlignVCenter
    horizontalAlignment: Text.AlignLeft
    elide: Text.ElideRight
  }

  background.visible: enabled
  indicator.visible: enabled
}
