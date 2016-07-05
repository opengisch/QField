import QtQuick 2.0
import QtQuick.Controls 1.4

Item {
  signal valueChanged(var value)
  height: childrenRect.height

  TextField {
    height: config['IsMultiline'] !== true ? undefined : 0
    anchors.left: parent.left
    anchors.right: parent.right

    text: value || ''

    onTextChanged: {
      valueChanged( text )
    }
  }

  TextArea {
    height: config['IsMultiline'] === true ? undefined : 0
    anchors.left: parent.left
    anchors.right: parent.right

    text: value || ''
    textFormat: config['UseHtml'] ? TextEdit.RichText : TextEdit.PlainText
    onTextChanged: {
      valueChanged( text )
    }
  }
}
