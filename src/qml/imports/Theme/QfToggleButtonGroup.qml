import QtQuick
import QtQuick.Controls.Material
import QtQuick.Controls.Material.impl
import org.qfield
import org.qgis
import Theme

/**
 * Provides a consistent UI for selecting single values from a list of options.
 * \ingroup qml
 */
Item {
  id: toggleButtonGroup

  property alias model: repeater.model
  property string textRole: ""
  property string checkedRole: ""
  property int selectedIndex: -1
  property bool editing: true
  property bool editable: true

  /**
   * When true, clicking on an already-selected button will deselect it,
   * setting selectedIndex to -1 and emitting the deselected() signal.
   */
  property bool allowDeselect: false

  /**
   * When true, multiple items can be selected, the checkedRole mdoel
   * value will define whether a given index is selected or not.
   */
  property bool allowMultipleSelection: false

  /**
   * Minimum width for buttons to handle empty text gracefully
   */
  property real buttonMininumWidth: 48

  /**
   * Spacing between buttons
   */
  property real buttonSpacing: 8

  /**
   * Emitted when user selects a button
   * \param index The index of the selected item
   * \param modelData The model data for the selected item
   */
  signal itemSelected(int index, var modelData)

  /**
   * Emitted when the user deselects the currently selected item (only when allowDeselect is true).
   * The selectedIndex will be -1 after this signal is emitted.
   */
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
          font: Theme.defaultFont
          color: !toggleButtonGroup.editable && toggleButtonGroup.editing ? Theme.mainTextDisabledColor : selected && toggleButtonGroup.editing ? Theme.buttonTextColor : Theme.mainTextColor
        }

        FontMetrics {
          id: fontMetrics
          font: Theme.defaultFont
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
}
