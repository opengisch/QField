import QtQuick.Controls 2.11
import QtQuick 2.12

SwitchDelegate {
    width: 48 * dp
    padding: 10 * dp
    indicator: Rectangle {
        implicitWidth: 48 * dp
        implicitHeight: 26 * dp
        x: parent.leftPadding
        y: ( parent.height + parent.topPadding - 6 ) / 2 - height / 2
        radius: 13 * dp
        color: parent.checked ? Theme.mainColor : Theme.lightGray
        border.color: parent.checked ? Theme.mainColor : Theme.lightGray
    
        Rectangle {
            x: parent.parent.checked ? parent.width - width : 0
            width: 26 * dp
            height: 26 * dp
            radius: 13 * dp
            color: parent.parent.down ? Theme.lightGray : Theme.light
            border.color: parent.parent.checked ? Theme.mainColor : Theme.lightGray
            Behavior on x {
                PropertyAnimation {
                    duration: 150
                    easing.type: Easing.Linear
                }
            }
        }
    }
}
