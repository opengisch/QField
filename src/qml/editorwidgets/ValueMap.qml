import QtQuick 2.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0

import org.qfield 1.0
import Theme 1.0

import "."

EditorWidgetBase {
  id: valueMap

  anchors {
    left: parent.left
    right: parent.right
    rightMargin: 10
  }

  property var currentKeyValue: value
  // Workaround to get a signal when the value has changed
  onCurrentKeyValueChanged: {
    comboBox.currentIndex = comboBox.model.keyToIndex(currentKeyValue)
  }

  height: childrenRect.height + 10
  enabled: isEnabled


  ComboBox {
    id: comboBox

    anchors { left: parent.left; right: parent.right }

    currentIndex: model.keyToIndex(value)

    model: ValueMapModel {
      id: listModel

      onMapChanged: {
        comboBox.currentIndex = keyToIndex(valueMap.currentKeyValue)
      }
    }

    Component.onCompleted:
    {
      comboBox.popup.z = 10000 // 1000s are embedded feature forms, use a higher value to insure popups always show above embedded feature formes
      model.valueMap = config['map']
    }

    textRole: 'value'

    onCurrentTextChanged: {
      var key = model.keyForValue(currentText)
      valueChanged(key, false)
    }

    MouseArea {
      anchors.fill: parent
      propagateComposedEvents: true

      onClicked: mouse.accepted = false
      onPressed: { forceActiveFocus(); mouse.accepted = false; }
      onReleased: mouse.accepted = false;
      onDoubleClicked: mouse.accepted = false;
      onPositionChanged: mouse.accepted = false;
      onPressAndHold: mouse.accepted = false;
    }

    background: Item {
      implicitWidth: 120
      implicitHeight: 36

      Rectangle {
        y: textLabel.height - 8
        width: comboBox.width
        height: comboBox.activeFocus ? 2: 1
        color: comboBox.activeFocus ? "#4CAF50" : "#C8E6C9"
      }

      Rectangle {
        visible: enabled
        anchors.fill: parent
        id: backgroundRect
        border.color: comboBox.pressed ? "#4CAF50" : "#C8E6C9"
        border.width: comboBox.visualFocus ? 2 : 1
        color: Theme.lightGray
        radius: 2
      }
    }
  }

  FontMetrics {
    id: fontMetrics
    font: textLabel.font
  }
}
