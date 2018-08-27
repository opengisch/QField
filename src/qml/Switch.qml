// [hidpi fixes]
import QtQuick 2.6
import QtQuick.Controls 2.0

Item {
    id: item

    property alias checked: qfieldswitch.checked
    property alias text: qfieldswitch.text

    height: 48 * dp

    Switch {
        id: qfieldswitch

        anchors.verticalCenter: parent.verticalCenter

        indicator: Rectangle {
            implicitHeight: 24 * dp
            implicitWidth: 48 * dp
            x: qfieldswitch.leftPadding
            radius: 12 * dp
            color: qfieldswitch.checked ? "#B9CCA3" : "#cccccc"
            anchors.verticalCenter: parent.verticalCenter

            Rectangle {
                x: qfieldswitch.checked ? parent.width - width : 0
                width: 24 * dp
                height: 24 * dp
                radius: 12 * dp
                color:  qfieldswitch.checked ? qfieldswitch.down ? "#cccccc" : "#80CC28" : qfieldswitch.down ? "#80CC28" : "#ffffff"
                border.color: "#cccccc"
            }
        }

        contentItem: Text {
            text: qfieldswitch.text
            height: 36 * dp
            font.pointSize: 12
            verticalAlignment: Text.AlignVCenter
            leftPadding: qfieldswitch.indicator.width + qfieldswitch.indicator.width / 2
        }
    }
}
// [/hidpi fixes]
