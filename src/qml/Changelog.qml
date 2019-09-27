import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.2
import QtGraphicalEffects 1.0
import "." as QField

Item {
  signal close()

  height: childrenRect.height
  width: parent.width

  GridLayout {
    id: mainGrid

    anchors.left: parent.left
    anchors.right: parent.right
    anchors.margins: 20 * dp

    columns: 1

    Item {
        // top margin
        height: 20 * dp
    }

    Text {
      id: title
      text: qsTr( "What's new in the latest QField" )
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

    Text {
      color: '#95000000'
      text: qsTr( "Changelog %1" ).arg( version )
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
      Layout.fillHeight: true //preferredHeight: Math.min( 3 * itemHeight, changesListView.count * itemHeight ) + 20 * dp
      Layout.minimumHeight: changesListView.count * 24 * dp
      border.color: '#30000000'
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
        width: parent.width - 20 * dp
        anchors.verticalCenter: parent.verticalCenter
        height: parent.height - 20 * dp //Math.min( 3 * changelogBox.itemHeight, changesListView.count * changelogBox.itemHeight )
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

    Text {
      color: '#90000000'
      text: qsTr( "Do you enjoy QField? Show some love and support the crowdfunding campaign. Before October 9." )
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
      Layout.maximumHeight:  width / 3
      fillMode: Image.PreserveAspectFit
      source: 'qrc:/pictures/qfield-love.png'
    }


    Text {
      color: '#90000000'
      text: qsTr( "www.opengis.ch/projects/qfield-love/" )
      font.pointSize: 12
      font.bold: true

      fontSizeMode: Text.VerticalFit
      wrapMode: Text.WordWrap
      Layout.fillWidth: true
      Layout.fillHeight: true
      Layout.minimumHeight: contentHeight
      Layout.maximumHeight: contentHeight
    }

    GridLayout{
      id: buttons
      columns: 1

      Layout.maximumHeight: 96 * dp
      Layout.preferredHeight: 96 * dp
      Layout.minimumHeight: 72 * dp
      Layout.fillWidth: true

      Button {
        id: closeButton
        Layout.fillWidth: true
        Layout.fillHeight: true

        text: qsTr( "Let's give love" )

        font.pointSize: 12

        contentItem: Text {
          text: closeButton.text
          font: closeButton.font
          color: 'white'
          horizontalAlignment: Text.AlignHCenter
          verticalAlignment: Text.AlignVCenter
          elide: Text.ElideRight
        }

        background: Rectangle {
          color: laterButton.down ? '#8080CC28' : '#80CC28'
        }

        onClicked: {
            Qt.openUrlExternally("https://www.opengis.ch/projects/qfield-love/")
            settings.setValue( "/QField/CurrentVersion", versionCode )
            close()
        }
      }

      GridLayout{
          Button {
            id: laterButton
            Layout.fillWidth: true
            Layout.fillHeight: true

            text: qsTr( "Maybe later" )

            font.pointSize: 12

            contentItem: Text {
                text: laterButton.text
                font: laterButton.font
                color: 'white'
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }

            background: Rectangle {
                height: parent.height
                color: laterButton.down ? '#40000000' : '#60000000'
            }

            property var remindDate: new Date()

            onClicked: {
              settings.setValue( "/QField/CurrentVersion", versionCode )
              remindDate.setDate( remindDate.getDate()+1 )
              settings.setValue( "/QField/RemindDateForCrowdfunding", remindDate )
              close()
            }
          }

          Button {
            id: noButton
            Layout.fillWidth: true
            Layout.fillHeight: true

            text: qsTr( "No" )

            font.pointSize: 12

            contentItem: Text {
                text: noButton.text
                font: noButton.font
                color: 'white'
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }

            background: Rectangle {
                color: laterButton.down ? '#40000000' : '#60000000'
            }

            onClicked: {
              settings.setValue( "/QField/CurrentVersion", versionCode )
              close()
            }
          }
      }
    }

    Item {
        // bottom
        height: 20 * dp
    }
  }
}
