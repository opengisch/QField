import QtQuick 2.0
import QtQuick.Controls 1.4

Item {
  signal valueChanged( var value, bool isNull )

  height: childrenRect.height
  anchors {
    right: parent.right
    left: parent.left
  }

  CheckBox {
    property var currentValue: value

    checked: value == config.value('CheckedState')

    onCheckedChanged: {
      valueChanged( checked ? config.value('CheckedState') : config.value('UncheckedState'), false )
      forceActiveFocus()
    }

    // Workaround to get a signal when the value has changed
    onCurrentValueChanged: {
      checked = currentValue == config.value('CheckedState')
    }
  }
}
