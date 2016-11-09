import QtQuick 2.0
import QtQuick.Controls 2.0

Item {
  signal valueChanged(var value, bool isNull)
  height: childrenRect.height

  TextField {
    id: textField
    height: textArea.height == 0 ? undefined : 0
    visible: height !== 0
    anchors.left: parent.left
    anchors.right: parent.right

    text: value || ''

    inputMethodHints: field.isNumeric || widget == 'Range' ? Qt.ImhDigitsOnly : Qt.ImhNone

    onTextChanged: {
      valueChanged( text, text == '' )
    }

    background: Rectangle {
      color: control.enabled ? "transparent" : "#353637"
      border.color: control.enabled ? constraintValid ? "#21be2b" : "#c0392b" : "transparent"
    }
  }

  TextArea {
    id: textArea
    height: config['IsMultiline'] === true ? undefined : 0
    visible: height !== 0
    anchors.left: parent.left
    anchors.right: parent.right

    text: value || ''
    textFormat: config['UseHtml'] ? TextEdit.RichText : TextEdit.PlainText

    onEditingFinished: {
      valueChanged( text, text == '' )
    }
  }
}
