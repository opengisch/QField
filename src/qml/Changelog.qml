import QtQuick 2.0
import QtQuick.Layouts 1.2
import "." as QField

Item {
  signal close()

  Rectangle {
    id: rectangle
    anchors.fill: parent
    color: 'white'

    GridLayout {
        id: mainGrid

        anchors.fill: parent
        anchors.margins: 48 * dp

        columns: 1
        rowSpacing : 20 * dp
        columnSpacing : 20 * dp

        Text {
          color: '#95000000'
          text: qsTr( "What's new on QField %1").arg( version )
          font.pointSize: 25
          wrapMode: Text.WordWrap

          Layout.fillWidth: true
          Layout.fillHeight: true
        }

        Text {
          color: '#95000000'
          text: qsTr( "Changelog")
          font.pointSize: 16
          wrapMode: Text.WordWrap

          Layout.fillWidth: true
          Layout.fillHeight: true
        }

        Rectangle{
          id: changelogBox

          Layout.fillWidth: true
          Layout.preferredHeight: Math.max( changesListView.height, itemHeight)

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
              text: " - " + description
              height: changelogBox.itemHeight
              color: '#90000000'
            }
            focus: true
            clip: true
            highlightRangeMode: ListView.StrictlyEnforceRange
          }
        }

        Text {
          color: '#95000000'
          text: qsTr( "Support our crowdfunding project, if you love QField" )
          font.pointSize: 16
          wrapMode: Text.WordWrap

          Layout.fillWidth: true
          Layout.fillHeight: true
        }
        Image {
          Layout.fillWidth: true
          Layout.fillHeight: true
          Layout.preferredHeight: sourceSize.height
          fillMode: Image.PreserveAspectFit
          source: 'qrc:/pictures/qfield-love.png'
        }

        GridLayout{
          id: buttons
          columns: 2

          Layout.preferredHeight: 48 * dp
          Layout.fillWidth: true

          QField.Button {
            id: closeButton

            Layout.fillWidth: true
            Layout.fillHeight: true

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
            Layout.preferredHeight: 48 * dp

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
