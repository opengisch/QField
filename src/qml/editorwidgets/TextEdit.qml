import QtQuick 2.0
import QtQuick.Controls 1.4

Item {
  signal valueChanged( var value )

  TextField {
    visible: config['IsMultiLine'] !== true
    anchors.left: parent.left
    anchors.right: parent.right

    text: value || ''

    onTextChanged: {
      valueChanged( text )
    }
  }

  TextArea {
    visible: config['IsMultiLine'] === true
    anchors.left: parent.left
    anchors.right: parent.right

    text: value || ''
    textFormat: config['UseHtml'] ? TextEdit.RichText : TextEdit.PlainText
    onTextChanged: {
      valueChanged( text )
    }
  }
}
