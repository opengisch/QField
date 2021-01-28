import QtQuick 2.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.12

import org.qfield 1.0
import org.qgis 1.0
import Theme 1.0

import ".."
import "."

EditorWidgetBase {
  id: valueRelation

  height: Number(config['AllowMulti']) !== 1 ? valueRelationCombobox.height : valueRelationList.height
  enabled: true

  LayerResolver {
      id: layerResolver

      layerId: config['Layer']
      layerName: config['LayerName']
      layerSource: config['LayerSource']
      layerProviderName: config['LayerProviderName']
      project: qgisProject
  }

  RelationCombobox {
    id: valueRelationCombobox

    property var _relation: undefined

    useCompleter: !!config['UseCompleter']
    enabled: isEnabled
    visible: Number(config['AllowMulti']) !== 1

    FeatureCheckListModel {
      id: featureListModel

      attributeField: field
      //passing "" instead of undefined, so the model is cleared on adding new features
      attributeValue: value !== undefined ? value : ""
      currentLayer: layerResolver.currentLayer
      currentFormFeature: currentFeature
      keyField: config['Key']
      displayValueField: config['Value']
      addNull: config['AllowNull']
      orderByValue: config['OrderByValue']
      filterExpression: config['FilterExpression']
      allowMulti: Number(config['AllowMulti']) === 1
      onListUpdated: {
        valueRelation.valueChanged( attributeValue, false )
      }
    }
  }

  Rectangle {
    id: valueRelationList

    visible: Number(config['AllowMulti']) === 1

    height: Math.max( valueListView.height, itemHeight)

    width: parent.width

    property int itemHeight: 32

    border.color: 'lightgray'
    border.width: 1

    FeatureCheckListModel {
        id: listModel
        attributeField: field
        //passing "" instead of undefined, so the model is cleared on adding new features
        attributeValue: value !== undefined ? value : ""
        currentLayer: layerResolver.currentLayer
        currentFormFeature: currentFeature
        keyField: config['Key']
        displayValueField: config['Value']
        addNull: config['AllowNull']
        orderByValue: config['OrderByValue']
        filterExpression: config['FilterExpression']
        allowMulti: true
        onListUpdated: {
            valueRelation.valueChanged( attributeValue, false )
        }
    }

    //the list
    ListView {
      id: valueListView
      model: listModel
      width: parent.width
      height: Math.min( 5 * valueRelationList.itemHeight, valueListView.count * valueRelationList.itemHeight )
      delegate: listComponent
      focus: true
      clip: true
      highlightRangeMode: ListView.StrictlyEnforceRange

      property int storedIndex

      onModelChanged:
        currentIndex = storedIndex
      onCurrentIndexChanged:
        storedIndex = currentIndex
    }

    //list components
    Component {
      id: listComponent

      Item {
        id: listItem
        anchors { left: parent ? parent.left : undefined; right: parent ? parent.right : undefined }
        height: Math.max( valueRelationList.itemHeight, valueText.height )

        focus: true

        Row {
          id: checkBoxRow
          height: listItem.height

          CheckDelegate {
            id: checkBox
            width: parent.height
            height: parent.height
            enabled: isEnabled

            checked: model.checked

            onCheckedChanged: {
              model.checked = checked
            }

            indicator.height: 16
            indicator.width: 16
            indicator.implicitHeight: 24
            indicator.implicitWidth: 24
          }

          Text {
            id: valueText
            anchors.verticalCenter: parent.verticalCenter
            font: Theme.defaultFont
            color: !isEnabled ? 'grey' : 'black'
            text: { text: model.displayString }
          }
        }

        MouseArea {
          anchors.fill: parent

          onClicked: {
            if (isEnabled) {
              checkBox.checked = !checkBox.checked
            }
          }
        }

        Rectangle {
          id: bottomLine
          anchors.bottom: parent.bottom
          height: 1
          color: 'lightGray'
          width: parent.width
        }
      }
    }
  }

  function siblingValueChanged(field, feature) {
    featureListModel.currentFormFeature = feature
  }
}

