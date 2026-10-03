

# File QfChangelog.qml

[**File List**](files.md) **>** [**app**](dir_5194e5fea318fda12687127c23f8aba1.md) **>** [**qml**](dir_408f7dc2ccecf14179dc3d37678eaa5b.md) **>** [**QfChangelog.qml**](QfChangelog_8qml.md)

[Go to the documentation of this file](QfChangelog_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield.core
import org.qfield.app
import org.qfield.gui

QfPopup {
  id: changelogPopup

  parent: mainWindow.contentItem
  width: mainWindow.width - QfTheme.popupScreenEdgeHorizontalMargin * 2
  height: mainWindow.height - Math.max(QfTheme.popupScreenEdgeVerticalMargin * 2, mainWindow.sceneTopMargin * 2 + 4, mainWindow.sceneBottomMargin * 2 + 4)
  x: QfTheme.popupScreenEdgeHorizontalMargin
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
            visible: changelogContents.status != QfChangelogContents.LoadingStatus

            color: QfTheme.mainTextColor
            font: QfTheme.tipFont

            fontSizeMode: Text.VerticalFit
            textFormat: Text.MarkdownText
            wrapMode: Text.WordWrap

            text: {
              switch (changelogContents.status) {
              case QfChangelogContents.IdleStatus:
              case QfChangelogContents.LoadingStatus:
                return '';
              case QfChangelogContents.SuccessStatus:
                return changelogContents.markdown;
              case QfChangelogContents.ErrorStatus:
                return qsTr('Error while fetching changelog, try again later.');
              }
            }

            onLinkActivated: link => {
              Qt.openUrlExternally(link);
            }
          }

          BusyIndicator {
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            running: changelogContents.status === QfChangelogContents.LoadingStatus
            visible: running
          }
        }
      }

      QfButton {
        id: sponsorshipButton
        Layout.fillWidth: true
        icon.source: QfTheme.getThemeVectorIcon('ic_sponsor_white_24dp')

        text: qsTr('Support QField')
        onClicked: Qt.openUrlExternally("https://github.com/sponsors/opengisch")
      }
    }
  }

  QfChangelogContents {
    id: changelogContents
    onMarkdownChanged: {
      if (changelogContents.markdown) {
        settings.setValue("/QField/isLoadingChangelog", false);
        settings.remove("/QField/isCrashingSslDevice");
      }
    }
  }

  onClosed: {
    settings.setValue("/QField/ChangelogVersion", Qfield.version);
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
    if (changelogContents.status === QfChangelogContents.SuccessStatus || changelogContents.status === QfChangelogContents.LoadingStatus) {
      return;
    }
    settings.remove("/QField/isLoadingChangelog");
    settings.setValue("/QField/isLoadingChangelog", true);
    settings.sync();
    changelogContents.request();
  }
}
```


