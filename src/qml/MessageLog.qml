import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import org.qfield 1.0
import Theme 1.0

Page {
  id: messageLog

  property alias model: table.model
  property bool unreadMessages: false

  signal finished

  header: PageHeader {
      title: qsTr( 'Message Logs' )

      showBackButton: true
      showApplyButton: false
      showCancelButton: false

      topMargin: mainWindow.sceneTopMargin

      onFinished: messageLog.finished()
    }

  ColumnLayout {
    anchors.margins: 8
    anchors.fill: parent
    Layout.margins: 0
    spacing: 10

    Rectangle {
        Layout.fillWidth: true
        Layout.fillHeight: true
        color: "white"
        border.color: "lightgray"
        border.width: 1

      ListView {
        id: table
        objectName: 'messagesList'
        flickableDirection: Flickable.VerticalFlick
        boundsBehavior: Flickable.StopAtBounds
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
              text: MessageDateTime.replace(' ','\n')
            }
            Rectangle {
              id: separator
              width: 0
            }
            Text {
              id: tagtext
              objectName: 'tagText'
              padding: MessageTag ? 5: 0
              text: MessageTag
              font.bold: true
            }
            Text {
              id: messagetext
              objectName: 'messageText'
              padding: 5
              width: rectangle.width - datetext.width - tagtext.width - separator.width - 3 * line.spacing
              text: Message
              wrapMode: Text.WordWrap
              textFormat: Text.RichText

              MouseArea {
                anchors.fill: parent
                onClicked:
                {
                    copyHelper.text = messagetext.text
                    copyHelper.selectAll()
                    copyHelper.copy()
                    displayToast(qsTr("Message text copied"))
                }
              }
            }
          }
        }
      }
    }

    TextEdit{
        id: copyHelper
        visible: false
    }

    QfButton {
        text: qsTr("Clear message log")
        Layout.fillWidth: true

        onClicked: {
            table.model.clear()
            displayToast(qsTr("Message log cleared"))
            messageLog.finished()
        }
    }

    QfButton {
        id: submitLog
        Layout.fillWidth: true
        text: qsTr( 'Send application log' )
        visible: qfieldSettings.enableInfoCollection && platformUtilities.capabilities & PlatformUtilities.SentryFramework

        onClicked: {
            iface.sendLog("Manual log submission")
            displayToast(qsTr("Your application log is being sent"))
        }
    }
  }

  Connections {
    target: model

    function onRowsInserted(parent, first, last) {
      if ( !visible )
        unreadMessages = true
    }
  }

  onVisibleChanged: {
    if ( visible )
      unreadMessages = false
  }
}
