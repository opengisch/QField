import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import org.qgis 1.0
import org.qfield 1.0

import Theme 1.0

Popup {
  id: popup

  width: Math.min(400, mainWindow.width - Theme.popupScreenEdgeMargin)
  height: mainWindow.height - 160
  x: (parent.width - width) / 2
  y: (parent.height - height) / 2
  padding: 0

  Page {
    id: page
    width: parent.width
    height: parent.height
    padding: 10
    header: QfPageHeader {
      id: pageHeader
      title: qsTr( "Plugins" )

      showBackButton: false
      showApplyButton: false
      showCancelButton: true
      backgroundFill: false

      onCancel: {
        popup.close()
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

        text: qsTr('No plugins have been installed yet. To learn more about plugins, %1read the documentation%2.').arg('<a href="https://docs.qfield.org/">').arg('</a>')
        font: Theme.defaultFont
        wrapMode: Text.WordWrap
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        onLinkActivated: Qt.openUrlExternally(link)
      }

      ListView {
        id: pluginsList
        Layout.fillWidth: true
        Layout.fillHeight: true
        visible: model.count > 0
        clip: true

        model: ListModel {}

        delegate: Rectangle {
          id: rectangle
          width: parent.width
          height: inner.height + 10
          color: "transparent"

          GridLayout {
            id: inner
            width: parent.width
            anchors.leftMargin: 20
            anchors.rightMargin: 20

            columns: 2
            columnSpacing: 0
            rowSpacing: 2

            RowLayout {
              Layout.fillWidth: true

              Image {
                Layout.preferredWidth: 24
                Layout.preferredHeight: 24

                source: Icon !== '' ? 'file://' + Icon : ''
                fillMode: Image.PreserveAspectFit
                mipmap: true
              }

              Label {
                Layout.fillWidth: true
                text: Name
                font: Theme.defaultFont
                color: Theme.mainTextColor
                wrapMode: Text.WordWrap

                MouseArea {
                  anchors.fill: parent
                  onClicked: {
                    toggleEnabledPlugin.checked = !toggleEnabledPlugin.checked
                    toggleEnabledPlugin.clicked()
                  }
                }
              }
            }

            QfSwitch {
              id: toggleEnabledPlugin
              Layout.preferredWidth: implicitContentWidth
              checked: pluginManager.isAppPluginEnabled(Uuid)

              onClicked: {
                if (checked) {
                  pluginManager.enableAppPlugin(Uuid)
                } else {
                  pluginManager.disableAppPlugin(Uuid)
                }
              }
            }

            ColumnLayout {
              Layout.fillWidth: true

              Label {
                Layout.fillWidth: true
                text: qsTr('Authored by %1').arg(Author)
                font: Theme.tipFont
                color: Theme.secondaryTextColor
                wrapMode: Text.WordWrap
              }

              Label {
                Layout.fillWidth: true
                text: Description
                font: Theme.tipFont
                color: Theme.secondaryTextColor
                wrapMode: Text.WordWrap
              }
            }
          }
        }
      }

      QfButton {
        id: installFromUrlButton
        Layout.fillWidth: true

        text: qsTr("Install plugin from URL")

        onClicked: {
          installFromUrlDialog.open()
        }
      }

      QfButton {
        id: clearPermissionsButton
        Layout.fillWidth: true

        text: qsTr("Clear remembered permissions")

        onClicked: {
          pluginManager.clearPluginPermissions()
        }
      }
    }
  }

  Dialog {
    id: installFromUrlDialog
    title: "Install Plugin from URL"
    parent: mainWindow.contentItem
    focus: true
    font: Theme.defaultFont

    x: ( mainWindow.width - width ) / 2
    y: ( mainWindow.height - height - 80 ) / 2

    onAboutToShow: {
      installFromUrlInput.text = ''
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

      QfTextField {
        id: installFromUrlInput
        width: installFromUrlLabel.width
      }
    }

    standardButtons: Dialog.Ok | Dialog.Cancel
    onAccepted: {
      pluginManager.installFromUrl(installFromUrlInput.text)
    }
  }

  Connections {
    target: pluginManager

    function onInstallTriggered(name) {
      pageHeader.busyIndicatorState = "on"
      pageHeader.busyIndicatorValue = 0

      displayToast(qsTr("Installing %1").arg(name))
    }

    function onInstallProgress(progress) {
      pageHeader.busyIndicatorValue = progress
    }

    function onInstallEnded(uuid, error) {
      pageHeader.busyIndicatorState = "off"

      if (uuid !== '') {
        pluginManager.enableAppPlugin(uuid)
      } else {
        displayToast(qsTr('Plugin installation failed: '+error, 'error'))
      }
    }

    function onAvailableAppPluginsChanged() {
      refreshAppPluginsList()
    }
  }

  function refreshAppPluginsList() {
    pluginsList.model.clear()

    for (const plugin of pluginManager.availableAppPlugins) {
      pluginsList.model.append({"Uuid":plugin.uuid, "Name":plugin.name, "Description":plugin.description, "Author":plugin.author, "Icon": plugin.icon})
    }
  }

  Component.onCompleted: {
    refreshAppPluginsList()
  }
}
