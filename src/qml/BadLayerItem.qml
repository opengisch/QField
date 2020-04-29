import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3

import org.qfield 1.0
import Theme 1.0
import "."

Page {
  property alias model: table.model
  signal finished


  header: PageHeader {
      title: qsTr( 'Unable to load some layers' )

      showApplyButton: false
      showCancelButton: true

      onFinished: parent.finished()
    }

  ColumnLayout {
    anchors.margins: 8 * dp
    anchors.fill: parent
    Layout.margins: 0
    spacing: 10 * dp

    Label {
      text: qsTr( "The following layers could not be loaded, please review those and reconfigure the QGIS project." )
      font: Theme.defaultFont

      wrapMode: Text.WordWrap
      Layout.fillWidth: true
      Layout.fillHeight: true
      Layout.minimumHeight: contentHeight
      Layout.maximumHeight: contentHeight
    }

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
            spacing: 2
            anchors.fill: parent

            delegate: Rectangle {
                id: rectangle
                width: parent.width
                height: line.height
                color: "transparent"

                Column {
                    id: line
                    spacing: 0
                    Text {
                        id: name
                        padding: 5 * dp
                        text: LayerName
                        font: Theme.strongTipFont
                    }
                    Text {
                        id: uri
                        padding: 5 * dp
                        text: DataSource
                        font: Theme.tipFont
                    }
                }
            }
        }
    }

    Label {
      text: qsTr( "You may check the Portable Project section in the QField documentation for more help." )
      font.pointSize: 14
      font.italic: true

      wrapMode: Text.WordWrap
      Layout.fillWidth: true
      Layout.fillHeight: true
      Layout.minimumHeight: contentHeight
      Layout.maximumHeight: contentHeight
    }
  }
}
