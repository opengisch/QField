import QtQuick 2.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.12

import org.qfield 1.0
import org.qgis 1.0
import Theme 1.0

Item {
  id: relationCombobox

  property bool useCompleter: false

  Component.onCompleted: {
    comboBox.currentIndex = featureListModel.findKey(value)
    comboBox.visible = !useCompleter && (_relation !== undefined ? _relation.isValid : true)
    searchableLabel.visible = !comboBox.visible
    searchButton.visible = comboBox.visible
    addButton.visible = _relation !== undefined ? _relation.isValid : false
    invalidWarning.visible = _relation !== undefined ? !(_relation.isValid) : false
  }

  anchors {
    left: parent.left
    right: parent.right
    rightMargin: 10
  }
  height: childrenRect.height + 10

  property var currentKeyValue: value
  property EmbeddedFeatureForm embeddedFeatureForm: embeddedPopup

  onCurrentKeyValueChanged: {
    comboBox._cachedCurrentValue = currentKeyValue
    comboBox.currentIndex = featureListModel.findKey(currentKeyValue)
  }

  EmbeddedFeatureForm {
      id: addFeaturePopup

      embeddedLevel: form.embeddedLevel + 1
      digitizingToolbar: form.digitizingToolbar

      onFeatureSaved: {
          var referencedValue = addFeaturePopup.attributeFormModel.attribute(relationCombobox._relation.resolveReferencedField(field.name))
          var index = featureListModel.findKey(referencedValue)
          if ( index < 0 ) {
            // model not yet reloaded - keep the value and set it onModelReset
            comboBox._cachedCurrentValue = referencedValue
          } else {
            comboBox.currentIndex = index
          }
      }
  }


  Popup {
    id: searchFeaturePopup

    parent: ApplicationWindow.overlay
    x: 24
    y: 24
    z: 10000 // 1000s are embedded feature forms, use a higher value to insure feature form popups always show above embedded feature formes
    width: parent.width - 48
    height: parent.height - 48
    padding: 0
    modal: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
    focus: visible

    onOpened: {
      searchField.forceActiveFocus()
    }

    onClosed: {
      searchField.text = ''
    }

    Page {
      anchors.fill: parent

      header: PageHeader {
        title: fieldLabel
        showApplyButton: false
        showCancelButton: true
        onCancel: searchFeaturePopup.close()
      }

      TextField {
        z: 1
        id: searchField
        anchors.left: parent.left
        anchors.right: parent.right

        placeholderText: qsTr("Search…")
        placeholderTextColor: Theme.mainColor

        height: fontMetrics.height * 2.5
        padding: 24
        bottomPadding: 9
        font: Theme.defaultFont
        selectByMouse: true
        verticalAlignment: TextInput.AlignVCenter

        inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase

        onDisplayTextChanged: {
          featureListModel.searchTerm = searchField.displayText
        }
      }

      Rectangle {
          id: clearButtonRect
          z: 1
          width: fontMetrics.height
          height: fontMetrics.height
          anchors { top: searchField.top; right: searchField.right; topMargin: height - 7; rightMargin: height - 7 }
          color: "transparent"

          Image {
              id: clearButton
              z: 1
              width: 20
              height: 20
              source: Theme.getThemeIcon("ic_clear_black_18dp")
              sourceSize.width: 20 * screen.devicePixelRatio
              sourceSize.height: 20 * screen.devicePixelRatio
              fillMode: Image.PreserveAspectFit
              anchors.centerIn: clearButtonRect
              opacity: searchField.displayText.length > 0 ? 1 : 0.25
          }

          MouseArea {
              anchors.fill: parent
              onClicked: {
                  searchField.text = '';
              }
          }
      }

      ScrollView {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: searchField.bottom

        padding: 0
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        ScrollBar.vertical.policy: ScrollBar.AsNeeded
        contentItem: resultsList
        contentWidth: resultsList.width
        contentHeight: resultsList.height
        clip: true

        ListView {
          id: resultsList
          anchors.top: parent.top
          model: featureListModel
          width: parent.width
          height: searchFeaturePopup.height - searchField.height - 50
          clip: true

          delegate: Rectangle {
            id: delegateRect

            property int idx: index

            anchors.margins: 10
            height: radioButton.visible ? radioButton.height : checkBoxButton.height
            width: parent ? parent.width : undefined
            color: model.checked ? Theme.mainColor : 'transparent'

            RadioButton {
              id: radioButton

              visible: !featureListModel.allowMulti
              checked: model.checked
              anchors.verticalCenter: parent.verticalCenter
              anchors.left: parent.left
              text: displayString
              width: parent.width
              padding: 12
              ButtonGroup.group: buttonGroup
              font.weight: model.checked ? Font.DemiBold : Font.Normal

              indicator: Rectangle {}
              contentItem: Text {
                text: parent.text
                font: parent.font
                width: parent.width
                verticalAlignment: Text.AlignVCenter
                leftPadding: parent.indicator.width + parent.spacing
                elide: Text.ElideRight
                color: model.checked ? Theme.light : Theme.darkGray
              }
            }

            CheckBox {
              id: checkBoxButton

              visible: !!featureListModel.allowMulti
              anchors.verticalCenter: parent.verticalCenter
              anchors.left: parent.left
              text: displayString
              padding: 12
            }

            /* bottom border */
            Rectangle {
              anchors.bottom: parent.bottom
              height: 1
              color: "lightGray"
              width: parent.width
            }

            function performClick() {
              model.checked = true
            }
          }

          MouseArea {
            anchors.fill: parent
            propagateComposedEvents: true

            onClicked: {
              var allowMulti = resultsList.model.allowMulti;
              var popupRef = searchFeaturePopup;
              var item = resultsList.itemAt(mouse.x, mouse.y)

              if (!item)
                return;

              item.performClick()

              // after this line, all the references get wrong, that's why we have `popupRef` defined above
              model.checked = !model.checked

              if (!allowMulti) {
                popupRef.close()
              }
            }
          }
        }
      }
    }
  }

  ButtonGroup { id: buttonGroup }

  RowLayout {
    anchors { left: parent.left; right: parent.right }

    ComboBox {
      id: comboBox

      property var _cachedCurrentValue
      Layout.fillWidth: true
      textRole: 'display'
      model: featureListModel

      onCurrentIndexChanged: {
        var newValue = featureListModel.dataFromRowIndex(currentIndex, FeatureListModel.KeyFieldRole)
        valueChanged(newValue, false)
      }

      Connections {
        target: featureListModel

        function onModelReset() {
          comboBox.currentIndex = featureListModel.findKey(comboBox._cachedCurrentValue)
        }
      }

      MouseArea {
        anchors.fill: parent
        propagateComposedEvents: true

        onClicked: { mouse.accepted = false; }
        onPressed: {
          forceActiveFocus();
          mouse.accepted = false;
        }
        onReleased: mouse.accepted = false;
        onDoubleClicked: mouse.accepted = false;
        onPositionChanged: mouse.accepted = false;
        onPressAndHold: mouse.accepted = false;
      }

      Component.onCompleted: {
          comboBox.popup.z = 10000 // 1000s are embedded feature forms, use a higher value to insure popups always show above embedded feature formes
      }

      contentItem: Text {
        id: textLabel
        height: fontMetrics.height + 20
        text: comboBox.displayText
        font: Theme.defaultFont
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
        color: value === undefined || !enabled ? 'gray' : 'black'
      }

      FontMetrics {
        id: fontMetrics
        font: textLabel.font
      }

      background: Item {
        implicitWidth: 120
        implicitHeight: 36

        Rectangle {
          y: textLabel.height - 8
          width: comboBox.width
          height: comboBox.activeFocus ? 2 : 1
          color: comboBox.activeFocus ? "#4CAF50" : "#C8E6C9"
        }

        Rectangle {
          visible: enabled
          anchors.fill: parent
          border.color: comboBox.pressed ? "#4CAF50" : "#C8E6C9"
          border.width: comboBox.visualFocus ? 2 : 1
          color: Theme.lightGray
          radius: 2
        }
      }
    }

    Rectangle {
      id: searchableLabel
      height: fontMetrics.height + 10
      Layout.fillWidth: true

      Text {
        padding: 5
        width: parent.width
        text: comboBox.displayText
        font: Theme.defaultFont
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
        color: value === undefined || !enabled ? 'gray' : 'black'
      }

      visible: enabled
      border.color: comboBox.pressed ? "#4CAF50" : "#C8E6C9"
      border.width: comboBox.visualFocus ? 2 : 1
      color: Theme.lightGray
      radius: 2


      MouseArea {
        anchors.fill: parent
        onClicked: {
            mouse.accepted = true;
            searchFeaturePopup.open()
        }
      }
    }

    Image {
      id: searchButton

      Layout.margins: 4
      Layout.preferredWidth: width
      Layout.preferredHeight: 18
      source: Theme.getThemeIcon("ic_baseline_search_black")
      width: visible ? 18 : 0
      height: 18
      opacity: enabled ? 1 : 0.3

      MouseArea {
        anchors.fill: parent
        onClicked: {
          searchFeaturePopup.open()
        }
      }
    }

    Image {
      Layout.margins: 4
      Layout.preferredWidth: width
      Layout.preferredHeight: 18
      id: addButton
      source: Theme.getThemeIcon("ic_add_black_48dp")
      width: comboBox.enabled ? 18 : 0
      height: 18
      opacity: enabled ? 1 : 0.3

      MouseArea {
        anchors.fill: parent
        onClicked: {
            embeddedPopup.state = 'Add'
            embeddedPopup.currentLayer = relationCombobox._relation ? relationCombobox._relation.referencedLayer : null
            embeddedPopup.open()
        }
      }
    }

    Text {
      id: invalidWarning
      visible: false
      text: qsTr( "Invalid relation")
      color: Theme.errorColor
    }
  }

  EmbeddedFeatureForm {
    id: embeddedPopup

    embeddedLevel: form.embeddedLevel + 1
    digitizingToolbar: form.digitizingToolbar

    onFeatureSaved: {
      var referencedValue = embeddedPopup.attributeFormModel.attribute(relationCombobox._relation.resolveReferencedField(field.name))
      var index = featureListModel.findKey(referencedValue)
      if ( ( featureListModel.addNull == true && index < 1 ) || index < 0 ) {
        // model not yet reloaded - keep the value and set it onModelReset
        comboBox._cachedCurrentValue = referencedValue
      } else {
        comboBox.currentIndex = index
      }
    }
  }
}
