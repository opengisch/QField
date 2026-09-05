

# File QfCloudDeltaHistory.qml

[**File List**](files.md) **>** [**app**](dir_5194e5fea318fda12687127c23f8aba1.md) **>** [**qml**](dir_408f7dc2ccecf14179dc3d37678eaa5b.md) **>** [**QfCloudDeltaHistory.qml**](QfCloudDeltaHistory_8qml.md)

[Go to the documentation of this file](QfCloudDeltaHistory_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qgis
import org.qfield.core
import org.qfield.gui

QfPopup {
  id: popup

  property alias model: deltaListModel

  parent: mainWindow.contentItem
  x: (parent.width - width) / 2
  y: (parent.height - height) / 2
  closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
  focus: visible

  width: Math.min(400, mainWindow.width - QfTheme.popupScreenEdgeHorizontalMargin * 2)
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
      const maximumHeight = mainWindow.height - Math.max(QfTheme.popupScreenEdgeVerticalMargin * 2, mainWindow.sceneTopMargin * 2 + 4, mainWindow.sceneBottomMargin * 2 + 4);
      let contentHeight = 0;
      if (popup.model.isRefreshing && deltaList.count === 0) {
        contentHeight = loadingIndicator.height;
      } else {
        contentHeight = deltaList.contentHeight;
      }

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
        text: qsTr("Upload History")
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
            font: QfTheme.tipFont
            color: QfTheme.secondaryTextColor
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

        font: QfTheme.tipFont
        color: QfTheme.mainTextDisabledColor
        horizontalAlignment: Text.AlignHCenter
        wrapMode: Text.WordWrap
        text: qsTr("No changes have been uploaded yet!")
      }

      ListView {
        id: deltaList

        property bool overshootFetchNextPage: false

        Layout.fillWidth: true
        Layout.fillHeight: true
        visible: count !== 0
        clip: true
        spacing: 5
        ScrollBar.vertical: QfScrollBar {}

        model: QfDeltaListModel {
          id: deltaListModel
        }

        delegate: QfCollapsibleMessage {
          width: parent ? parent.width : undefined
          color: popup.deltaStatusColor(Status)
          detailsColor: QfTheme.secondaryTextColor
          font: QfTheme.tipFont
          iconSource: popup.deltaStatusIcon(Status)
          titleText: Summary + "\n" + qsTr("Uploaded by %1 on %2").arg(CreatedBy).arg(CreatedAt.toLocaleString(Qt.locale(), Locale.ShortFormat))
          detailsText: Output
        }

        onMovingChanged: {
          if (!moving && overshootFetchNextPage && deltaListModel.hasNextPage) {
            deltaListModel.fetchNextPage();
          }
          overshootFetchNextPage = false;
        }

        onVerticalOvershootChanged: {
          if (verticalOvershoot > 10) {
            overshootFetchNextPage = true;
          }
        }
      }
    }
  }

  function deltaStatusColor(status) {
    switch (status) {
    case QfCloudDelta.AppliedStatus:
      return QfTheme.mainColor;
    case QfCloudDelta.PendingStatus:
    case QfCloudDelta.BusyStatus:
      return QfTheme.cloudColor;
    case QfCloudDelta.ConflictStatus:
    case QfCloudDelta.NotAppliedStatus:
      return QfTheme.warningColor;
    case QfCloudDelta.ErrorStatus:
    case QfCloudDelta.UnpermittedStatus:
      return QfTheme.errorColor;
    default:
      return QfTheme.secondaryTextColor;
    }
  }

  function deltaStatusLabel(status) {
    switch (status) {
    case QfCloudDelta.AppliedStatus:
      return qsTr('Applied');
    case QfCloudDelta.PendingStatus:
      return qsTr('Pending');
    case QfCloudDelta.BusyStatus:
      return qsTr('Busy');
    case QfCloudDelta.ConflictStatus:
      return qsTr('Conflict');
    case QfCloudDelta.NotAppliedStatus:
      return qsTr('Not applied');
    case QfCloudDelta.ErrorStatus:
      return qsTr('Error');
    case QfCloudDelta.IgnoredStatus:
      return qsTr('Ignored');
    case QfCloudDelta.UnpermittedStatus:
      return qsTr('Unpermitted');
    default:
      return qsTr('Unknown');
    }
  }

  function deltaStatusIcon(status) {
    switch (status) {
    case QfCloudDelta.AppliedStatus:
      return QfTheme.getThemeVectorIcon('ic_check_white_24dp');
    case QfCloudDelta.PendingStatus:
    case QfCloudDelta.BusyStatus:
      return QfTheme.getThemeVectorIcon('ic_hourglass_black_24dp');
    default:
      return QfTheme.getThemeVectorIcon('ic_error_outline_24dp');
    }
  }
}
```


