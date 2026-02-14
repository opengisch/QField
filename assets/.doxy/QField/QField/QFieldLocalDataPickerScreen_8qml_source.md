

# File QFieldLocalDataPickerScreen.qml

[**File List**](files.md) **>** [**qml**](dir_1d81f7f4362e4398c0a695c4e466fcb0.md) **>** [**QFieldLocalDataPickerScreen.qml**](QFieldLocalDataPickerScreen_8qml.md)

[Go to the documentation of this file](QFieldLocalDataPickerScreen_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material.impl
import QtQuick.Layouts
import QtQml.Models
import org.qfield
import Theme

Page {
  id: qfieldLocalDataPickerScreen

  property bool openedOnce: false
  property bool projectFolderView: false
  property alias model: table.model

  signal finished(var loading)

  focus: visible
  leftPadding: mainWindow.sceneLeftMargin
  rightPadding: mainWindow.sceneRightMargin

  onVisibleChanged: {
    if (visible) {
      openedOnce = true;
    }
  }

  header: QfPageHeader {
    title: projectFolderView ? qsTr("Project Folder") : qsTr("Local Projects & Datasets")

    showBackButton: true
    showApplyButton: false
    showCancelButton: false
    showMenuButton: localFilesModel.inSelectionMode && (table.selectedItemsPushableToQField || table.selectedItemsWebDavConfigured || table.selectedItemsDeletable)
    backAsCancel: localFilesModel.inSelectionMode

    topMargin: mainWindow.sceneTopMargin

    onCancel: {
      localFilesModel.clearSelection();
    }

    onBack: {
      if (table.model.currentDepth > 1) {
        table.model.moveUp();
      } else {
        parent.finished(false);
      }
    }

    onOpenMenu: {
      selectionMenu.open();
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

          onInSelectionModeChanged: {
            if (!inSelectionMode) {
              table.selectedList = [];
            }
          }
        }

        anchors.fill: parent
        anchors.margins: 1
        spacing: -1

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

        property var selectedList: []
        property bool selectedItemsWebDavConfigured
        property bool selectedItemsPushableToQField
        property bool selectedItemsDeletable

        delegate: Rectangle {
          id: rectangle

          property int itemIndex: index
          property int itemMetaType: ItemMetaType
          property int itemType: ItemType
          property string itemTitle: ItemTitle
          property string itemPath: ItemPath
          property bool itemIsFavorite: ItemIsFavorite
          property bool itemChecked: ItemChecked
          property bool itemWithinQFieldCloudProjectFolder: cloudProjectsModel.currentProjectId !== "" && itemPath.search(cloudProjectsModel.currentProjectId) !== -1
          property bool itemHasWebdavConfiguration: ItemHasWebdavConfiguration
          property bool itemMenuLoadable: !projectFolderView && (ItemMetaType === LocalFilesModel.Project || ItemMetaType === LocalFilesModel.Dataset)
          property bool itemMenuVisible: ((ItemType === LocalFilesModel.SimpleFolder || ItemMetaType == LocalFilesModel.Dataset || ItemMetaType == LocalFilesModel.File) && table.model.currentPath !== 'root') || ((platformUtilities.capabilities & PlatformUtilities.CustomExport || platformUtilities.capabilities & PlatformUtilities.CustomSend) && (ItemMetaType === LocalFilesModel.Dataset)) || (ItemMetaType === LocalFilesModel.Dataset && ItemType === LocalFilesModel.RasterDataset && cloudProjectsModel.currentProjectId)

          width: parent ? parent.width : undefined
          height: line.height
          color: Theme.controlBackgroundColor
          border.color: Theme.controlBorderColor
          border.width: 1
          radius: 2

          RowLayout {
            id: line
            width: parent.width
            anchors.verticalCenter: parent.verticalCenter
            spacing: 2

            CheckBox {
              checked: itemChecked
              visible: localFilesModel.inSelectionMode
            }

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
                color: Theme.mainTextColor
                wrapMode: Text.Wrap
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
                color: Theme.secondaryTextColor
                wrapMode: Text.WordWrap
              }
            }
            QfToolButton {
              visible: itemMenuVisible && !localFilesModel.inSelectionMode
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
                itemMenu.itemWithinQFieldCloudProjectFolder = rectangle.itemWithinQFieldCloudProjectFolder;
                itemMenu.itemHasWebdavConfiguration = ItemHasWebdavConfiguration;
                itemMenu.popup(gc.x + width - itemMenu.width, gc.y - height);
              }
            }
          }
        }

        function updateSelection(item) {
          const newCheckedState = !item.itemChecked;
          table.model.setChecked(item.itemIndex, newCheckedState);
          const itemIndexInList = table.selectedList.findIndex(q => q === item.itemIndex);
          if (newCheckedState && itemIndexInList === -1) {
            table.selectedList.push(item.itemIndex);
          } else if (!newCheckedState && itemIndexInList !== -1) {
            table.selectedList.splice(itemIndexInList, 1);
          }
          if (table.selectedList.length === 0) {
            table.selectedItemsWebDavConfigured = false;
            table.selectedItemsPushableToQField = false;
            table.selectedItemsDeletable = false;
            return;
          }
          table.selectedItemsWebDavConfigured = true;
          table.selectedItemsPushableToQField = true;
          table.selectedItemsDeletable = true;
          for (let i = 0; i < table.selectedList.length; ++i) {
            const selectedItem = table.model.get(table.selectedList[i]);
            table.selectedItemsWebDavConfigured = table.selectedItemsWebDavConfigured && webdavConnectionLoader.item.hasWebdavConfiguration(selectedItem.path);
            const itemWithinQFieldCloudProjectFolder = cloudProjectsModel.currentProjectId !== "" && selectedItem.path.search(cloudProjectsModel.currentProjectId) !== -1;
            table.selectedItemsPushableToQField = (table.selectedItemsPushableToQField && selectedItem.metaType == LocalFilesModel.Dataset && selectedItem.type == LocalFilesModel.RasterDataset && cloudProjectsModel.currentProjectId) || (selectedItem.metaType == LocalFilesModel.Folder && itemWithinQFieldCloudProjectFolder);
            table.selectedItemsDeletable = table.selectedItemsDeletable && FileUtils.isDeletable(selectedItem.path);
          }
        }

        MouseArea {
          property Item pressedItem
          anchors.fill: parent
          anchors.rightMargin: 48
          onClicked: mouse => {
            const item = table.itemAt(table.contentX + mouse.x, table.contentY + mouse.y);
            if (item && localFilesModel.inSelectionMode) {
              table.updateSelection(item);
              return;
            }
            if (itemMenu.visible) {
              itemMenu.close();
            } else if (importMenu.visible) {
              importMenu.close();
            } else {
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
            const item = table.itemAt(table.contentX + mouse.x, table.contentY + mouse.y);
            if (item && item.itemMenuLoadable) {
              pressedItem = item.children[0].children[2].children[0];
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
            if (localFilesModel.currentTitle === qsTr("Home")) {
              return;
            }
            const item = table.itemAt(table.contentX + mouse.x, table.contentY + mouse.y);
            if (item)
              table.updateSelection(item);
          }
        }
      }

      Connections {
        target: nativeLocalDataPickerButton.__projectSource

        function onProjectOpened(path) {
          finished(true);
          iface.loadFile(path);
        }
      }

      QfToolButton {
        id: nativeLocalDataPickerButton
        round: true

        property ProjectSource __projectSource

        visible: platformUtilities.capabilities & PlatformUtilities.NativeLocalDataPicker && table.model.currentPath === 'root'

        anchors.bottom: actionButton.top
        anchors.right: parent.right
        anchors.bottomMargin: 4
        anchors.rightMargin: 10

        bgcolor: Theme.mainColor
        iconSource: Theme.getThemeVectorIcon("ic_open_black_24dp")
        iconColor: Theme.toolButtonColor

        onClicked: {
          __projectSource = platformUtilities.openProject(this);
        }
      }

      QfToolButton {
        id: actionButton
        round: true

        // Since the project menu only has one action for now, hide if PlatformUtilities.UpdateProjectFromArchive is missing
        property bool isLocalProject: qgisProject && QFieldCloudUtils.getProjectId(qgisProject.fileName) === '' && (projectInfo.filePath.endsWith('.qgs') || projectInfo.filePath.endsWith('.qgz'))
        property bool isLocalProjectActionAvailable: updateProjectFromArchive.enabled || uploadProjectToWebdav.enabled
        visible: (projectFolderView && isLocalProject && table.model.currentDepth === 1) || table.model.currentPath === 'root'

        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.bottomMargin: 10
        anchors.rightMargin: 10

        bgcolor: Theme.mainColor
        iconSource: Theme.getThemeVectorIcon("ic_add_white_24dp")
        iconColor: Theme.toolButtonColor

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

    QfMenu {
      id: itemMenu

      property int itemMetaType: 0
      property int itemType: 0
      property string itemPath: ''
      property bool itemIsFavorite: false
      property bool itemHasWebdavConfiguration: false
      property bool itemWithinQFieldCloudProjectFolder: false

      title: qsTr('Item Actions')

      topMargin: sceneTopMargin
      bottomMargin: sceneBottomMargin
      paddingMultiplier: 2

      MenuItem {
        id: viewFile

        enabled: itemMenu.itemMetaType != LocalFilesModel.Folder
        visible: enabled

        font: Theme.defaultFont
        width: parent.width
        height: enabled ? 48 : 0
        leftPadding: Theme.menuItemLeftPadding

        text: qsTr("View file")
        onTriggered: {
          platformUtilities.open(itemMenu.itemPath);
        }
      }

      // File items
      MenuItem {
        id: sendDatasetTo
        enabled: itemMenu.itemMetaType === LocalFilesModel.File || (platformUtilities.capabilities & PlatformUtilities.CustomSend && itemMenu.itemMetaType == LocalFilesModel.Dataset)
        visible: enabled

        font: Theme.defaultFont
        width: parent.width
        height: enabled ? 48 : 0
        leftPadding: Theme.menuItemLeftPadding

        text: qsTr("Send to...")
        onTriggered: {
          platformUtilities.sendDatasetTo(itemMenu.itemPath);
        }
      }

      MenuItem {
        id: pushDatasetToCloud
        enabled: (itemMenu.itemMetaType == LocalFilesModel.File) || (itemMenu.itemMetaType == LocalFilesModel.Dataset && itemMenu.itemType == LocalFilesModel.RasterDataset && cloudProjectsModel.currentProjectId) || (itemMenu.itemMetaType == LocalFilesModel.Folder && itemMenu.itemWithinQFieldCloudProjectFolder)
        visible: enabled

        font: Theme.defaultFont
        width: parent.width
        height: enabled ? 48 : 0
        leftPadding: Theme.menuItemLeftPadding

        text: qsTr("Push to QFieldCloud")
        onTriggered: {
          pushFilesToQFieldCloudConnection.enabled = true;
          pushFilesToQFieldCloudConnection.sendingMultiple = true;
          QFieldCloudUtils.addPendingAttachments(cloudConnection.userInformation.username, QFieldCloudUtils.getProjectId(table.model.currentPath), [itemMenu.itemPath], cloudConnection, true);
        }
      }

      MenuItem {
        id: exportDatasetTo
        enabled: platformUtilities.capabilities & PlatformUtilities.CustomExport && itemMenu.itemMetaType == LocalFilesModel.Dataset
        visible: enabled

        font: Theme.defaultFont
        width: parent.width
        height: enabled ? 48 : 0
        leftPadding: Theme.menuItemLeftPadding

        text: qsTr("Export to folder...")
        onTriggered: {
          platformUtilities.exportDatasetTo(itemMenu.itemPath);
        }
      }

      // Folder items
      MenuItem {
        id: toggleFavoriteState
        enabled: itemMenu.itemMetaType == LocalFilesModel.Folder && localFilesModel.isPathFavoriteEditable(itemMenu.itemPath)
        visible: enabled

        font: Theme.defaultFont
        width: parent.width
        height: enabled ? 48 : 0
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

      MenuSeparator {
        enabled: toggleFavoriteState.visible && (exportFolderTo.visible || sendCompressedFolderTo.visible || uploadFolderToWebdav.visible || downloadFolderFromWebdav.visible)
        visible: enabled
        width: parent.width
        height: enabled ? undefined : 0
      }

      MenuItem {
        id: exportFolderTo
        enabled: platformUtilities.capabilities & PlatformUtilities.CustomExport && itemMenu.itemMetaType == LocalFilesModel.Folder
        visible: enabled

        font: Theme.defaultFont
        width: parent.width
        height: enabled ? 48 : 0
        leftPadding: Theme.menuItemLeftPadding

        text: qsTr("Export to folder...")
        onTriggered: {
          platformUtilities.exportFolderTo(itemMenu.itemPath);
        }
      }

      MenuItem {
        id: sendCompressedFolderTo
        enabled: platformUtilities.capabilities & PlatformUtilities.CustomSend && itemMenu.itemMetaType == LocalFilesModel.Folder
        visible: enabled

        font: Theme.defaultFont
        width: parent.width
        height: enabled ? 48 : 0
        leftPadding: Theme.menuItemLeftPadding

        text: qsTr("Send compressed folder to...")
        onTriggered: {
          platformUtilities.sendCompressedFolderTo(itemMenu.itemPath);
        }
      }

      MenuItem {
        id: uploadFolderToWebdav
        enabled: itemMenu.itemHasWebdavConfiguration
        visible: enabled

        font: Theme.defaultFont
        width: parent.width
        height: enabled ? 48 : 0
        leftPadding: Theme.menuItemLeftPadding

        text: qsTr("Upload folder to WebDAV server")
        onTriggered: {
          if (webdavConnectionLoader.item) {
            webdavConnectionLoader.item.uploadPaths([itemMenu.itemPath]);
          }
        }
      }

      MenuItem {
        id: downloadFolderFromWebdav
        enabled: itemMenu.itemHasWebdavConfiguration
        visible: enabled

        font: Theme.defaultFont
        width: parent.width
        height: enabled ? 48 : 0
        leftPadding: Theme.menuItemLeftPadding

        text: qsTr("Download folder from WebDAV server")
        onTriggered: {
          if (webdavConnectionLoader.item) {
            webdavConnectionLoader.item.downloadPath(itemMenu.itemPath);
          }
        }
      }

      MenuSeparator {
        enabled: deleteFile.visible || removeDataset.visible || removeProjectFolder.visible
        visible: enabled
        width: parent.width
        height: enabled ? undefined : 0
      }

      MenuItem {
        id: deleteFile
        enabled: FileUtils.isDeletable(itemMenu.itemPath)
        visible: enabled

        font: Theme.defaultFont
        width: parent.width
        height: enabled ? 48 : 0
        leftPadding: Theme.menuItemLeftPadding

        text: qsTr("Delete file")
        onTriggered: {
          confirmRemoveDialog.itemsToRemove = [itemMenu.itemPath];
          confirmRemoveDialog.open();
        }
      }

      MenuItem {
        id: removeDataset
        enabled: itemMenu.itemMetaType == LocalFilesModel.Dataset && !qfieldLocalDataPickerScreen.projectFolderView && table.model.isDeletedAllowedInCurrentPath
        visible: enabled

        font: Theme.defaultFont
        width: parent.width
        height: enabled ? 48 : 0
        leftPadding: Theme.menuItemLeftPadding

        text: qsTr("Remove dataset")
        onTriggered: {
          platformUtilities.removeDataset(itemMenu.itemPath);
          table.model.resetToPath(table.model.currentPath);
        }
      }

      MenuItem {
        id: removeProjectFolder
        enabled: itemMenu.itemMetaType == LocalFilesModel.Folder && !qfieldLocalDataPickerScreen.projectFolderView && table.model.isDeletedAllowedInCurrentPath
        visible: enabled

        font: Theme.defaultFont
        width: parent.width
        height: enabled ? 48 : 0
        leftPadding: Theme.menuItemLeftPadding

        text: qsTr("Remove folder")
        onTriggered: {
          platformUtilities.removeFolder(itemMenu.itemPath);
          table.model.resetToPath(table.model.currentPath);
        }
      }
    }

    QfMenu {
      id: importMenu

      title: qsTr('Import Actions')

      topMargin: sceneTopMargin
      bottomMargin: sceneBottomMargin
      paddingMultiplier: 2

      MenuItem {
        id: importProjectFromFolder

        enabled: platformUtilities.capabilities & PlatformUtilities.CustomImport
        visible: enabled
        font: Theme.defaultFont
        width: parent.width
        height: enabled ? 48 : 0
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
        height: enabled ? 48 : 0
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
        height: enabled ? 48 : 0
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
        height: 48
        leftPadding: Theme.menuItemLeftPadding

        text: qsTr("Import URL")
        onTriggered: {
          importUrlDialog.open();
          importUrlInput.focus = true;
        }
      }

      MenuItem {
        id: importWebdav

        font: Theme.defaultFont
        width: parent.width
        height: 48
        leftPadding: Theme.menuItemLeftPadding

        text: qsTr("Import WebDAV folder")
        onTriggered: {
          importWebdavDialog.open();
          importWebdavUrlInput.focus = true;
        }
      }

      MenuSeparator {
        width: parent.width
      }

      MenuItem {
        id: storageHelp

        font: Theme.defaultFont
        width: parent.width
        height: 48
        leftPadding: Theme.menuItemLeftPadding

        text: qsTr("Storage management help")
        onTriggered: {
          Qt.openUrlExternally("https://docs.qfield.org/how-to/project-setup/storage/");
        }
      }
    }

    QfMenu {
      id: projectMenu

      title: qsTr('Project Actions')

      topMargin: sceneTopMargin
      bottomMargin: sceneBottomMargin
      paddingMultiplier: 2

      MenuItem {
        id: updateProjectFromArchive

        enabled: platformUtilities.capabilities & PlatformUtilities.UpdateProjectFromArchive
        visible: enabled
        font: Theme.defaultFont
        width: parent.width
        height: enabled ? 48 : 0
        leftPadding: Theme.menuItemLeftPadding

        text: qsTr("Update project from ZIP")
        onTriggered: {
          platformUtilities.updateProjectFromArchive(projectInfo.filePath);
        }
      }

      MenuItem {
        id: uploadProjectToWebdav

        enabled: webdavConnectionLoader.item ? webdavConnectionLoader.item.hasWebdavConfiguration(FileUtils.absolutePath(projectInfo.filePath)) : false
        visible: enabled
        font: Theme.defaultFont
        width: parent.width
        height: enabled ? 48 : 0
        leftPadding: Theme.menuItemLeftPadding

        text: qsTr("Upload project to WebDAV")
        onTriggered: {
          if (webdavConnectionLoader.item) {
            webdavConnectionLoader.item.uploadPaths([FileUtils.absolutePath(projectInfo.filePath)]);
          }
        }
      }

      MenuItem {
        id: downloadProjectToWebdav

        enabled: uploadProjectToWebdav.enabled
        visible: enabled
        font: Theme.defaultFont
        width: parent.width
        height: enabled ? 48 : 0
        leftPadding: Theme.menuItemLeftPadding

        text: qsTr("Download project from WebDAV")
        onTriggered: {
          if (webdavConnectionLoader.item) {
            webdavConnectionLoader.item.openedProjectPath = projectInfo.filePath;
            iface.clearProject();
            webdavConnectionLoader.item.downloadPath(FileUtils.absolutePath(projectInfo.filePath));
          }
        }
      }
    }

    QfMenu {
      id: selectionMenu
      x: parent.width - width - 8
      paddingMultiplier: 2

      MenuItem {
        id: uploadToWebdav

        enabled: table.selectedItemsWebDavConfigured
        visible: enabled

        font: Theme.defaultFont
        width: parent.width
        height: enabled ? 48 : 0
        leftPadding: Theme.menuItemLeftPadding

        text: qsTr("Upload file(s) to WebDAV")
        onTriggered: {
          var fileNames = [];
          for (let i = 0; i < table.selectedList.length; ++i) {
            const item = table.model.get(table.selectedList[i]);
            fileNames.push(item.path);
          }
          if (webdavConnectionLoader.item && fileNames.length > 0) {
            webdavConnectionLoader.item.uploadPaths(fileNames);
          }
        }
      }

      MenuItem {
        id: pushToQfieldCloud

        enabled: table.selectedItemsPushableToQField
        visible: enabled

        font: Theme.defaultFont
        width: parent.width
        height: enabled ? 48 : 0
        leftPadding: Theme.menuItemLeftPadding

        text: qsTr("Push file(s) to QFieldCloud")
        onTriggered: {
          var fileNames = [];
          for (let i = 0; i < table.selectedList.length; ++i) {
            const item = table.model.get(table.selectedList[i]);
            const itemWithinQFieldCloudProjectFolder = cloudProjectsModel.currentProjectId !== "" && item.path.search(cloudProjectsModel.currentProjectId) !== -1;
            const pushableToCloud = (item.metaType === LocalFilesModel.Dataset && item.type === LocalFilesModel.RasterDataset && cloudProjectsModel.currentProjectId) || (item.metaType === LocalFilesModel.Folder && itemWithinQFieldCloudProjectFolder);
            if (pushableToCloud)
              fileNames.push(item.path);
          }
          if (fileNames.length > 0) {
            pushFilesToQFieldCloudConnection.enabled = true;
            QFieldCloudUtils.addPendingAttachments(cloudConnection.userInformation.username, QFieldCloudUtils.getProjectId(table.model.currentPath), fileNames, cloudConnection, true);
          } else {
            displayToast(qsTr("Please select one or more files to push to QFieldCloud."));
          }
        }
      }

      MenuItem {
        id: deleteSelectedFiles

        enabled: table.selectedItemsDeletable
        visible: enabled

        font: Theme.defaultFont
        width: parent.width
        height: enabled ? 48 : 0
        leftPadding: Theme.menuItemLeftPadding

        text: qsTr("Delete file(s)")
        onTriggered: {
          var fileNames = [];
          for (let i = 0; i < table.selectedList.length; ++i) {
            const item = table.model.get(table.selectedList[i]);
            fileNames.push(item.path);
          }
          if (fileNames.length > 0) {
            confirmRemoveDialog.itemsToRemove = fileNames;
            confirmRemoveDialog.open();
          }
        }
      }
    }
  }

  Connections {
    id: pushFilesToQFieldCloudConnection
    enabled: false
    target: cloudConnection

    property bool sendingMultiple: false

    function onPendingAttachmentsAdded() {
      platformUtilities.uploadPendingAttachments(cloudConnection);
      if (pushFilesToQFieldCloudConnection.sendingMultiple) {
        displayToast(qsTr("‘%1’ is being uploaded to QFieldCloud").arg(FileUtils.fileName(itemMenu.itemPath)));
        pushFilesToQFieldCloudConnection.sendingMultiple = false;
      } else {
        localFilesModel.clearSelection();
        displayToast(qsTr("Items being uploaded to QFieldCloud"));
      }
      pushFilesToQFieldCloudConnection.enabled = false;
    }
  }

  QfDialog {
    id: confirmRemoveDialog
    parent: mainWindow.contentItem
    title: qsTr("Remove File(s)")

    property list<string> itemsToRemove: []

    Label {
      width: parent.width
      wrapMode: Text.WordWrap
      text: qsTr("Are you sure you want to remove %n files?", "", confirmRemoveDialog.itemsToRemove.length)
    }

    onAccepted: {
      const results = FileUtils.deleteFiles(itemsToRemove);
      let allSucceeded = true;
      let failedCount = 0;
      for (let i = 0; i < itemsToRemove.length; i++) {
        if (!results[itemsToRemove[i]]) {
          allSucceeded = false;
          failedCount++;
        }
      }
      if (allSucceeded) {
        displayToast(qsTr("%n file(s) deleted successfully", "", confirmRemoveDialog.itemsToRemove.length));
      } else {
        displayToast(qsTr("Failed to delete %n file(s)", "", failedCount));
      }
      table.model.resetToPath(table.model.currentPath);
      localFilesModel.clearSelection();
    }

    onRejected: {
      visible = false;
    }
  }

  QfDialog {
    id: importUrlDialog
    title: qsTr("Import URL")
    focus: visible
    parent: mainWindow.contentItem

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

      TextField {
        id: importUrlInput
        width: importUrlLabel.width
      }
    }

    onAccepted: {
      iface.importUrl(importUrlInput.text);
    }
  }

  Loader {
    id: webdavConnectionLoader
    active: qfieldLocalDataPickerScreen.openedOnce
    sourceComponent: Component {
      WebdavConnection {
        id: webdavConnection

        property string openedProjectPath: ""

        onIsImportingPathChanged: {
          if (isImportingPath) {
            busyOverlay.text = qsTr("Importing WebDAV folder");
            busyOverlay.showProgress = true;
            busyOverlay.progress = 0;
            busyOverlay.state = "visible";
          } else {
            busyOverlay.state = "hidden";
          }
        }

        onIsDownloadingPathChanged: {
          if (isDownloadingPath) {
            busyOverlay.text = qsTr("Downloading WebDAV folder");
            busyOverlay.showProgress = true;
            busyOverlay.progress = 0;
            busyOverlay.state = "visible";
          } else {
            busyOverlay.state = "hidden";
            if (openedProjectPath) {
              iface.loadFile(openedProjectPath);
              openedProjectPath = "";
            }
          }
        }

        onIsUploadingPathChanged: {
          if (isUploadingPath) {
            busyOverlay.text = qsTr("Uploading WebDAV folder");
            busyOverlay.showProgress = true;
            busyOverlay.progress = 0;
            busyOverlay.state = "visible";
          } else {
            busyOverlay.state = "hidden";
            localFilesModel.clearSelection();
          }
        }

        onProgressChanged: {
          if (isImportingPath || isDownloadingPath || isUploadingPath) {
            busyOverlay.progress = progress;
          }
        }

        onLastErrorChanged: {
          displayToast(qsTr("WebDAV error: ") + lastError);
        }

        onConfirmationRequested: (host, username) => {
          downloadUploadWebdavDialog.isUploadingPath = isUploadingPath;
          downloadUploadWebdavDialog.host = host;
          downloadUploadWebdavDialog.username = username;
          downloadUploadWebdavDialog.open();
        }

        onImportSuccessful: path => {
          table.model.currentPath = path;
        }

        onIsFetchingAvailablePathsChanged: {
          if (!isFetchingAvailablePaths && importWebdavDialog.visible) {
            swipeDialog.currentIndex = 1;
            importWebdavPathInput.model = availablePaths;
            importWebdavPathInput.currentIndex = importWebdavPathInput.model.indexOf(importWebdavPathInput.lastIndexPath);
          }
        }
      }
    }
  }

  QfDialog {
    id: downloadUploadWebdavDialog
    title: isUploadingPath ? qsTr("WebDAV upload") : qsTr("WebDAV download")
    focus: visible
    parent: mainWindow.contentItem

    property bool isUploadingPath: false
    property string host: ""
    property string username: ""

    onAboutToShow: {
      if (webdavConnectionLoader.item) {
        webdavConnectionLoader.item.password = downloadUploadWebdavPasswordInput.text;
        webdavConnectionLoader.item.storePassword = downloadUploadWebdavPasswordCheck.checked;
      }
    }

    Column {
      width: childrenRect.width
      height: childrenRect.height
      spacing: 10

      TextMetrics {
        id: downloadUploadWebdavIntroMetrics
        font: downloadUploadWebdavIntroLabel.font
        text: downloadUploadWebdavIntroLabel.text
      }

      Label {
        id: downloadUploadWebdavIntroLabel
        width: mainWindow.width - 60 < downloadUploadWebdavIntroMetrics.width ? mainWindow.width - 60 : downloadUploadWebdavIntroMetrics.width
        text: downloadUploadWebdavDialog.isUploadingPath ? qsTr("You are about to upload modified content into <b>%1</b> using user <b>%2</b>.<br><br>This operation will overwrite data stored remotely, make sure this is what you want to do.").arg(downloadUploadWebdavDialog.host).arg(downloadUploadWebdavDialog.username) : qsTr("You are about to download modified content from <b>%1</b> using user <b>%2</b>.<br><br>This operation will overwrite data stored locally, make sure this is what you want to do.").arg(downloadUploadWebdavDialog.host).arg(downloadUploadWebdavDialog.username)
        wrapMode: Text.WordWrap
        font: Theme.defaultFont
        color: Theme.mainTextColor
      }

      TextField {
        id: downloadUploadWebdavPasswordInput
        enabled: !webdavConnectionLoader.item || !webdavConnectionLoader.item.isFetchingAvailablePaths
        width: downloadUploadWebdavIntroLabel.width
        rightPadding: leftPadding + (downloadUploadWebdavShowPasswordInput.width - leftPadding)
        placeholderText: text === "" && webdavConnectionLoader.item && webdavConnectionLoader.item.isPasswordStored ? qsTr("Password (leave empty to use remembered)") : qsTr("Password")
        echoMode: TextInput.Password
        passwordMaskDelay: Qt.platform.os === "ios" || Qt.platform.os === "android" ? 1000 : 0

        onDisplayTextChanged: {
          if (webdavConnectionLoader.item) {
            webdavConnectionLoader.item.password = text;
          }
        }

        QfToolButton {
          id: downloadUploadWebdavShowPasswordInput

          property int originalEchoMode: TextInput.Normal

          visible: (!!parent.echoMode && parent.echoMode !== TextInput.Normal) || originalEchoMode !== TextInput.Normal
          iconSource: parent.echoMode === TextInput.Normal ? Theme.getThemeVectorIcon('ic_hide_green_48dp') : Theme.getThemeVectorIcon('ic_show_green_48dp')
          iconColor: Theme.mainColor
          anchors.right: parent.right
          anchors.verticalCenter: parent.verticalCenter
          opacity: parent.text.length > 0 ? 1 : 0.25
          z: 1

          onClicked: {
            if (parent.echoMode !== TextInput.Normal) {
              originalEchoMode = parent.echoMode;
              parent.echoMode = TextInput.Normal;
            } else {
              parent.echoMode = originalEchoMode;
            }
          }
        }
      }

      CheckBox {
        id: downloadUploadWebdavPasswordCheck
        width: downloadUploadWebdavIntroLabel.width
        enabled: !webdavConnectionLoader.item || !webdavConnectionLoader.item.isFetchingAvailablePaths
        text: qsTr('Remember password')
        font: Theme.defaultFont
        checked: true
      }
    }

    onAccepted: {
      if (webdavConnectionLoader.item) {
        webdavConnectionLoader.item.confirmRequest();
      }
    }

    onRejected: {
      if (webdavConnectionLoader.item) {
        webdavConnectionLoader.item.cancelRequest();
      }
    }
  }

  QfDialog {
    id: importWebdavDialog
    title: qsTr("Import WebDAV folder")
    focus: visible
    parent: mainWindow.contentItem

    property var importHistory: undefined

    function reloadHistory() {
      if (webdavConnectionLoader.item) {
        importHistory = webdavConnectionLoader.item.importHistory();
        importWebdavUrlInput.model = [""].concat(Object.keys(importHistory["urls"]));
        if (importHistory["lastUrl"] !== "") {
          importWebdavUrlInput.editText = importHistory["lastUrl"];
          importWebdavUserInput.model = [""].concat(Object.keys(importHistory["urls"][importHistory["lastUrl"]]["users"]));
          importWebdavUserInput.editText = importHistory["urls"][importHistory["lastUrl"]]["lastUser"];
        } else {
          importWebdavUserInput.model = [];
        }
        webdavConnectionLoader.item.url = importWebdavUrlInput.editText;
        webdavConnectionLoader.item.username = importWebdavUserInput.editText;
        webdavConnectionLoader.item.password = importWebdavPasswordInput.text;
        webdavConnectionLoader.item.storePassword = importWebdavStorePasswordCheck.checked;
      }
    }

    onAboutToShow: {
      swipeDialog.currentIndex = 0;
      importWebdavDialog.standardButton(Dialog.Ok).enabled = false;
      reloadHistory();
    }

    TextMetrics {
      id: importWebdavUrlLabelMetrics
      font: importWebdavUrlLabel.font
      text: importWebdavUrlLabel.text
    }

    width: swipeDialog.width + padding * 2
    SwipeView {
      id: swipeDialog
      width: mainWindow.width - 60 < importWebdavUrlLabelMetrics.width + 40 ? mainWindow.width - 60 : importWebdavUrlLabelMetrics.width + 40
      clip: true
      interactive: false

      GridLayout {
        id: firstPage
        width: swipeDialog.width
        rowSpacing: 10
        columnSpacing: 5
        columns: 2

        Label {
          id: importWebdavUrlLabel
          Layout.fillWidth: true
          Layout.columnSpan: 2
          text: qsTr("Type the WebDAV details below to import a remote folder:")
          wrapMode: Text.WordWrap
          font: Theme.defaultFont
          color: Theme.mainTextColor
        }

        Label {
          Layout.fillWidth: true
          Layout.columnSpan: 2
          text: qsTr("WebDAV server URL")
          wrapMode: Text.WordWrap
          font: Theme.defaultFont
          color: Theme.secondaryTextColor
        }

        ComboBox {
          id: importWebdavUrlInput
          Layout.fillWidth: true
          enabled: !webdavConnectionLoader.item || !webdavConnectionLoader.item.isFetchingAvailablePaths
          editable: true

          Connections {
            target: importWebdavUrlInput.contentItem
            ignoreUnknownSignals: true

            function onDisplayTextChanged() {
              if (webdavConnectionLoader.item && webdavConnectionLoader.item.url !== importWebdavUrlInput.editText) {
                webdavConnectionLoader.item.url = importWebdavUrlInput.editText;
                if (importWebdavDialog.importHistory["urls"][importWebdavUrlInput.editText] !== undefined) {
                  importWebdavUserInput.model = [""].concat(Object.keys(importWebdavDialog.importHistory["urls"][importWebdavUrlInput.editText]["users"]));
                  importWebdavUserInput.editText = importWebdavDialog.importHistory["urls"][importWebdavUrlInput.editText]["lastUser"];
                } else {
                  importWebdavUserInput.model = [];
                }
              }
            }
          }
        }

        QfToolButton {
          bgcolor: "transparent"
          iconSource: Theme.getThemeVectorIcon('ic_delete_forever_white_24dp')
          iconColor: enabled ? Theme.mainTextColor : Theme.mainTextDisabledColor
          enabled: importWebdavUrlInput.editText !== ""

          onClicked: {
            if (webdavConnectionLoader.item) {
              webdavForgetConfirmationDialog.url = importWebdavUrlInput.editText;
              webdavForgetConfirmationDialog.username = "";
              webdavForgetConfirmationDialog.open();
            }
          }
        }

        Label {
          Layout.fillWidth: true
          Layout.columnSpan: 2
          text: qsTr("User and password")
          wrapMode: Text.WordWrap
          font: Theme.defaultFont
          color: Theme.secondaryTextColor
        }

        ComboBox {
          id: importWebdavUserInput
          enabled: !webdavConnectionLoader.item || !webdavConnectionLoader.item.isFetchingAvailablePaths
          Layout.fillWidth: true
          editable: true

          Connections {
            target: importWebdavUserInput.contentItem
            ignoreUnknownSignals: true

            function onDisplayTextChanged() {
              if (webdavConnectionLoader.item) {
                webdavConnectionLoader.item.username = importWebdavUserInput.editText;
              }
            }
          }
        }

        QfToolButton {
          Layout.rowSpan: 2
          Layout.alignment: Qt.AlignVCenter
          bgcolor: "transparent"
          iconSource: Theme.getThemeVectorIcon('ic_delete_forever_white_24dp')
          iconColor: enabled ? Theme.mainTextColor : Theme.mainTextDisabledColor
          enabled: importWebdavUrlInput.editText !== "" && importWebdavUserInput.editText !== ""

          onClicked: {
            if (webdavConnectionLoader.item) {
              webdavForgetConfirmationDialog.url = importWebdavUrlInput.editText;
              webdavForgetConfirmationDialog.username = importWebdavUserInput.editText;
              webdavForgetConfirmationDialog.open();
            }
          }
        }

        TextField {
          id: importWebdavPasswordInput
          enabled: !webdavConnectionLoader.item || !webdavConnectionLoader.item.isFetchingAvailablePaths
          Layout.fillWidth: true
          rightPadding: leftPadding + (importWebdavShowPasswordInput.width - leftPadding)
          placeholderText: text === "" && webdavConnectionLoader.item && webdavConnectionLoader.item.isPasswordStored ? qsTr("leave empty to use remembered") : ""
          echoMode: TextInput.Password
          passwordMaskDelay: Qt.platform.os === "ios" || Qt.platform.os === "android" ? 1000 : 0

          onDisplayTextChanged: {
            if (webdavConnectionLoader.item) {
              webdavConnectionLoader.item.password = text;
            }
          }

          QfToolButton {
            id: importWebdavShowPasswordInput

            property int originalEchoMode: TextInput.Normal

            visible: (!!parent.echoMode && parent.echoMode !== TextInput.Normal) || originalEchoMode !== TextInput.Normal
            iconSource: parent.echoMode === TextInput.Normal ? Theme.getThemeVectorIcon('ic_hide_green_48dp') : Theme.getThemeVectorIcon('ic_show_green_48dp')
            iconColor: Theme.mainColor
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            opacity: parent.text.length > 0 ? 1 : 0.25
            z: 1

            onClicked: {
              if (parent.echoMode !== TextInput.Normal) {
                originalEchoMode = parent.echoMode;
                parent.echoMode = TextInput.Normal;
              } else {
                parent.echoMode = originalEchoMode;
              }
            }
          }
        }

        CheckBox {
          id: importWebdavStorePasswordCheck
          Layout.fillWidth: true
          Layout.columnSpan: 2
          enabled: !webdavConnectionLoader.item || !webdavConnectionLoader.item.isFetchingAvailablePaths
          text: qsTr('Remember password')
          font: Theme.defaultFont
          checked: true
        }

        QfButton {
          id: importWebdavFetchFoldersButton
          Layout.fillWidth: true
          Layout.columnSpan: importWebdavFetchFoldersIndicator.visible ? 1 : 2
          enabled: !webdavConnectionLoader.item || !webdavConnectionLoader.item.isFetchingAvailablePaths
          text: !enabled ? qsTr("Fetching remote folders") : qsTr("Fetch remote folders")

          onClicked: {
            webdavConnectionLoader.item.fetchAvailablePaths();
          }
        }

        BusyIndicator {
          id: importWebdavFetchFoldersIndicator
          Layout.preferredWidth: 48
          Layout.preferredHeight: 48
          visible: webdavConnectionLoader.item && webdavConnectionLoader.item.isFetchingAvailablePaths
          running: visible
        }
      }

      Column {
        width: swipeDialog.width
        spacing: 10

        Row {
          spacing: 5

          Label {
            anchors.verticalCenter: importWebdavRefetchFoldersButton.verticalCenter
            width: parent.parent.width - importWebdavRefetchFoldersButton.width - parent.spacing
            text: qsTr("Select the remote folder to import:")
            wrapMode: Text.WordWrap
            font: Theme.defaultFont
            color: Theme.mainTextColor
          }

          QfToolButton {
            id: importWebdavRefetchFoldersButton
            enabled: !webdavConnectionLoader.item || !webdavConnectionLoader.item.isFetchingAvailablePaths
            visible: !webdavConnectionLoader.item || !webdavConnectionLoader.item.isFetchingAvailablePaths
            iconSource: Theme.getThemeVectorIcon("refresh_24dp")
            iconColor: Theme.mainTextColor
            bgcolor: "transparent"
            width: 36
            height: 36
            padding: 0

            onClicked: {
              importWebdavPathInput.currentIndex = -1;
              webdavConnectionLoader.item.fetchAvailablePaths();
            }
          }

          BusyIndicator {
            id: importWebdavRefreshFoldersIndicator
            anchors.verticalCenter: importWebdavRefetchFoldersButton.verticalCenter
            width: importWebdavRefetchFoldersButton.width
            height: importWebdavRefetchFoldersButton.width
            visible: webdavConnectionLoader.item && webdavConnectionLoader.item.isFetchingAvailablePaths
            running: visible
          }
        }

        MaterialTextContainer {
          id: importWebdavPathContainer
          width: parent.width
          height: 340
          outlineColor: importWebdavPathInput.Material.hintTextColor
          focusedOutlineColor: importWebdavPathInput.Material.accentColor
          controlHasActiveFocus: importWebdavPathInput.activeFocus

          ListView {
            id: importWebdavPathInput
            anchors.fill: parent
            anchors.margins: 1
            enabled: !webdavConnectionLoader.item || !webdavConnectionLoader.item.isFetchingAvailablePaths
            ScrollBar.vertical: QfScrollBar {}
            clip: true
            model: []

            property string lastIndexPath: ""
            property var expandedPaths: []
            property int expandedPathsClicks: 0

            onCurrentIndexChanged: {
              if (swipeDialog.currentIndex === 1) {
                importWebdavDialog.standardButton(Dialog.Ok).enabled = currentIndex > -1;
              }
            }

            delegate: Rectangle {
              id: rectangleDialog
              width: parent ? parent.width : undefined
              height: lineDialog.isVisible ? lineDialog.height + 20 : 0
              color: importWebdavPathInput.currentIndex == index ? Theme.mainColor : "transparent"
              radius: 4
              clip: true

              Row {
                id: lineDialog
                anchors.verticalCenter: parent.verticalCenter
                spacing: 5

                property string label: {
                  let parts = modelData.split('/');
                  if (parts.length > 1) {
                    return parts[parts.length - 2];
                  }
                  return "";
                }
                property int level: Math.max(0, modelData.split('/').length - 2)
                property bool isVisible: {
                  let parts = modelData.split('/').slice(1, -2);
                  while (parts.length > 0) {
                    if (importWebdavPathInput.expandedPaths.indexOf("/" + parts.join("/") + "/") == -1) {
                      return false;
                    }
                    parts = parts.slice(0, -1);
                  }
                  return true;
                }
                property bool hasChildren: {
                  if (webdavConnectionLoader.item.checkedPaths.indexOf(modelData) === -1) {
                    return true;
                  }

                  for (const availablePath of importWebdavPathInput.model) {
                    if (availablePath.indexOf(modelData) === 0 && availablePath !== modelData) {
                      return true;
                    }
                  }
                  return false;
                }
                property string importedPath: {
                  if (importWebdavDialog.importHistory["urls"][importWebdavUrlInput.editText] !== undefined && importWebdavDialog.importHistory["urls"][importWebdavUrlInput.editText]["users"][importWebdavUserInput.editText] !== undefined) {
                    if (importWebdavDialog.importHistory["urls"][importWebdavUrlInput.editText]["users"][importWebdavUserInput.editText]["importPaths"][modelData] !== undefined) {
                      return importWebdavDialog.importHistory["urls"][importWebdavUrlInput.editText]["users"][importWebdavUserInput.editText]["importPaths"][modelData];
                    }
                  }
                  return "";
                }
                property bool isImported: importedPath != ""

                Item {
                  id: expandSpacing
                  height: 35
                  width: 20 * Math.max(1, lineDialog.level) - 1
                }

                QfToolButton {
                  id: epxandButton
                  height: 35
                  width: height
                  anchors.verticalCenter: parent.verticalCenter
                  iconSource: Theme.getThemeVectorIcon('ic_legend_collapsed_state_24dp')
                  iconColor: Theme.mainTextColor
                  bgcolor: "transparent"
                  enabled: false
                  opacity: lineDialog.level > 0 && lineDialog.hasChildren && !lineDialog.isImported ? 1 : 0
                  rotation: importWebdavPathInput.expandedPaths.indexOf(modelData) > -1 ? 90 : 0

                  Behavior on rotation {
                    NumberAnimation {
                      duration: 100
                    }
                  }
                }

                Column {
                  width: rectangleDialog.width - epxandButton.width - expandSpacing.width - 10
                  anchors.verticalCenter: parent.verticalCenter

                  Text {
                    id: contentTextDialog
                    width: parent.width
                    leftPadding: 5
                    font: Theme.defaultFont
                    elide: Text.ElideRight
                    wrapMode: Text.WordWrap
                    color: !lineDialog.isImported ? Theme.mainTextColor : Theme.secondaryTextColor
                    text: lineDialog.label !== "" ? lineDialog.label : "(" + qsTr("root folder") + ")"
                  }
                  Text {
                    id: noteTextDialog
                    width: parent.width
                    visible: lineDialog.isImported
                    leftPadding: 5
                    font: Theme.tipFont
                    elide: Text.ElideRight
                    wrapMode: Text.WordWrap
                    color: Theme.secondaryTextColor
                    text: qsTr("Available locally in ‘%1’").arg(lineDialog.importedPath)
                  }
                }
              }

              /* bottom border */
              Rectangle {
                anchors {
                  bottom: parent.bottom
                  left: parent.left
                  leftMargin: 2
                }
                visible: lineDialog.isVisible && importWebdavPathInput.currentIndex != index
                height: 1
                color: Theme.controlBorderColor
                width: parent.width - anchors.leftMargin * 2
              }

              MouseArea {
                enabled: !lineDialog.isImported
                anchors.fill: parent
                anchors.rightMargin: 48
                onClicked: mouse => {
                  importWebdavPathInput.currentIndex = index;
                  importWebdavImportedFolderName.text = (lineDialog.label !== "" ? lineDialog.label : qsTr("root folder")) + " - " + webdavConnectionLoader.item.username;
                }
                onDoubleClicked: mouse => {
                  const index = importWebdavPathInput.expandedPaths.indexOf(modelData);
                  if (importWebdavPathInput.expandedPaths.indexOf(modelData) == -1) {
                    importWebdavPathInput.expandedPaths.push(modelData);
                    if (webdavConnectionLoader.item.checkedPaths.indexOf(modelData) === -1) {
                      importWebdavPathInput.lastIndexPath = modelData;
                      webdavConnectionLoader.item.fetchAvailablePaths(modelData);
                    }
                  } else {
                    importWebdavPathInput.expandedPaths.splice(index, 1);
                  }
                  importWebdavPathInput.expandedPathsChanged();
                }
              }
            }
          }
        }

        Label {
          width: parent.width
          text: qsTr("Imported folder name")
          wrapMode: Text.WordWrap
          font: Theme.defaultFont
          color: Theme.secondaryTextColor
        }

        TextField {
          id: importWebdavImportedFolderName
          enabled: !webdavConnectionLoader.item || !webdavConnectionLoader.item.isFetchingAvailablePaths
          width: parent.width
        }
      }
    }

    onAccepted: {
      if (importWebdavPathInput.currentIndex > -1 && webdavConnectionLoader.item) {
        webdavConnectionLoader.item.url = importWebdavUrlInput.editText;
        webdavConnectionLoader.item.username = importWebdavUserInput.editText;
        webdavConnectionLoader.item.password = importWebdavPasswordInput.text;
        webdavConnectionLoader.item.storePassword = importWebdavStorePasswordCheck.checked;
        webdavConnectionLoader.item.importPath(importWebdavPathInput.model[importWebdavPathInput.currentIndex], platformUtilities.applicationDirectory() + "/Imported Projects/", importWebdavImportedFolderName.text);
      }
    }
  }

  QfDialog {
    id: webdavForgetConfirmationDialog
    title: qsTr("Confirm")
    focus: visible
    parent: mainWindow.contentItem

    property string url: ""
    property string username: ""

    Column {
      width: childrenRect.width
      height: childrenRect.height
      spacing: 10

      TextMetrics {
        id: webdavForgetConfirmationMetrics
        font: webdavForgetConfirmationLabel.font
        text: webdavForgetConfirmationLabel.text
      }

      Label {
        id: webdavForgetConfirmationLabel
        width: mainWindow.width - 60 < webdavForgetConfirmationMetrics.width ? mainWindow.width - 60 : webdavForgetConfirmationMetrics.width
        text: qsTr("You are about to remove a saved WebDAV item, proceed?")
        wrapMode: Text.WordWrap
        font: Theme.defaultFont
        color: Theme.mainTextColor
      }
    }

    onAccepted: {
      if (webdavForgetConfirmationDialog.username != "") {
        webdavConnectionLoader.item.forgetHistory(webdavForgetConfirmationDialog.url, webdavForgetConfirmationDialog.username);
      } else {
        webdavConnectionLoader.item.forgetHistory(webdavForgetConfirmationDialog.url);
      }
      importWebdavUrlInput.currentIndex = 0;
      importWebdavUserInput.currentIndex = 0;
      importWebdavPasswordInput.text = '';
      importWebdavDialog.reloadHistory();
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
}
```


