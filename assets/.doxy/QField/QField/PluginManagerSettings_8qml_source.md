

# File PluginManagerSettings.qml

[**File List**](files.md) **>** [**qml**](dir_1d81f7f4362e4398c0a695c4e466fcb0.md) **>** [**PluginManagerSettings.qml**](PluginManagerSettings_8qml.md)

[Go to the documentation of this file](PluginManagerSettings_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qgis
import org.qfield
import Theme

QfPopup {
  id: popup

  property bool availablePluginsFetched: false

  width: Math.min(500, mainWindow.width - Theme.popupScreenEdgeHorizontalMargin)
  height: mainWindow.height - 120
  x: (parent.width - width) / 2
  y: (parent.height - height) / 2
  closePolicy: Popup.CloseOnEscape
  parent: Overlay.overlay
  focus: visible

  onAboutToShow: {
    pluginManager.pluginModel.refresh(false);
  }

  Page {
    id: page
    width: parent.width
    height: parent.height
    padding: 5
    header: QfPageHeader {
      id: pageHeader
      title: qsTr("Plugins")

      showBackButton: false
      showApplyButton: false
      showCancelButton: true
      backgroundFill: false

      onCancel: {
        popup.close();
      }
    }

    ColumnLayout {
      id: pluginsLayout
      spacing: 4
      width: parent.width
      height: parent.height

      QfTabBar {
        id: filterBar
        Layout.fillWidth: true
        Layout.preferredHeight: defaultHeight
        model: [qsTr("Local Plugins"), qsTr("Available Plugins")]
        currentIndex: 0

        onClicked: {
          if (currentIndex == 1 && !popup.availablePluginsFetched) {
            pluginManager.pluginModel.refresh(true);
          }
        }
      }

      QfSearchBar {
        id: searchBar
        visible: filterBar.currentIndex === 1
        Layout.fillWidth: true
        Layout.preferredHeight: 41
        placeHolderText: qsTr("Search for plugin")
      }

      ListView {
        id: pluginsList
        Layout.fillWidth: true
        Layout.fillHeight: true
        visible: count > 0
        leftMargin: 6
        rightMargin: 8
        clip: true

        property var downloadingUuids: []

        ScrollBar.vertical: QfScrollBar {
          verticalPadding: 15
        }

        model: PluginProxyModel {
          sourceModel: pluginManager.pluginModel
          searchTerm: filterBar.currentIndex === 1 ? searchBar.searchTerm : ""
          filter: filterBar.currentIndex === 0 ? PluginProxyModel.LocalPlugin : PluginProxyModel.RemotePlugin
        }

        delegate: PluginItem {
          width: parent ? parent.width : 10
          icon: Icon !== '' ? UrlUtils.fromString(Icon) : ''
          name: Name
          itemEnabled: Enabled
          itemDownloading: pluginsList.downloadingUuids.indexOf(Uuid) > -1

          onAuthorDetailsClicked: {
            authorDetails.authorName = Author;
            authorDetails.authorHomepage = Homepage;
            authorDetails.authorTrusted = Trusted;
            authorDetails.open();
          }

          onUninstallConfirmationClicked: {
            uninstallConfirmation.pluginName = Name;
            uninstallConfirmation.pluginUuid = Uuid;
            uninstallConfirmation.open();
          }

          onToggleEnabledPlugin: checked => {
            Enabled = checked;
            if (Enabled) {
              pluginManager.enableAppPlugin(Uuid);
            } else {
              pluginManager.disableAppPlugin(Uuid);
            }
          }

          onPluginNameClicked: {
            if (!Enabled) {
              pluginManager.enableAppPlugin(Uuid);
            } else {
              pluginManager.disableAppPlugin(Uuid);
            }
          }

          onConfigClicked: {
            pluginManager.configureAppPlugin(Uuid);
          }

          onUpdateClicked: {
            let uuids = pluginsList.downloadingUuids;
            uuids.push(Uuid);
            pluginsList.downloadingUuids = uuids;
            pluginManager.installFromRepository(Uuid);
          }

          onDownloadClicked: {
            let uuids = pluginsList.downloadingUuids;
            uuids.push(Uuid);
            pluginsList.downloadingUuids = uuids;
            pluginManager.installFromRepository(Uuid);
          }
        }
      }

      Label {
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.margins: 20
        visible: pluginsList.count === 0

        text: pluginManager.pluginModel.isRefreshing ? qsTr('Fetching available plugins') : qsTr('No plugins have been installed yet, switch to the %1available plugins%3 tab to try some right away.<br><br>For more information, %2read the documentation%3.').arg('<a href="#">').arg('<a href="https://docs.qfield.org/ro/how-to/advanced-how-tos/plugins/">').arg('</a>')
        font: Theme.defaultFont
        wrapMode: Text.WordWrap
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        onLinkActivated: link => {
          if (link === '#') {
            filterBar.currentIndex = 1;
            if (!popup.availablePluginsFetched) {
              pluginManager.pluginModel.refresh(true);
            }
          } else {
            Qt.openUrlExternally(link);
          }
        }
      }

      RowLayout {
        Layout.fillWidth: true

        QfButton {
          id: installFromUrlButton
          Layout.fillWidth: true
          dropdown: true
          enabled: !busyIndicator.running

          text: qsTr("Install plugin from URL")

          onClicked: {
            if (!popup.availablePluginsFetched) {
              pluginManager.pluginModel.refresh(true);
            }
            installFromUrlDialog.open();
          }

          onDropdownClicked: {
            pluginsManagementMenu.popup(installFromUrlButton.width - pluginsManagementMenu.width + 10, installFromUrlButton.y + 10);
          }
        }

        BusyIndicator {
          id: busyIndicator
          Layout.preferredWidth: 48
          Layout.preferredHeight: 48
          visible: running
          running: pluginManager.pluginModel.isRefreshing
        }
      }
    }
  }

  QfMenu {
    id: pluginsManagementMenu
    title: qsTr('Plugins management menu')

    MenuItem {
      text: qsTr('Clear remembered permissions')

      font: Theme.defaultFont
      height: 48
      leftPadding: Theme.menuItemLeftPadding

      onTriggered: {
        pluginManager.clearPluginPermissions();
      }
    }
  }

  QfDialog {
    id: authorDetails
    title: authorName
    parent: mainWindow.contentItem

    property string authorName: ""
    property string authorHomepage: ""
    property bool authorTrusted: false

    Column {
      width: mainWindow.width - 60 < authorWarningLabelMetrics.width ? mainWindow.width - 60 : authorWarningLabelMetrics.width
      height: childrenRect.height
      spacing: 10

      TextMetrics {
        id: authorWarningLabelMetrics
        font: authorWarningLabel.font
        text: authorWarningLabel.text
      }

      Label {
        id: authorHomepageLabel
        visible: authorDetails.authorHomepage !== ""
        width: parent.width
        text: "<a href='" + authorDetails.authorHomepage + "'>" + authorDetails.authorHomepage + "</a>"
        wrapMode: Text.WordWrap
        font: Theme.defaultFont
        color: Theme.mainTextColor

        onLinkActivated: link => {
          Qt.openUrlExternally(link);
        }
      }

      Label {
        id: authorWarningLabel
        width: parent.width
        visible: !authorDetails.authorTrusted
        text: "⚠ " + qsTr("The author details shown above are self-reported by the plugin and not independently verified. Please make sure you trust the plugin's origin.")
        wrapMode: Text.WordWrap
        font: Theme.defaultFont
        color: Theme.mainTextColor
      }
    }

    standardButtons: Dialog.Close
  }

  QfDialog {
    id: installFromUrlDialog
    title: "Install Plugin from URL"
    parent: mainWindow.contentItem

    onAboutToShow: {
      installFromUrlDialog.standardButton(Dialog.Ok).enabled = popup.availablePluginsFetched;
      installFromUrlInput.text = '';
    }

    Column {
      width: childrenRect.width
      height: childrenRect.height
      spacing: 10

      TextMetrics {
        id: installFromUrlLabelMetrics
        font: installFromUrlLabel.font
        text: installFromUrlLabel.text
      }

      Label {
        id: installFromUrlLabel
        width: mainWindow.width - 60 < installFromUrlLabelMetrics.width ? mainWindow.width - 60 : installFromUrlLabelMetrics.width
        text: qsTr("Type a URL below to download and install a plugin:")
        wrapMode: Text.WordWrap
        font: Theme.defaultFont
        color: Theme.mainTextColor
      }

      TextField {
        id: installFromUrlInput
        width: installFromUrlLabel.width
      }
    }

    onAccepted: {
      pluginManager.installFromUrl(installFromUrlInput.text);
    }
  }

  QfDialog {
    id: uninstallConfirmation
    title: "Uninstall Plugin"
    parent: mainWindow.contentItem

    property string pluginName: ""
    property string pluginUuid: ""

    Column {
      width: childrenRect.width
      height: childrenRect.height
      spacing: 10

      TextMetrics {
        id: uninstallLabelMetrics
        font: uninstallLabel.font
        text: uninstallLabel.text
      }

      Label {
        id: uninstallLabel
        width: mainWindow.width - 60 < uninstallLabelMetrics.width ? mainWindow.width - 60 : uninstallLabelMetrics.width
        text: qsTr("Are you sure you want to uninstall `%1`?").arg(uninstallConfirmation.pluginName)
        wrapMode: Text.WordWrap
        font: Theme.defaultFont
        color: Theme.mainTextColor
      }
    }

    onAccepted: {
      pluginManager.uninstall(pluginUuid);
    }
  }

  Connections {
    target: pluginManager.pluginModel

    function onRemoteFetched() {
      popup.availablePluginsFetched = true;
      installFromUrlDialog.standardButton(Dialog.Ok).enabled = true;
    }
  }

  Connections {
    target: pluginManager

    function onInstallTriggered(name) {
      pageHeader.busyIndicatorState = "on";
      pageHeader.busyIndicatorValue = 0;
      displayToast(qsTr("Installing %1").arg(name));
    }

    function onInstallProgress(progress) {
      pageHeader.busyIndicatorValue = progress;
    }

    function onInstallEnded(uuid, error) {
      pageHeader.busyIndicatorState = "off";
      if (uuid !== '') {
        pluginManager.enableAppPlugin(uuid);
      } else {
        displayToast(qsTr('Plugin installation failed: ' + error, 'error'));
      }
      const downloadingIndex = pluginsList.downloadingUuids.indexOf(uuid);
      if (downloadingIndex > -1) {
        let uuids = pluginsList.downloadingUuids;
        uuids.splice(downloadingIndex, 1);
        pluginsList.downloadingUuids = uuids;
      }
    }

    function onAppPluginEnabled(uuid) {
      pluginManager.pluginModel.updatePluginEnabledStateByUuid(uuid, true, pluginManager.isAppPluginConfigurable(uuid));
    }

    function onAppPluginDisabled(uuid) {
      pluginManager.pluginModel.updatePluginEnabledStateByUuid(uuid, false, false);
    }
  }
}
```


