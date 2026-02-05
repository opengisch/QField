import QtQuick
import QtQuick.Controls
import QtQuick.Templates as T
import QtQuick.Controls.impl
import QtQuick.Controls.Material
import QtQuick.Controls.Material.impl

/**
 * \ingroup qml
 */
ComboBox {
  id: comboBox

  property alias text: contentText

  contentItem: T.TextField {
    id: contentText
    leftPadding: comboBox.background.visible ? Material.textFieldHorizontalPadding : 0
    topPadding: Material.textFieldVerticalPadding
    bottomPadding: Material.textFieldVerticalPadding

    text: comboBox.editable ? comboBox.editText : comboBox.displayText

    enabled: comboBox.editable
    autoScroll: comboBox.editable
    readOnly: comboBox.down
    inputMethodHints: comboBox.inputMethodHints
    validator: comboBox.validator
    selectByMouse: comboBox.selectTextByMouse

    color: comboBox.enabled ? comboBox.Material.foreground : comboBox.Material.hintTextColor
    selectionColor: comboBox.Material.accentColor
    selectedTextColor: comboBox.Material.primaryHighlightedTextColor
    verticalAlignment: Text.AlignVCenter

    cursorDelegate: CursorDelegate {}
  }
}
