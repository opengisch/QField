import QtQuick 2.12
import QtQuick.Controls 2.11
import QtQuick.Layouts 1.2
import Theme 1.0

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
    anchors.fill: parent
    anchors.margins: 20 * dp

    Label {
      Layout.alignment: Qt.AlignCenter
      Layout.maximumWidth: parent.width
      horizontalAlignment: Text.AlignHCenter
      color: Theme.light
      font: Theme.strongFont
      wrapMode: Text.WordWrap
      text: qsTr( "QField Version: %1 (code: %2)").arg( version ).arg( versionCode )
    }
    Label {
      Layout.alignment: Qt.AlignCenter
      Layout.maximumWidth: parent.width
      horizontalAlignment: Text.AlignHCenter
      color: Theme.light
      font: Theme.strongFont
      wrapMode: Text.WordWrap

      text: qsTr( "QField Settings folder: %1" ).arg( '<br><font color="%1">'.arg(Theme.lightGray) + platformUtilities.configDir + '</font>' )
    }
    Label {
      Layout.alignment: Qt.AlignCenter
      Layout.maximumWidth: parent.width
      horizontalAlignment: Text.AlignHCenter
      color: Theme.light
      font: Theme.strongFont
      wrapMode: Text.WordWrap

      text: qsTr( "QField Shared items folder: %1" ).arg( '<br><font color="%1">'.arg(Theme.lightGray) + platformUtilities.shareDir + '</font>' )
    }
    Item{
      Layout.minimumHeight: 20 * dp
    }

    Label {
      Layout.alignment: Qt.AlignCenter
      font: Theme.strongFont
      color: Theme.light
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
      onClicked: Qt.openUrlExternally("https://opengis.ch")
    }
    Label {
      Layout.alignment: Qt.AlignCenter
      font: Theme.strongFont
      color: Theme.light
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
      onClicked: Qt.openUrlExternally("https://www.opengis.ch/android-gis/qfield/donate-and-sponsor/")
    }
  }
}
