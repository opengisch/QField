import QtQuick 2.0
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.0
import "." as QField
import Theme 1.0

import org.qfield 1.0
import org.qgis 1.0

Item {
  id: relationCombobox

  Component.onCompleted: {
    comboBox.currentIndex = featureListModel.findKey(comboBox.value)
    comboBox.visible = _relation !== undefined ? _relation.isValid : true
    addButton.visible = _relation !== undefined ? _relation.isValid : false
    invalidWarning.visible = _relation !== undefined ? !(_relation.isValid) : false
  }

  anchors {
    left: parent.left
    right: parent.right
    rightMargin: 10 * dp
  }

  height: childrenRect.height + 10 * dp

  RowLayout {
    anchors { left: parent.left; right: parent.right }

    ComboBox {
      id: comboBox

      property var currentValue: value
      property var _cachedCurrentValue

      textRole: 'display'

      Layout.fillWidth: true

      model: featureListModel

      onCurrentIndexChanged: {
        var idx = featureListModel.index(currentIndex, 0, undefined)
        var newValue = featureListModel.data(idx, FeatureListModel.KeyFieldRole)
        valueChanged(newValue, false)
      }

      // Workaround to get a signal when the value has changed
      onCurrentValueChanged: {
        currentIndex = featureListModel.findKey(currentValue)
      }

      Connections {
        target: featureListModel

        onModelAboutToBeReset: {
          comboBox._cachedCurrentValue = comboBox.currentValue
        }

        onModelReset: {
          comboBox.currentIndex = featureListModel.findKey(comboBox.currentValue)
        }
      }

      MouseArea {
        anchors.fill: parent
        propagateComposedEvents: true

        onClicked: mouse.accepted = false
        onPressed: { forceActiveFocus(); mouse.accepted = false; }
        onReleased: mouse.accepted = false;
        onDoubleClicked: mouse.accepted = false;
        onPositionChanged: mouse.accepted = false;
        onPressAndHold: mouse.accepted = false;
      }

      // [hidpi fixes]
      delegate: ItemDelegate {
        width: comboBox.width
        height: 36 * dp
        text: comboBox.textRole ? (Array.isArray(comboBox.model) ? modelData[comboBox.textRole] : model[comboBox.textRole]) : modelData
        font.weight: comboBox.currentIndex === index ? Font.DemiBold : Font.Normal
        font.pointSize: Theme.defaultFont.pointSize
        highlighted: comboBox.highlightedIndex == index
      }

      contentItem: Text {
        height: 36 * dp
        text: comboBox.displayText
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
      }

      background: Item {
        implicitWidth: 120 * dp
        implicitHeight: 36 * dp

        Rectangle {
          anchors.fill: parent
          id: backgroundRect
          border.color: comboBox.pressed ? "#17a81a" : "#21be2b"
          border.width: comboBox.visualFocus ? 2 : 1
          color: Theme.lightGray
          radius: 2
        }
      }
      // [/hidpi fixes]
    }

    Image {
      id: addButton
      source: Theme.getThemeIcon("ic_add_black_48dp")
      MouseArea {
        anchors.fill: parent
        onClicked: {
            attributeFormModel.featureModel.resetAttributes()
            addFeatureForm.active = true
        }
      }
    }

    Text {
      id: invalidWarning
      visible: false
      text: qsTr( "Invalid relation")
      color: "red"
    }
  }

  AttributeFormModel {
   id: attributeFormModel
   featureModel: FeatureModel {
       currentLayer: relationCombobox._relation.referencedLayer
     }
  }

  Loader {
    id: addFeatureForm
    sourceComponent: addFeatureFormComponent
    active: false
    onLoaded: {
      item.open()
    }
  }

  Component {
    id: addFeatureFormComponent
    Popup {
      id: popup
      parent: ApplicationWindow.overlay

      x: 24 * dp
      y: 24 * dp
      width: parent.width - 48 * dp
      height: parent.height - 48 * dp
      padding: 0
      modal: true
      focus: true
      closePolicy: Popup.CloseOnEscape

      FeatureForm {
        model: attributeFormModel

        anchors.fill: parent

        state: "Add"
        embedded: true

        onSaved: {
          var referencedValue = attributeFormModel.attribute(relationCombobox._relation.resolveReferencedField(field.name))
          comboBox.currentValue = referencedValue
          popup.close()
        }

        onCancelled: {
          popup.close()
        }
      }

      onClosed: {
        addFeatureForm.active = false
      }
    }
  }
}
