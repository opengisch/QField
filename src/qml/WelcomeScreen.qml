import QtQuick 2.11
import QtQuick.Controls 1.4
import "js/style.js" as Style

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
            text: "QField for QGIS"
            font.pointSize: 25
          }

          Text {
            width: parent.width
            text: "To get started you first need a QGIS project."
                + " It is very easy to create one on your desktop PC and then transfer it"
                + " to this device."
            wrapMode: Text.WordWrap
            font.pointSize: 16
          }

          Text {
            width: parent.width
            text: "Once you have a project on this device, use the button below to"
                + " locate it on the filesystem, open it and start working."
            wrapMode: Text.WordWrap
            font.pointSize: 16
          }

          Button {
            iconSource:  Style.getThemeIcon( "ic_map_white_48dp" )

            width: 48*dp
            height: 48*dp

            onClicked: {
              showOpenProjectDialog()
              settings.setValue( "/QField/FirstRunFlag", false )
            }
          }
        }

        Image {
          id: logo
          anchors.right: parent.right
          source: "qrc:/images/qfield-logo.svg"
          width: 96*dp
          height: 96*dp
          fillMode: Image.PreserveAspectFit
        }
      }
    }
  }
}
