import QtQuick 2.0
import QtQuick.Layouts 1.2
import QtGraphicalEffects 1.0
import "." as QField

Item {
  signal close()

  GridLayout {
    id: mainGrid

    anchors.fill: parent
    anchors.margins: 20 * dp

    columns: 1

    Text {
      id: title
      text: qsTr( "What's new in QField %1").arg( version )
      color: '#80CC28'
      font.pointSize: 20
      minimumPixelSize: 12

      fontSizeMode: Text.VerticalFit
      wrapMode: Text.WordWrap
      Layout.fillWidth: true
      Layout.fillHeight: true
      Layout.minimumHeight: contentHeight
      Layout.maximumHeight: contentHeight
    }

    Rectangle {
      Layout.fillWidth: true
      Layout.fillHeight: true
    }

    Text {
      color: '#95000000'
      text: qsTr( "Changelog")
      font.bold: true
      font.pointSize: 12

      fontSizeMode: Text.VerticalFit
      wrapMode: Text.WordWrap
      Layout.fillWidth: true
      Layout.fillHeight: true
      Layout.minimumHeight: contentHeight
      Layout.maximumHeight: contentHeight
    }

    Rectangle{
      id: changelogBox

      Layout.fillWidth: true
      Layout.preferredHeight: Math.min( 3 * itemHeight, changesListView.count * itemHeight ) + 20 * dp

      property int itemHeight: 24 * dp
      border.color: '#95000000'
      border.width: 1 * dp

      //the model
      ListModel {
          id: changesListModel
          ListElement {
            type: "New Feature"
            description: "Value relation widget with multiple selection support"
          }
          ListElement {
            type: "New Feature"
            description: "Full snapping support providing snapping results and Z values of snapped feature"
          }
          ListElement {
            type: "New Feature"
            description: "Login dialog for WMS and WFS layers"
          }
          ListElement {
            type: "Fix"
            description: "Fix of unreliable checkbox widget"
          }
          ListElement {
            type: "Fix"
            description: "Other fixes (printing, locator)"
          }
          ListElement {
            type: "Fix"
            description: "Improved log"
          }
      }

      //the list
      ListView {
        id: changesListView
        model: changesListModel
        width: parent.width
        anchors.verticalCenter: parent.verticalCenter
        height: Math.min( 3 * changelogBox.itemHeight, changesListView.count * changelogBox.itemHeight )
        delegate: Rectangle{
            id: item
            x: 1 * dp
            width: parent.width - 2 * dp
            height: text.height + 10 * dp

            Text {
                id: dash
                text: " -"
                font.pointSize: 12
                Layout.minimumWidth: contentWidth
                fontSizeMode: Text.VerticalFit
                wrapMode: Text.WordWrap
                color: '#95000000'
            }
            Text {
                id: text
                text: description
                font.pointSize: 12
                Layout.minimumHeight: contentHeight
                Layout.maximumHeight: contentHeight
                width: parent.width - 20 * dp
                x: dash.width + 10 * dp
                fontSizeMode: Text.VerticalFit
                wrapMode: Text.WordWrap
                color: '#95000000'
            }
        }
        focus: true
        clip: true
        highlightRangeMode: ListView.StrictlyEnforceRange
      }
    }

    Rectangle {
      id: secondSpace
      Layout.fillWidth: true
      Layout.fillHeight: true
    }

    Text {
      color: '#90000000'
      text: qsTr( "Support our crowdfunding project, if you love QField" )
      font.pointSize: 12
      font.bold: true

      fontSizeMode: Text.VerticalFit
      wrapMode: Text.WordWrap
      Layout.fillWidth: true
      Layout.fillHeight: true
      Layout.minimumHeight: contentHeight
      Layout.maximumHeight: contentHeight
    }

    Image {
      id: image
      Layout.fillWidth: true
      Layout.fillHeight: true
      Layout.maximumHeight: parent.width < parent.height ? width / 3 : parent.height
      fillMode: Image.PreserveAspectFit
      source: 'qrc:/pictures/qfield-love.png'
    }

    GridLayout{
      id: buttons
      columns: 2

      Layout.maximumHeight: 48 * dp
      Layout.preferredHeight: 48 * dp
      Layout.fillWidth: true
      Layout.minimumHeight: 36 * dp

      QField.Button {
        id: closeButton

        Layout.fillWidth: true
        Layout.fillHeight: true

        text: qsTr( "Let's give love" )

        bgcolor: '#80CC28'

        onClicked: {
          Qt.openUrlExternally("https://www.opengis.ch/projects/qfield-love/")
          settings.setValue( "/QField/CurrentVersion", versionCode )
          close()
        }
      }

      QField.Button {
        id: laterButton

        Layout.fillWidth: true
        Layout.fillHeight: true

        text:  qsTr( "Maybe later" )
        bgcolor: '#80CC28'

        onClicked: {
          settings.setValue( "/QField/CurrentVersion", versionCode )
          close()
        }
      }
    }
  }
}
