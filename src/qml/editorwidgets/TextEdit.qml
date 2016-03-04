import QtQuick 2.0

TextEdit {
  signal valueChanged( var value )

  anchors.left: parent.left
  anchors.right: parent.right

  text: value
  textFormat: config['UseHtml'] ? TextEdit.RichText : TextEdit.PlainText

  onTextChanged: {
    valueChanged( text )
  }
}
