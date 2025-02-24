import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield
import org.qgis
import Theme
import ".."

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
    keyField: config['Key'] ? config['Key'] : ""
    displayValueField: config['Value'] ? config['Value'] : ""
    groupField: config['Group'] ? config['Group'] : ""
    displayGroupName: config['DisplayGroupName'] ? config['DisplayGroupName'] : ""
    addNull: config['AllowNull'] ? config['AllowNull'] : ""
    orderByValue: config['OrderByValue'] ? config['OrderByValue'] : ""
    filterExpression: config['FilterExpression'] ? config['FilterExpression'] : ""

    // passing "" instead of undefined, so the model is cleared on adding new features
    // attributeValue has to be the last property set to make sure its given value is handled properly (e.g. allow multiple)
    attributeValue: value !== undefined ? value : ""

    onListUpdated: {
      valueChangeRequested(attributeValue, attributeValue === "");
    }
  }

  RelationCombobox {
    id: valueRelationCombobox
    featureListModel: listModel

    useCompleter: !!config['UseCompleter']
    enabled: isEnabled
    visible: Number(config['AllowMulti']) !== 1
    relation: undefined
  }

  Rectangle {
    id: valueRelationList

    property int itemHeight: 32

    visible: Number(config['AllowMulti']) === 1
    width: parent.width
    height: Math.min(8 * valueRelationList.itemHeight, valueListView.contentHeight)

    color: Theme.mainBackgroundColor
    border.color: Theme.controlBorderColor
    border.width: 1

    ListView {
      id: valueListView

      property int storedIndex

      onModelChanged: currentIndex = storedIndex
      onCurrentIndexChanged: storedIndex = currentIndex

      anchors.fill: parent
      anchors.margins: 1
      model: listModel
      focus: true
      clip: true

      section.property: listModel.groupField != "" ? "groupFieldValue" : ""
      section.labelPositioning: ViewSection.CurrentLabelAtStart | ViewSection.InlineLabels
      section.delegate: Rectangle {
        width: parent.width
        height: listModel.groupField != "" ? listModel.displayGroupName ? 30 : 5 : 0
        color: Theme.controlBorderColor

        Text {
          anchors {
            horizontalCenter: parent.horizontalCenter
            verticalCenter: parent.verticalCenter
          }
          font.bold: true
          font.pointSize: Theme.resultFont.pointSize
          color: Theme.mainTextColor
          text: section
          visible: listModel.displayGroupName
        }
      }

      delegate: Item {
        id: listItem
        anchors {
          left: parent ? parent.left : undefined
          right: parent ? parent.right : undefined
        }
        height: Math.max(valueRelationList.itemHeight, valueText.height)

        focus: true

        Rectangle {
          id: checkBoxRow
          width: parent.width
          height: listItem.height
          color: Theme.mainBackgroundColor

          CheckDelegate {
            id: checkBox
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            width: valueRelationList.itemHeight
            height: valueRelationList.itemHeight
            enabled: isEnabled

            checked: model.checked

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
            color: !isEnabled ? Theme.mainTextDisabledColor : Theme.mainTextColor
            text: model.displayString
            wrapMode: Text.WordWrap
          }
        }

        MouseArea {
          anchors.fill: parent

          onClicked: {
            if (isEnabled) {
              model.checked = !model.checked;
            }
          }
        }

        Rectangle {
          id: bottomLine
          anchors.bottom: parent.bottom
          height: 1
          color: Theme.controlBackgroundAlternateColor
          width: parent.width
        }
      }
    }
  }
}
