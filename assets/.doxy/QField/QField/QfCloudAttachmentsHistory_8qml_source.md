

# File QfCloudAttachmentsHistory.qml

[**File List**](files.md) **>** [**app**](dir_5194e5fea318fda12687127c23f8aba1.md) **>** [**qml**](dir_408f7dc2ccecf14179dc3d37678eaa5b.md) **>** [**QfCloudAttachmentsHistory.qml**](QfCloudAttachmentsHistory_8qml.md)

[Go to the documentation of this file](QfCloudAttachmentsHistory_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield.core
import org.qfield.gui

QfPopup {
  id: popup

  property alias model: attachmentsModel

  parent: mainWindow.contentItem
  x: (parent.width - width) / 2
  y: (parent.height - height) / 2
  closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
  focus: visible

  width: Math.min(400, mainWindow.width - QfTheme.popupScreenEdgeHorizontalMargin * 2)
  height: page.height + padding * 2

  onAboutToShow: {
    model.refresh();
  }

  onClosed: {
    model.cloudProject = null;
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
      const maximumHeight = mainWindow.height - Math.max(QfTheme.popupScreenEdgeVerticalMargin * 2, mainWindow.sceneTopMargin * 2 + 4, mainWindow.sceneBottomMargin * 2 + 4);
      const contentHeight = attachmentsList.count === 0 ? emptyLabel.height : attachmentsList.contentHeight;
      return Math.min(headerHeight + contentHeight, maximumHeight);
    }
    padding: 5
    header: ToolBar {
      id: toolBar
      height: QfTheme.toolButtonSize

      topPadding: 0
      leftPadding: 0
      rightPadding: 0
      bottomPadding: 0

      background: Rectangle {
        color: "transparent"
      }

      Label {
        anchors.centerIn: parent
        leftPadding: QfTheme.toolButtonSize
        rightPadding: QfTheme.toolButtonSize
        width: parent.width - 20
        text: qsTr("Attachments")
        font: QfTheme.strongFont
        color: QfTheme.mainTextColor
        horizontalAlignment: Text.AlignHCenter
        wrapMode: Text.WordWrap
      }

      QfToolButton {
        id: closeButton
        anchors {
          top: parent.top
          right: parent.right
        }
        iconSource: QfTheme.getThemeVectorIcon('ic_close_white_24dp')
        iconColor: QfTheme.mainTextColor
        bgcolor: QfTheme.mainBackgroundColor

        onClicked: {
          popup.close();
        }
      }
    }

    ColumnLayout {
      width: parent.width
      height: parent.height
      spacing: 4

      Label {
        id: emptyLabel
        Layout.fillWidth: true
        leftPadding: 48
        rightPadding: 48
        visible: attachmentsList.count === 0

        font: QfTheme.tipFont
        color: QfTheme.mainTextDisabledColor
        horizontalAlignment: Text.AlignHCenter
        wrapMode: Text.WordWrap
        text: qsTr("No attachments have been uploaded yet!")
      }

      ListView {
        id: attachmentsList

        Layout.fillWidth: true
        Layout.fillHeight: true
        visible: count !== 0
        clip: true
        spacing: 5
        ScrollBar.vertical: QfScrollBar {}

        model: QfCloudAttachmentsModel {
          id: attachmentsModel
        }

        delegate: QfCollapsibleMessage {
          width: parent ? parent.width : undefined
          color: popup.attachmentStatusColor(Status)
          detailsColor: QfTheme.secondaryTextColor
          font: QfTheme.tipFont
          iconSource: popup.attachmentStatusIcon(Status)
          titleText: FileName + "\n" + popup.attachmentStatusLabel(Status, Timestamp)
          detailsText: Status === QfCloudAttachmentsModel.FailedStatus ? popup.attachmentFailureReason(HttpCode) : ""
        }
      }
    }
  }

  function attachmentStatusColor(status) {
    switch (status) {
    case QfCloudAttachmentsModel.UploadedStatus:
      return QfTheme.mainColor;
    case QfCloudAttachmentsModel.PendingStatus:
    case QfCloudAttachmentsModel.LocalStatus:
      return QfTheme.cloudColor;
    case QfCloudAttachmentsModel.FailedStatus:
      return QfTheme.errorColor;
    default:
      return QfTheme.secondaryTextColor;
    }
  }

  function attachmentStatusLabel(status, timestamp) {
    const hasTimestamp = timestamp instanceof Date && !isNaN(timestamp);
    switch (status) {
    case QfCloudAttachmentsModel.UploadedStatus:
      return hasTimestamp ? qsTr('Uploaded on %1').arg(timestamp.toLocaleString(Qt.locale(), Locale.ShortFormat)) : qsTr('Uploaded');
    case QfCloudAttachmentsModel.PendingStatus:
      return qsTr('Waiting to be uploaded');
    case QfCloudAttachmentsModel.LocalStatus:
      return qsTr('Not uploaded yet, upload your changes to send it');
    case QfCloudAttachmentsModel.FailedStatus:
      return hasTimestamp ? qsTr('Upload failed on %1').arg(timestamp.toLocaleString(Qt.locale(), Locale.ShortFormat)) : qsTr('Upload failed');
    default:
      return qsTr('Unknown');
    }
  }

  function attachmentStatusIcon(status) {
    switch (status) {
    case QfCloudAttachmentsModel.UploadedStatus:
      return QfTheme.getThemeVectorIcon('ic_check_white_24dp');
    case QfCloudAttachmentsModel.PendingStatus:
    case QfCloudAttachmentsModel.LocalStatus:
      return QfTheme.getThemeVectorIcon('ic_hourglass_black_24dp');
    default:
      return QfTheme.getThemeVectorIcon('ic_error_outline_24dp');
    }
  }

  function attachmentFailureReason(httpCode) {
    switch (httpCode) {
    case 400:
      return qsTr('The file name contains invalid characters or the file is empty.');
    case 402:
      return qsTr("The project owner's available storage is full.");
    case 403:
      return qsTr('You do not have the permission to upload attachments to this project.');
    case 404:
      return qsTr('The cloud project could not be found or your access to it was revoked.');
    default:
      return qsTr('The server answered with HTTP code %1.').arg(httpCode);
    }
  }
}
```


