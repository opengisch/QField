import QtQuick 2.0
import QtQuick.Controls 1.2

CheckBox {
  signal valueChanged( var value )

  checked: value === config['checkedState']

  onCheckedChanged: {
    valueChanged( checked ? config['CheckedState'] : config['UncheckedState'] )
  }
}

