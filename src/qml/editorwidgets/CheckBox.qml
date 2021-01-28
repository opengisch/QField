import QtQuick 2.12
import QtQuick.Controls 2.12

import "."

EditorWidgetBase {
  height: childrenRect.height
  enabled: isEnabled

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

    indicator.height: 16
    indicator.width: 16
    indicator.implicitHeight: 24
    indicator.implicitWidth: 24
  }
}
