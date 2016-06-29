import QtQuick 2.0
import QtQuick.Controls 1.2
import "js/style.js" as Style

Item {
  signal showOpenProjectDialog

  Rectangle {
    anchors.fill: parent
    color: "white"

    Item {
      anchors.fill: parent
      anchors.margins: 10*dp

      Column {
        Text {
          text: "QField for QGIS"
          font.pointSize: 25
        }

        Text {
          text: "<p>The app is now up and running. Well done!</p>"
              + "<p>To get started you first need a QGIS project."
              + " It is very easy to create one on your desktop PC and then transfer it"
              + " to this device.</p>"
              + "<p>Once you have a project on this device, use the buttons below to"
              + " locate it on the filesystem, open it and start working.</p>"

          wrapMode: Text.WordWrap
        }

        Button {
          iconSource:  Style.getThemeIcon( "ic_map_white_48dp" )

          width: 96*dp
          height: 96*dp

          onClicked: {
            showOpenProjectDialog()
            settings.setValue( "/QField/FirstRunFlag", false )
          }
        }
      }
    }
  }
}
