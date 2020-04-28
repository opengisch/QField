import QtQuick 2.11
import QtQuick.Controls 2.11
import QtQuick.Controls.Styles 1.4

import Theme 1.0

Item {
  id: item

  property alias iconColor: button.icon.color
  property alias iconSource: button.icon.source
  property alias checked: button.checked
  property alias checkable: button.checkable
  property bool round: false
  property bool roundborder: false
  property alias text: button.text
  property color bgcolor: Theme.darkGray
  property color borderColor: bgcolor

  signal pressAndHold
  signal clicked

  height: 48 * dp
  width: 48 * dp

  RoundButton {
    id: button
    anchors.fill: parent
    topInset:0
    bottomInset:0
    leftInset:0
    rightInset:0
    padding:0

    icon.color: "white"
    icon.height: 24 * dp
    icon.width: icon.height

    MouseArea {
        anchors.fill: parent
        onClicked: item.clicked()
        onPressAndHold: item.pressAndHold()
    }

    background: Rectangle {
        implicitWidth: 100
        implicitHeight: 25
        border.width: round && roundborder ? height / 6 : !round
        border.color: borderColor
        color: bgcolor
        radius: round ? height / 2 : 0

        Behavior on color {
            ColorAnimation {
                duration: 200
            }

        }
    }
    transform: Rotation {
        id: rotation

        origin.x: width / 2
        origin.y: height / 2

        Behavior on angle {
            NumberAnimation {
                duration: 200
            }

        }
    }
  }
}
