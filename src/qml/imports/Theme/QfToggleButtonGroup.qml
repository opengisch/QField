import QtQuick
import QtQuick.Controls.Material.impl
import org.qfield
import org.qgis
import Theme

/**
 * Provides a consistent UI for selecting single values from a list of options.
 */
Item {
  id: toggleButtonGroup

  property var model: null
  property string textRole: "displayString"

  property int selectedIndex: -1
  property bool editing: false
  property bool editable: true

  // Minimum width for buttons to handle empty text gracefully
  readonly property real buttonMinWidth: 48

  signal itemClicked(int index, var modelData)
  signal itemCompleted(int index, var modelData, bool selected)

  height: flow.height + flow.anchors.topMargin + flow.anchors.bottomMargin

  Flow {
    id: flow
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.top: parent.top
    anchors.topMargin: 6
    anchors.bottomMargin: 6
    spacing: 8

    Repeater {
      id: repeater
      model: toggleButtonGroup.model

      delegate: Rectangle {
        id: toggleButton

        property bool selected: toggleButtonGroup.selectedIndex === index
        property string text: toggleButtonGroup.textRole ? (model[toggleButtonGroup.textRole] ?? "") : ""

        width: Math.max(toggleButtonGroup.buttonMinWidth, Math.min(flow.width - 16, innerText.implicitWidth + 16))
        height: 34
        radius: 4
        color: selected ? toggleButtonGroup.editable && toggleButtonGroup.editing ? Theme.mainColor : Theme.controlBorderColor : "transparent"
        border.color: toggleButtonGroup.editing ? selected ? Theme.mainColor : Theme.secondaryTextColor : "transparent"
        border.width: 1

        Component.onCompleted: {
          toggleButtonGroup.itemCompleted(index, model, selected);
        }

        Behavior on color  {
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
          font: Theme.defaultFont
          color: !toggleButtonGroup.editable && toggleButtonGroup.editing ? Theme.mainTextDisabledColor : selected && toggleButtonGroup.editing ? Theme.buttonTextColor : Theme.mainTextColor
        }

        MouseArea {
          id: mouseArea
          anchors.fill: parent
          enabled: toggleButtonGroup.enabled

          onClicked: {
            toggleButtonGroup.itemClicked(index, model);
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
}
