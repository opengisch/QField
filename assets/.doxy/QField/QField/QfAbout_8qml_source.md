

# File QfAbout.qml

[**File List**](files.md) **>** [**app**](dir_5194e5fea318fda12687127c23f8aba1.md) **>** [**qml**](dir_408f7dc2ccecf14179dc3d37678eaa5b.md) **>** [**QfAbout.qml**](QfAbout_8qml.md)

[Go to the documentation of this file](QfAbout_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield.core
import org.qfield.gui

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
            font: QfTheme.strongFont
            color: QfTheme.light
            textFormat: Text.RichText
            wrapMode: Text.WordWrap

            text: {
              let links = '<a href="https://github.com/opengisch/QField/commit/' + Qfield.gitRevision + '">' + Qfield.gitRevision.substr(0, 7) + '</a>';
              if (Qfield.version && Qfield.version !== '1.0.0' && Qfield.version !== '0') {
                links += ' <a href="https://github.com/opengisch/QField/releases/tag/' + Qfield.version + '">' + Qfield.version + '</a>';
              }

              let title = Qfield.name;
              if (Qfield.name === "QField") {
                title += "<br>" + Qfield.versionString + " (" + links + ")";
              } else {
                title += "<br>" + qsTr("Powered by QField") + " (" + links + ")";
              }

              // the QGIS version has the format `<int>.<int>.<int>-<any text>`, so we get everything before the first `-`
              const dependencies = [["QGIS", Qfield.qgisVersion.split("-", 1)[0]], ["GDAL/OGR", Qfield.gdalVersion], ["Qt", Qfield.qtVersion]];
              return title + "<br>" + dependencies.map(pair => pair.join(" ")).join(" | ");
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
            font: QfTheme.strongFont
            color: QfTheme.light
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
      font: QfTheme.tinyFont
      color: QfTheme.secondaryTextColor
      textFormat: Text.RichText
      wrapMode: Text.WordWrap

      text: {
        let label = '';
        let isDesktopPlatform = Qt.platform.os !== "ios" && Qt.platform.os !== "android";
        let dataDirs = platformUtilities.appDataDirs();
        if (dataDirs.length > 0) {
          label = dataDirs.length > 1 ? qsTr('%1 app directories').arg(Qfield.name) : qsTr('%1 app directory').arg(Qfield.name);
          for (let dataDir of dataDirs) {
            if (isDesktopPlatform) {
              label += '<br><a href="' + QfUrlUtils.fromString(dataDir) + '">' + dataDir + '</a>';
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
      icon.source: QfTheme.getThemeVectorIcon('ic_sponsor_white_24dp')
      enabled: Qfield.name === "QField"
      visible: enabled

      text: qsTr('Support QField')
      onClicked: Qt.openUrlExternally("https://github.com/sponsors/opengisch")
    }

    QfButton {
      id: linksButton
      dropdown: Qfield.name === "QField"
      Layout.fillWidth: true
      icon.source: QfTheme.getThemeVectorIcon('ic_book_white_24dp')

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

      font: QfTheme.defaultFont
      height: 48
      leftPadding: QfTheme.menuItemLeftPadding
      icon.source: QfTheme.getThemeVectorIcon('ic_speaker_white_24dp')

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
```


