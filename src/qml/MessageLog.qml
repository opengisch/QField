import QtQuick.Controls 1.4
import QtQuick 2.5
import org.qgis 1.0

Item {
  property alias model: table.model
  signal clicked

  TableView {
    id: table
    anchors.fill: parent

    TableViewColumn {
      role: "MessageTag"
      title: "Tag"
    }
    TableViewColumn {
      role: "Message"
      title: "Message"
    }

    itemDelegate: Item {
      Text {
        anchors.verticalCenter: parent.verticalCenter
        color: styleData.textColor
        elide: styleData.elideMode
        text: styleData.value
      }
    }
  }

  onVisibleChanged: table.resizeColumnsToContents()
}
