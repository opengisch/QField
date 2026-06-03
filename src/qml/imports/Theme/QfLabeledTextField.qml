import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Theme

/**
 * \ingroup qml
 */
Item {
  id: root

  property string label: ""
  property string suffixText: ""
  property alias text: textField.text
  property alias validator: textField.validator
  property alias inputMethodHints: textField.inputMethodHints
  property alias horizontalAlignment: textField.horizontalAlignment
  property alias placeholderText: textField.placeholderText

  implicitHeight: layout.implicitHeight
  implicitWidth: layout.implicitWidth

  RowLayout {
    id: layout
    anchors.fill: parent
    spacing: 8

    Label {
      visible: root.label !== ""
      text: root.label
      font: Theme.defaultFont
      color: Theme.mainTextColor
      Layout.fillWidth: true
    }

    QfTextField {
      id: textField
      font: Theme.defaultFont
      horizontalAlignment: TextInput.AlignRight
      inputMethodHints: Qt.ImhFormattedNumbersOnly
      rightPadding: suffixLabel.visible ? suffixLabel.implicitWidth + 16 : 10

      Label {
        id: suffixLabel
        visible: root.suffixText !== ""
        text: root.suffixText
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.verticalCenter: parent.verticalCenter
        font: parent.font
        color: Theme.mainTextDisabledColor
      }
    }
  }
}
