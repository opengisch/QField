import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield
import org.qgis
import Theme
import ".."

EditorWidgetBase {
  id: valueRelation

  height: (Number(config['AllowMulti']) !== 1 ? valueRelationCombobox.height : valueRelationListComponent.height) + 4
  enabled: true

  LayerResolver {
    id: layerResolver

    layerId: config['Layer'] !== undefined ? config['Layer'] : ''
    layerName: config['LayerName'] !== undefined ? config['LayerName'] : ''
    layerSource: config['LayerSource'] !== undefined ? config['LayerSource'] : ''
    layerProviderName: config['LayerProviderName'] !== undefined ? config['LayerProviderName'] : ''
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
    appExpressionContextScopesGenerator: appScopesGenerator
    filterExpression: config['FilterExpression'] ? config['FilterExpression'] : ""

    // passing "" instead of undefined, so the model is cleared on adding new features
    // attributeValue has to be the last property set to make sure its given value is handled properly (e.g. allow multiple)
    attributeValue: value !== undefined ? value : ""

    searchTerm: allowMulti ? searchBar.searchTerm : ""
    sortCheckedFirst: allowMulti && !isEnabled

    onListUpdated: {
      valueChangeRequested(attributeValue, attributeValue === "");
    }
  }

  RelationCombobox {
    id: valueRelationCombobox
    featureListModel: config && !listModel.allowMulti ? listModel : null

    useCompleter: !!config['UseCompleter']
    enabled: isEnabled
    visible: !listModel.allowMulti
    relation: undefined
    layerResolver: layerResolver
    allowAddFeature: currentLayer && currentLayer.customProperty('QFieldSync/allow_value_relation_feature_addition') !== undefined ? currentLayer.customProperty('QFieldSync/allow_value_relation_feature_addition') : false
  }

  Column {
    id: valueRelationListComponent
    width: parent.width
    anchors.top: parent.top
    anchors.topMargin: 4
    visible: listModel.allowMulti
    spacing: 4

    QfSearchBar {
      id: searchBar
      objectName: "ValueRelationSearchBar"
      width: parent.width
      height: 40
      visible: enabled
      enabled: isEnabled

      onEnabledChanged: {
        if (!enabled) {
          clear();
        }
      }
    }

    Rectangle {
      id: valueRelationList

      property int itemHeight: 32

      width: parent.width
      height: Math.min(8 * itemHeight, valueGridView.implicitHeight + 2)

      color: Theme.mainBackgroundColor
      border.color: Theme.controlBorderColor
      border.width: 1

      Flickable {
        anchors.fill: parent
        anchors.margins: 1
        contentHeight: valueGridView.height
        contentWidth: parent.width - 2
        boundsBehavior: Flickable.StopAtBounds
        clip: true

        GridLayout {
          id: valueGridView
          objectName: "ValueRelationGridView"
          anchors.left: parent.left
          anchors.right: parent.right
          anchors.top: parent.top
          columns: config['NofColumns'] && !listModel.groupField === "" ? Math.min(config['NofColumns'], parent.width / 100) : 1
          columnSpacing: 1
          rowSpacing: 0

          Repeater {
            id: repeater
            model: listModel.allowMulti ? listModel : 0
            objectName: "ValueRelationRepeater"

            delegate: Item {
              id: listItem
              Layout.fillWidth: true
              Layout.fillHeight: true
              Layout.minimumHeight: Math.max(valueText.height, valueRelationList.itemHeight) + (header.visible ? header.height : 0)

              property var groupFieldVal: groupFieldValue ? groupFieldValue : ""
              property bool selected: model.checked
              property alias headerItem: header

              // Check if any item in the current row has a visible header item.
              // If found, mark the current listItem's row as containing a header.
              property bool rowContainsHeader: {
                const row = Math.floor(index / valueGridView.columns);
                const start = row * valueGridView.columns;
                const end = Math.min(start + valueGridView.columns, repeater.count);
                for (let i = start; i < end; ++i) {
                  const item = repeater.itemAt(i);
                  if (item && item.headerItem.visible) {
                    return true;
                  }
                }
                return false;
              }

              Rectangle {
                id: header

                property bool isVisible: listModel.groupField && groupFieldValue !== "" && (index === 0 || (index > 0 ? groupFieldValue !== repeater.itemAt(index - 1).groupFieldVal : false))
                property int visibleHeight: listModel.displayGroupName ? groupFieldValueText.height + 2 : 4

                visible: isVisible
                width: parent.width
                height: isVisible ? visibleHeight : 0

                color: Theme.controlBorderColor
                border.color: Theme.controlBorderColor
                border.width: 1
                clip: true

                Text {
                  id: groupFieldValueText
                  width: parent.width
                  text: groupFieldValue ? groupFieldValue : ""
                  font.bold: true
                  font.pointSize: Theme.resultFont.pointSize
                  color: Theme.mainTextColor
                  horizontalAlignment: Text.AlignHCenter
                  anchors.verticalCenter: parent.verticalCenter
                  wrapMode: Text.WordWrap
                  elide: Text.ElideRight
                  visible: listModel.displayGroupName
                }
              }

              Rectangle {
                id: checkBoxRow
                width: parent.width
                anchors.top: header.bottom
                anchors.topMargin: !header.visible && rowContainsHeader ? header.visibleHeight : 0
                anchors.bottom: parent.bottom
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
                  anchors.rightMargin: 4
                  width: parent.width - checkBox.width
                  topPadding: 4
                  bottomPadding: 4
                  font: Theme.defaultFont
                  color: !isEnabled ? Theme.mainTextDisabledColor : Theme.mainTextColor
                  text: model.displayString
                  wrapMode: Text.WordWrap
                  elide: Text.ElideRight
                }
              }

              MouseArea {
                anchors.fill: parent
                enabled: isEnabled

                onClicked: {
                  model.checked = !model.checked;
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
    }
  }
}
