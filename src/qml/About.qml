import QtQuick 2.0

Item {
  Rectangle {
    color: "black"
    opacity: 0.8
    anchors.fill: parent
    MouseArea {
      anchors.fill: parent
      onClicked: {
        parent.parent.visible = false
      }
    }
  }

  Column {
    anchors.leftMargin: 10*dp
    anchors.fill: parent
    spacing: 20*dp
    Text {
      font.bold: true
      color: "white"
      text: qsTr( "QField Version: %1").arg( version )
    }
    Text {
      font.bold: true
      color: "white"
      text: qsTr( "QField Settings folder: %1").arg(
                settings.value( "/QField/App/DOTQGIS2_DIR", "DDDOTQGIS2_DIR" ) )
    }
    Text {
      font.bold: true
      color: "white"
      text: qsTr( "QField Shared items folder: %1").arg(
                settings.value( "/QField/App/SHARE_DIR", "DDSHARE_DIR" ) )
    }
    Row{
        spacing: 20*dp
        anchors.fill: parent.parent
        Column{
            spacing: 10*dp
            Text {
              font.bold: true
              color: "white"
              text: qsTr( "Developed by" )
            }
            MouseArea {
              width: opengis_logo.width
              height: opengis_logo.height
              Image {
                  id: opengis_logo
                  source: "qrc:/images/opengis-logo.svg"
              }
              onClicked: Qt.openUrlExternally("http://opengis.ch")
            }
        }
        Column{
            spacing: 10*dp
            Text {
                font.bold: true
                color: "white"
                text: qsTr( "Support the development" )
            }
            MouseArea {
                width: donate_image.width
                height: donate_image.height
                Image {
                    id: donate_image
                    source: "qrc:/images/karma-logo.svg"
                }
                onClicked: Qt.openUrlExternally("http://qfield.opengis.ch/supportus")
            }
        }
        Column{
            spacing: 10*dp
            Text {
                font.bold: true
                color: "white"
                text: qsTr( "Buy the Karma edition" )
            }
            MouseArea {
                width: karma_image.width
                height: karma_image.height
                Image {
                    id: karma_image
                    source: "qrc:/images/karma-logo-playstore.svg"
                }
                onClicked: Qt.openUrlExternally("https://play.google.com/store/apps/details?id=ch.opengis.qfieldkarmaedition")
            }
        }
    }
  }
}

