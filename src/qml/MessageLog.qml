import QtQuick.Controls 1.4
import QtQuick 2.5
import org.qgis 1.0

Item {
  property alias model: table.model
  property bool unreadMessages: false

  id: item
  anchors.fill: parent

  ListView {
    id: table
    anchors.fill: parent

    delegate:
        Rectangle {
         color: index % 2 == 0 ? 'lightgrey' : 'white'
         width: parent.width
         height: line.height
         Row {
             id: line
             spacing: 40 * dp
             Text {
               text: MessageDateTime
             }
             Text {
               text: MessageTag || '-'
               font.bold: true
             }
             Text {
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
