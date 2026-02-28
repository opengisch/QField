

# File Changelog.qml

[**File List**](files.md) **>** [**qml**](dir_1d81f7f4362e4398c0a695c4e466fcb0.md) **>** [**Changelog.qml**](Changelog_8qml.md)

[Go to the documentation of this file](Changelog_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield
import Theme

QfPopup {
  id: changelogPopup

  parent: mainWindow.contentItem
  width: mainWindow.width - Theme.popupScreenEdgeHorizontalMargin * 2
  height: mainWindow.height - Math.max(Theme.popupScreenEdgeVerticalMargin * 2, mainWindow.sceneTopMargin * 2 + 4, mainWindow.sceneBottomMargin * 2 + 4)
  x: Theme.popupScreenEdgeHorizontalMargin
  y: (mainWindow.height - height) / 2
  closePolicy: Popup.CloseOnEscape
  focus: visible

  Page {
    focus: true
    anchors.fill: parent
    padding: 5

    header: QfPageHeader {
      title: qsTr("What's new in QField")

      showApplyButton: false
      showCancelButton: false
      showBackButton: true

      onBack: {
        changelogPopup.close();
      }
    }

    ColumnLayout {
      anchors.fill: parent
      anchors.margins: 10

      Flickable {
        id: changelogFlickable
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.topMargin: 10
        Layout.bottomMargin: 10
        flickableDirection: Flickable.VerticalFlick
        interactive: true
        contentWidth: parent.width
        contentHeight: changelogGrid.height
        clip: true

        GridLayout {
          id: changelogGrid

          anchors.left: parent.left
          anchors.right: parent.right

          columns: 1

          Text {
            id: changelogBody
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.minimumHeight: contentHeight
            Layout.maximumHeight: contentHeight
            visible: changelogContents.status != ChangelogContents.LoadingStatus

            color: Theme.mainTextColor
            font: Theme.tipFont

            fontSizeMode: Text.VerticalFit
            textFormat: Text.MarkdownText
            wrapMode: Text.WordWrap

            text: {
              switch (changelogContents.status) {
              case ChangelogContents.IdleStatus:
              case ChangelogContents.LoadingStatus:
                return '';
              case ChangelogContents.SuccessStatus:
                return changelogContents.markdown;
              case ChangelogContents.ErrorStatus:
                return qsTr('Error while fetching changelog, try again later.');
              }
            }

            onLinkActivated: link => {
              Qt.openUrlExternally(link);
            }
          }

          BusyIndicator {
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

            visible: changelogContents.status == ChangelogContents.LoadingStatus
            running: visible
          }
        }
      }

      QfButton {
        id: sponsorshipButton
        Layout.fillWidth: true
        icon.source: Theme.getThemeVectorIcon('ic_sponsor_white_24dp')

        text: qsTr('Support QField')
        onClicked: Qt.openUrlExternally("https://github.com/sponsors/opengisch")
      }
    }
  }

  ChangelogContents {
    id: changelogContents
    onMarkdownChanged: {
      if (changelogContents.markdown) {
        settings.setValue("/QField/isLoadingChangelog", false);
        settings.remove("/QField/isCrashingSslDevice");
      }
    }
  }

  onClosed: {
    settings.setValue("/QField/ChangelogVersion", appVersion);
    changelogFlickable.contentY = 0;
  }

  onOpened: {
    if (settings.valueBool("/QField/isLoadingChangelog", false)) {
      settings.setValue("/QField/isCrashingSslDevice", true);
    } else {
      settings.remove("/QField/isCrashingSslDevice");
    }
    if (settings.valueBool("/QField/isCrashingSslDevice", false) === true) {
      changelogBody.text = qsTr("Check the latest QField changes on ") + ' <a href="https://github.com/opengisch/qfield/releases">' + qsTr('QField releases page') + '</a>.';
      return;
    }
    if (changelogContents.status === ChangelogContents.SuccessStatus || changelogContents.status === ChangelogContents.LoadingStatus)
      return;
    settings.remove("/QField/isLoadingChangelog");
    settings.setValue("/QField/isLoadingChangelog", true);
    settings.sync();
    changelogContents.request();
  }
}
```


