import QtQuick 2.0
import QtQuick.Controls 1.2

Item {
  signal valueChanged( var value, bool isNull )

  height: childrenRect.height
  anchors { right: parent.right; left: parent.left }

  CheckBox {
    checked: value === config['CheckedState']

    onCheckedChanged: {
      valueChanged( checked ? config['CheckedState'] : config['UncheckedState'], false )
    }
  }
}
