

# File MessageLog.qml

[**File List**](files.md) **>** [**qml**](dir_1d81f7f4362e4398c0a695c4e466fcb0.md) **>** [**MessageLog.qml**](MessageLog_8qml.md)

[Go to the documentation of this file](MessageLog_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield
import Theme

Page {
  id: messageLog

  property alias model: table.model
  property int unreadMessagesCount: 0
  property bool unreadMessages: false

  signal finished

  visible: false
  focus: visible
  leftPadding: mainWindow.sceneLeftMargin
  rightPadding: mainWindow.sceneRightMargin

  header: QfPageHeader {
    title: qsTr('Message Logs')

    showBackButton: true
    showApplyButton: false
    showCancelButton: false

    topMargin: mainWindow.sceneTopMargin

    onFinished: messageLog.finished()
  }

  ColumnLayout {
    anchors.margins: 8
    anchors.bottomMargin: 8 + mainWindow.sceneBottomMargin
    anchors.fill: parent
    Layout.margins: 0
    spacing: 10

    Rectangle {
      Layout.fillWidth: true
      Layout.fillHeight: true
      color: Theme.controlBackgroundColor
      border.color: Theme.controlBorderColor
      border.width: 1

      ListView {
        id: table
        objectName: 'messagesList'
        flickableDirection: Flickable.VerticalFlick
        boundsBehavior: Flickable.StopAtBounds
        ScrollBar.vertical: QfScrollBar {}
        clip: true
        anchors.fill: parent
        spacing: 2

        delegate: Rectangle {
          id: rectangle
          objectName: 'messageItem_' + index
          width: parent ? parent.width : undefined
          height: line.height
          color: "transparent"

          Row {
            id: line
            spacing: 5
            Text {
              id: datetext
              objectName: 'dateText'
              padding: 5
              text: MessageDateTime.replace(' ', '\n')
              font: Theme.tipFont
              color: Theme.secondaryTextColor
            }
            Rectangle {
              id: separator
              width: 0
            }
            Text {
              id: tagtext
              objectName: 'tagText'
              padding: MessageTag ? 5 : 0
              text: MessageTag
              font.pointSize: Theme.tipFont.pointSize
              font.bold: true
              color: Theme.secondaryTextColor
            }
            Text {
              id: messagetext
              objectName: 'messageText'
              padding: 5
              width: rectangle.width - datetext.width - tagtext.width - separator.width - 3 * line.spacing
              text: Message.replace(new RegExp('\n', "gi"), '<br>')
              font: Theme.tipFont
              color: Theme.mainTextColor
              wrapMode: Text.WordWrap
              textFormat: Text.RichText

              MouseArea {
                anchors.fill: parent
                onClicked: {
                  copyHelper.text = messagetext.text;
                  copyHelper.selectAll();
                  copyHelper.copy();
                  displayToast(qsTr("Message text copied"));
                }
              }
            }
          }
        }
      }
    }

    TextEdit {
      id: copyHelper
      visible: false
    }

    QfButton {
      text: qsTr("Log runtime profiler")
      Layout.fillWidth: true

      onClicked: {
        iface.logRuntimeProfiler();
      }
    }

    QfButton {
      text: qsTr("Clear message log")
      Layout.fillWidth: true

      onClicked: {
        table.model.clear();
        displayToast(qsTr("Message log cleared"));
        messageLog.finished();
      }
    }

    QfButton {
      id: submitLog
      Layout.fillWidth: true
      text: qsTr("Send application log")
      visible: qfieldSettings.enableInfoCollection && platformUtilities.capabilities & PlatformUtilities.SentryFramework

      onClicked: {
        applicationLogDialog.open();
      }
    }
  }

  QfDialog {
    id: applicationLogDialog
    title: qsTr("Send application log")
    focus: visible
    parent: mainWindow.contentItem

    onAboutToShow: {
      appliationLogInput.text = '';
    }

    Column {
      width: childrenRect.width
      height: childrenRect.height
      spacing: 10

      TextMetrics {
        id: applicationLogLabelMetrics
        font: applicationLogLabel.font
        text: applicationLogLabel.text
      }

      Label {
        id: applicationLogLabel
        width: mainWindow.width - 60 < applicationLogLabelMetrics.width ? mainWindow.width - 60 : applicationLogLabelMetrics.width
        text: qsTr("This will send a log of your current session to the development team. You only need to do this when you are asked for it.")
        wrapMode: Text.WordWrap
        font: Theme.defaultFont
        color: Theme.mainTextColor
      }

      TextField {
        id: appliationLogInput
        width: applicationLogLabel.width
        placeholderText: qsTr("Type optional details")
      }

      CheckBox {
        id: includeCloudInformationCheckBox
        width: applicationLogLabel.width
        topPadding: 5
        bottomPadding: 5
        text: qsTr('Include cloud user details')
        font: Theme.defaultFont
        visible: cloudConnection.status === QFieldCloudConnection.LoggedIn
        checked: cloudConnection.status === QFieldCloudConnection.LoggedIn
        indicator.height: 16
        indicator.width: 16
        indicator.implicitHeight: 24
        indicator.implicitWidth: 24
      }
    }

    onAccepted: {
      var applicationLogMessage = appliationLogInput.text.trim();
      iface.sendLog(applicationLogMessage != '' ? applicationLogMessage : 'Manual log submission', includeCloudInformationCheckBox.checked ? cloudConnection.username : '');
      displayToast(qsTr("Your application log is being sent…"));
    }
  }

  Connections {
    target: model

    function onRowsInserted(parent, first, last) {
      if (!messageLog.visible) {
        messageLog.unreadMessages = true;
        messageLog.unreadMessagesCount += (last - first + 1);
      }
    }
  }

  onVisibleChanged: {
    if (visible) {
      unreadMessages = false;
      unreadMessagesCount = 0;
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


