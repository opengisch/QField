import QtQuick 2.14
import QtQuick.Controls 2.14

SwitchDelegate {
    property bool small: false

    width: small ? 34 : 48
    padding: 10
    indicator: Rectangle {
        implicitWidth: small ? 34 : 48
        implicitHeight: small ? 16 : 26
        x: parent.leftPadding
        y: ( parent.height + parent.topPadding - 6 ) / 2 - height / 2
        radius: implicitHeight / 2
        color: parent.checked ? Theme.mainColor : Theme.lightGray
        border.color: parent.checked ? Theme.mainColor : Theme.lightGray

        Rectangle {
            x: parent.parent.checked ? parent.width - width : 0
            width: parent.implicitHeight
            height: parent.implicitHeight
            radius: parent.implicitHeight / 2
            color: parent.parent.down ? Theme.lightestGray : Theme.light
            border.color: parent.parent.checked ? Theme.mainColor : Theme.lightGray
            Behavior on x {
                PropertyAnimation {
                    duration: 150
                    easing.type: Easing.Linear
                }
            }
        }
    }

    background: Rectangle {
        implicitWidth: 100
        implicitHeight: 40
        visible: false
        color: "transparent"
    }
}
