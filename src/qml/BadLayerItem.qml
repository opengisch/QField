import QtQuick 2.11
import QtQuick.Controls 1.4 as Controls
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3

import org.qfield 1.0
import Theme 1.0
import "."

Page {
  property alias model: table.model
  signal finished


  header: ToolBar {
    id: toolbar
    height: 48 * dp
    visible: true

    anchors {
      top: parent.top
      left: parent.left
      right: parent.right
    }

    background: Rectangle {
      color: '#80CC28'
    }

    RowLayout {
      anchors.fill: parent
      Layout.margins: 0

      Controls.Label {
        id: titleLabel
        text: qsTr( 'Unable to load some layers' )
        font.pointSize: 14
        font.bold: true
        color: "#FFFFFF"
        horizontalAlignment: Qt.AlignHCenter
        verticalAlignment: Qt.AlignVCenter
        Layout.fillWidth: true
      }

      Button {
        id: closeButton

        Layout.alignment: Qt.AlignTop | Qt.AlignRight

        width: 48 * dp
        height: 48 * dp
        bgcolor: Theme.darkGray

        iconSource: Theme.getThemeIcon( "ic_close_white_24dp" )

        onClicked: {
          finished()
        }
      }
    }
  }

  ColumnLayout {
    anchors.margins: 8 * dp
    anchors.fill: parent
    Layout.margins: 0
    spacing: 10 * dp

    Label {
      text: qsTr( "The following layers could not be loaded, please review those and reconfigure the QGIS project." )
      font.pointSize: 14

      wrapMode: Text.WordWrap
      Layout.fillWidth: true
      Layout.fillHeight: true
      Layout.minimumHeight: contentHeight
      Layout.maximumHeight: contentHeight
    }

    Controls.TableView {
      id: table
      Layout.fillWidth: true
      Layout.fillHeight: true

      Controls.TableViewColumn {
        role: "LayerName"
        title: qsTr( "Layer Name" )
        width: 200 * dp
      }

      Controls.TableViewColumn {
        role: "DataSource"
        title: qsTr( "Data Source" )
        width: table.width - 200 * dp
      }

      style: TableViewStyle {
          headerDelegate: Text {
              height: 40 * dp
              font.pointSize: 12
              font.bold: true
              verticalAlignment: Text.AlignVCenter
              horizontalAlignment: styleData.textAlignment
              text: styleData.value
              elide: Text.ElideRight
          }
          rowDelegate: Item {
              height: 40 * dp
              }
          itemDelegate: Text {
              anchors.fill: parent
              font.pointSize: 12
              verticalAlignment: Text.AlignVCenter
              horizontalAlignment: styleData.textAlignment
              text: styleData.value
              elide: Text.ElideRight
              renderType: Text.NativeRendering
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
