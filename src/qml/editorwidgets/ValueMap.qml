import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material
import QtQuick.Controls.Material.impl
import org.qfield
import org.qgis
import Theme
import ".."

EditorWidgetBase {
  id: valueMap

  anchors {
    left: parent.left
    right: parent.right
  }

  property var currentKeyValue: value

  // Workaround to get a signal when the value has changed
  onCurrentKeyValueChanged: {
    comboBox.currentIndex = comboBox.model.keyToIndex(currentKeyValue);
  }

  height: childrenRect.height
  enabled: isEnabled

  states: [
    // showing QfToggleButton without search
    State {
      name: "toggleButtonsView"
      PropertyChanges {
        target: toggleButtons
        visible: true
      }
      PropertyChanges {
        target: comboBox
        visible: false
      }
      PropertyChanges {
        target: searchButton
        visible: false
      }
    },
    // showing ComboBox with search option
    State {
      name: "comboBoxItemView"
      PropertyChanges {
        target: toggleButtons
        visible: false
      }
      PropertyChanges {
        target: comboBox
        visible: true
      }
      PropertyChanges {
        target: searchButton
        visible: searchButton.enabled
      }
    }
  ]

  // Using the search and comboBox when there are less than X items in the dropdown proves to be poor UI on normally
  // sized and oriented phones. Some empirical tests proved 6 to be a good number for now.
  readonly property int toggleButtonsThreshold: currentLayer && currentLayer.customProperty('QFieldSync/value_map_button_interface_threshold') !== undefined ? currentLayer.customProperty('QFieldSync/value_map_button_interface_threshold') : 0
  state: currentItemCount < toggleButtonsThreshold ? "toggleButtonsView" : "comboBoxItemView"

  property real currentItemCount: comboBox.count

  ValueMapModel {
    id: listModel
    filterCaseSensitivity: Qt.CaseInsensitive
    onMapChanged: {
      comboBox.currentIndex = keyToIndex(valueMap.currentKeyValue);
    }
  }

  RowLayout {
    anchors {
      left: parent.left
      right: parent.right
    }

    Item {
      id: toggleButtons
      Layout.fillWidth: true
      Layout.minimumHeight: flow.height + flow.anchors.topMargin + flow.anchors.bottomMargin

      property real selectedIndex: comboBox.currentIndex
      property string currentSelectedKey: ""
      property string currentSelectedValue: ""

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
          model: comboBox.model

          delegate: Rectangle {
            id: item
            width: Math.min(flow.width - 16, innerText.width + 16)
            height: 34
            radius: 4
            color: selected ? isEnabled ? Theme.mainColor : Theme.accentLightColor : "transparent"
            border.color: isEnabled ? selected ? Theme.mainColor : valueMap.Material.hintTextColor : "transparent"
            border.width: 1

            property bool selected: toggleButtons.selectedIndex == index

            Component.onCompleted: {
              if (selected) {
                toggleButtons.currentSelectedKey = key;
                toggleButtons.currentSelectedValue = value;
              }
            }

            Behavior on color  {
              ColorAnimation {
                duration: 150
              }
            }

            Text {
              id: innerText
              width: Math.min(flow.width - 32, implicitWidth)
              text: value
              elide: Text.ElideRight
              anchors.centerIn: parent
              font: Theme.defaultFont
              color: (!isEditable && isEditing) ? Theme.mainTextDisabledColor : Theme.mainTextColor
            }

            MouseArea {
              id: mouseArea
              anchors.fill: parent
              onClicked: {
                if (toggleButtons.selectedIndex != index) {
                  comboBox.currentIndex = index;
                  toggleButtons.currentSelectedKey = key;
                  toggleButtons.currentSelectedValue = value;
                } else {
                  comboBox.currentIndex = -1;
                  toggleButtons.currentSelectedKey = "";
                  toggleButtons.currentSelectedValue = "";
                }
                valueChangeRequested(toggleButtons.currentSelectedKey, false);
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

    QfComboBox {
      id: comboBox
      Layout.fillWidth: true
      font: Theme.defaultFont
      popup.font: Theme.defaultFont
      popup.topMargin: mainWindow.sceneTopMargin
      popup.bottomMargin: mainWindow.sceneTopMargin
      currentIndex: model.keyToIndex(value)
      model: listModel
      textRole: 'value'
      text.color: (!isEditable && isEditing) ? Theme.mainTextDisabledColor : Theme.mainTextColor
      background.visible: isEnabled || (!isEditable && isEditing)
      indicator.visible: isEnabled || (!isEditable && isEditing)
      text.leftPadding: isEnabled || (!isEditable && isEditing) ? 10 : 0

      Component.onCompleted: {
        comboBox.popup.z = 10000; // 1000s are embedded feature forms, use a higher value to insure popups always show above embedded feature formes
        model.valueMap = config['map'];
      }

      onCurrentTextChanged: {
        if (searchFeaturePopup.opened || valueMap.state !== "comboBoxItemView") {
          return;
        }
        var key = model.keyForValue(currentText);
        if (currentKeyValue !== key) {
          if (valueMap.state === "comboBoxItemView") {
            valueChangeRequested(key, false);
          }
        }
      }

      MouseArea {
        anchors.fill: parent
        propagateComposedEvents: true

        onClicked: mouse.accepted = false
        onPressed: mouse => {
          forceActiveFocus();
          mouse.accepted = false;
        }
        onReleased: mouse.accepted = false
        onDoubleClicked: mouse.accepted = false
        onPositionChanged: mouse.accepted = false
        onPressAndHold: mouse.accepted = false
      }
    }

    FontMetrics {
      id: fontMetrics
      font: comboBox.font
    }

    QfToolButton {
      id: searchButton

      Layout.preferredWidth: enabled ? 48 : 0
      Layout.preferredHeight: 48

      bgcolor: "transparent"
      iconSource: Theme.getThemeVectorIcon("ic_baseline_search_white")
      iconColor: Theme.mainTextColor

      onClicked: {
        searchFeaturePopup.open();
      }
    }

    QfPopup {
      id: searchFeaturePopup

      parent: mainWindow.contentItem
      width: mainWindow.width - Theme.popupScreenEdgeHorizontalMargin * 2
      height: mainWindow.height - Math.max(Theme.popupScreenEdgeVerticalMargin * 2, mainWindow.sceneTopMargin * 2 + 4, mainWindow.sceneBottomMargin * 2 + 4)
      x: Theme.popupScreenEdgeHorizontalMargin
      y: (mainWindow.height - height) / 2
      z: 10000 // 1000s are embedded feature forms, use a higher value to insure feature form popups always show above embedded feature formes
      closePolicy: Popup.CloseOnEscape
      focus: visible

      onOpened: {
        if (resultsList.contentHeight > resultsList.height) {
          searchBar.focusOnTextField();
        }
      }

      onClosed: {
        searchBar.clear();
      }

      Page {
        anchors.fill: parent
        padding: 5

        header: QfPageHeader {
          title: fieldLabel
          showBackButton: false
          showApplyButton: false
          showCancelButton: true
          onCancel: {
            searchFeaturePopup.close();
            listModel.setFilterFixedString('');
            comboBox.currentIndex = listModel.keyToIndex(valueMap.currentKeyValue);
          }
        }

        QfSearchBar {
          id: searchBar
          z: 1
          anchors.left: parent.left
          anchors.right: parent.right
          height: childrenRect.height

          onSearchTermChanged: {
            listModel.setFilterFixedString(searchTerm);
          }

          onReturnPressed: {
            if (listModel.rowCount() === 1) {
              resultsList.itemAtIndex(0).performClick();
              searchFeaturePopup.close();
            }
          }
        }

        ListView {
          id: resultsList
          anchors.left: parent.left
          anchors.right: parent.right
          anchors.top: searchBar.bottom
          model: listModel
          width: parent.width
          height: searchFeaturePopup.height - searchBar.height - 50
          boundsBehavior: Flickable.StopAtBounds
          clip: true
          ScrollBar.vertical: QfScrollBar {
          }
          delegate: Rectangle {
            id: delegateRect

            property string itemText: value ? value : "NULL"
            property bool itemChecked: currentKeyValue == key

            anchors.margins: 10
            height: radioButton.height
            width: parent ? parent.width : undefined
            color: itemChecked ? Theme.mainColor : searchFeaturePopup.Material ? searchFeaturePopup.Material.dialogColor : Theme.mainBackgroundColor

            RadioButton {
              id: radioButton

              anchors.verticalCenter: parent.verticalCenter
              width: resultsList.width - padding * 2
              padding: 12

              font.pointSize: Theme.defaultFont.pointSize
              font.weight: itemChecked ? Font.DemiBold : Font.Normal
              font.italic: value ? false : true

              checked: itemChecked
              indicator: Rectangle {
              }

              text: StringUtils.highlightText(itemText, searchBar.searchTerm, Theme.mainTextColor)

              contentItem: Text {
                text: parent.text
                font: parent.font
                width: parent.width
                verticalAlignment: Text.AlignVCenter
                leftPadding: parent.indicator.width + parent.spacing
                elide: Text.ElideRight
                color: searchBar.searchTerm !== '' ? Theme.secondaryTextColor : Theme.mainTextColor
                textFormat: Text.RichText
              }
            }

            /* bottom border */
            Rectangle {
              anchors.bottom: parent.bottom
              height: 1
              color: Theme.controlBorderColor
              width: resultsList.width
            }

            function performClick() {
              if (key === currentKeyValue) {
                return;
              }
              listModel.setFilterFixedString('');
              valueChangeRequested(key, false);
              searchFeaturePopup.close();
            }
          }

          MouseArea {
            anchors.fill: parent
            propagateComposedEvents: true

            onClicked: mouse => {
              var item = resultsList.itemAt(resultsList.contentX + mouse.x, resultsList.contentY + mouse.y);
              if (!item)
                return;
              item.performClick();
            }
          }

          onMovementStarted: {
            Qt.inputMethod.hide();
          }
        }
      }
    }
  }
}
