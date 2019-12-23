import QtQuick 2.4
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import Theme 1.0

Popup {
    property alias itemVisible: itemVisibleCheckBox.checked
    padding: 0

    Page {
        padding: 10 * dp
        header: Label {
            padding: 10 * dp
            topPadding: 20 * dp
            bottomPadding: 5 * dp
            anchors.left:parent.left
            anchors.right:parent.right
            text: title
            font: Theme.strongFont
        }

        CheckBox {
            id: itemVisibleCheckBox
            text: qsTr("Show on map canvas")
            font: Theme.defaultFont

            indicator.height: 16 * dp
            indicator.width: 16 * dp
            indicator.implicitHeight: 24 * dp
            indicator.implicitWidth: 24 * dp
        }
    }
}
