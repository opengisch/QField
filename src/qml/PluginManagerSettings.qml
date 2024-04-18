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
    header: ToolBar {
      id: toolBar
      height: 48

      background: Rectangle {
        color: "transparent"
      }

      Label {
        anchors.centerIn: parent
        leftPadding: 48
        rightPadding: 48
        width: parent.width - 20
        text: qsTr( "Plugins" )
        font: Theme.strongFont
        color: Theme.mainColor
        horizontalAlignment: Text.AlignHCenter
        wrapMode: Text.WordWrap
      }

      QfToolButton {
        id: closeButton
        anchors {
          top: parent.top
          right: parent.right
        }
        iconSource: Theme.getThemeIcon( 'ic_close_black_24dp' )
        iconColor: Theme.mainTextColor
        bgcolor: "transparent"

        onClicked: {
          popup.close();
        }
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
          height: inner.height
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
              }

              Label {
                Layout.fillWidth: true
                leftPadding: 4
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
          pluginManager.clearProjectPluginPermissions()
        }
      }

      QfButton {
        id: clearPermissionsButton
        Layout.fillWidth: true

        text: qsTr("Clear project permissions")

        onClicked: {
          pluginManager.clearProjectPluginPermissions()
        }
      }
    }
  }

  Component.onCompleted: {
    pluginsList.model.clear()

    for (const plugin of pluginManager.availableAppPlugins) {
      pluginsList.model.append({"Uuid":plugin.uuid, "Name":plugin.name, "Description":plugin.description, "Author":plugin.author, "Icon": plugin.icon})
    }
  }
}
