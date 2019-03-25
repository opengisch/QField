import QtQuick 2.0
import QtQuick.Controls 2.4

Item {
  signal valueChanged( var value, bool isNull )

  height: childrenRect.height
  anchors {
    right: parent.right
    left: parent.left
  }

  CheckBox {
    property var currentValue: value

    checked: value == config['CheckedState']

    onCheckedChanged: {
      valueChanged( checked ? config['CheckedState'] : config['UncheckedState'], false )
      forceActiveFocus()
    }

    // Workaround to get a signal when the value has changed
    onCurrentValueChanged: {
      checked = currentValue == config['CheckedState']
    }

    indicator.height: 16 * dp
    indicator.width: 16 * dp
    icon.height: 16 * dp
    icon.width: 16 * dp
  }
}
