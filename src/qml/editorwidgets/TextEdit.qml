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
    font.pointSize: 14

    text: value || ''

    inputMethodHints: field.isNumeric || widget == 'Range' ? Qt.ImhFormattedNumbersOnly : Qt.ImhNone

    onTextChanged: {
      valueChanged( text, text == '' )
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
