import QtQuick 2.0
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0
import org.qfield 1.0
import Theme 1.0

Item {
  id: valueMap
  signal valueChanged(var value, bool isNull)

  anchors {
    left: parent.left
    right: parent.right
    rightMargin: 10 * dp
  }

  property var currentKeyValue: value
  // Workaround to get a signal when the value has changed
  onCurrentKeyValueChanged: {
    comboBox.currentIndex = comboBox.model.keyToIndex(currentKeyValue)
  }

  height: childrenRect.height + 10 * dp


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

    // [hidpi fixes]
    delegate: ItemDelegate {
      width: comboBox.width
      height: fontMetrics.height + 20 * dp
      text: model.value
      font.weight: comboBox.currentIndex === index ? Font.DemiBold : Font.Normal
      font.pointSize: Theme.defaultFont.pointSize
      highlighted: comboBox.highlightedIndex == index
    }

    contentItem: Text {
      id: textLabel
      height: fontMetrics.height + 20 * dp
      text: comboBox.displayText
      font: Theme.defaultFont
      horizontalAlignment: Text.AlignLeft
      verticalAlignment: Text.AlignVCenter
      elide: Text.ElideRight
      color: value === undefined || !enabled ? 'gray' : 'black'
    }

    background: Item {
      implicitWidth: 120 * dp
      implicitHeight: 36 * dp

      Rectangle {
        y: textLabel.height - 8 * dp
        width: comboBox.width
        height: comboBox.activeFocus ? 2 * dp : 1 * dp
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
    // [/hidpi fixes]
  }
}
