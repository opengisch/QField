import QtQuick 2.4
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Popup {
    property alias itemVisible: itemVisibleCheckBox.checked

    Page {
        Layout.margins: 50 * dp

        header: Label {
            text: title
        }

        CheckBox {
            id: itemVisibleCheckBox
            text: qsTr("Layer visitibility")
        }
    }
}
