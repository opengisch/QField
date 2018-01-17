import QtQuick 2.0
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0

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

    property var reverseConfig: ({})
    property var currentValue: value
    property var currentMap
    property var currentKey


    anchors { left: parent.left; right: parent.right }

    currentIndex: find(reverseConfig[value])

    ListModel {
        id: listModel
    }

    Component.onCompleted: {
      if( config['map'] )
      {
        if( config['map'].length )
        {
          //it's a list (>=QGIS3.0)
          for(var i=0; i<config['map'].length; i++)
          {
            currentMap = config['map'][i]
            currentKey = Object.keys(currentMap)[0]
            listModel.append( { text: currentKey } )
            reverseConfig[currentMap[currentKey]] = currentKey;
          }
          model=listModel
          textRole = 'text'
        }
        else
        {
          //it's a map (<=QGIS2.18)
          model = Object.keys(config['map']);
          for(var key in config['map']) {
            reverseConfig[config['map'][key]] = key;
          }
        }
      }

      currentIndex = find(reverseConfig[value])
    }

    onCurrentTextChanged: {
      currentMap= config['map'].length ? config['map'][currentIndex] : config['map']
      valueChanged(currentMap[currentText], false)
    }

    // Workaround to get a signal when the value has changed
    onCurrentValueChanged: {
      currentIndex = find(reverseConfig[value])
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
      text: config['map'].length ? model.text : modelData
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
