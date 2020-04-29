import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3

import org.qfield 1.0
import Theme 1.0

Page {
  property alias model: table.model
  signal finished
  property bool unreadMessages: false


  header: PageHeader {
      title: qsTr( 'Message Logs' )

      showApplyButton: false
      showCancelButton: true

      onFinished: parent.finished()
    }

  ColumnLayout {
    anchors.margins: 8 * dp
    anchors.fill: parent
    Layout.margins: 0
    spacing: 10 * dp

    Rectangle {
        Layout.fillWidth: true
        Layout.fillHeight: true
        color: "white"
        border.color: "lightgray"
        border.width: 1

      ListView {
        id: table
        flickableDirection: Flickable.VerticalFlick
        boundsBehavior: Flickable.StopAtBounds
        clip: true
        anchors.fill: parent
        spacing: 2

        delegate: Rectangle {
          id: rectangle
          width: parent.width
          height: line.height
          color: "transparent"

          Row {
            id: line
            spacing: 5 * dp
            Text {
              id: datetext
              padding: 5 * dp
              text: MessageDateTime.replace(' ','\n')
            }
            Rectangle {
              id: separator
              width: 0 * dp
            }
            Text {
              id: tagtext
              padding: MessageTag ? 5 * dp : 0
              text: MessageTag
              font.bold: true
            }
            Text {
              id: messagetext
              padding: 5 * dp
              width: rectangle.width - datetext.width - tagtext.width - separator.width - 3 * line.spacing
              text: Message
              wrapMode: Text.WordWrap
            }
          }
        }
      }
    }
  }

  Connections {
    target: model

    onRowsInserted: {
      if ( !visible )
        unreadMessages = true
    }
  }

  onVisibleChanged: {
    if ( visible )
      unreadMessages = false
  }
}
