import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import org.qfield 1.0

import Theme 1.0

Item {
  width: parent.width

  property string deviceName: ''
  property string deviceAddress: ''

  function generateName() {
    return deviceName;
  }

  function setSettings(settings) {
    deviceName = settings['name'];
    deviceAddress = settings['address'];
  }

  function getSettings() {
    return {'name': deviceName,
            'address': deviceAddress};
  }

  GridLayout {
    width: parent.width
    columns: 2
    columnSpacing: 0
    rowSpacing: 5

    Label {
      Layout.fillWidth: true
      Layout.columnSpan: 2
      text: qsTr( "Select the Bluetooth device from the list below:" )
      font: Theme.defaultFont

      wrapMode: Text.WordWrap
    }

    RowLayout {
      Layout.fillWidth: true
      Layout.columnSpan: 2

      ComboBox {
        id: bluetoothDeviceComboBox
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignVCenter
        font: Theme.defaultFont
        popup.font: Theme.defaultFont

        enabled: bluetoothDeviceModel.scanningStatus !== BluetoothDeviceModel.Scanning
        model: BluetoothDeviceModel {
          id: bluetoothDeviceModel
        }

        property string selectedBluetoothDevice

        onCurrentIndexChanged: {
          var modelIndex = bluetoothDeviceModel.index(currentIndex, 0);
          deviceName = bluetoothDeviceModel.data(modelIndex, BluetoothDeviceModel.DeviceNameRole);
          deviceAddress = bluetoothDeviceModel.data(modelIndex, BluetoothDeviceModel.DeviceAddressRole);
          selectedBluetoothDevice = bluetoothDeviceAddress.text;
        }

        Connections {
          target: bluetoothDeviceModel

          function onModelReset() {
            bluetoothDeviceComboBox.currentIndex = selectedBluetoothDevice
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
    }

    Label {
        id: bluetoothDeviceName
        Layout.fillWidth: true
        Layout.columnSpan: 2
        font: Theme.defaultFont
        color: Theme.gray
        text: qsTr('Bluetooth device name:') + '\n' + (deviceName !== '' ? deviceName : qsTr(' N/A'))
        wrapMode: Text.WordWrap
    }

    Label {
        id: bluetoothDeviceAddress
        Layout.fillWidth: true
        Layout.columnSpan: 2
        font: Theme.defaultFont
        color: Theme.gray
        text: qsTr('Bluetooth device address:') + '\n' + (deviceAddress !== '' ? deviceAddress : qsTr(' N/A'))
        wrapMode: Text.WordWrap
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
