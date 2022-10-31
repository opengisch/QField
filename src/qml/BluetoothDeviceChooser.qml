import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import org.qfield 1.0

import Theme 1.0

GridLayout {
    Layout.fillWidth: true

    columns: 2
    columnSpacing: 0
    rowSpacing: 5

    Label {
        Layout.fillWidth: true
        Layout.columnSpan: 2
        text: qsTr( "Positioning device in use:" )
        font: Theme.defaultFont

        wrapMode: Text.WordWrap
    }

    RowLayout {
        Layout.fillWidth: true
        Layout.columnSpan: 2

        ComboBox {
            id: bluetoothDeviceComboBox
            enabled: bluetoothDeviceModel.scanningStatus !== BluetoothDeviceModel.Scanning
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignVCenter
            font: Theme.defaultFont
            popup.font: Theme.defaultFont
            textRole: 'display'
            model: BluetoothDeviceModel {
                id: bluetoothDeviceModel
            }

            property string selectedPositioningDevice

            onCurrentIndexChanged: {
                if( bluetoothDeviceModel.scanningStatus !== BluetoothDeviceModel.Scanning )
                {
                    selectedPositioningDevice = bluetoothDeviceModel.data(bluetoothDeviceModel.index(currentIndex, 0), BluetoothDeviceModel.DeviceAddressRole );
                }
                if( positioningSettings.positioningDevice !== selectedPositioningDevice )
                {
                    positioningSettings.positioningDevice = selectedPositioningDevice
                    positioningSettings.positioningDeviceName = bluetoothDeviceModel.data(bluetoothDeviceModel.index(currentIndex, 0), BluetoothDeviceModel.DeviceNameRole );
                }
            }

            Component.onCompleted: {
                currentIndex = bluetoothDeviceModel.findAddressIndex(settings.value('positioningDevice',''))
            }

            Connections {
                target: bluetoothDeviceModel

                function onModelReset() {
                    bluetoothDeviceComboBox.currentIndex = bluetoothDeviceModel.findAddressIndex(positioningSettings.positioningDevice)
                }

                function onScanningStatusChanged(scanningStatus) {
                    if( scanningStatus === BluetoothDeviceModel.Scanning )
                    {
                        displayToast( qsTr('Scanning for paired devices') )
                    }
                    if( scanningStatus === BluetoothDeviceModel.Failed )
                    {
                        displayToast( qsTr('Scanning failed: %1').arg( bluetoothDeviceModel.lastError ), 'error' )
                    }
                    if( scanningStatus === BluetoothDeviceModel.Succeeded )
                    {
                        var message = qsTr('Scanning done')
                        if ( bluetoothDeviceModel.rowCount() > 1 )
                        {
                            message += ': ' + qsTr( '%n device(s) found', '', bluetoothDeviceModel.rowCount() - 1 )
                        }
                        displayToast( message )
                    }
                    if( scanningStatus === BluetoothDeviceModel.Canceled )
                    {
                        displayToast( qsTr('Scanning canceled') )
                    }
                }
            }
        }

        Rectangle {
            color: "transparent"
            Layout.preferredWidth: childrenRect.width
            Layout.preferredHeight: childrenRect.height
            Layout.alignment: Qt.AlignVCenter

            QfButton {
                id: scanButton
                leftPadding: 10
                rightPadding: 10
                text: qsTr('Scan')

                onClicked: {
                    bluetoothDeviceModel.startServiceDiscovery( false )
                }
                onPressAndHold: {
                    fullDiscoveryDialog.open()
                }

                enabled: bluetoothDeviceModel.scanningStatus !== BluetoothDeviceModel.Scanning
                opacity: enabled ? 1 : 0
            }


            BusyIndicator {
                id: busyIndicator
                anchors.centerIn: scanButton
                width: 36
                height: 36
                running: bluetoothDeviceModel.scanningStatus === BluetoothDeviceModel.Scanning
            }
        }

        Dialog {
            id: fullDiscoveryDialog
            parent: mainWindow.contentItem

            visible: false
            modal: true
            font: Theme.defaultFont

            x: ( mainWindow.width - width ) / 2
            y: ( mainWindow.height - height ) / 2

            title: qsTr( "Make a full service discovery" )
            Label {
                width: parent.width
                wrapMode: Text.WordWrap
                text: qsTr( 'A full device scan can take longer. You really want to do it?\nCancel to make a minimal device scan instead.')
            }

            standardButtons: Dialog.Ok | Dialog.Cancel
            onAccepted: {
                bluetoothDeviceModel.startServiceDiscovery( true )
                visible = false
            }
            onRejected: {
                bluetoothDeviceModel.startServiceDiscovery( false )
                visible = false
            }
        }
    }

    QfButton {
        id: connectButton
        Layout.fillWidth: true
        Layout.columnSpan: 2
        Layout.topMargin: 5
        text: {
            switch (positionSource.device.socketState) {
                case QAbstractSocket.ConnectedState:
                    return qsTr('Connected to %1').arg(positioningSettings.positioningDeviceName)
                case QAbstractSocket.UnconnectedState:
                    return qsTr('Connect to %1').arg(positioningSettings.positioningDeviceName)
                default:
                    return qsTr('Connecting to %1').arg(positioningSettings.positioningDeviceName)
            }
        }
        enabled: positionSource.device.socketState === QAbstractSocket.UnconnectedState
        visible: positionSource.deviceId !== ''

        onClicked: {
            // make sure positioning is active when connecting to the bluetooth device
            if (!positioningSettings.positioningActivated) {
                positioningSettings.positioningActivated = true
            } else {
                positionSource.device.connectDevice()
            }
        }
    }

    Label {
        text: qsTr("Use orthometric altitude from device")
        font: Theme.defaultFont
        wrapMode: Text.WordWrap
        Layout.fillWidth: true
        visible: positioningSettings.positioningDevice !== ''

        MouseArea {
            anchors.fill: parent
            onClicked: reportOrthometricAltitude.toggle()
        }
    }

    QfSwitch {
        id: reportOrthometricAltitude
        Layout.preferredWidth: implicitContentWidth
        Layout.alignment: Qt.AlignTop
        visible: positioningSettings.positioningDevice !== ''
        checked: !positioningSettings.ellipsoidalElevation
        onCheckedChanged: {
            positioningSettings.ellipsoidalElevation = !checked
        }
    }
}
