import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qgis
import org.qfield
import Theme

/**
 * \ingroup qml
 */
QfPopup {
  id: popup
  focus: true

  property alias model: deltaList.model

  function deltaStatusColor(status) {
    switch (status) {
    case DeltaListModel.AppliedStatus:
      return Theme.mainColor;
    case DeltaListModel.PendingStatus:
    case DeltaListModel.BusyStatus:
      return Theme.cloudColor;
    case DeltaListModel.ConflictStatus:
    case DeltaListModel.NotAppliedStatus:
      return Theme.warningColor;
    case DeltaListModel.ErrorStatus:
    case DeltaListModel.UnpermittedStatus:
      return Theme.errorColor;
    default:
      return Theme.secondaryTextColor;
    }
  }

  function deltaStatusLabel(status) {
    switch (status) {
    case DeltaListModel.AppliedStatus:
      return qsTr('Applied');
    case DeltaListModel.PendingStatus:
      return qsTr('Pending');
    case DeltaListModel.BusyStatus:
      return qsTr('Busy');
    case DeltaListModel.ConflictStatus:
      return qsTr('Conflict');
    case DeltaListModel.NotAppliedStatus:
      return qsTr('Not applied');
    case DeltaListModel.ErrorStatus:
      return qsTr('Error');
    case DeltaListModel.IgnoredStatus:
      return qsTr('Ignored');
    case DeltaListModel.UnpermittedStatus:
      return qsTr('Unpermitted');
    default:
      return qsTr('Unknown');
    }
  }

  function deltaStatusIcon(status) {
    switch (status) {
    case DeltaListModel.AppliedStatus:
      return Theme.getThemeVectorIcon('ic_check_white_24dp');
    case DeltaListModel.PendingStatus:
    case DeltaListModel.BusyStatus:
      return Theme.getThemeVectorIcon('ic_cloud_active_24dp');
    default:
      return Theme.getThemeVectorIcon('ic_error_outline_24dp');
    }
  }

  width: Math.min(400, mainWindow.width - Theme.popupScreenEdgeHorizontalMargin * 2)
  height: page.height + padding * 2
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

    Keys.onReleased: event => {
      if (event.key === Qt.Key_Back || event.key === Qt.Key_Escape) {
        event.accepted = true;
        popup.close();
      }
    }

    width: parent.width
    height: {
      const chromeHeight = toolBar.childrenRect.height + 20;
      const maximumHeight = mainWindow.height - Math.max(Theme.popupScreenEdgeVerticalMargin * 2, mainWindow.sceneTopMargin * 2 + 4, mainWindow.sceneBottomMargin * 2 + 4);
      let contentHeight = deltaList.contentHeight;
      if (!popup.model) {
        contentHeight = loadingIndicator.implicitHeight;
      } else if (deltaList.count === 0) {
        contentHeight = emptyLabel.implicitHeight;
      }
      return Math.min(contentHeight + chromeHeight, maximumHeight);
    }
    padding: 5
    background: null
    header: ToolBar {
      id: toolBar
      height: Theme.toolButtonSize

      topPadding: 0
      leftPadding: 0
      rightPadding: 0
      bottomPadding: 0

      background: Rectangle {
        color: "transparent"
      }

      Label {
        anchors.centerIn: parent
        leftPadding: Theme.toolButtonSize
        rightPadding: Theme.toolButtonSize
        width: parent.width - 20
        text: qsTr("Upload History")
        font: Theme.strongFont
        color: Theme.mainTextColor
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

      Item {
        id: loadingIndicator
        Layout.fillWidth: true
        implicitHeight: loadingContent.height + 30
        visible: !popup.model

        Column {
          id: loadingContent
          anchors.centerIn: parent
          width: parent.width
          spacing: 10

          BusyIndicator {
            id: busyIndicator
            anchors.horizontalCenter: parent.horizontalCenter
            width: 48
            height: 48
          }

          Label {
            width: parent.width
            font: Theme.tipFont
            color: Theme.secondaryTextColor
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
            text: qsTr("Fetching upload history…")
          }
        }
      }

      Label {
        id: emptyLabel
        Layout.fillWidth: true
        leftPadding: 48
        rightPadding: 48
        width: parent.width
        visible: !!popup.model && deltaList.count === 0

        text: qsTr("No changes have been uploaded yet!")
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
        spacing: 5
        ScrollBar.vertical: QfScrollBar {}

        delegate: QfCollapsibleMessage {
          width: parent ? parent.width : undefined
          color: popup.deltaStatusColor(Status)
          detailsColor: Theme.secondaryTextColor
          font: Theme.tipFont
          iconSource: popup.deltaStatusIcon(Status)
          titleText: popup.deltaStatusLabel(Status) + ' • ' + new Date(CreatedAt).toLocaleString(Qt.locale(), Locale.ShortFormat)
          detailsText: Output
        }
      }
    }
  }
}
