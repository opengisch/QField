import QtQuick

Item {
  id: controller

  property var model
  property real selectedIndex: 0
  property real contentHeight: flow.height
  property string currentSelectedKey: ""
  property string currentSelectedValue: ""

  signal currentValueChanged

  Flow {
    id: flow
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.top: parent.top
    spacing: 8

    Repeater {
      id: repeater
      model: controller.model

      Rectangle {
        id: item
        width: txt.width + 16
        height: 35
        radius: 4

        border.width: 1
        border.color: selected ? Theme.mainColor : Theme.accentColor
        color: selected ? Theme.mainColor : "transparent"

        property bool selected: selectedIndex == index
        property string key: Object.keys(modelData)[0]
        property string value: modelData[key]

        Component.onCompleted: {
          if(selected){
            currentSelectedKey = key
            currentSelectedValue = value
          }
        }

        Behavior on color {
          ColorAnimation {
            duration: 200
          }
        }

        Text {
          id: txt
          text: key
          anchors.centerIn: parent
          font: Theme.defaultFont
          color: Theme.mainTextColor

        }

        MouseArea {
          anchors.fill: parent
          onClicked: {
            selectedIndex = index
            currentSelectedKey = key
            currentSelectedValue = value
            currentValueChanged()
          }

          QfRipple {
            mouseArea: parent
            anchors.fill: parent
          }
        }
      }
    }
  }
}
