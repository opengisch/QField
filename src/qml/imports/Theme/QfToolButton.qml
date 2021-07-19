import QtQuick 2.12
import QtQuick.Controls 2.12

import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Material.impl 2.12
import QtGraphicalEffects 1.12

import Theme 1.0

Item {
    id: item

    property string iconSource: ''
    property string bottomRightIndicatorText: ''
    property string bottomRightIndicatorBgColor: Theme.cloudColor
    property string bottomRightIndicatorFgColor: Theme.light
    property alias checked: button.checked
    property alias checkable: button.checkable
    property alias enabled: button.enabled
    property bool round: false
    property bool roundborder: false
    property alias text: button.text
    property color bgcolor: 'transparent'
    property color borderColor: bgcolor

    signal clicked
    signal doubleClicked
    signal pressed
    signal pressAndHold
    signal released
    signal canceled

    height: 48
    width: 48

    RoundButton {
        id: button
        focusPolicy: Qt.NoFocus
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

            Ripple {
                id: ripple
                clip: true
                width: parent.width
                height: parent.height
                clipRadius: 4
                pressed: button.down
                anchor: parent
                active: button.down
                color: bgcolor == "#ffffff" ? "#10000000" : "#22ffffff"

                layer.enabled: true
                layer.effect: OpacityMask {
                    maskSource: Rectangle
                    {
                        width: ripple.width
                        height: ripple.height
                        radius: ripple.height / 2
                    }
                }
            }
        }

        onClicked: item.clicked()
        onDoubleClicked: item.doubleClicked()
        onPressed: item.pressed()
        onPressAndHold: item.pressAndHold()
        onReleased: item.released()
        onCanceled: item.canceled()

        contentItem: Rectangle {
            implicitWidth: 100
            implicitHeight: 25
            color: "transparent"
            z: 1

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

        Rectangle {
            id: bottomRightIndicator
            color: bottomRightIndicatorBgColor
            anchors.bottom: button.bottom
            anchors.right: button.right
            anchors.bottomMargin: 7
            anchors.rightMargin: 5
            width: button.width / 2.5
            height: width
            visible: bottomRightIndicatorText
            radius: width
            border.width: 2
            border.color: Theme.mainColor
            z: 2

            Text {
              anchors.fill: parent
              color: bottomRightIndicatorFgColor
              text: bottomRightIndicatorText
              font.pixelSize: bottomRightIndicatorText.length == 1
                ? height / 1.6
                : height / 1.8
              horizontalAlignment: Qt.AlignHCenter
              verticalAlignment: Qt.AlignVCenter
            }
        }
    }
}
