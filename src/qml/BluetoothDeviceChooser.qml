import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield
import Theme

/**
 * \ingroup qml
 */
Item {
  id: bluetoothDeviceChooser

  property bool isScanning: bluetoothDeviceModel.scanningStatus === BluetoothDeviceModel.FastScanning || bluetoothDeviceModel.scanningStatus === BluetoothDeviceModel.FullScanning
  property bool scannedOnce: false

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
    return {
      "name": deviceName,
      "address": deviceAddress
    };
  }

  function close() {
    if (isScanning) {
      bluetoothDeviceModel.stopServiceDiscovery();
    }
  }

  ColumnLayout {
    width: parent.width
    spacing: 5

    RowLayout {
      Layout.fillWidth: true
      spacing: 2

      QfButton {
        id: scanButton
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignVCenter
        enabled: !bluetoothDeviceChooser.isScanning
        width: parent.width
        text: qsTr('Scan for nearby devices')

        onClicked: {
          bluetoothDeviceModel.startServiceDiscovery();
        }
      }

      BusyIndicator {
        id: busyIndicator
        Layout.preferredWidth: width
        Layout.alignment: Qt.AlignVCenter
        visible: running
        width: 36
        height: width
        running: bluetoothDeviceChooser.isScanning
      }
    }

    Label {
      Layout.fillWidth: true
      text: bluetoothDeviceComboBox.count > 0 ? qsTr("Select the Bluetooth device from the list below:") : !bluetoothDeviceChooser.isScanning && bluetoothDeviceChooser.scannedOnce ? qsTr("No Bluetooth devices detected.") : ""
      font: Theme.defaultFont
      wrapMode: Text.WordWrap
    }

    ComboBox {
      id: bluetoothDeviceComboBox
      Layout.fillWidth: true
      visible: bluetoothDeviceComboBox.count > 0
      font: Theme.defaultFont

      popup.font: Theme.defaultFont
      popup.topMargin: mainWindow.sceneTopMargin
      popup.bottomMargin: mainWindow.sceneTopMargin

      textRole: 'display'
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
          bluetoothDeviceComboBox.currentIndex = selectedBluetoothDevice;
        }

        function onLastErrorChanged(lastError) {
          displayToast(qsTr('Scanning error: %1').arg(lastError), 'error');
          console.log(lastError);
        }

        function onScanningStatusChanged(scanningStatus) {
          switch (scanningStatus) {
          case BluetoothDeviceModel.FastScanning:
            displayToast(qsTr('Scanning for paired devices'));
            break;
          case BluetoothDeviceModel.FullScanning:
            displayToast(qsTr('Deeper scanning for paired devices'));
            break;
          case BluetoothDeviceModel.Failed:
            displayToast(qsTr('Scanning failed: %1').arg(bluetoothDeviceModel.lastError), 'error');
            break;
          case BluetoothDeviceModel.Succeeded:
            let message = qsTr('Scanning done');
            if (bluetoothDeviceModel.rowCount() > 1) {
              message += ': ' + qsTr('%n device(s) found', '', bluetoothDeviceModel.rowCount() - 1);
            }
            displayToast(message);
            break;
          case BluetoothDeviceModel.Canceled:
            displayToast(qsTr('Scanning canceled'));
            break;
          }
        }
      }
    }

    Label {
      id: bluetoothDeviceName
      Layout.fillWidth: true
      visible: deviceAddress != ''
      font: Theme.defaultFont
      color: Theme.secondaryTextColor
      text: qsTr('Bluetooth device name:') + '\n ' + deviceName
      wrapMode: Text.WordWrap
    }

    Label {
      id: bluetoothDeviceAddress
      Layout.fillWidth: true
      visible: deviceAddress != ''
      font: Theme.defaultFont
      color: Theme.secondaryTextColor
      text: qsTr('Bluetooth device address:') + '\n ' + deviceAddress
      wrapMode: Text.WordWrap
    }
  }
}
