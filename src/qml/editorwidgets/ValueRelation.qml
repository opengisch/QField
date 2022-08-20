import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

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


  FeatureCheckListModel {
    id: listModel
    allowMulti: Number(config['AllowMulti']) === 1
    attributeField: field
    currentLayer: layerResolver.currentLayer
    currentFormFeature: currentFeature
    keyField: config['Key']
    displayValueField: config['Value']
    addNull: config['AllowNull']
    orderByValue: config['OrderByValue']
    filterExpression: config['FilterExpression']

    // passing "" instead of undefined, so the model is cleared on adding new features
    // attributeValue has to be the last property set to make sure its given value is handled properly (e.g. allow multiple)
    attributeValue: value !== undefined ? value : ""

    onListUpdated: {
      valueChangeRequested( attributeValue, false )
    }
  }

  RelationCombobox {
    id: valueRelationCombobox
    featureListModel: listModel

    property var _relation: undefined

    useCompleter: !!config['UseCompleter']
    enabled: isEnabled
    visible: Number(config['AllowMulti']) !== 1
  }

  Rectangle {
    id: valueRelationList

    visible: Number(config['AllowMulti']) === 1

    height: Math.max( valueListView.height, itemHeight)

    width: parent.width

    property int itemHeight: 32

    border.color: 'lightgray'
    border.width: 1

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

        Rectangle {
          id: checkBoxRow
          width: parent.width
          height: listItem.height
          color: "transparent"

          CheckDelegate {
            id: checkBox
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            width: valueRelationList.itemHeight
            height: valueRelationList.itemHeight
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
            anchors.right: parent.right
            anchors.left: checkBox.right
            anchors.leftMargin: 4
            width: parent.width - checkBox.width
            topPadding: 4
            bottomPadding: 4
            font: Theme.defaultFont
            color: !isEnabled ? 'grey' : 'black'
            text: model.displayString
            wrapMode: Text.WordWrap
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
    listModel.currentFormFeature = feature
  }
}

