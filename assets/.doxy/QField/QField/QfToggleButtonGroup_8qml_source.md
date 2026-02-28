

# File QfToggleButtonGroup.qml

[**File List**](files.md) **>** [**imports**](dir_3be62dd4600925273911e91e0c7964f3.md) **>** [**Theme**](dir_1633596792308d5fdfbf00fb99c556ce.md) **>** [**QfToggleButtonGroup.qml**](QfToggleButtonGroup_8qml.md)

[Go to the documentation of this file](QfToggleButtonGroup_8qml.md)


```C++
import QtQuick
import QtQuick.Controls.Material
import QtQuick.Controls.Material.impl
import org.qfield
import org.qgis

Item {
  id: toggleButtonGroup

  property alias model: repeater.model
  property string textRole: ""
  property string checkedRole: ""
  property int selectedIndex: -1
  property bool editing: true
  property bool editable: true
  property alias font: fontMetrics.font

  property bool allowDeselect: false

  property bool allowMultipleSelection: false

  property real buttonMininumWidth: 48

  property real buttonSpacing: 8

  signal itemSelected(int index, var modelData)

  signal itemDeselected

  height: flow.height

  Flow {
    id: flow
    width: parent.width
    spacing: toggleButtonGroup.buttonSpacing

    Repeater {
      id: repeater

      delegate: Rectangle {
        id: toggleButton

        property bool selected: toggleButtonGroup.allowMultipleSelection ? model[toggleButtonGroup.checkedRole] : toggleButtonGroup.selectedIndex === index
        property string text: modelData !== undefined ? modelData : toggleButtonGroup.textRole ? (model[toggleButtonGroup.textRole] ?? "") : ""

        visible: text !== ""
        width: visible ? Math.max(toggleButtonGroup.buttonMininumWidth, Math.min(flow.width - 16, innerText.implicitWidth + 16)) : 0
        height: visible ? Material.textFieldHeight : 0
        radius: 4
        color: selected ? toggleButtonGroup.editable && toggleButtonGroup.editing ? Theme.mainColor : Theme.controlBorderColor : "transparent"
        border.color: toggleButtonGroup.editing ? selected ? Theme.mainColor : mouseArea.containsMouse ? Material.primaryTextColor : Material.hintTextColor : "transparent"
        border.width: 1

        Behavior on color {
          ColorAnimation {
            duration: 150
          }
        }

        Text {
          id: innerText
          width: Math.min(flow.width - 32, Math.max(1, implicitWidth))
          text: toggleButton.text
          elide: Text.ElideRight
          anchors.centerIn: parent
          font: fontMetrics.font
          color: !toggleButtonGroup.editable && toggleButtonGroup.editing ? Theme.mainTextDisabledColor : selected && toggleButtonGroup.editing ? Theme.buttonColor : Theme.mainTextColor
        }

        MouseArea {
          id: mouseArea
          anchors.fill: parent
          enabled: toggleButtonGroup.enabled
          hoverEnabled: true

          onClicked: {
            if (toggleButtonGroup.allowMultipleSelection) {
              model[toggleButtonGroup.checkedRole] = !model[toggleButtonGroup.checkedRole];
            } else {
              if (toggleButton.selected && toggleButtonGroup.allowDeselect) {
                toggleButtonGroup.selectedIndex = -1;
                toggleButtonGroup.itemDeselected();
              } else {
                toggleButtonGroup.selectedIndex = index;
                toggleButtonGroup.itemSelected(index, model);
              }
            }
          }

          Ripple {
            clip: true
            width: parent.width
            height: parent.height
            pressed: mouseArea.pressed
            anchor: parent
            color: Theme.darkTheme ? "#22ffffff" : "#22000000"
          }
        }
      }
    }
  }

  FontMetrics {
    id: fontMetrics
    font: Theme.defaultFont
  }
}
```


