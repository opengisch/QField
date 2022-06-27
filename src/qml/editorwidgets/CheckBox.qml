import QtQuick 2.14
import QtQuick.Controls 2.14

import Theme 1.0

import "."

EditorWidgetBase {
  height: childrenRect.height

  property string checkedLabel: config['TextDisplayMethod'] === 1 && config['CheckedState'] != null && config['CheckedState'] !== '' ? config['CheckedState'] : qsTr('True')
  property string uncheckedLabel: config['TextDisplayMethod'] === 1 && config['UncheckedState'] != null && config['UncheckedState'] !== '' ? config['UncheckedState'] : qsTr('False')

  anchors {
    right: parent.right
    left: parent.left
  }

  Label {
      id: checkValue
      height: fontMetrics.height + 20
      anchors {
          left: parent.left
          right: checkBox.left
      }

      topPadding: 10
      bottomPadding: 10
      font: Theme.defaultFont
      color: isEnabled ? 'black' : 'gray'

      text: checkBox.checked ? checkedLabel : uncheckedLabel

      MouseArea {
          id: checkArea
          enabled: isEnabled
          anchors.fill: parent

          onClicked: {
              checkBox.checked = !checkBox.checked
              checkBox.forceActiveFocus();
          }
      }
  }

  QfSwitch {
    id: checkBox
    enabled: isEnabled
    visible: isEnabled
    width: implicitContentWidth
    small: true

    anchors {
      right: parent.right
      verticalCenter: checkValue.verticalCenter
    }

    //if the field type is boolean, ignore the configured 'CheckedState' and 'UncheckedState' values and work with true/false always
    readonly property bool isBool: field.type == 1 //needs type coercion

    checked: {
        if( isBool ) {
            return value !== undefined ? value : false;
        } else {
            return String(value) === config['CheckedState']
        }
    }

    onCheckedChanged: {
        valueChangeRequested( isBool ? checked : checked ? config['CheckedState'] : config['UncheckedState'], false )
        forceActiveFocus()
    }
  }

  Rectangle {
      id: backgroundRect
      anchors.left: parent.left
      anchors.right: parent.right
      y: checkValue.height - height - checkValue.bottomPadding / 2
      implicitWidth: 120
      height: checkBox.activeFocus || checkBox.pressed || checkArea.containsPress ? 2: 1
      color: checkBox.activeFocus || checkBox.pressed || checkArea.containsPress ? Theme.accentColor : Theme.accentLightColor
  }

  FontMetrics {
    id: fontMetrics
    font: checkValue.font
  }
}
