import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qgis
import org.qfield.core
import org.qfield.gui

/**
 * \ingroup qml
 */
QfPopup {
  id: popup

  property alias model: deltaListModel

  parent: mainWindow.contentItem
  x: (parent.width - width) / 2
  y: (parent.height - height) / 2
  closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
  focus: visible

  width: Math.min(400, mainWindow.width - Theme.popupScreenEdgeHorizontalMargin * 2)
  height: page.height + padding * 2

  onAboutToShow: {
    if (model.cloudProjectId != "") {
      model.refresh();
    }
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
      const headerHeight = toolBar.childrenRect.height + 20;
      const maximumHeight = mainWindow.height - Math.max(Theme.popupScreenEdgeVerticalMargin * 2, mainWindow.sceneTopMargin * 2 + 4, mainWindow.sceneBottomMargin * 2 + 4);
      let contentHeight = 0;
      if (popup.model.isRefreshing || deltaList.count === 0) {
        contentHeight = loadingIndicator.height;
      } else {
        contentHeight = deltaList.contentHeight;
      }

      return Math.min(headerHeight + contentHeight, maximumHeight);
    }
    padding: 5
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
      width: parent.width
      height: parent.height
      spacing: 4

      Item {
        id: loadingIndicator
        Layout.fillWidth: true
        Layout.preferredHeight: loadingContent.height + 30
        visible: popup.model.isRefreshing

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
        visible: !popup.model.isRefreshing && deltaList.count === 0

        font: Theme.tipFont
        color: Theme.mainTextDisabledColor
        horizontalAlignment: Text.AlignHCenter
        wrapMode: Text.WordWrap
        text: qsTr("No changes have been uploaded yet!")
      }

      ListView {
        id: deltaList
        Layout.fillWidth: true
        Layout.fillHeight: true
        visible: count !== 0
        clip: true
        spacing: 5
        ScrollBar.vertical: QfScrollBar {}

        model: DeltaListModel {
          id: deltaListModel
        }

        delegate: QfCollapsibleMessage {
          width: parent ? parent.width : undefined
          color: popup.deltaStatusColor(Status)
          detailsColor: Theme.secondaryTextColor
          font: Theme.tipFont
          iconSource: popup.deltaStatusIcon(Status)
          titleText: Summary + "\n" + qsTr("Uploaded by %1 on %2").arg(CreatedBy).arg(CreatedAt.toLocaleString(Qt.locale(), Locale.ShortFormat))
          detailsText: Output
        }
      }
    }
  }

  function deltaStatusColor(status) {
    switch (status) {
    case QFieldCloudDelta.AppliedStatus:
      return Theme.mainColor;
    case QFieldCloudDelta.PendingStatus:
    case QFieldCloudDelta.BusyStatus:
      return Theme.cloudColor;
    case QFieldCloudDelta.ConflictStatus:
    case QFieldCloudDelta.NotAppliedStatus:
      return Theme.warningColor;
    case QFieldCloudDelta.ErrorStatus:
    case QFieldCloudDelta.UnpermittedStatus:
      return Theme.errorColor;
    default:
      return Theme.secondaryTextColor;
    }
  }

  function deltaStatusLabel(status) {
    switch (status) {
    case QFieldCloudDelta.AppliedStatus:
      return qsTr('Applied');
    case QFieldCloudDelta.PendingStatus:
      return qsTr('Pending');
    case QFieldCloudDelta.BusyStatus:
      return qsTr('Busy');
    case QFieldCloudDelta.ConflictStatus:
      return qsTr('Conflict');
    case QFieldCloudDelta.NotAppliedStatus:
      return qsTr('Not applied');
    case QFieldCloudDelta.ErrorStatus:
      return qsTr('Error');
    case QFieldCloudDelta.IgnoredStatus:
      return qsTr('Ignored');
    case QFieldCloudDelta.UnpermittedStatus:
      return qsTr('Unpermitted');
    default:
      return qsTr('Unknown');
    }
  }

  function deltaStatusIcon(status) {
    switch (status) {
    case QFieldCloudDelta.AppliedStatus:
      return Theme.getThemeVectorIcon('ic_check_white_24dp');
    case QFieldCloudDelta.PendingStatus:
    case QFieldCloudDelta.BusyStatus:
      return Theme.getThemeVectorIcon('ic_hourglass_black_24dp');
    default:
      return Theme.getThemeVectorIcon('ic_error_outline_24dp');
    }
  }
}
