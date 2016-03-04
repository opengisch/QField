import QtQuick 2.0
import QtQuick.Controls 1.4

ComboBox {
  signal valueChanged( var value )

  anchors.left: parent.left
  anchors.right: parent.right

  currentIndex: find(value)

  onCurrentTextChanged: {
    valueChanged( currentText )
  }

  Component.onCompleted: {
    model = Object.keys(config);
  }
}
