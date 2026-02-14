

# File ValueMap.qml

[**File List**](files.md) **>** [**editorwidgets**](dir_7621275d8427768f32eeef555af1c8d1.md) **>** [**ValueMap.qml**](ValueMap_8qml.md)

[Go to the documentation of this file](ValueMap_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
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

  // Workaround to get a signal when the value has changed
  property var currentKeyValue: value
  onCurrentKeyValueChanged: {
    comboBox.currentIndex = comboBox.model.keyToIndex(currentKeyValue);
    toggleButtons.selectedIndex = toggleButtons.model.keyToIndex(currentKeyValue);
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
  property bool useToggleButtons: comboBox.count < toggleButtonsThreshold
  state: useToggleButtons ? "toggleButtonsView" : "comboBoxItemView"

  ValueMapModel {
    id: listModel
    filterCaseSensitivity: Qt.CaseInsensitive
    onMapChanged: {
      comboBox.currentIndex = keyToIndex(valueMap.currentKeyValue);
      toggleButtons.selectedIndex = keyToIndex(valueMap.currentKeyValue);
    }
  }

  RowLayout {
    anchors {
      left: parent.left
      right: parent.right
    }

    QfToggleButtonGroup {
      id: toggleButtons
      Layout.fillWidth: true
      Layout.minimumHeight: toggleButtons.height

      model: listModel
      textRole: "value"
      allowDeselect: true
      editing: isEditing
      editable: isEditable
      enabled: isEnabled

      onItemSelected: function (index, itemModel) {
        valueChangeRequested(itemModel !== undefined ? itemModel.key : "", false);
      }

      onItemDeselected: function () {
        valueChangeRequested("", false);
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
          ScrollBar.vertical: QfScrollBar {}
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
              indicator: Rectangle {}

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
```


