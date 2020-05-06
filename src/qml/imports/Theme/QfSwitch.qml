import QtQuick.Controls 2.12
import QtQuick 2.12

SwitchDelegate {
    width: 48
    padding: 10
    indicator: Rectangle {
        implicitWidth: 48
        implicitHeight: 26
        x: parent.leftPadding
        y: ( parent.height + parent.topPadding - 6 ) / 2 - height / 2
        radius: 13
        color: parent.checked ? Theme.mainColor : Theme.lightGray
        border.color: parent.checked ? Theme.mainColor : Theme.lightGray
    
        Rectangle {
            x: parent.parent.checked ? parent.width - width : 0
            width: 26
            height: 26
            radius: 13
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
