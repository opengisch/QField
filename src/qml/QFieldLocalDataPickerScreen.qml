import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQml.Models
import org.qfield
import Theme

Page {
  id: qfieldLocalDataPickerScreen

  property bool projectFolderView: false
  property alias model: table.model

  signal finished(var loading)

  header: QfPageHeader {
    title: projectFolderView ? qsTr("Project Folder") : qsTr("Local Projects & Datasets")

    showBackButton: true
    showApplyButton: false
    showCancelButton: false

    topMargin: mainWindow.sceneTopMargin

    onBack: {
      if (table.model.currentDepth > 1) {
        table.model.moveUp();
      } else {
        parent.finished(false);
      }
    }
  }

  ColumnLayout {
    id: files
    anchors.fill: parent
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
          color: Theme.mainTextColor
          wrapMode: Text.NoWrap
          elide: Text.ElideMiddle
        }
        Text {
          Layout.fillWidth: true
          visible: text !== ''
          text: table.model.currentPath !== 'root' ? table.model.currentPath : ''
          font: Theme.tipFont
          color: Theme.mainTextColor
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
      Layout.bottomMargin: 10 + mainWindow.sceneBottomMargin
      color: Theme.controlBackgroundColor
      border.color: Theme.controlBorderColor
      border.width: 1

      ListView {
        id: table

        model: LocalFilesModel {
          id: localFilesModel
        }

        anchors.fill: parent
        anchors.margins: 1

        clip: true

        section.property: "ItemMetaType"
        section.labelPositioning: ViewSection.CurrentLabelAtStart | ViewSection.InlineLabels
        section.delegate: Component {
          Rectangle {
            width: parent.width
            height: 30
            color: Theme.controlBorderColor

            Text {
              anchors {
                horizontalCenter: parent.horizontalCenter
                verticalCenter: parent.verticalCenter
              }
              font.bold: true
              font.pointSize: Theme.resultFont.pointSize
              color: Theme.mainTextColor
              text: {
                switch (parseInt(section)) {
                case LocalFilesModel.Folder:
                  return qsTr('Folders');
                case LocalFilesModel.Project:
                  return qsTr('Projects');
                case LocalFilesModel.Dataset:
                  return qsTr('Datasets');
                case LocalFilesModel.File:
                  return qsTr('Files');
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
          property bool itemIsFavorite: ItemIsFavorite
          property bool itemMenuLoadable: !projectFolderView && (ItemMetaType === LocalFilesModel.Project || ItemMetaType === LocalFilesModel.Dataset)
          property bool itemMenuVisible: ((ItemType === LocalFilesModel.SimpleFolder || ItemMetaType == LocalFilesModel.File) && table.model.currentPath !== 'root') || ((platformUtilities.capabilities & PlatformUtilities.CustomExport || platformUtilities.capabilities & PlatformUtilities.CustomSend) && (ItemMetaType === LocalFilesModel.Dataset)) || (ItemMetaType === LocalFilesModel.Dataset && ItemType === LocalFilesModel.RasterDataset && cloudProjectsModel.currentProjectId)

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
                  switch (ItemType) {
                  case LocalFilesModel.ApplicationFolder:
                    return Theme.getThemeVectorIcon('ic_folder_qfield_gray_48dp');
                  case LocalFilesModel.ExternalStorage:
                    return Theme.getThemeVectorIcon('ic_sd_card_gray_48dp');
                  case LocalFilesModel.SimpleFolder:
                    return Theme.getThemeVectorIcon(ItemMetaType == LocalFilesModel.Folder && ItemIsFavorite ? 'ic_folder_favorite_gray_48dp' : 'ic_folder_gray_48dp');
                  case LocalFilesModel.ProjectFile:
                    return Theme.getThemeVectorIcon('ic_map_green_48dp');
                  case LocalFilesModel.VectorDataset:
                  case LocalFilesModel.RasterDataset:
                  case LocalFilesModel.OtherFile:
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
                color: itemMenuLoadable ? Theme.mainColor : Theme.mainTextColor
                wrapMode: Text.WordWrap
              }
              Text {
                id: itemInfo
                Layout.fillWidth: true
                Layout.preferredHeight: contentHeight

                text: {
                  var info = '';
                  switch (ItemType) {
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
                color: Theme.secondaryTextColor
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
              iconSource: Theme.getThemeVectorIcon("ic_dot_menu_black_24dp")
              iconColor: Theme.mainTextColor

              onClicked: {
                var gc = mapToItem(qfieldLocalDataPickerScreen, 0, 0);
                itemMenu.itemMetaType = ItemMetaType;
                itemMenu.itemType = ItemType;
                itemMenu.itemPath = ItemPath;
                itemMenu.itemIsFavorite = ItemIsFavorite;
                itemMenu.popup(gc.x + width - itemMenu.width, gc.y - height);
              }
            }
          }
        }

        MouseArea {
          property Item pressedItem
          anchors.fill: parent
          anchors.rightMargin: 48
          onClicked: mouse => {
            if (itemMenu.visible) {
              itemMenu.close();
            } else if (importMenu.visible) {
              importMenu.close();
            } else {
              var item = table.itemAt(table.contentX + mouse.x, table.contentY + mouse.y);
              if (item) {
                if (item.itemMetaType === LocalFilesModel.Folder || item.itemMetaType === LocalFilesModel.Favorite) {
                  table.model.currentPath = item.itemPath;
                } else if (!qfieldLocalDataPickerScreen.projectFolderView && (item.itemMetaType === LocalFilesModel.Project || item.itemMetaType === LocalFilesModel.Dataset)) {
                  iface.loadFile(item.itemPath, item.itemTitle);
                  finished(true);
                }
              }
            }
          }
          onPressed: mouse => {
            if (itemMenu.visible || importMenu.visible)
              return;
            var item = table.itemAt(table.contentX + mouse.x, table.contentY + mouse.y);
            if (item && item.itemMenuLoadable) {
              pressedItem = item.children[0].children[1].children[0];
              pressedItem.color = "#5a8725";
            }
          }
          onCanceled: {
            if (pressedItem) {
              pressedItem.color = Theme.mainColor;
              pressedItem = null;
            }
          }
          onReleased: {
            if (pressedItem) {
              pressedItem.color = Theme.mainColor;
              pressedItem = null;
            }
          }

          onPressAndHold: mouse => {
            var item = table.itemAt(table.contentX + mouse.x, table.contentY + mouse.y);
            if (item && item.itemMenuVisible) {
              itemMenu.itemMetaType = item.itemMetaType;
              itemMenu.itemType = item.itemType;
              itemMenu.itemPath = item.itemPath;
              itemMenu.itemIsFavorite = item.itemIsFavorite;
              itemMenu.popup(mouse.x, mouse.y);
            }
          }
        }
      }

      QfToolButton {
        id: actionButton
        round: true

        // Since the project menu only has one action for now, hide if PlatformUtilities.UpdateProjectFromArchive is missing
        property bool isLocalProject: qgisProject && QFieldCloudUtils.getProjectId(qgisProject.fileName) === '' && (projectInfo.filePath.endsWith('.qgs') || projectInfo.filePath.endsWith('.qgz')) && platformUtilities.capabilities & PlatformUtilities.UpdateProjectFromArchive
        visible: (projectFolderView && isLocalProject && table.model.currentDepth === 1) || table.model.currentPath === 'root'

        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.bottomMargin: 10
        anchors.rightMargin: 10

        bgcolor: Theme.mainColor
        iconSource: Theme.getThemeVectorIcon("ic_add_white_24dp")

        onClicked: {
          var xy = mapToItem(mainWindow.contentItem, actionButton.width, actionButton.height);
          if (projectFolderView) {
            projectMenu.popup(xy.x - projectMenu.width, xy.y - projectMenu.height - header.height);
          } else {
            importMenu.popup(xy.x - importMenu.width, xy.y - importMenu.height - header.height);
          }
        }
      }
    }

    Menu {
      id: itemMenu

      property int itemMetaType: 0
      property int itemType: 0
      property string itemPath: ''
      property bool itemIsFavorite: false

      title: qsTr('Item Actions')

      width: {
        let result = 50;
        let padding = 0;
        for (let i = 0; i < count; ++i) {
          let item = itemAt(i);
          result = Math.max(item.contentItem.implicitWidth, result);
          padding = Math.max(item.leftPadding + item.rightPadding, padding);
        }
        return mainWindow.width > 0 ? Math.min(result + padding * 2, mainWindow.width - 20) : result + padding;
      }

      topMargin: sceneTopMargin
      bottomMargin: sceneBottomMargin

      MenuItem {
        id: sendDatasetTo
        enabled: itemMenu.itemMetaType === LocalFilesModel.File || (platformUtilities.capabilities & PlatformUtilities.CustomSend && itemMenu.itemMetaType == LocalFilesModel.Dataset)
        visible: enabled

        font: Theme.defaultFont
        width: parent.width
        height: enabled ? undefined : 0
        leftPadding: Theme.menuItemLeftPadding

        text: qsTr("Send to...")
        onTriggered: {
          platformUtilities.sendDatasetTo(itemMenu.itemPath);
        }
      }

      MenuItem {
        id: pushDatasetToCloud
        enabled: itemMenu.itemMetaType == LocalFilesModel.Dataset && itemMenu.itemType == LocalFilesModel.RasterDataset && cloudProjectsModel.currentProjectId
        visible: enabled

        font: Theme.defaultFont
        width: parent.width
        height: enabled ? undefined : 0
        leftPadding: Theme.menuItemLeftPadding

        text: qsTr("Push to QFieldCloud...")
        onTriggered: {
          QFieldCloudUtils.addPendingAttachment(cloudProjectsModel.currentProjectId, itemMenu.itemPath);
          platformUtilities.uploadPendingAttachments(cloudConnection);
          displayToast(qsTr("‘%1’ is being uploaded to QFieldCloud").arg(FileUtils.fileName(itemMenu.itemPath)));
        }
      }

      MenuItem {
        id: exportDatasetTo
        enabled: platformUtilities.capabilities & PlatformUtilities.CustomExport && itemMenu.itemMetaType == LocalFilesModel.Dataset
        visible: enabled

        font: Theme.defaultFont
        width: parent.width
        height: enabled ? undefined : 0
        leftPadding: Theme.menuItemLeftPadding

        text: qsTr("Export to folder...")
        onTriggered: {
          platformUtilities.exportDatasetTo(itemMenu.itemPath);
        }
      }

      MenuItem {
        id: removeDataset
        enabled: itemMenu.itemMetaType == LocalFilesModel.Dataset && !qfieldLocalDataPickerScreen.projectFolderView && table.model.isDeletedAllowedInCurrentPath
        visible: enabled

        font: Theme.defaultFont
        width: parent.width
        height: enabled ? undefined : 0
        leftPadding: Theme.menuItemLeftPadding

        text: qsTr("Remove dataset")
        onTriggered: {
          platformUtilities.removeDataset(itemMenu.itemPath);
        }
      }

      MenuItem {
        id: exportFolderTo
        enabled: platformUtilities.capabilities & PlatformUtilities.CustomExport && itemMenu.itemMetaType == LocalFilesModel.Folder
        visible: enabled

        font: Theme.defaultFont
        width: parent.width
        height: enabled ? undefined : 0
        leftPadding: Theme.menuItemLeftPadding

        text: qsTr("Export to folder...")
        onTriggered: {
          platformUtilities.exportFolderTo(itemMenu.itemPath);
        }
      }

      MenuItem {
        id: toggleFavoriteState
        enabled: itemMenu.itemMetaType == LocalFilesModel.Folder && localFilesModel.isPathFavoriteEditable(itemMenu.itemPath)
        visible: enabled

        font: Theme.defaultFont
        width: parent.width
        height: enabled ? undefined : 0
        leftPadding: Theme.menuItemLeftPadding

        text: !itemMenu.itemIsFavorite ? qsTr("Add to favorites") : qsTr("Remove from favorites")
        onTriggered: {
          if (!itemMenu.itemIsFavorite) {
            localFilesModel.addToFavorites(itemMenu.itemPath);
          } else {
            localFilesModel.removeFromFavorites(itemMenu.itemPath);
          }
        }
      }

      MenuItem {
        id: sendCompressedFolderTo
        enabled: platformUtilities.capabilities & PlatformUtilities.CustomSend && itemMenu.itemMetaType == LocalFilesModel.Folder
        visible: enabled

        font: Theme.defaultFont
        width: parent.width
        height: enabled ? undefined : 0
        leftPadding: Theme.menuItemLeftPadding

        text: qsTr("Send compressed folder to...")
        onTriggered: {
          platformUtilities.sendCompressedFolderTo(itemMenu.itemPath);
        }
      }

      MenuItem {
        id: removeProjectFolder
        enabled: itemMenu.itemMetaType == LocalFilesModel.Folder && !qfieldLocalDataPickerScreen.projectFolderView && table.model.isDeletedAllowedInCurrentPath
        visible: enabled

        font: Theme.defaultFont
        width: parent.width
        height: enabled ? undefined : 0
        leftPadding: Theme.menuItemLeftPadding

        text: qsTr("Remove project folder")
        onTriggered: {
          platformUtilities.removeFolder(itemMenu.itemPath);
        }
      }
    }

    Menu {
      id: importMenu

      title: qsTr('Import Actions')

      width: {
        let result = 50;
        let padding = 0;
        for (let i = 0; i < count; ++i) {
          let item = itemAt(i);
          result = Math.max(item.contentItem.implicitWidth, result);
          padding = Math.max(item.leftPadding + item.rightPadding, padding);
        }
        return mainWindow.width > 0 ? Math.min(result + padding * 2, mainWindow.width - 20) : result + padding;
      }

      topMargin: sceneTopMargin
      bottomMargin: sceneBottomMargin

      MenuItem {
        id: importProjectFromFolder

        enabled: platformUtilities.capabilities & PlatformUtilities.CustomImport
        visible: enabled
        font: Theme.defaultFont
        width: parent.width
        height: enabled ? undefined : 0
        leftPadding: Theme.menuItemLeftPadding

        text: qsTr("Import project from folder")
        onTriggered: {
          platformUtilities.importProjectFolder();
        }
      }

      MenuItem {
        id: importProjectFromZIP

        enabled: platformUtilities.capabilities & PlatformUtilities.CustomImport
        visible: enabled
        font: Theme.defaultFont
        width: parent.width
        height: enabled ? undefined : 0
        leftPadding: Theme.menuItemLeftPadding

        text: qsTr("Import project from ZIP")
        onTriggered: {
          platformUtilities.importProjectArchive();
        }
      }

      MenuItem {
        id: importDataset

        enabled: platformUtilities.capabilities & PlatformUtilities.CustomImport
        visible: enabled
        font: Theme.defaultFont
        width: parent.width
        height: enabled ? undefined : 0
        leftPadding: Theme.menuItemLeftPadding

        text: qsTr("Import dataset(s)")
        onTriggered: {
          platformUtilities.importDatasets();
        }
      }

      MenuSeparator {
        enabled: platformUtilities.capabilities & PlatformUtilities.CustomImport
        visible: enabled
        width: parent.width
        height: enabled ? undefined : 0
      }

      MenuItem {
        id: importUrl

        font: Theme.defaultFont
        width: parent.width
        leftPadding: Theme.menuItemLeftPadding

        text: qsTr("Import URL")
        onTriggered: {
          importUrlDialog.open();
          importUrlInput.focus = true;
        }
      }

      MenuSeparator {
        width: parent.width
      }

      MenuItem {
        id: storageHelp

        font: Theme.defaultFont
        width: parent.width
        leftPadding: Theme.menuItemLeftPadding

        text: qsTr("Storage management help")
        onTriggered: {
          Qt.openUrlExternally("https://docs.qfield.org/get-started/storage/");
        }
      }
    }

    Menu {
      id: projectMenu

      title: qsTr('Project Actions')

      width: {
        let result = 50;
        let padding = 0;
        for (let i = 0; i < count; ++i) {
          let item = itemAt(i);
          result = Math.max(item.contentItem.implicitWidth, result);
          padding = Math.max(item.leftPadding + item.rightPadding, padding);
        }
        return mainWindow.width > 0 ? Math.min(result + padding * 2, mainWindow.width - 20) : result + padding;
      }

      topMargin: sceneTopMargin
      bottomMargin: sceneBottomMargin

      MenuItem {
        id: updateProjectFromArchive

        enabled: platformUtilities.capabilities & PlatformUtilities.UpdateProjectFromArchive
        visible: enabled
        font: Theme.defaultFont
        width: parent.width
        height: enabled ? undefined : 0
        leftPadding: Theme.menuItemLeftPadding

        text: qsTr("Update project from ZIP")
        onTriggered: {
          platformUtilities.updateProjectFromArchive(projectInfo.filePath);
        }
      }
    }
  }

  Dialog {
    id: importUrlDialog
    title: "Import URL"
    focus: true
    font: Theme.defaultFont

    x: (mainWindow.width - width) / 2
    y: (mainWindow.height - height - 80) / 2

    onAboutToShow: {
      importUrlInput.text = '';
    }

    Column {
      width: childrenRect.width
      height: childrenRect.height
      spacing: 10

      TextMetrics {
        id: importUrlLabelMetrics
        font: importUrlLabel.font
        text: importUrlLabel.text
      }

      Label {
        id: importUrlLabel
        width: mainWindow.width - 60 < importUrlLabelMetrics.width ? mainWindow.width - 60 : importUrlLabelMetrics.width
        text: qsTr("Type a URL below to download and import the project or dataset:")
        wrapMode: Text.WordWrap
        font: Theme.defaultFont
        color: Theme.mainTextColor
      }

      QfTextField {
        id: importUrlInput
        width: importUrlLabel.width
      }
    }

    standardButtons: Dialog.Ok | Dialog.Cancel
    onAccepted: {
      iface.importUrl(importUrlInput.text);
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

  Keys.onReleased: event => {
    if (event.key === Qt.Key_Back || event.key === Qt.Key_Escape) {
      event.accepted = true;
      if (table.model.currentDepth > 1) {
        table.model.moveUp();
      } else {
        finished(false);
      }
    }
  }

  onVisibleChanged: {
    focus = visible;
  }
}
