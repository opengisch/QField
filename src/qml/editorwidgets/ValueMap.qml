import QtQuick 2.0
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0
import org.qfield 1.0

Item {
  signal valueChanged(var value, bool isNull)

  anchors {
    left: parent.left
    right: parent.right
    rightMargin: 10 * dp
  }

  height: childrenRect.height + 10 * dp


  ComboBox {
    id: comboBox

    property var currentValue: value

    anchors { left: parent.left; right: parent.right }

    currentIndex: model.keyToIndex(value)

    model: ValueMapModel {
      id: listModel

      onMapChanged: {
        comboBox.currentIndex = keyToIndex(comboBox.currentValue)
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

    // Workaround to get a signal when the value has changed
    onCurrentValueChanged: {
      currentIndex = model.keyToIndex(currentValue)
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
      font.pointSize: 14
      highlighted: comboBox.highlightedIndex == index
    }

    contentItem: Text {
      id: textLabel
      height: fontMetrics.height + 20 * dp
      text: comboBox.displayText
      font.pointSize: 14
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
        color: "#dddddd"
        radius: 2
      }
    }
    // [/hidpi fixes]
  }
}
