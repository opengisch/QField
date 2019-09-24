import QtQuick 2.0
import QtQuick.Layouts 1.2
import "." as QField

Item {
  signal close()

  Rectangle {
    id: rectangle
    anchors.fill: parent
    color: 'black'

  ColumnLayout {
    id: layout
    anchors.centerIn: parent
    anchors.leftMargin: 20 * dp

    Text {
      font.bold: true
      font.pointSize: 25
      color: 'white'
      text: qsTr( "What's new on QField %1").arg( version )
    }

    Rectangle{
      height: 64 * dp
    }

    Text {
      font.bold: true
      font.pointSize: 16
      color: 'white'
      text: qsTr( "Changelog")
    }

    Rectangle{
      id: changelogBox

      height: Math.max( changesListView.height, itemHeight)
      Layout.fillWidth: true

      property int itemHeight: 32 * dp

      border.color: 'lightgray'
      border.width: 1 * dp

      //the model
      ListModel {
          id: changesListModel
          ListElement {
            type: "New Feature"
            description: "Relation Editor Widget for many-to-many relations"
          }
          ListElement {
            type: "New Feature"
            description: "Measurement Tool for lines and polygons"
          }
          ListElement {
            type: "New Feature"
            description: "Value Relation Widget with multi selection"
          }
      }

      //the list
      ListView {
        id: changesListView
        model: changesListModel
        width: parent.width
        height: Math.min( 5 * changelogBox.itemHeight, changesListView.count * changelogBox.itemHeight )
        delegate: Text {
          text: "- " + description
          height: changelogBox.itemHeight
        }
        focus: true
        clip: true
        highlightRangeMode: ListView.StrictlyEnforceRange
      }
    }

    Rectangle{
      height: 64 * dp
    }

      Text {
        font.bold: true
        font.pointSize: 16
        color: 'white'
        text: qsTr( "Support our crowdfunding project, if you love QField" )
      }

      Image {
        Layout.fillWidth: true
        source: 'qrc:/pictures/qfield-love.png'
      }

      RowLayout{
        id: buttons
        QField.Button {
          id: closeButton

          Layout.fillWidth: true
          height: 48 * dp

          text: qsTr( "Yeah! Let's give love" )
          bgcolor: '#80CC28'
          borderColor: 'white'

          onClicked: {
            Qt.openUrlExternally("https://www.opengis.ch/projects/qfield-love/")
            settings.setValue( "/QField/CurrentVersion", versionCode )
            close()
          }
        }

        QField.Button {
          id: laterButton

          Layout.fillWidth: true
          height: 48 * dp

          text: qsTr( "Uh... Maybe later" )
          bgcolor: '#80CC28'
          borderColor: 'white'

          onClicked: {
            settings.setValue( "/QField/CurrentVersion", versionCode )
            close()
          }
        }
      }
    }
  }
}
