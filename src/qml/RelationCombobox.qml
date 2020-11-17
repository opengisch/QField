import QtQuick 2.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.12

import org.qfield 1.0
import org.qgis 1.0
import Theme 1.0

Item {
  id: relationCombobox
  anchors {
    left: parent.left
    right: parent.right
    rightMargin: 10
  }
  height: childrenRect.height + 10

  property EmbeddedFeatureForm embeddedFeatureForm: embeddedPopup

  Popup {
    id: popup

    parent: ApplicationWindow.overlay
    x: 24
    y: 24
    width: parent.width - 48
    height: parent.height - 48
    padding: 0
    modal: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
    focus: visible
    visible: true

    Page {
      anchors.fill: parent

      header: PageHeader {
        title: qsTr('Related Features')
        showApplyButton: true
        showCancelButton: false
        onFinished: popup.visible = false
      }

      TextField {
        z: 1
        id: searchField
        anchors.left: parent.left
        anchors.right: parent.right

        placeholderText: qsTr("Search…")
        placeholderTextColor: Theme.mainColor

        topPadding: 0
        leftPadding: 24
        rightPadding: 24
        bottomPadding: 0
        font: Theme.defaultFont
        selectByMouse: true
        verticalAlignment: TextInput.AlignVCenter

        background: Rectangle {
          anchors.fill: searchField
          color: 'lightgreen'
        }

        inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase

        onDisplayTextChanged: {
          featureListModel.searchTerm = searchField.displayText
        }
      }

      ScrollView {
        padding: 20

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: searchField.bottom

        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        ScrollBar.vertical.policy: ScrollBar.AsNeeded
//        contentItem: resultsList
//        contentWidth: resultsList.width
//        contentHeight: resultsList.height
//        clip: true

        ListView {
          id: resultsList
          anchors.top: parent.top
          model: featureListModel
          width: parent.width
//            height: resultsList.count > 0
//                    ? Math.min( delegateRect.height, mainWindow.height / 2 - searchField.height - 10 )
//                    : 0
          height: 200
          clip: true

          delegate: Rectangle {
            id: delegateRect

//            anchors.top: parent ? parent.bottom : undefined
            anchors.margins: 10
            height: textCell.height
            width: parent ? parent.width : undefined

            Text {
              id: textCell
              text: displayString
              anchors.verticalCenter: parent.verticalCenter
              anchors.left: parent.left
              leftPadding: 5
              font.pointSize: Theme.resultFont.pointSize
              elide: Text.ElideRight
              horizontalAlignment: Text.AlignLeft
            }

            /* bottom border */
            Rectangle {
              anchors.bottom: parent.bottom
              height: 1
              color: "lightGray"
              width: parent.width
            }

            MouseArea {
              anchors.left: parent.left
              anchors.top: parent.top
              anchors.bottom: parent.bottom
              onClicked: {
                locator.triggerResultAtRow(index)
                locatorItem.state = "off"
              }
            }
          }
        }
      }
    }
  }

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

        onModelReset: {
          comboBox.currentIndex = featureListModel.findKey(comboBox._cachedCurrentValue)
        }
      }

      MouseArea {
        anchors.fill: parent
        propagateComposedEvents: true

        onClicked: { mouse.accepted = false; }
        onPressed: { forceActiveFocus(); mouse.accepted = false; }
        onReleased: mouse.accepted = false;
        onDoubleClicked: mouse.accepted = false;
        onPositionChanged: mouse.accepted = false;
        onPressAndHold: mouse.accepted = false;
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
          id: backgroundRect
          border.color: comboBox.pressed ? "#4CAF50" : "#C8E6C9"
          border.width: comboBox.visualFocus ? 2 : 1
          color: Theme.lightGray
          radius: 2
        }
      }
    }
  }

    Image {
      Layout.margins: 4
      Layout.preferredWidth: comboBox.enabled ? 18 : 0
      Layout.preferredHeight: 18
      id: addButton
      source: Theme.getThemeIcon("ic_add_black_48dp")
      width: comboBox.enabled ? 18 : 0
      height: 18

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
