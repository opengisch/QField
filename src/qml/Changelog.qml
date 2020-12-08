import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.0

import Theme 1.0
import org.qfield 1.0



Popup {
  id: changelogPopup

  parent: ApplicationWindow.overlay
  x: 24
  y: 24
  width: parent.width - 48
  height: parent.height - 48
  padding: 0
  modal: true
  closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
  focus: visible

  Flickable {
    id: changelogFlickable
    anchors.fill: parent
    flickableDirection: Flickable.VerticalFlick
    interactive: true
    contentWidth: parent.width; contentHeight: changelogGrid.height
    clip: true

    GridLayout {
      id: changelogGrid

      anchors.left: parent.left
      anchors.right: parent.right
      anchors.margins: 20

      columns: 1

      Item {
          // top margin
          height: 20
      }

      Text {
        id: title
        text: qsTr( "What's new in the latest QField" )
        color: Theme.mainColor
        font: Theme.titleFont
        minimumPixelSize: 12


        fontSizeMode: Text.VerticalFit
        wrapMode: Text.WordWrap
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.minimumHeight: contentHeight
        Layout.maximumHeight: contentHeight
      }

      Text {
        id: changelogBody
        color: '#95000000'
        font: Theme.tipFont
        visible: changelogContents.status != ChangelogContents.LoadingStatus

        fontSizeMode: Text.VerticalFit
        textFormat: Text.MarkdownText
        wrapMode: Text.WordWrap
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.minimumHeight: contentHeight
        Layout.maximumHeight: contentHeight

        text: {
          switch ( changelogContents.status ) {
            case ChangelogContents.IdleStatus:
            case ChangelogContents.LoadingStatus:
              return ''
            case ChangelogContents.SuccessStatus:
              return changelogContents.markdown
            case ChangelogContents.ErrorStatus:
              return qsTr( 'Error while fetching changelog. Try again later.' )
          }
        }

        onLinkActivated: Qt.openUrlExternally(link)
      }

      BusyIndicator {
        Layout.alignment: parent.Layout.alignment

        visible: changelogContents.status == ChangelogContents.LoadingStatus
        running: visible
      }

      QfButton {
        id: closeButton
        Layout.fillWidth: true

        text: qsTr( 'OK' )
        onClicked: changelogPopup.close()
      }

      Item {
          // bottom
          height: 20
      }
    }
  }

  ChangelogContents {
    id: changelogContents
  }

  onClosed: {
    if ( changelogContents.status === ChangelogContents.SuccessStatus ) {
      settings.setValue( "/QField/ChangelogVersion", versionCode )
    }

    changelogFlickable.contentY = 0
  }

  onOpened: {
    if ( changelogContents.status === ChangelogContents.SuccessStatus || changelogContents.status === ChangelogContents.LoadingStatus )
      return

    changelogContents.request()
  }
}
