

# File QFieldCloudDeltaHistory.qml

[**File List**](files.md) **>** [**qml**](dir_1d81f7f4362e4398c0a695c4e466fcb0.md) **>** [**QFieldCloudDeltaHistory.qml**](QFieldCloudDeltaHistory_8qml.md)

[Go to the documentation of this file](QFieldCloudDeltaHistory_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qgis
import org.qfield
import Theme

QfPopup {
  id: popup

  property alias model: deltaList.model

  width: Math.min(400, mainWindow.width - Theme.popupScreenEdgeHorizontalMargin * 2)
  height: page.height
  x: (mainWindow.width - width) / 2
  y: (mainWindow.height - height) / 2

  onOpened: function () {
    if (cloudProjectsModel.currentProjectId) {
      cloudProjectsModel.refreshProjectDeltaList(cloudProjectsModel.currentProjectId);
    }
  }

  onClosed: function () {
    deltaList.model = null;
  }

  Page {
    id: page
    width: parent.width
    height: Math.min(deltaList.contentHeight + toolBar.childrenRect.height + 20, mainWindow.height - Math.max(Theme.popupScreenEdgeVerticalMargin * 2, mainWindow.sceneTopMargin * 2 + 4, mainWindow.sceneBottomMargin * 2 + 4))
    padding: 5
    header: ToolBar {
      id: toolBar
      height: 48

      topPadding: 0
      leftPadding: 0
      rightPadding: 0
      bottomPadding: 0

      background: Rectangle {
        color: "transparent"
      }

      Label {
        anchors.centerIn: parent
        leftPadding: 48
        rightPadding: 48
        width: parent.width - 20
        text: !!model ? qsTr("Push History") : qsTr("Loading…")
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
        iconSource: Theme.getThemeVectorIcon('ic_close_white_24dp')
        iconColor: Theme.mainTextColor
        bgcolor: Theme.mainBackgroundColor

        onClicked: {
          popup.close();
        }
      }
    }

    ColumnLayout {
      anchors.fill: parent
      spacing: 4

      Label {
        Layout.fillWidth: true
        leftPadding: 48
        rightPadding: 48
        width: parent.width
        visible: deltaList.count === 0

        text: qsTr("No changes have been pushed yet!")
        color: Theme.mainTextDisabledColor
        horizontalAlignment: Text.AlignHCenter
        wrapMode: Text.WordWrap
      }

      ListView {
        id: deltaList
        Layout.fillWidth: true
        Layout.fillHeight: true
        visible: count !== 0
        clip: true
        ScrollBar.vertical: QfScrollBar {}

        delegate: Rectangle {
          id: rectangle
          width: parent ? parent.width : undefined
          height: inner.height
          color: "transparent"

          ColumnLayout {
            id: inner
            width: parent.width

            Text {
              Layout.fillWidth: true
              topPadding: 5
              leftPadding: 5
              text: {
                var dt = new Date(CreatedAt);
                return dt.toLocaleString();
              }
              font: Theme.defaultFont
              color: Theme.mainTextColor
              wrapMode: Text.WordWrap
            }

            Text {
              Layout.fillWidth: true
              leftPadding: 5
              bottomPadding: 5
              text: {
                var status = '';
                switch (Status) {
                case DeltaListModel.PendingStatus:
                  status = 'pending';
                  break;
                case DeltaListModel.BusyStatus:
                  status = 'busy';
                  break;
                case DeltaListModel.AppliedStatus:
                  status = 'applied';
                  break;
                case DeltaListModel.ConflictStatus:
                  status = 'conflict';
                  break;
                case DeltaListModel.NotAppliedStatus:
                  status = 'not applied';
                  break;
                case DeltaListModel.ErrorStatus:
                  status = 'error';
                  break;
                }
                return 'Status: ' + status + (Output != '' ? ' (' + Output + ')' : '');
              }
              font: Theme.tipFont
              color: Theme.secondaryTextColor
              wrapMode: Text.WordWrap
            }
          }
        }
      }
    }
  }
}
```


