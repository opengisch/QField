

# File PluginItem.qml

[**File List**](files.md) **>** [**qml**](dir_1d81f7f4362e4398c0a695c4e466fcb0.md) **>** [**PluginItem.qml**](PluginItem_8qml.md)

[Go to the documentation of this file](PluginItem_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield
import Theme

Rectangle {
  height: pluginItemContent.height + 10
  color: "transparent"

  property alias icon: pluginImage.source
  property alias name: pluginName.text

  property bool itemEnabled
  property bool itemConfigurable
  property bool itemDownloading: false

  signal authorDetailsClicked
  signal uninstallConfirmationClicked
  signal toggleEnabledPlugin(bool checked)
  signal pluginNameClicked
  signal configClicked
  signal updateClicked
  signal downloadClicked

  GridLayout {
    id: pluginItemContent
    anchors.left: parent.left
    anchors.right: parent.right

    columns: 4
    columnSpacing: 0
    rowSpacing: 2

    RowLayout {
      Layout.fillWidth: true

      Image {
        id: pluginImage
        Layout.preferredWidth: 24
        Layout.preferredHeight: 24

        fillMode: Image.PreserveAspectFit
        mipmap: true
      }

      Label {
        id: pluginName
        Layout.fillWidth: true
        font: Theme.defaultFont
        color: Theme.mainTextColor
        wrapMode: Text.WordWrap

        MouseArea {
          anchors.fill: parent
          onClicked: {
            pluginNameClicked();
          }
        }
      }
    }

    QfToolButton {
      id: configureEnabledPlugin
      enabled: Configurable
      Layout.preferredWidth: enabled ? 48 : 0

      iconSource: Theme.getThemeVectorIcon("ic_tune_white_24dp")
      iconColor: Theme.mainTextColor

      onClicked: {
        configClicked();
      }
    }

    QfToolButton {
      id: updatePlugin
      enabled: InstalledLocally && AvailableUpdate
      Layout.preferredWidth: enabled ? 48 : 0

      iconSource: Theme.getThemeVectorIcon("ic_update_white_24dp")
      iconColor: Theme.mainColor

      onClicked: {
        updateClicked();
      }
    }

    QfToolButton {
      id: downloadPlugin
      enabled: !InstalledLocally && !itemDownloading
      Layout.preferredWidth: enabled ? 48 : 0
      visible: enabled
      iconSource: Theme.getThemeVectorIcon('ic_download_white_24dp')
      iconColor: Theme.mainColor

      onClicked: {
        downloadClicked();
      }
    }

    BusyIndicator {
      id: busyIndicator
      Layout.preferredWidth: itemDownloading ? 48 : 0
      Layout.preferredHeight: 48
      running: itemDownloading
      visible: !InstalledLocally && itemDownloading
    }

    QfSwitch {
      Layout.preferredWidth: implicitContentWidth
      checked: itemEnabled
      visible: InstalledLocally

      onClicked: {
        toggleEnabledPlugin(checked);
      }
    }

    ColumnLayout {
      Layout.columnSpan: 3
      Layout.fillWidth: true

      Label {
        Layout.fillWidth: true
        text: qsTr('Authored by %1%2%3').arg('<a href="details">').arg(Author).arg(Trusted ? '' : ' ⚠</a>')
        font: Theme.tipFont
        color: Theme.secondaryTextColor
        wrapMode: Text.WordWrap
        onLinkActivated: link => {
          authorDetailsClicked();
        }
      }

      Label {
        Layout.fillWidth: true
        text: Description
        font: Theme.tipFont
        color: Theme.secondaryTextColor
        wrapMode: Text.WordWrap
      }

      Label {
        Layout.fillWidth: true
        text: "<a href='delete'>" + (Version != "" ? qsTr("Uninstall version %1").arg(Version) : qsTr("Uninstall plugin")) + "</a>"
        font: Theme.tipFont
        color: Theme.secondaryTextColor
        wrapMode: Text.WordWrap
        visible: InstalledLocally && !Bundled

        onLinkActivated: link => {
          uninstallConfirmationClicked();
        }
      }
    }
  }
}
```


