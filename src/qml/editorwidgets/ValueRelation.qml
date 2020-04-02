import QtQuick 2.12
import QtQuick.Controls 2.12


import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.0
import ".."
import Theme 1.0

import org.qfield 1.0
import org.qgis 1.0

Item {
  id: valueRelation

  signal valueChanged(var value, bool isNull)

  height: !config['AllowMulti'] ? valueRelationCombobox.height : valueRelationList.height

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
        attributeField: field
        //passing "" instead of undefined, so the model is cleared on adding new features
        attributeValue: value !== undefined ? value : ""
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
            enabled: !readOnly

            checked: model.checked

            onCheckedChanged: {
              model.checked = checked
            }

            indicator.height: 16 * dp
            indicator.width: 16 * dp
            indicator.implicitHeight: 24 * dp
            indicator.implicitWidth: 24 * dp
          }
        }

        Text {
          id: valueText
          anchors { leftMargin: 10 * dp ; left: checkBoxRow.right; right: parent.right; verticalCenter: parent.verticalCenter }
          font.bold: true
          color: readOnly ? 'grey' : 'black'
          text: { text: model.displayString }
        }

        MouseArea {
          anchors.fill: parent

          onClicked: {
            if( !readOnly ){
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
}

