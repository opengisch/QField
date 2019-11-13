import QtQuick.Controls 1.4
import QtQuick 2.12
import org.qgis 1.0

Item {
  property alias model: table.model
  property bool unreadMessages: false

  id: item
  anchors.fill: parent

  Rectangle {
    color: 'lightgrey'
    anchors.fill: parent
  }

  ListView {
    id: table
    anchors.fill: parent

    delegate: Rectangle {
      id: rectangle
      color: index % 2 == 0 ? 'lightgrey' : 'white'
      width: parent.width
      height: line.height
      Row {
        id: line
        spacing: 5 * dp
        Text {
          id: datetext
          padding: 5 * dp
          text: MessageDateTime
        }
        Rectangle {
          id: separator
          color: index % 2 == 0 ? 'white' : 'lightgrey'
          height: line.height
          width: 5 * dp
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

  MouseArea {
    anchors.fill: parent
    onClicked: {
      parent.visible = false
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
