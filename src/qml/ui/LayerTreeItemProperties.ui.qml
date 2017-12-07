import QtQuick 2.4
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Popup {
    property alias itemVisible: itemVisibleCheckBox.checked

    Page {
        header: Label {
            text: title
            font.pointSize: 14
        }

        CheckBox {
            id: itemVisibleCheckBox
            text: qsTr("Show on map canvas")
            font.pointSize: 14
        }
    }
}
