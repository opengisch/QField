import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import QtQml.Models 2.14

import org.qfield 1.0
import Theme 1.0

Page {
  id: qfieldLocalDataPickerScreen

  property bool projectFolderView: false
  property alias model: table.model

  signal finished(var loading)

  header: PageHeader {
    title: projectFolderView
           ? qsTr("Project Folder")
           : qsTr("Local Projects & Datasets")

    showBackButton: true
    showApplyButton: false
    showCancelButton: false

    topMargin: mainWindow.sceneTopMargin

    onBack: {
      if (table.model.currentDepth > 1) {
        table.model.moveUp();
      } else {
        parent.finished(false)
      }
    }
  }

  ColumnLayout {
    id: files
    anchors.fill: parent
    Layout.fillWidth: true
    Layout.fillHeight: true
    spacing: 2

    RowLayout {
      Layout.margins: 10
      spacing: 2

      ColumnLayout {
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignVCenter
        spacing: 2

        Text {
          Layout.fillWidth: true
          text: table.model.currentTitle
          font.pointSize: Theme.defaultFont.pointSize
          font.bold: true
          wrapMode: Text.NoWrap
          elide: Text.ElideMiddle
        }
        Text {
          Layout.fillWidth: true
          visible: text !== ''
          text: table.model.currentPath !== 'root'
                ? table.model.currentPath
                : ''
          font: Theme.tipFont
          wrapMode: Text.NoWrap
          elide: Text.ElideMiddle
          opacity: 0.35
        }
      }
    }

    Rectangle {
      Layout.fillWidth: true
      Layout.fillHeight: true
      Layout.margins: 10
      Layout.topMargin: 0
      color: "white"
      border.color: "lightgray"
      border.width: 1

      ListView {
        id: table

        model: LocalFilesModel {}

        anchors.fill: parent
        anchors.margins: 1

        clip: true

        section.property: "ItemMetaType"
        section.labelPositioning: ViewSection.CurrentLabelAtStart | ViewSection.InlineLabels
        section.delegate: Component {
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
                case LocalFilesModel.Dataset:
                  return qsTr('Datasets');
                case LocalFilesModel.Favorite:
                  return qsTr('Favorites');
                }
                return '';
              }
            }
          }
        }

        delegate: Rectangle {
          id: rectangle

          property int itemMetaType: ItemMetaType
          property int itemType: ItemType
          property string itemTitle: ItemTitle
          property string itemPath: ItemPath
          property bool itemMenuLoadable: !projectFolderView &&
                                          (ItemMetaType === LocalFilesModel.Project || ItemMetaType === LocalFilesModel.Dataset)
          property bool itemMenuVisible: (platformUtilities.capabilities & PlatformUtilities.CustomExport ||
                                          platformUtilities.capabilities & PlatformUtilities.CustomSend) &&
                                         (ItemMetaType === LocalFilesModel.Dataset
                                          || (ItemType === LocalFilesModel.SimpleFolder && table.model.currentPath !== 'root'))

          width: parent ? parent.width : undefined
          height: line.height
          color: "transparent"

          RowLayout {
            id: line
            width: parent.width
            anchors.verticalCenter: parent.verticalCenter
            spacing: 2

            Image {
              id: type
              Layout.alignment: Qt.AlignVCenter
              Layout.topMargin: 5
              Layout.bottomMargin: 5
              Layout.leftMargin: 4
              Layout.preferredWidth: 48
              Layout.preferredHeight: 48
              asynchronous: true
              source: {
                if (ItemHasThumbnail) {
                  return "image://localfiles/" + ItemPath;
                } else {
                  switch(ItemType) {
                  case LocalFilesModel.ApplicationFolder:
                    return Theme.getThemeVectorIcon('ic_folder_qfield_gray_48dp');
                  case LocalFilesModel.ExternalStorage:
                    return Theme.getThemeVectorIcon('ic_sd_card_gray_48dp');
                  case LocalFilesModel.SimpleFolder:
                    return Theme.getThemeVectorIcon('ic_folder_gray_48dp');
                  case LocalFilesModel.ProjectFile:
                    return Theme.getThemeVectorIcon('ic_map_green_48dp');
                  case LocalFilesModel.VectorDataset:
                  case LocalFilesModel.RasterDataset:
                    return Theme.getThemeVectorIcon('ic_file_green_48dp');
                  }
                }
              }
              sourceSize.width: 92
              sourceSize.height: 92
              fillMode: Image.PreserveAspectFit
              width: 48
              height: 48
            }
            ColumnLayout {
              id: inner
              Layout.alignment: Qt.AlignVCenter
              Layout.fillWidth: true
              Layout.preferredHeight: childrenRect.height
              Layout.topMargin: 5
              Layout.bottomMargin: 5
              Layout.leftMargin: 2
              Layout.rightMargin: 4
              spacing: 1
              Text {
                id: itemTitle
                Layout.fillWidth: true
                Layout.preferredHeight: contentHeight
                clip: true

                text: ItemTitle + (ItemType !== LocalFilesModel.ProjectFile && ItemFormat !== '' ? '.' + ItemFormat : '')

                font.pointSize: Theme.defaultFont.pointSize
                font.underline: itemMenuLoadable
                color: itemMenuLoadable ? Theme.mainColor : "black"
                wrapMode: Text.WordWrap
              }
              Text {
                id: itemInfo
                Layout.fillWidth: true
                Layout.preferredHeight: contentHeight

                text: {
                  var info = '';
                  switch(ItemType) {
                  case LocalFilesModel.ProjectFile:
                    info = qsTr('Project file');
                    break;
                  case LocalFilesModel.VectorDataset:
                    info = qsTr('Vector dataset') + ' (' + FileUtils.representFileSize(ItemSize) + ')';
                    break;
                  case LocalFilesModel.RasterDataset:
                    info = qsTr('Raster dataset') + ' (' + FileUtils.representFileSize(ItemSize) + ')';
                    break;
                  }
                  return info;
                }

                visible: text != ""
                font.pointSize: Theme.tipFont.pointSize - 2
                font.italic: true
                wrapMode: Text.WordWrap
                opacity: 0.35
              }
            }
            QfToolButton {
              visible: itemMenuVisible
              round: true
              opacity: 0.5

              Layout.topMargin: 5
              Layout.bottomMargin: 5

              bgcolor: "transparent"
              iconSource: Theme.getThemeIcon( "ic_dot_menu_gray_24dp" )

              onClicked: {
                var gc = mapToItem(qfieldLocalDataPickerScreen, 0, 0)

                itemMenu.itemMetaType = ItemMetaType
                itemMenu.itemType = ItemType
                itemMenu.itemPath = ItemPath
                itemMenu.popup(gc.x + width - itemMenu.width,
                               gc.y - height)
              }
            }
          }
        }

        MouseArea {
          property Item pressedItem
          anchors.fill: parent
          anchors.rightMargin: 48
          onClicked: {
            if (itemMenu.visible) {
              itemMenu.close();
            } else if (importMenu.visible) {
              importMenu.close();
            } else {
              var item = table.itemAt(
                    table.contentX + mouse.x,
                    table.contentY + mouse.y
                    )
              if (item) {
                if (item.itemMetaType === LocalFilesModel.Folder ||
                    item.itemMetaType === LocalFilesModel.Favorite) {
                  table.model.currentPath = item.itemPath;
                } else if (!qfieldLocalDataPickerScreen.projectFolderView &&
                           (item.itemMetaType === LocalFilesModel.Project
                           || item.itemMetaType === LocalFilesModel.Dataset)) {
                  iface.loadFile(item.itemPath, item.itemTitle);
                  finished(true);
                }
              }
            }
          }
          onPressed: {
            if (itemMenu.visible || importMenu.visible)
              return;

            var item = table.itemAt(
                  table.contentX + mouse.x,
                  table.contentY + mouse.y
                  )
            if (item && item.itemMenuLoadable) {
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
            if (item && item.itemMenuVisible) {
              itemMenu.itemMetaType = item.itemMetaType
              itemMenu.itemType = item.itemType
              itemMenu.itemPath = item.itemPath
              itemMenu.popup(mouse.x, mouse.y)
            }
          }
        }
      }

      QfToolButton {
        id: importButton
        round: true
        visible: platformUtilities.capabilities & PlatformUtilities.CustomImport
                 && table.model.currentPath === 'root'

        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.bottomMargin: 10
        anchors.rightMargin: 10

        bgcolor: Theme.mainColor
        iconSource: Theme.getThemeIcon( "ic_add_white_24dp" )

        onClicked: {
          importMenu.popup(mainWindow.width - importMenu.width - 10,
                           mainWindow.height - importMenu.height - 58)
        }
      }
    }

    Menu {
      id: itemMenu

      property int itemMetaType: 0
      property int itemType: 0
      property string itemPath: ''

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

      MenuItem {
        id: sendDatasetTo
        visible: platformUtilities.capabilities & PlatformUtilities.CustomSend
                 && itemMenu.itemMetaType == LocalFilesModel.Dataset

        font: Theme.defaultFont
        width: parent.width
        height: visible ? 48 : 0
        leftPadding: 10

        text: qsTr( "Send to..." )
        onTriggered: { platformUtilities.sendDatasetTo(itemMenu.itemPath); }
      }

      MenuItem {
        id: exportDatasetTo
        visible: platformUtilities.capabilities & PlatformUtilities.CustomExport
                 && itemMenu.itemMetaType == LocalFilesModel.Dataset

        font: Theme.defaultFont
        width: parent.width
        height: visible ? 48 : 0
        leftPadding: 10

        text: qsTr( "Export to folder..." )
        onTriggered: { platformUtilities.exportDatasetTo(itemMenu.itemPath); }
      }

      MenuItem {
        id: removeDataset
        visible: itemMenu.itemMetaType == LocalFilesModel.Dataset
                 && !qfieldLocalDataPickerScreen.projectFolderView
                 && table.model.isDeletedAllowedInCurrentPath

        font: Theme.defaultFont
        width: parent.width
        height: visible ? 48 : 0
        leftPadding: 10

        text: qsTr( "Remove dataset" )
        onTriggered: { platformUtilities.removeDataset(itemMenu.itemPath); }
      }

      MenuItem {
        id: exportFolderTo
        visible: platformUtilities.capabilities & PlatformUtilities.CustomExport &&
                 itemMenu.itemMetaType == LocalFilesModel.Folder

        font: Theme.defaultFont
        width: parent.width
        height: visible ? 48 : 0
        leftPadding: 10

        text: qsTr( "Export to folder..." )
        onTriggered: { platformUtilities.exportFolderTo(itemMenu.itemPath); }
      }

      MenuItem {
        id: sendCompressedFolderTo
        visible: platformUtilities.capabilities & PlatformUtilities.CustomSend
                 && itemMenu.itemMetaType == LocalFilesModel.Folder

        font: Theme.defaultFont
        width: parent.width
        height: visible ? 48 : 0
        leftPadding: 10

        text: qsTr( "Send compressed folder to..." )
        onTriggered: { platformUtilities.sendCompressedFolderTo(itemMenu.itemPath); }
      }

      MenuItem {
        id: removeProjectFolder
        visible: itemMenu.itemMetaType == LocalFilesModel.Folder
                 && !qfieldLocalDataPickerScreen.projectFolderView
                 && table.model.isDeletedAllowedInCurrentPath

        font: Theme.defaultFont
        width: parent.width
        height: visible ? 48 : 0
        leftPadding: 10

        text: qsTr( "Remove project folder" )
        onTriggered: { platformUtilities.removeFolder(itemMenu.itemPath); }
      }
    }

    Menu {
      id: importMenu

      title: 'Import Actions'
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

      MenuItem {
        id: importProjectFromFolder

        font: Theme.defaultFont
        width: parent.width
        height: visible ? 48 : 0
        leftPadding: 10

        text: qsTr( "Import project from folder" )
        onTriggered: { platformUtilities.importProjectFolder(); }
      }

      MenuItem {
        id: importProjectFromZIP

        font: Theme.defaultFont
        width: parent.width
        height: visible ? 48 : 0
        leftPadding: 10

        text: qsTr( "Import project from ZIP" )
        onTriggered: { platformUtilities.importProjectArchive(); }
      }

      MenuItem {
        id: importDataset

        font: Theme.defaultFont
        width: parent.width
        height: visible ? 48 : 0
        leftPadding: 10

        text: qsTr( "Import dataset(s)" )
        onTriggered: { platformUtilities.importDatasets(); }
      }

      MenuSeparator { width: parent.width }

      MenuItem {
        id: storageHelp

        font: Theme.defaultFont
        width: parent.width
        height: visible ? 48 : 0
        leftPadding: 10

        text: qsTr( "Storage management help" )
        onTriggered: { Qt.openUrlExternally("https://docs.qfield.org/get-started/storage/") }
      }
    }
  }

  Connections {
    target: iface

    function onOpenPath(path) {
      if (visible) {
        table.model.currentPath = path;
      }
    }
  }

  Keys.onReleased: (event) => {
    if (event.key === Qt.Key_Back || event.key === Qt.Key_Escape) {
      event.accepted = true
      if (table.model.currentDepth > 1) {
        table.model.moveUp();
      } else {
        finished(false);
      }
    }
  }

  onVisibleChanged: {
    focus = visible
  }
}
