import QtQuick 2.0

Item {
  Rectangle {
    color: "black"
    opacity: 0.8
    anchors.fill: parent
  }

  Column {
    anchors.fill: parent
    anchors.margins: 10 * dp
    Text {
      font.bold: true
      color: "white"
      text: qsTr( "QField Version: %1").arg( version )
    }
    Item {
      // Spacer
      height: 30*dp
      width: 1
    }
    Text {
      font.bold: true
      color: "white"
      text: qsTr( "QField Settings folder: %1").arg(
                settings.value( "/QField/App/DOTQGIS2_DIR", "DDDOTQGIS2_DIR" ) )
    }
    Item {
      // Spacer
      height: 30*dp
      width: 1
    }

    Text {
      font.bold: true
      color: "white"
      text: qsTr( "QField Shared items folder: %1").arg(
                settings.value( "/QField/App/SHARE_DIR", "DDSHARE_DIR" ) )
    }
    Item {
      // Spacer
      height: 30*dp
      width: 1
    }

    Text {
      font.bold: true
      color: "white"
      text: qsTr( "Developed by" )
    }
    Item {
      // Spacer
      height: 10*dp
      width: 1
    }
    Image {
      source: "qrc:/images/opengis-logo.svg"
    }
  }
}

