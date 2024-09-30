import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qgis
import org.qfield
import Theme

Popup {
  id: popup

  property alias model: deltaList.model

  width: Math.min(400, mainWindow.width - Theme.popupScreenEdgeMargin * 2)
  x: (parent.width - width) / 2
  y: (parent.height - page.height) / 2
  padding: 0

  onOpened: function () {
    page.height = mainWindow.height - 160 + 60;
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
    height: deltaList.height + 60
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

    Column {
      spacing: 4
      width: parent.width

      Label {
        leftPadding: 48
        rightPadding: 48
        width: parent.width
        visible: !!model && model.rowCount === 0

        text: qsTr("No changes have been pushed yet!")
        color: Theme.mainTextDisabledColor
        horizontalAlignment: Text.AlignHCenter
        wrapMode: Text.WordWrap
      }

      ListView {
        id: deltaList
        width: parent.width
        height: visible ? mainWindow.height - 160 : 0
        visible: deltaList && deltaList.model !== undefined && deltaList.model.rowCount !== 0
        clip: true

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
