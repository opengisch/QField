import QtQuick 2.0
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.0
import ".."
import "../js/style.js" as Style

import org.qfield 1.0
import org.qgis 1.0

Item {
  id: relationWidget
  signal valueChanged(var value, bool isNull)
  property var _relation

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

      model: FeatureListModel {
        id: featureListModel
        addNull: config['AllowNULL']
        orderByValue: config['OrderByValue']
      }

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
        font.pointSize: 12
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
          color: "#dddddd"
          radius: 2
        }
      }
      // [/hidpi fixes]
    }

    Button {
      id: addButton
      iconSource: Style.getThemeIcon( "ic_add_black_48dp" )
      bgcolor: "white"
      onClicked: {
        attributeFormModel.featureModel.resetAttributes()
        addFeatureForm.active = true
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
       currentLayer: relationWidget._relation.referencedLayer
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
      modal: true
      focus: true
      closePolicy: Popup.CloseOnEscape

      FeatureForm {
        model: attributeFormModel

        anchors.fill: parent

        state: "Add"
        embedded: true

        onSaved: {
          var referencedValue = attributeFormModel.attribute(relationWidget._relation.resolveReferencedField(field.name))
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
