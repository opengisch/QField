/**
  A swapable like close button tool
  */

import QtQuick 2.12
import QtQuick.Controls 2.12
import Theme 1.0

ToolButton {
    id: closeTool

    property string toolText: qsTr("close")

    signal closedTool()

    height: 48
    width: height + buttonText.width + 32
    onClicked: {
        closedTool();
    }

    contentItem: Rectangle {
        anchors.fill: parent
        color: "#80000000"
        radius: height / 2

        Row {
            spacing: 8

            Rectangle {
                height: 48
                width: 48
                radius: height / 2
                color: Theme.darkGray

                Image {
                    anchors.fill: parent
                    fillMode: Image.Pad
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    source: Theme.getThemeIcon("ic_close_white_24dp")
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

}
