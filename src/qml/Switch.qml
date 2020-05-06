// [hidpi fixes]
import QtQuick 2.12
import QtQuick.Controls 2.12

import Theme 1.0

Item {
    id: item

    property alias checked: qfieldswitch.checked
    property alias text: qfieldswitch.text

    height: 48

    Switch {
        id: qfieldswitch

        anchors.verticalCenter: parent.verticalCenter

        indicator: Rectangle {
            implicitHeight: 18
            implicitWidth: 36
            x: qfieldswitch.leftPadding
            radius: 9
            color: qfieldswitch.checked ? "#B9CCA3" : "#cccccc"
            anchors.verticalCenter: parent.verticalCenter

            Rectangle {
                x: qfieldswitch.checked ? parent.width - width : 0
                width: 18
                height: 18
                radius: 9
                color:  qfieldswitch.checked ? qfieldswitch.down ? "#cccccc" : Theme.mainColor : qfieldswitch.down ? Theme.mainColor : "#ffffff"
                border.color: "#cccccc"
            }
        }

        contentItem: Text {
            text: qfieldswitch.text
            height: 36
            font.pointSize: 12
            verticalAlignment: Text.AlignVCenter
            leftPadding: qfieldswitch.indicator.width + qfieldswitch.indicator.width / 2
        }
    }
}
// [/hidpi fixes]
