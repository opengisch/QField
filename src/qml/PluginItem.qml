import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Theme

/**
 * \ingroup qml
 */
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
  signal downloadClicked

  GridLayout {
    id: pluginItemContent
    anchors.left: parent.left
    anchors.right: parent.right

    columns: 3
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
      Layout.preferredWidth: enabled ? 48 : 0
      enabled: itemConfigurable

      iconSource: Theme.getThemeVectorIcon("ic_tune_white_24dp")
      iconColor: Theme.mainTextColor

      onClicked: {
        configClicked();
      }
    }

    QfSwitch {
      Layout.preferredWidth: implicitContentWidth
      checked: itemEnabled
      visible: InstalledLocally

      onClicked: {
        toggleEnabledPlugin(checked);
      }
    }

    BusyIndicator {
      id: busyIndicator
      running: itemDownloading
      visible: !InstalledLocally && itemDownloading
    }

    QfButton {
      Layout.preferredHeight: 40
      Layout.preferredWidth: 40
      visible: !InstalledLocally && !itemDownloading
      icon.source: Theme.getThemeVectorIcon('ic_download_white_24dp')
      color: "transparent"
      onClicked: {
        downloadClicked();
      }
    }

    ColumnLayout {
      Layout.columnSpan: 2
      Layout.fillWidth: true

      Label {
        Layout.fillWidth: true
        text: qsTr('Authored by %1%2%3').arg('<a href="details">').arg(Author).arg(' ⚠</a>')
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
        visible: InstalledLocally

        onLinkActivated: link => {
          uninstallConfirmationClicked();
        }
      }
    }
  }
}
