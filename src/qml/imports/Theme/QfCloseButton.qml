import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Controls.Material.impl
import Theme

ToolButton {
  id: button

  property string toolImage: ''
  property string toolText: qsTr("close")

  signal close

  height: 48
  width: height + buttonText.width + 32 + 24

  background: Rectangle {
    width: parent.width
    height: 48
    color: '#80000000'
    radius: height / 2

    QfToolButton {
      anchors.left: parent.left
      anchors.top: parent.top
      width: 48
      height: 48
      enabled: false
      round: true
      iconSource: button.toolImage
      iconColor: "white"
      bgcolor: Theme.darkGray
    }

    Ripple {
      id: ripple
      clip: true
      width: parent.width
      height: 48
      clipRadius: 4
      pressed: button.down
      anchor: parent
      active: button.down
      color: "#22aaaaaa"
    }
  }

  contentItem: Row {
    anchors.left: parent.left
    anchors.leftMargin: 48 + 8
    spacing: 8
    padding: 0

    Text {
      id: buttonText
      anchors.verticalCenter: parent.verticalCenter
      verticalAlignment: Text.AlignVCenter
      text: button.toolText
      color: Theme.light
      font: Theme.strongFont
    }

    Image {
      anchors.verticalCenter: parent.verticalCenter
      width: 24
      height: 24
      fillMode: Image.PreserveAspectFit
      horizontalAlignment: Image.AlignHCenter
      verticalAlignment: Image.AlignVCenter
      source: Theme.getThemeVectorIcon("ic_close_white_24dp")
      sourceSize.width: 24 * screen.devicePixelRatio
      sourceSize.height: 24 * screen.devicePixelRatio
    }
  }

  onClicked: {
    close();
  }
}
