import QtQuick 2.0

TextEdit {
  signal valueChanged( variant value )

  anchors.left: parent.left
  anchors.right: parent.right

  text: value

  onTextChanged: {
    valueChanged( text )
  }
}
