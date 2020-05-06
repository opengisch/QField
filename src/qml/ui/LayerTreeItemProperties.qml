import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import Theme 1.0

Popup {
    property alias itemVisible: itemVisibleCheckBox.checked

    signal trackingButtonClicked

    padding: 0

    Page {
        padding: 10
        header: Label {
            padding: 10
            topPadding: 20
            bottomPadding: 5
            anchors.left:parent.left
            anchors.right:parent.right
            text: title
            font: Theme.strongFont
        }

        ColumnLayout{
            spacing: 4
            width: Math.min(mainWindow.width - 20, parent.width)

            CheckBox {
                id: itemVisibleCheckBox
                Layout.fillWidth: true
                text: qsTr("Show on map canvas")
                font: Theme.defaultFont

                indicator.height: 16
                indicator.width: 16
                indicator.implicitHeight: 24
                indicator.implicitWidth: 24
            }

            QfButton {
                id: trackingButton
                Layout.fillWidth: true
                font: Theme.defaultFont
                text: trackingButtonText
                visible: trackingButtonVisible

                onClicked: {
                    trackingButtonClicked()
                }
            }
        }

    }
}
