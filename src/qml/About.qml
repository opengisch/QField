import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import Theme 1.0
import org.qfield 1.0

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
    anchors.margins: 20

    Label {
      Layout.alignment: Qt.AlignCenter
      Layout.maximumWidth: parent.width
      horizontalAlignment: Text.AlignHCenter
      color: Theme.light
      font: Theme.strongFont
      wrapMode: Text.WordWrap
      text: {
        var links = '<a href="https://github.com/opengisch/QField/commit/' + gitRev + '">' + gitRev.substr(0, 6) + '</a>'

        if (appVersion)
          links += ' <a href="https://github.com/opengisch/QField/releases/tag/' + appVersion + '">' + appVersion + '</a>'

        qsTr( "QField Version: %1 (%2)").arg( appVersionStr ).arg( links )
      }
      onLinkActivated: Qt.openUrlExternally(link)
    }

    Item{
      Layout.minimumHeight: 20
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
    QfButton {
      id: changelogButton
      Layout.fillWidth: true

      text: qsTr( 'Changelog' )

      onClicked: changelogPopup.open()
    }
  }
}
