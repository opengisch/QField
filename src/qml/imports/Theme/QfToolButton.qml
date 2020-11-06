import QtQuick 2.12
import QtQuick.Controls 2.12

import Theme 1.0

Item {
    id: item

    property string iconSource: ''
    property alias checked: button.checked
    property alias checkable: button.checkable
    property bool round: false
    property bool roundborder: false
    property alias text: button.text
    property color bgcolor: Theme.darkGray
    property color borderColor: bgcolor

    signal clicked
    signal pressed
    signal pressAndHold
    signal released
    signal canceled

    height: 48
    width: 48

    RoundButton {
        id: button
        anchors.fill: parent
        topInset:0
        bottomInset:0
        leftInset:0
        rightInset:0
        padding:10
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

        onClicked: item.clicked()
        onPressed: item.pressed()
        onPressAndHold: item.pressAndHold()
        onReleased: item.released()
        onCanceled: item.canceled()

        contentItem: Rectangle {
            implicitWidth: 100
            implicitHeight: 25
            color: "transparent"

            Image {
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                horizontalAlignment: Image.AlignHCenter
                verticalAlignment: Image.AlignVCenter
                source: iconSource
                sourceSize.width: 24 * screen.devicePixelRatio
                sourceSize.height: 24 * screen.devicePixelRatio
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
