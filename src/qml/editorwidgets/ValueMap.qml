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
      console.warn(value + " " + model.keyToIndex(value))
      currentIndex = model.keyToIndex(value)
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
      height: 36 * dp
      text: model.value
      font.weight: comboBox.currentIndex === index ? Font.DemiBold : Font.Normal
      font.pointSize: 12
      highlighted: comboBox.highlightedIndex == index
    }

    contentItem: Text {
      height: 36 * dp
      text: comboBox.displayText
      horizontalAlignment: Text.AlignLeft
      verticalAlignment: Text.AlignVCenter
      elide: Text.ElideRight
    }

    background: Item {
      implicitWidth: 120 * dp
      implicitHeight: 36 * dp

      Rectangle {
        anchors.fill: parent
        id: backgroundRect
        border.color: comboBox.pressed ? "#17a81a" : "#21be2b"
        border.width: comboBox.visualFocus ? 2 : 1
        color: "#dddddd"
        radius: 2
      }
    }
    // [/hidpi fixes]
  }
}
