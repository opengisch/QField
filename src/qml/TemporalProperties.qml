import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import org.qgis 1.0
import org.qfield 1.0

import Theme 1.0

Popup {
    id: popup

    property MapSettings mapSettings

    width: Math.min(350, mainWindow.width - 20 )
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    padding: 0

    Page {
        width: parent.width
        padding: 10
        header: Label {
            padding: 10
            topPadding: 15
            bottomPadding: 0
            width: parent.width - 20
            text: qsTr('Temporal Properties')
            font: Theme.strongFont
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
        }

        ColumnLayout {
            spacing: 4
            width: parent.width

            CheckBox {
                id: isTemporalCheckBox
                Layout.fillWidth: true
                topPadding: 5
                bottomPadding: 5
                text: qsTr('Enable temporal filter')
                font: Theme.defaultFont
                // visible for all layer tree items but nonspatial vector LayerSelector
                indicator.height: 16
                indicator.width: 16
                indicator.implicitHeight: 24
                indicator.implicitWidth: 24
                checked: mapCanvas.mapSettings.isTemporal

                onClicked: {
                    mapCanvas.mapSettings.isTemporal = !mapCanvas.mapSettings.isTemporal;
                }
            }


                TextField {
                    id: beginField
                    Layout.fillWidth: true
                    Layout.leftMargin: 20
                    Layout.rightMargin: 20

                    enabled: mapCanvas.mapSettings.isTemporal
                    font: Theme.defaultFont
                    horizontalAlignment: TextInput.AlignHCenter

                    inputMethodHints: Qt.ImhDigitsOnly
                    inputMask: "9999-99-99 99:99:99;_"

                    text: Qt.formatDateTime(mapCanvas.mapSettings.temporalBegin, "yyyy-MM-dd HH:mm:ss")

                    background: Rectangle {
                        y: beginField.height - height * 2 - beginField.bottomPadding / 2
                        implicitWidth: 120
                        height: beginField.activeFocus ? 2 : 1
                        color: beginField.activeFocus ? "#4CAF50" : "#C8E6C9"
                    }

                    onTextEdited: {
                        var newDate = Date.fromLocaleString(Qt.locale(), beginField.text, "yyyy-MM-dd HH:mm:ss")
                        if ( newDate.toLocaleString() !== "" )
                        {
                            mapCanvas.mapSettings.temporalBegin = newDate
                        }
                        else
                        {
                            mapCanvas.mapSettings.temporalBegin = undefined
                        }
                    }
                }

                Label {
                    Layout.fillWidth: true

                    enabled: mapCanvas.mapSettings.isTemporal
                    font: Theme.defaultFont
                    horizontalAlignment: TextInput.AlignHCenter
                    text: '≤ t ≤'
                }

                TextField {
                    id: endField
                    Layout.fillWidth: true
                    Layout.leftMargin: 20
                    Layout.rightMargin: 20

                    enabled: mapCanvas.mapSettings.isTemporal
                    font: Theme.defaultFont
                    horizontalAlignment: TextInput.AlignHCenter

                    inputMethodHints: Qt.ImhDigitsOnly
                    inputMask: "9999-99-99 99:99:99;_"

                    text: Qt.formatDateTime(mapCanvas.mapSettings.temporalEnd, "yyyy-MM-dd HH:mm:ss")

                    background: Rectangle {
                        y: endField.height - height * 2 - endField.bottomPadding / 2
                        implicitWidth: 120
                        height: endField.activeFocus ? 2 : 1
                        color: endField.activeFocus ? "#4CAF50" : "#C8E6C9"
                    }

                    onTextEdited: {
                        var newDate = Date.fromLocaleString(Qt.locale(), endField.text, "yyyy-MM-dd HH:mm:ss")
                        if ( newDate.toLocaleString() !== "" )
                        {
                            mapCanvas.mapSettings.temporalEnd = newDate
                        }
                        else
                        {
                            mapCanvas.mapSettings.temporalEnd = undefined
                        }
                    }
            }
        }
    }
}
