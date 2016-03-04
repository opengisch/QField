import QtQuick 2.0
import QtQuick.Controls 1.4

Item {
  signal valueChanged( var value )

  anchors.left: parent.left
  anchors.right: parent.right

  ComboBox {
    id: comboBox

    currentIndex: find(value)

    Component.onCompleted: {
      model = Object.keys(config);
      currentIndex = find(value)
    }

    onCurrentTextChanged: {
      valueChanged( currentText )
    }
  }
}
