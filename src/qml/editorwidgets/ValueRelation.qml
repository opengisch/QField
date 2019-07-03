import QtQuick 2.11
import QtQuick.Controls 2.4


import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.0
import ".."
import "../js/style.js" as Style

import org.qfield 1.0
import org.qgis 1.0

Item {
  id: valueRelation

  signal valueChanged(var value, bool isNull)

  RelationCombobox {
    id: valueRelationCombobox
    visible: !config['AllowMulti']
    property var _relation: undefined

    FeatureListModel {
      id: featureListModel

      currentLayer: qgisProject.mapLayer(config['Layer'])
      keyField: config['Key']
      displayValueField: config['Value']
      addNull: config['AllowNull']
      orderByValue: config['OrderByValue']
    }
  }

  Rectangle{
    id: valueRelationList

    visible: config['AllowMulti']

    height: Math.max( valueListView.height, itemHeight)
    width: parent.width

    property int itemHeight: 32 * dp

    border.color: 'lightgray'
    border.width: 1 * dp

    FeatureCheckListModel {
      id: listModel
        attributeValue: value
        currentLayer: qgisProject.mapLayer(config['Layer'])
        keyField: config['Key']
        displayValueField: config['Value']
        addNull: config['AllowNull']
        orderByValue: config['OrderByValue']
        onListUpdated: {
          valueChanged( attributeValue, false )
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

            checked: model.checked

            onCheckedChanged: {
              model.checked = checked
            }
          }
        }

        Text {
          id: valueText
          anchors { leftMargin: 10 * dp ; left: checkBoxRow.right; right: parent.right; verticalCenter: parent.verticalCenter }
          font.bold: true
          color: readOnly ? 'grey' : 'black'
          text: { text: model.displayString }
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
}

