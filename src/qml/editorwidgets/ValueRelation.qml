import QtQuick 2.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.12

import org.qfield 1.0
import org.qgis 1.0
import Theme 1.0
import ".."


Item {
  id: valueRelation

  signal valueChanged(var value, bool isNull)

  height: !config['AllowMulti'] ? valueRelationCombobox.height : valueRelationList.height
  enabled: true

  RelationCombobox {
    id: valueRelationCombobox
    visible: !config['AllowMulti']
    property var _relation: undefined
    enabled: isEnabled

    FeatureListModel {
      id: featureListModel

      currentLayer: qgisProject.mapLayer(config['Layer'])
      currentFormFeature: currentFeature
      keyField: config['Key']
      displayValueField: config['Value']
      addNull: config['AllowNull']
      orderByValue: config['OrderByValue']
      filterExpression: config['FilterExpression']
    }
  }

  Rectangle{
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
        currentLayer: qgisProject.mapLayer(config['Layer'])
        currentFormFeature: currentFeature
        keyField: config['Key']
        displayValueField: config['Value']
        addNull: config['AllowNull']
        orderByValue: config['OrderByValue']
        filterExpression: config['FilterExpression']
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
        anchors { left: parent.left; right: parent.right }

        focus: true

        height: Math.max( valueRelationList.itemHeight, valueText.height )

        Row{
          id: checkBoxRow
          anchors { top: parent.top; left: parent.left }
          height: listItem.height

          CheckBox {
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
        }

        Text {
          id: valueText
          anchors { leftMargin: 10; left: checkBoxRow.right; right: parent.right; verticalCenter: parent.verticalCenter }
          font.bold: true
          color: !isEnabled ? 'grey' : 'black'
          text: { text: model.displayString }
        }

        MouseArea {
          anchors.fill: parent

          onClicked: {
            if( isEnabled ){
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

