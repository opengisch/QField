import QtQuick 2.12
import QtQuick.Controls 2.12

Item {
  signal valueChanged( var value, bool isNull )

  height: childrenRect.height

  anchors {
    right: parent.right
    left: parent.left
  }

  CheckBox {
    id: checkBox

    property var currentValue: value
    //if the field type is boolean, ignore the configured 'CheckedState' and 'UncheckedState' values and work with true/false always
    readonly property bool isBool: field.type == 1 //needs type coercion

    checked: if( isBool ) {
                 if( currentValue !== undefined ) {
                     currentValue
                 } else {
                     false
                 }
             } else {
                 String(currentValue) === config['CheckedState']
             }

    onCheckedChanged: {
      valueChanged( isBool ? checked : checked ? config['CheckedState'] : config['UncheckedState'], false )
      forceActiveFocus()
    }

    indicator.height: 16 * dp
    indicator.width: 16 * dp
    indicator.implicitHeight: 24 * dp
    indicator.implicitWidth: 24 * dp
  }
}
