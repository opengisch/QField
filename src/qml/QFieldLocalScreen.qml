import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.4
import QtQml.Models 2.2
import QtGraphicalEffects 1.12

import org.qfield 1.0
import Theme 1.0

Page {
  id: qfieldLocalScreen

  signal finished(var loading)

  header: PageHeader {
      title: qsTr("Local Projects & Datasets")

      showBackButton: true
      showApplyButton: false
      showCancelButton: false

      onFinished: parent.finished(false)
    }

    ColumnLayout {
      id: files
      anchors.fill: parent
      Layout.fillWidth: true
      Layout.fillHeight: true
      spacing: 2

      Rectangle {
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.margins: 10
        color: "white"
        border.color: "lightgray"
        border.width: 1

        ListView {
            id: table

            model: LocalFilesModel {}

            anchors.fill: parent
            anchors.margins: 1

            clip: true

            section.property: "ItemType"
            section.labelPositioning: ViewSection.CurrentLabelAtStart | ViewSection.InlineLabels
            section.delegate: Component {
              /* section header: layer name */
              Rectangle {
                width:parent.width
                height: 30
                color: Theme.lightestGray

                Text {
                  anchors { horizontalCenter: parent.horizontalCenter; verticalCenter: parent.verticalCenter }
                  font.bold: true
                  font.pointSize: Theme.resultFont.pointSize
                  text: { switch (parseInt(section)) {
                        case LocalFilesModel.Folder:
                            return qsTr('Folders');
                        case LocalFilesModel.Project:
                            return qsTr('Projects');
                        case LocalFilesModel.VectorDataset:
                            return qsTr('Vector Datasets');
                        case LocalFilesModel.RasterDataset:
                            return qsTr('Raster Datasets');
                        }
                      return '';
                  }
                }
              }
            }

            delegate: Rectangle {
                id: rectangle

                property int itemType: ItemType
                property string itemTitle: ItemTitle
                property string itemPath: ItemPath

                width: parent ? parent.width : undefined
                height: line.height
                color: "transparent"

                Row {
                    id: line
                    Layout.fillWidth: true
                    leftPadding: 6
                    rightPadding: 10
                    topPadding: 4
                    bottomPadding: 8
                    spacing: 0

                    Image {
                        id: type
                        anchors.verticalCenter: inner.verticalCenter
                        source: {
                            switch(ItemType) {
                              case LocalFilesModel.Folder:
                                  return Theme.getThemeIcon('ic_folder_gray_48dp');
                              case LocalFilesModel.Project:
                                  return Theme.getThemeIcon('ic_map_green_48dp');
                              case LocalFilesModel.VectorDataset:
                              case LocalFilesModel.RasterDataset:
                                  return Theme.getThemeIcon('ic_file_green_48dp');
                            }
                        }
                        sourceSize.width: 80
                        sourceSize.height: 80
                        width: 40
                        height: 40
                    }
                    ColumnLayout {
                        id: inner
                        width: rectangle.width - type.width - 10
                        Text {
                            id: itemTitle
                            topPadding: 5
                            leftPadding: 3
                            text: ItemTitle
                            font.pointSize: Theme.tipFont.pointSize
                            font.underline: true
                            color: Theme.mainColor
                            wrapMode: Text.WordWrap
                            Layout.fillWidth: true
                        }
                        Text {
                            id: itemInfo
                            leftPadding: 3
                            text: {
                                var info = '';
                                switch(ItemType) {
                                  case LocalFilesModel.Project:
                                      info = qsTr('Project file');
                                      break;
                                  case LocalFilesModel.VectorDataset:
                                      info = qsTr('Vector dataset') + ' (' + ItemFormat + ')';
                                      break;
                                  case LocalFilesModel.RasterDataset:
                                      info = qsTr('Raster dataset') + ' (' + ItemFormat + ')';
                                      break;
                                }
                                return info;
                            }

                            visible: text != ""
                            font.pointSize: Theme.tipFont.pointSize - 2
                            font.italic: true
                            wrapMode: Text.WordWrap
                            Layout.fillWidth: true
                        }
                    }
                }
            }

            MouseArea {
              property Item pressedItem
              anchors.fill: parent
              onClicked: {
                var item = table.itemAt(
                  table.contentX + mouse.x,
                  table.contentY + mouse.y
                )
                if (item) {
                  if (item.itemType === LocalFilesModel.Folder) {
                      table.model.currentPath = item.itemPath;
                  } else if (item.itemType === LocalFilesModel.Project
                             || item.itemType === LocalFilesModel.VectorDataset
                             || item.itemType === LocalFilesModel.RasterDataset) {
                      iface.loadFile(item.itemPath, item.itemTitle);
                      finished(true);
                  }
                }
              }
              onPressed: {
                var item = table.itemAt(
                  table.contentX + mouse.x,
                  table.contentY + mouse.y
                )
                if (item) {
                  pressedItem = item.children[0].children[1].children[0]
                  pressedItem.color = "#5a8725"
                }
              }
              onCanceled: {
                if (pressedItem) {
                  pressedItem.color = Theme.mainColor
                  pressedItem = null
                }
              }
              onReleased: {
                if (pressedItem) {
                  pressedItem.color = Theme.mainColor
                  pressedItem = null
                }
              }

              onPressAndHold: {
                  var item = table.itemAt(
                    table.contentX + mouse.x,
                    table.contentY + mouse.y
                  )
                  if (item) {
                    //itemActions.popup(mouse.x, mouse.y)
                  }
              }
            }
        }
      }

      Menu {
        id: itemActions

        title: 'Item Actions'
        width: {
            var result = 0;
            var padding = 0;
            for (var i = 0; i < count; ++i) {
                var item = itemAt(i);
                result = Math.max(item.contentItem.implicitWidth, result);
                padding = Math.max(item.padding, padding);
            }
            return Math.min( result + padding * 2,mainWindow.width - 20);
        }
      }
  }

  Keys.onReleased: {
    if (event.key === Qt.Key_Back || event.key === Qt.Key_Escape) {
      event.accepted = true
      if (table.model.currentDepth > 1) {
          table.model.moveUp();
      } else {
          finished(false);
      }
    }
  }
}
