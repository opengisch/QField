import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import org.qgis 1.0
import org.qfield 1.0
import Theme 1.0
import QFieldControls 1.0

Popup {
    id: popup

    property MapSettings mapSettings

    width: Math.min(350, mainWindow.width - Theme.popupScreenEdgeMargin )
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
                // visible for all layer tree items but nonspatial vector layer
                indicator.height: 16
                indicator.width: 16
                indicator.implicitHeight: 24
                indicator.implicitWidth: 24
                checked: mapCanvas.mapSettings.isTemporal

                onClicked: {
                    mapCanvas.mapSettings.isTemporal = !mapCanvas.mapSettings.isTemporal;
                }
            }

            RowLayout {
                Layout.fillWidth: true
                Layout.leftMargin: 20
                Layout.rightMargin: 20

                QfTextField {
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

                    onTextEdited: {
                        var newDate = Date.fromLocaleString(Qt.locale(), beginField.text, "yyyy-MM-dd HH:mm:ss")
                        if ( newDate.toLocaleString() !== "" )
                        {
                            mapCanvas.mapSettings.temporalBegin = newDate
                        }
                        else
                        {
                            mapCanvas.mapSettings.temporalBegin = Date.fromLocaleString('')
                        }
                    }
                }

                QfToolButton {
                    iconSource: Theme.getThemeIcon("ic_calendar_month_black_24dp")
                    bgcolor: "transparent"
                    onClicked: {
                        calendarPanel.selectedDate = mapCanvas.mapSettings.temporalBegin
                        calendarPanel.temporalField = 'begin';
                        calendarPanel.open();
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

            RowLayout {
                Layout.fillWidth: true
                Layout.leftMargin: 20
                Layout.rightMargin: 20


                QfTextField {
                    id: endField
                    Layout.fillWidth: true

                    enabled: mapCanvas.mapSettings.isTemporal
                    font: Theme.defaultFont
                    horizontalAlignment: TextInput.AlignHCenter

                    inputMethodHints: Qt.ImhDigitsOnly
                    inputMask: "9999-99-99 99:99:99;_"

                    text: Qt.formatDateTime(mapCanvas.mapSettings.temporalEnd, "yyyy-MM-dd HH:mm:ss")

                    onTextEdited: {
                        var newDate = Date.fromLocaleString(Qt.locale(), endField.text, "yyyy-MM-dd HH:mm:ss")
                        if ( newDate.toLocaleString() !== "" )
                        {
                            mapCanvas.mapSettings.temporalEnd = newDate
                        }
                        else
                        {
                            mapCanvas.mapSettings.temporalEnd = Date.fromLocaleString('')
                        }
                    }
                }

                QfToolButton {
                    iconSource: Theme.getThemeIcon("ic_calendar_month_black_24dp")
                    bgcolor: "transparent"
                    onClicked: {
                        calendarPanel.selectedDate = mapCanvas.mapSettings.temporalEnd
                        calendarPanel.temporalField = 'end';
                        calendarPanel.open();
                    }
                }
            }
        }
    }

    CalendarPanel {
        id: calendarPanel

        property string temporalField: ''

        isDateTime: true

        onDateTimePicked: {
            if (temporalField == 'begin') {
                mapCanvas.mapSettings.temporalBegin = date
                beginField.text = Qt.formatDateTime(date, "yyyy-MM-dd HH:mm:ss");
            } else if (temporalField == 'end') {
                mapCanvas.mapSettings.temporalEnd = date
                endField.text = Qt.formatDateTime(date, "yyyy-MM-dd HH:mm:ss");
            }
            temporalField = '';
        }
    }
}
