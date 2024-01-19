import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import org.qfield 1.0
import Theme 1.0

Page {
  property alias model: table.model
  signal finished
  padding: 5

  header: PageHeader {
      title: qsTr( 'Unable to load some layers' )

      showBackButton: false
      showApplyButton: false
      showCancelButton: true

      topMargin: mainWindow.sceneTopMargin

      onFinished: parent.finished()
    }

  ColumnLayout {
    anchors.margins: 8
    anchors.bottomMargin: 8 + mainWindow.sceneBottomMargin
    anchors.fill: parent
    Layout.margins: 0
    spacing: 10

    Label {
      text: qsTr( "The following layers could not be loaded, please review those and reconfigure the QGIS project." )
      font: Theme.defaultFont
      color: Theme.mainTextColor

      wrapMode: Text.WordWrap
      Layout.fillWidth: true
      Layout.fillHeight: true
      Layout.minimumHeight: contentHeight
      Layout.maximumHeight: contentHeight
    }

    Rectangle {
        Layout.fillWidth: true
        Layout.fillHeight: true
        color: Theme.controlBackgroundColor
        border.color: Theme.controlBorderColor
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
                        color: Theme.mainTextColor
                        wrapMode: Text.WordWrap
                    }
                    Text {
                        id: uri
                        width: rectangle.width -  padding * 2
                        padding: 5
                        text: DataSource
                        font: Theme.tipFont
                        color: Theme.secondaryTextColor
                        wrapMode: Text.WordWrap
                    }
                }
            }
        }
    }

    Label {
      text: qsTr( 'You may check the %1Portable Project%2 documentation page for more help.')
        .arg( "<a href=\"https://docs.qfield.org/how-to/movable-project\">" )
        .arg( "</a>" )

      textFormat: Text.RichText
      font: Theme.tipFont
      color: Theme.secondaryTextColor

      wrapMode: Text.WordWrap
      Layout.fillWidth: true
      Layout.fillHeight: true
      Layout.minimumHeight: contentHeight
      Layout.maximumHeight: contentHeight
      Layout.topMargin: 5

      onLinkActivated: (link) => { Qt.openUrlExternally(link) }
    }
  }
}
