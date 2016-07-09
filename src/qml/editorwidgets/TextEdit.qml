import QtQuick 2.0
import QtQuick.Controls 1.4

Item {
  signal valueChanged(var value)
  height: childrenRect.height

  TextField {
    id: textField
    height: textArea.height == 0 ? undefined : 0
    visible: height !== 0
    anchors.left: parent.left
    anchors.right: parent.right

    text: value || ''

    onTextChanged: {
      valueChanged( text )
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
    onTextChanged: {
      valueChanged( text )
    }
  }
}
