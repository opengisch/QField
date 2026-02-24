import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield
import Theme

/**
 * \ingroup qml
 */
Item {
  id: aboutPanel

  visible: false
  focus: visible

  Rectangle {
    color: "black"
    opacity: 0.8
    anchors.fill: parent
  }

  ColumnLayout {
    id: aboutContainer
    spacing: 6
    anchors.fill: parent
    anchors.margins: 20
    anchors.topMargin: 20 + mainWindow.sceneTopMargin
    anchors.bottomMargin: 20 + mainWindow.sceneBottomMargin

    ScrollView {
      Layout.fillWidth: true
      Layout.fillHeight: true
      ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
      ScrollBar.vertical: QfScrollBar {}
      contentItem: information
      contentWidth: information.width
      contentHeight: information.height
      clip: true

      MouseArea {
        anchors.fill: parent
        onClicked: aboutPanel.visible = false
      }

      ColumnLayout {
        id: information
        spacing: 6
        width: aboutPanel.width - 40
        height: Math.max(mainWindow.height - sponsorshipButton.height - linksButton.height - qfieldAppDirectoryLabel.height - aboutContainer.spacing * 3 - aboutContainer.anchors.topMargin - aboutContainer.anchors.bottomMargin - 10, qfieldPart.height + opengisPart.height + spacing)

        ColumnLayout {
          id: qfieldPart
          Layout.fillWidth: true
          Layout.fillHeight: true

          MouseArea {
            Layout.preferredWidth: 138
            Layout.preferredHeight: 138
            Layout.alignment: Qt.AlignHCenter
            Image {
              id: qfieldLogo
              width: parent.width
              height: parent.height
              fillMode: Image.PreserveAspectFit
              source: "qrc:/images/app_logo.svg"
              sourceSize.width: width * screen.devicePixelRatio
              sourceSize.height: height * screen.devicePixelRatio
            }
            onClicked: Qt.openUrlExternally("https://qfield.org/")
          }

          Label {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignCenter
            horizontalAlignment: Text.AlignHCenter
            font: Theme.strongFont
            color: Theme.light
            textFormat: Text.RichText
            wrapMode: Text.WordWrap

            text: {
              let links = '<a href="https://github.com/opengisch/QField/commit/' + gitRev + '">' + gitRev.substr(0, 7) + '</a>';
              if (appVersion && appVersion !== '1.0.0') {
                links += ' <a href="https://github.com/opengisch/QField/releases/tag/' + appVersion + '">' + appVersion + '</a>';
              }
              // the `qgisVersion` has the format `<int>.<int>.<int>-<any text>`, so we get everything before the first `-`
              const qgisVersionWithoutName = qgisVersion.split("-", 1)[0];
              const dependencies = [["QGIS", qgisVersionWithoutName], ["GDAL/OGR", gdalVersion], ["Qt", qVersion]];
              const dependenciesStr = dependencies.map(pair => pair.join(" ")).join(" | ");
              return "QField<br>" + appVersionStr + " (" + links + ")<br>" + dependenciesStr;
            }

            onLinkActivated: link => Qt.openUrlExternally(link)
          }
        }

        ColumnLayout {
          id: opengisPart
          Layout.fillWidth: true
          Layout.fillHeight: true

          MouseArea {
            Layout.preferredWidth: 91
            Layout.preferredHeight: 113
            Layout.alignment: Qt.AlignHCenter
            Image {
              id: opengisLogo
              width: parent.width
              height: parent.height
              fillMode: Image.PreserveAspectFit
              source: "qrc:/images/opengis-logo.svg"
              sourceSize.width: width * screen.devicePixelRatio
              sourceSize.height: height * screen.devicePixelRatio
            }
            onClicked: Qt.openUrlExternally("https://opengis.ch")
          }

          Label {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignCenter
            horizontalAlignment: Text.AlignHCenter
            font: Theme.strongFont
            color: Theme.light
            textFormat: Text.RichText
            text: qsTr("Developed by") + '<br><a href="https://opengis.ch">OPENGIS.ch</a>'
            onLinkActivated: link => Qt.openUrlExternally(link)
          }
        }
      }
    }

    Label {
      id: qfieldAppDirectoryLabel
      Layout.fillWidth: true
      Layout.maximumWidth: parent.width
      Layout.alignment: Qt.AlignCenter
      Layout.bottomMargin: 10
      horizontalAlignment: Text.AlignHCenter
      font: Theme.tinyFont
      color: Theme.secondaryTextColor
      textFormat: Text.RichText
      wrapMode: Text.WordWrap

      text: {
        let label = '';
        let isDesktopPlatform = Qt.platform.os !== "ios" && Qt.platform.os !== "android";
        let dataDirs = platformUtilities.appDataDirs();
        if (dataDirs.length > 0) {
          label = dataDirs.length > 1 ? qsTr('%1 app directories').arg(appName) : qsTr('%1 app directory').arg(appName);
          for (let dataDir of dataDirs) {
            if (isDesktopPlatform) {
              label += '<br><a href="' + UrlUtils.fromString(dataDir) + '">' + dataDir + '</a>';
            } else {
              label += '<br>' + dataDir;
            }
          }
        }
        return label;
      }

      onLinkActivated: link => Qt.openUrlExternally(link)
    }

    QfButton {
      id: sponsorshipButton
      Layout.fillWidth: true
      icon.source: Theme.getThemeVectorIcon('ic_sponsor_white_24dp')
      enabled: appName == "QField"
      visible: enabled

      text: qsTr('Support QField')
      onClicked: Qt.openUrlExternally("https://github.com/sponsors/opengisch")
    }

    QfButton {
      id: linksButton
      dropdown: true
      Layout.fillWidth: true
      icon.source: Theme.getThemeVectorIcon('ic_book_white_24dp')

      text: qsTr('Documentation')

      onClicked: {
        Qt.openUrlExternally("https://docs.qfield.org/");
      }

      onDropdownClicked: {
        linksMenu.popup(linksButton.width - linksMenu.width + 10, linksButton.y + 10);
      }
    }
  }

  QfMenu {
    id: linksMenu
    title: qsTr("Links Menu")

    MenuItem {
      text: qsTr('Changelog')

      font: Theme.defaultFont
      height: 48
      leftPadding: Theme.menuItemLeftPadding
      icon.source: Theme.getThemeVectorIcon('ic_speaker_white_24dp')

      onTriggered: {
        changelogPopup.open();
      }
    }
  }

  Keys.onReleased: event => {
    if (event.key === Qt.Key_Back || event.key === Qt.Key_Escape) {
      event.accepted = true;
      visible = false;
    }
  }
}
