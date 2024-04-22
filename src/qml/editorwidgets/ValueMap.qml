import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts

import org.qfield 1.0
import org.qgis 1.0
import Theme 1.0

import "."
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
    comboBox.currentIndex = comboBox.model.keyToIndex(currentKeyValue)
  }

  height: childrenRect.height
  enabled: isEnabled

  RowLayout {
    anchors { left: parent.left; right: parent.right }

    ComboBox {
      id: comboBox

      Layout.fillWidth: true

      font: Theme.defaultFont

      popup.font: Theme.defaultFont
      popup.topMargin: mainWindow.sceneTopMargin
      popup.bottomMargin: mainWindow.sceneTopMargin

      currentIndex: model.keyToIndex(value)

      model: ValueMapModel {
        id: listModel

        onMapChanged: {
          comboBox.currentIndex = keyToIndex(valueMap.currentKeyValue)
        }
      }

      Component.onCompleted:
      {
        comboBox.popup.z = 10000 // 1000s are embedded feature forms, use a higher value to insure popups always show above embedded feature formes
        model.valueMap = config['map']
      }

      textRole: 'value'

      onCurrentTextChanged: {
        var key = model.keyForValue(currentText)
        valueChangeRequested(key, false)
      }

      MouseArea {
        anchors.fill: parent
        propagateComposedEvents: true

        onClicked: mouse.accepted = false
        onPressed: (mouse)=> { forceActiveFocus(); mouse.accepted = false; }
        onReleased: mouse.accepted = false;
        onDoubleClicked: mouse.accepted = false;
        onPositionChanged: mouse.accepted = false;
        onPressAndHold: mouse.accepted = false;
      }

      contentItem: Text {
        leftPadding: enabled ? 5 : 0

        text: comboBox.displayText
        font: comboBox.font
        color: enabled ? Theme.mainTextColor : Theme.mainTextDisabledColor
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignLeft
        elide: Text.ElideRight
      }

      background: Item {
        implicitWidth: 120
        implicitHeight: 36

        Rectangle {
          visible: !enabled
          y: comboBox.height - 12
          width: comboBox.width
          height: comboBox.activeFocus ? 2 : 1
          color: comboBox.activeFocus ? Theme.accentColor : Theme.accentLightColor
        }

        Rectangle {
          visible: enabled
          anchors.fill: parent
          id: backgroundRect
          border.color: comboBox.pressed ? Theme.accentColor : Theme.accentLightColor
          border.width: comboBox.visualFocus ? 2 : 1
          color: Theme.controlBackgroundAlternateColor
          radius: 2
        }
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
        iconSource: Theme.getThemeIcon("ic_baseline_search_black")
        iconColor: Theme.mainTextColor

        visible: enabled

        onClicked: {
            searchFeaturePopup.open()
        }
    }

    Popup {
      id: searchFeaturePopup

      parent: mainWindow.contentItem
      x: Theme.popupScreenEdgeMargin
      y: Theme.popupScreenEdgeMargin
      z: 10000 // 1000s are embedded feature forms, use a higher value to insure feature form popups always show above embedded feature formes
      width: parent.width - Theme.popupScreenEdgeMargin * 2
      height: parent.height - Theme.popupScreenEdgeMargin * 2
      padding: 0
      modal: true
      closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
      focus: visible

      onOpened: {
        if (resultsList.contentHeight > resultsList.height) {
          searchField.forceActiveFocus()
        }
      }

      onClosed: {
        searchField.text = ''
      }

      Page {
        anchors.fill: parent

        header: QfPageHeader {
          title: fieldLabel
          showBackButton: false
          showApplyButton: false
          showCancelButton: true
          onCancel: searchFeaturePopup.close()
        }

        TextField {
          z: 1
          id: searchField
          anchors.left: parent.left
          anchors.right: parent.right

          placeholderText: !focus && displayText === '' ? qsTr("Search…") : ''
          placeholderTextColor: Theme.mainColor

          height: fontMetrics.height * 2.5
          padding: 24
          bottomPadding: 9
          font: Theme.defaultFont
          selectByMouse: true
          verticalAlignment: TextInput.AlignVCenter

          inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase | Qt.ImhSensitiveData

          onDisplayTextChanged: listModel.setFilterFixedString(displayText)

          Keys.onPressed: (event)=> {
            if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter) {
              if (listModel.rowCount() === 1) {
                resultsList.itemAtIndex(0).performClick()
                searchFeaturePopup.close()
              }
            }
          }
        }

        QfToolButton {
          id: clearButton
          z: 1
          width: fontMetrics.height
          height: fontMetrics.height
          anchors { top: searchField.top; right: searchField.right; topMargin: height - 7; rightMargin: height - 7 }

          padding: 0
          iconSource: Theme.getThemeIcon("ic_clear_black_18dp")
          iconColor: Theme.mainTextColor
          bgcolor: "transparent"

          opacity: searchField.displayText.length > 0 ? 1 : 0.25

          onClicked: {
            searchField.text = '';
          }
        }

        ListView {
          id: resultsList
          anchors.left: parent.left
          anchors.right: parent.right
          anchors.top: searchField.bottom
          model: listModel

          width: parent.width
          height: searchFeaturePopup.height - searchField.height - 50
          clip: true

          ScrollBar.vertical: ScrollBar {
            policy: ScrollBar.AsNeeded
            width: 6
            contentItem: Rectangle {
              implicitWidth: 6
              implicitHeight: 25
              color: Theme.mainColor
            }
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
              indicator: Rectangle {}

              text: searchField.displayText !== ''
                    ? itemText.replace(new RegExp('('+searchField.displayText+')', "i"),
                                            '<span style="text-decoration:underline;' + Theme.toInlineStyles({color:Theme.mainTextColor}) + '">$1</span>')
                    : itemText

              contentItem: Text {
                text: parent.text
                font: parent.font
                width: parent.width
                verticalAlignment: Text.AlignVCenter
                leftPadding: parent.indicator.width + parent.spacing
                elide: Text.ElideRight
                color: searchField.displayText !== '' ? Theme.secondaryTextColor : Theme.mainTextColor
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
              if (key === currentKeyValue)
                return

              valueChangeRequested(key, false)
              searchFeaturePopup.close()
            }
          }

          MouseArea {
            anchors.fill: parent
            propagateComposedEvents: true

            onClicked: (mouse)=> {
              var item = resultsList.itemAt(resultsList.contentX + mouse.x, resultsList.contentY + mouse.y)
              if (!item)
                return;

              item.performClick()
            }
          }

          onMovementStarted: {
            Qt.inputMethod.hide()
          }
        }
      }
    }
  }
}
