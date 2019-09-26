import QtQuick 2.4
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Popup {
    property alias itemVisible: itemVisibleCheckBox.checked

    leftPadding: 20 * dp
    rightPadding: 20 * dp
    topPadding: 20 * dp
    bottomPadding: 10 * dp

    Page {
        header: Label {
            text: title
            font.bold: true
            font.pointSize: 18 * dp
        }

        CheckBox {
            id: itemVisibleCheckBox
            text: qsTr("Show on map canvas")
            font.pointSize: 18 * dp
        }
    }
}
