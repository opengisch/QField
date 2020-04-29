import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import Theme 1.0

Popup {
    property alias itemVisible: itemVisibleCheckBox.checked

    signal trackingButtonClicked

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

        ColumnLayout{
            spacing: 4 * dp

            CheckBox {
                id: itemVisibleCheckBox
                text: qsTr("Show on map canvas")
                font: Theme.defaultFont

                indicator.height: 16 * dp
                indicator.width: 16 * dp
                indicator.implicitHeight: 24 * dp
                indicator.implicitWidth: 24 * dp
            }

            Button {
                id: trackingButton
                Layout.fillHeight: true
                Layout.fillWidth: true
                font: Theme.defaultFont
                text: trackingButtonText
                visible: trackingButtonVisible
                background: Rectangle {
                    color: trackingButtonBgColor
                }

                onClicked: {
                    trackingButtonClicked()
                }
            }
        }

    }
}
