import QtQuick 2.12
import QtQuick.Controls 2.12
import Theme 1.0


ToolButton {

  signal closedTool()

  height: 48 * dp
  width: height + buttonText.width + 32 * dp

  contentItem: Rectangle {
    anchors.fill: parent
    color: '#80000000'
    radius: height / 2

    Row {
      spacing: 8 * dp
      Rectangle {
        height: 48 * dp
        width: 48 * dp
        radius: height / 2
        color: Theme.darkGray
        Image {
          anchors.fill: parent
          fillMode: Image.Pad
          horizontalAlignment: Image.AlignHCenter
          verticalAlignment: Image.AlignVCenter
          source: Theme.getThemeIcon( "ic_close_white_24dp" )
        }
      }

      Text {
        id: buttonText
        anchors.verticalCenter: parent.verticalCenter
        verticalAlignment: Text.AlignVCenter
        text: qsTr( 'Close measure tool' )
        color: Theme.light
        font: Theme.strongFont
      }
    }

    Behavior on color {
      ColorAnimation {
        duration: 200
      }
    }
  }

  onClicked: {
    closedTool()
  }
}

