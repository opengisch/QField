import QtQuick 2.11
import QtQuick.Controls 1.4
import "js/style.js" as Style
import "."

Item {
  signal showOpenProjectDialog

  Rectangle {
    anchors.fill: parent
    color: "white"

    Item {
      anchors.fill: parent
      anchors.margins: 10*dp

      Row {
        width: parent.width

        Column {
          id: column
          width: parent.width - logo.width

          Text {
            width: parent.width
            text: qsTr( "QField for QGIS" )
            font.pointSize: 25
          }

          Text {
            width: parent.width
            text: qsTr( "To get started you can use a demo project or your own QGIS project."
                + " It is very easy to create one on your desktop PC and then transfer it"
                + " to this device." )
            wrapMode: Text.WordWrap
            font.pointSize: 16
          }

          Text {
            width: parent.width
            text: qsTr( "Once you have a project on this device, or to use a demo project,"
                + " use the button below to locate it, open it and start working." )
            wrapMode: Text.WordWrap
            font.pointSize: 16
            bottomPadding: 20
          }

          Button {
            iconSource: Style.getThemeIcon( "ic_map_green_48dp" )
            width: 48*dp
            height: 48*dp
            bgcolor: "white"
            borderColor: "#80CC28"
            onClicked: {
              showOpenProjectDialog()
              settings.setValue( "/QField/FirstRunFlag", false )
            }
          }
        }

        Image {
          id: logo
          source: "qrc:/images/qfield-logo.svg"
          width: 96*dp
          height: 96*dp
          fillMode: Image.PreserveAspectFit
        }
      }
    }
  }
}
