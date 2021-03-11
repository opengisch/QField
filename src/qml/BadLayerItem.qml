import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import org.qfield 1.0
import Theme 1.0

Page {
  property alias model: table.model
  signal finished
  padding: 5

  header: PageHeader {
      title: qsTr( 'Unable to load some layers' )

      showApplyButton: false
      showCancelButton: true

      onFinished: parent.finished()
    }

  ColumnLayout {
    width: parent.width
    height: parent.height
    Layout.margins: 0
    spacing: 10

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
                width: parent ? parent.width : undefined
                height: line.height
                color: "transparent"

                Column {
                    id: line
                    width: parent.width
                    spacing: 0
                    Text {
                        id: name
                        width: rectangle.width - padding * 2
                        padding: 5
                        text: LayerName
                        font: Theme.strongTipFont
                        wrapMode: Text.WordWrap
                    }
                    Text {
                        id: uri
                        width: rectangle.width -  padding * 2
                        padding: 5
                        text: DataSource
                        font: Theme.tipFont
                        wrapMode: Text.WordWrap
                    }
                }
            }
        }
    }

    Label {
      text: qsTr( "You may check the Portable Project section in the QField documentation for more help." )
      font: Theme.tipFont

      wrapMode: Text.WordWrap
      Layout.fillWidth: true
      Layout.fillHeight: true
      Layout.minimumHeight: contentHeight
      Layout.maximumHeight: contentHeight
      Layout.topMargin: 5
    }
  }
}
