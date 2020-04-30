import QtQuick 2.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.0
import "." as QField
import Theme 1.0

import org.qfield 1.0
import org.qgis 1.0

Item {
  id: relationCombobox

  Component.onCompleted: {
    comboBox.currentIndex = featureListModel.findKey(value)
    comboBox.visible = _relation !== undefined ? _relation.isValid : true
    addButton.visible = _relation !== undefined ? _relation.isValid : false
    invalidWarning.visible = _relation !== undefined ? !(_relation.isValid) : false
  }

  anchors {
    left: parent.left
    right: parent.right
    rightMargin: 10 * dp
  }

  property var currentKeyValue: value
  onCurrentKeyValueChanged: {
    comboBox._cachedCurrentValue = currentKeyValue
    comboBox.currentIndex = featureListModel.findKey(currentKeyValue)
  }

  height: childrenRect.height + 10 * dp

  RowLayout {
    anchors { left: parent.left; right: parent.right }

    ComboBox {
      id: comboBox

      property var _cachedCurrentValue

      textRole: 'display'

      Layout.fillWidth: true

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
      Layout.margins: 4 * dp
      Layout.preferredWidth: 18 * dp
      Layout.preferredHeight: 18 * dp
      id: addButton
      source: Theme.getThemeIcon("ic_add_black_48dp")
      width: 18 * dp
      height: 18 * dp

      MouseArea {
        anchors.fill: parent
        onClicked: {
            addFeaturePopup.state = 'Add'
            addFeaturePopup.currentLayer = relationCombobox._relation ? relationCombobox._relation.referencedLayer : null
            addFeaturePopup.open()
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

  EmbeddedFeatureForm{
      id: addFeaturePopup

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
}
