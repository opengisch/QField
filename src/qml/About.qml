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
      text: qsTr( "Developed by" )
    }

    Image {
      source: "qrc:/images/opengis-logo.svg"
    }
  }
}

