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

  /**
   * When true, clicking on an already-selected button will deselect it,
   * setting selectedIndex to -1 and emitting the deselected() signal.
   */
  property bool allowDeselect: false

  // Minimum width for buttons to handle empty text gracefully
  readonly property real buttonMinWidth: 48

  /**
   * Emitted when user selects a button
   * \param index The index of the selected item
   * \param modelData The model data for the selected item
   */
  signal itemSelected(int index, var modelData)

  /**
   * Emitted when an item is completed (delegate created).
   * \param index The index of the completed item
   * \param modelData The model data for the item
   * \param selected Whether the item is currently selected
   */
  signal itemCompleted(int index, var modelData, bool selected)

  /**
   * Emitted when the user deselects the currently selected item (only when allowDeselect is true).
   * The selectedIndex will be -1 after this signal is emitted.
   */
  signal itemDeselected

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

        visible: text !== ""
        width: visible ? Math.max(toggleButtonGroup.buttonMinWidth, Math.min(flow.width - 16, innerText.implicitWidth + 16)) : 0
        height: visible ? 34 : 0
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
            if (toggleButton.selected && toggleButtonGroup.allowDeselect) {
              toggleButtonGroup.selectedIndex = -1;
              toggleButtonGroup.itemDeselected();
            } else {
              toggleButtonGroup.selectedIndex = index;
              toggleButtonGroup.itemSelected(index, model);
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
}
