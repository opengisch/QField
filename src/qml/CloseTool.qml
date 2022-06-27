import QtQuick 2.14
import QtQuick.Controls 2.14

import Theme 1.0

/**
  A swapable like close button tool
  */

ToolButton {
  id: closeTool

  property string toolText: qsTr("close")

  signal closedTool()

  height: 48
  width: height + buttonText.width + 32

  contentItem: Rectangle {
    anchors.fill: parent
    color: '#80000000'
    radius: height / 2

    Row {
      spacing: 8
      Rectangle {
        width: 48
        height: 48
        radius: height / 2
        color: Theme.darkGray
        Image {
          anchors.centerIn: parent
          width: 24
          height: 24
          fillMode: Image.PreserveAspectFit
          horizontalAlignment: Image.AlignHCenter
          verticalAlignment: Image.AlignVCenter
          source: Theme.getThemeIcon( "ic_close_white_24dp" )
          sourceSize.width: 24 * screen.devicePixelRatio
          sourceSize.height: 24 * screen.devicePixelRatio
        }
      }

      Text {
        id: buttonText
        anchors.verticalCenter: parent.verticalCenter
        verticalAlignment: Text.AlignVCenter
        text: closeTool.toolText
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

