import QtQuick 2.0

TextEdit {
  signal valueChanged( variant value )

  text: value

  onTextChanged: {
    valueChanged( text )
  }
}
