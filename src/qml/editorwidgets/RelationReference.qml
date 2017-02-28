import QtQuick 2.0
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.0
import ".."
import "../js/style.js" as Style

import org.qfield 1.0
import org.qgis 1.0

Item {
  signal valueChanged(var value, bool isNull)

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
      property var _relation
      property var _cachedCurrentValue

      textRole: 'display'

      Layout.fillWidth: true

      model: FeatureListModel {
        id: featureListModel
      }

      Component.onCompleted: {
        _relation = qgisProject.relationManager.relation(config['Relation'])
        featureListModel.currentLayer = _relation.referencedLayer
        featureListModel.keyField = _relation.resolveReferencedField(field.name)
        currentIndex = featureListModel.findKey(value)
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
          comboBox._cachedCurrentValue = value
        }

        onModelReset: {
          comboBox.currentIndex = featureListModel.findKey(value)
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
      iconSource: Style.getThemeIcon( "ic_add_black_48dp" )
      bgcolor: "white"
      onClicked: {
        addFeatureForm.active = true
      }
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
      height: parent.width - 48 * dp
      modal: true
      focus: true
      closePolicy: Popup.CloseOnEscape

      FeatureForm {
        model: AttributeFormModel {
          id: attributeFormModel

          featureModel: FeatureModel {
            currentLayer: comboBox._relation.referencedLayer
          }
        }

        anchors.fill: parent

        state: "Add"

        onSaved: {
          var referencedValue = attributeFormModel.attribute(comboBox._relation.resolveReferencedField(field.name))
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
