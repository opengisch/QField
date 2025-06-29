import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qgis
import org.qfield
import Theme

/**
 * \ingroup qml
 */
Popup {
  id: popup

  width: Math.min(400, mainWindow.width - Theme.popupScreenEdgeMargin)
  height: mainWindow.height - 160
  x: (parent.width - width) / 2
  y: (parent.height - height) / 2
  padding: 0
  modal: true
  closePolicy: Popup.CloseOnEscape
  parent: Overlay.overlay
  focus: visible

  Page {
    id: page
    width: parent.width
    height: parent.height
    padding: 10
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
      spacing: 4
      width: parent.width
      height: parent.height

      Label {
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.margins: 20
        visible: pluginsList.model.count === 0

        text: qsTr('No plugins have been installed yet. To learn more about plugins, %1read the documentation%2.').arg('<a href="https://docs.qfield.org/how-to/plugins/">').arg('</a>')
        font: Theme.defaultFont
        wrapMode: Text.WordWrap
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        onLinkActivated: link => Qt.openUrlExternally(link)
      }

      ListView {
        id: pluginsList
        Layout.fillWidth: true
        Layout.fillHeight: true
        visible: model.count > 0
        clip: true

        model: ListModel {
        }

        delegate: PluginItem {
          icon: Icon !== '' ? UrlUtils.fromString(Icon) : ''
          name: Name
          itemEnabled: Enabled
          itemConfigurable: Configurable

          onAuthorDetailsClicked: {
            authorDetails.authorName = Author;
            authorDetails.authorHomepage = Homepage;
            authorDetails.open();
          }

          onUninstallConfirmationClicked: {
            uninstallConfirmation.pluginName = Name;
            uninstallConfirmation.pluginUuid = Uuid;
            uninstallConfirmation.open();
          }

          onToggleEnabledPlugin: checked => {
            Enabled = checked == true;
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
        }
      }

      QfButton {
        id: installFromUrlButton
        Layout.fillWidth: true
        dropdown: true

        text: qsTr("Install plugin from URL")

        onClicked: {
          installFromUrlDialog.open();
        }

        onDropdownClicked: {
          pluginsManagementMenu.popup(installFromUrlButton.width - pluginsManagementMenu.width + 10, installFromUrlButton.y + 10);
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
    }
  }

  QfDialog {
    id: authorDetails
    title: authorName
    parent: mainWindow.contentItem

    property string authorName: ""
    property string authorHomepage: ""

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
    focus: true

    onAboutToShow: {
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
    }

    function onAppPluginEnabled(uuid) {
      for (let i = 0; i < pluginsList.model.count; i++) {
        if (pluginsList.model.get(i).Uuid === uuid) {
          pluginsList.model.get(i).Enabled = true;
          pluginsList.model.get(i).Configurable = pluginManager.isAppPluginConfigurable(uuid);
        }
      }
    }

    function onAppPluginDisabled(uuid) {
      for (let i = 0; i < pluginsList.model.count; i++) {
        if (pluginsList.model.get(i).Uuid === uuid) {
          pluginsList.model.get(i).Enabled = false;
          pluginsList.model.get(i).Configurable = false;
        }
      }
    }

    function onAvailableAppPluginsChanged() {
      refreshAppPluginsList();
    }
  }

  function refreshAppPluginsList() {
    pluginsList.model.clear();
    for (const plugin of pluginManager.availableAppPlugins) {
      const isEnabled = pluginManager.isAppPluginEnabled(plugin.uuid);
      pluginsList.model.append({
          "Uuid": plugin.uuid,
          "Enabled": isEnabled,
          "Configurable": isEnabled && pluginManager.isAppPluginConfigurable(plugin.uuid),
          "Name": plugin.name,
          "Description": plugin.description,
          "Author": plugin.author,
          "Homepage": plugin.homepage,
          "Icon": plugin.icon,
          "Version": plugin.version
        });
    }
  }

  Component.onCompleted: {
    refreshAppPluginsList();
  }
}
