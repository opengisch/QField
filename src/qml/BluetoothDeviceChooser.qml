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

  property bool isScanning: bluetoothDeviceModel.scanningStatus === BluetoothDeviceModel.Discovering
  property bool scannedOnce: false

  width: parent.width
  height: childrenRect.height

  property string deviceName: ''
  property string deviceAddress: ''
  property bool deviceBLE: false

  property bool deviceClassicSupport: false
  property bool deviceLowEnergySupport: false
  property bool deviceLowEnergyByDefault: false

  function generateName() {
    return deviceName + (deviceBLE ? ' (BLE)' : ' (BT)');
  }

  function setSettings(settings) {
    deviceName = settings['name'];
    deviceAddress = settings['address'];
    deviceBLE = !!settings['ble'];
  }

  function getSettings() {
    return {
      "name": deviceName,
      "address": deviceAddress,
      "ble": deviceBLE
    };
  }

  function close() {
    if (isScanning) {
      bluetoothDeviceModel.stopDeviceDiscovery();
    }
  }

  function pickConfiguration() {
    if (Qt.platform.os === "ios" || (preferBLESwitch.checked && deviceLowEnergySupport)) {
      deviceBLE = true;
    } else {
      deviceBLE = !deviceClassicSupport;
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
        showProgress: bluetoothDeviceChooser.isScanning
        progressValue: 0

        onClicked: {
          bluetoothDeviceModel.startDeviceDiscovery();
        }
      }
    }

    Label {
      Layout.fillWidth: true
      text: bluetoothDeviceComboBox.count > 0 ? qsTr("Select the Bluetooth device from the list below:") : !bluetoothDeviceChooser.isScanning && bluetoothDeviceChooser.scannedOnce ? qsTr("No Bluetooth devices detected.") : ""
      font: Theme.defaultFont
      wrapMode: Text.WordWrap
    }

    QfComboBox {
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

        onModelReset: {
          bluetoothDeviceComboBox.currentIndex = selectedBluetoothDevice;
        }

        onLastErrorChanged: lastError => {
          displayToast(qsTr('Scanning error: %1').arg(lastError), 'error');
          console.log(lastError);
        }

        onScanningStatusChanged: scanningStatus => {
          switch (scanningStatus) {
          case BluetoothDeviceModel.Discovering:
            displayToast(qsTr('Scanning for paired devices'));
            break;
          case BluetoothDeviceModel.Failed:
            displayToast(qsTr('Scanning failed: %1').arg(bluetoothDeviceModel.lastError), 'error');
            break;
          case BluetoothDeviceModel.Succeeded:
            let message = qsTr('Scanning done');
            if (bluetoothDeviceModel.lastDiscoveredCount > 0) {
              message += ': ' + qsTr('%n device(s) found', '', bluetoothDeviceModel.lastDiscoveredCount);
            }
            displayToast(message);
            break;
          case BluetoothDeviceModel.Canceled:
            displayToast(qsTr('Scanning canceled'));
            break;
          }
        }
      }

      property string selectedBluetoothDevice

      onCurrentIndexChanged: {
        let idx = bluetoothDeviceModel.index(currentIndex, 0);
        deviceName = bluetoothDeviceModel.data(idx, BluetoothDeviceModel.DeviceNameRole).trim();
        deviceAddress = bluetoothDeviceModel.data(idx, BluetoothDeviceModel.DeviceAddressRole);
        deviceClassicSupport = bluetoothDeviceModel.data(idx, BluetoothDeviceModel.DeviceClassicSupportRole);
        deviceLowEnergySupport = bluetoothDeviceModel.data(idx, BluetoothDeviceModel.DeviceLowEnergySupportRole);
        deviceLowEnergyByDefault = bluetoothDeviceModel.data(idx, BluetoothDeviceModel.DeviceLowEnergyByDefaultRole);
        selectedBluetoothDevice = bluetoothDeviceAddress.text;

        preferBLESwitch.checked = deviceLowEnergyByDefault;

        pickConfiguration();
      }
    }

    RowLayout {
      Layout.fillWidth: true
      visible: deviceLowEnergySupport

      Label {
        id: preferBLELabel
        Layout.fillWidth: true
        text: qsTr('Use Bluetooth Low Energy (BLE)')
        font: Theme.defaultFont
        wrapMode: Text.WordWrap
      }

      QfSwitch {
        id: preferBLESwitch
        Layout.preferredWidth: 48
        Layout.alignment: Qt.AlignVCenter
        visible: Qt.platform.os !== "ios" && deviceClassicSupport
        checked: false

        onToggled: {
          pickConfiguration();
        }
      }
    }

    Label {
      id: bluetoothDeviceName
      Layout.fillWidth: true
      Layout.leftMargin: 10
      visible: deviceAddress != ''
      font: Theme.defaultFont
      color: Theme.secondaryTextColor
      text: qsTr('Bluetooth device name:') + '\n ' + deviceName
      wrapMode: Text.WordWrap
    }

    Label {
      id: bluetoothDeviceAddress
      Layout.fillWidth: true
      Layout.leftMargin: 10
      visible: deviceAddress != ''
      font: Theme.defaultFont
      color: Theme.secondaryTextColor
      text: qsTr('Bluetooth device address:') + '\n ' + deviceAddress
      wrapMode: Text.WordWrap
    }

    Label {
      id: bluetoothDeviceCoreConfiguration
      Layout.fillWidth: true
      Layout.leftMargin: 10
      visible: deviceAddress != ''
      font: Theme.defaultFont
      color: Theme.secondaryTextColor
      text: qsTr('Bluetooth device configuration:') + '\n ' + (deviceBLE ? qsTr("Low Energy (BLE)") : qsTr("Classic (BT)"))
      wrapMode: Text.WordWrap
    }
  }
}
