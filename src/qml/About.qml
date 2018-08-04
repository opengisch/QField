import QtQuick 2.0
import QtQuick.Layouts 1.2

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

  ColumnLayout {
    id: informations
    anchors.centerIn: parent
    anchors.leftMargin: 20 * dp
    Text {
      font.bold: true
      color: "white"
      text: qsTr( "QField Version: %1").arg( version )
    }
    Text {
      font.bold: true
      color: "white"
      text: qsTr( "QField Settings folder: %1").arg( platformUtilities.configDir )
    }
    Text {
      font.bold: true
      color: "white"
      text: qsTr( "QField Shared items folder: %1").arg( platformUtilities.shareDir )
    }
    Item{
      Layout.minimumHeight: 20 * dp
    }

    RowLayout{
      id: buttons
      spacing: 20 * dp

      ColumnLayout{
        Layout.alignment: Qt.AlignTop
        Text {
          font.bold: true
          color: "white"
          text: qsTr( "Developed by" )
        }
        MouseArea {
          Layout.alignment: Qt.AlignCenter
          width: opengis_logo.width
          height: opengis_logo.height
          Image {
            id: opengis_logo
            source: "qrc:/images/opengis-logo.svg"
          }
          onClicked: Qt.openUrlExternally("http://opengis.ch")
        }
      }
      ColumnLayout{
        Layout.alignment: Qt.AlignTop
        Text {
          font.bold: true
          color: "white"
          text: qsTr( "Support the development" )
        }
        MouseArea {
          Layout.alignment: Qt.AlignCenter
          width: donate_image.width
          height: donate_image.height
          Image {
            id: donate_image
            source: "qrc:/images/karma-logo.svg"
          }
          onClicked: Qt.openUrlExternally("http://www.qfield.org/docs/development/index.html#make-it-grow-make-it-yours")
        }
      }
      ColumnLayout{
        Layout.alignment: Qt.AlignTop
        Text {
          font.bold: true
          color: "white"
          text: qsTr( "Buy the Karma edition" )
        }
        MouseArea {
          Layout.alignment: Qt.AlignCenter
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

