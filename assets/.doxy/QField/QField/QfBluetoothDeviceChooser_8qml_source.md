

# File QfBluetoothDeviceChooser.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfBluetoothDeviceChooser.qml**](QfBluetoothDeviceChooser_8qml.md)

[Go to the documentation of this file](QfBluetoothDeviceChooser_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield.core
import org.qfield.gui

Item {
  id: bluetoothDeviceChooser

  property bool isScanning: bluetoothDeviceModel.scanningStatus === QfBluetoothDeviceModel.Discovering
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
      font: QfTheme.defaultFont
      wrapMode: Text.WordWrap
    }

    QfComboBox {
      id: bluetoothDeviceComboBox
      Layout.fillWidth: true
      visible: bluetoothDeviceComboBox.count > 0
      font: QfTheme.defaultFont

      popup.font: QfTheme.defaultFont
      popup.topMargin: mainWindow.sceneTopMargin
      popup.bottomMargin: mainWindow.sceneTopMargin

      textRole: 'display'
      model: QfBluetoothDeviceModel {
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
          case QfBluetoothDeviceModel.Discovering:
            displayToast(qsTr('Scanning for paired devices'));
            break;
          case QfBluetoothDeviceModel.Failed:
            displayToast(qsTr('Scanning failed: %1').arg(bluetoothDeviceModel.lastError), 'error');
            break;
          case QfBluetoothDeviceModel.Succeeded:
            let message = qsTr('Scanning done');
            if (bluetoothDeviceModel.lastDiscoveredCount > 0) {
              message += ': ' + qsTr('%n device(s) found', '', bluetoothDeviceModel.lastDiscoveredCount);
            }
            displayToast(message);
            break;
          case QfBluetoothDeviceModel.Canceled:
            displayToast(qsTr('Scanning canceled'));
            break;
          }
        }
      }

      property string selectedBluetoothDevice

      onCurrentIndexChanged: {
        let idx = bluetoothDeviceModel.index(currentIndex, 0);
        deviceName = bluetoothDeviceModel.data(idx, QfBluetoothDeviceModel.DeviceNameRole).trim();
        deviceAddress = bluetoothDeviceModel.data(idx, QfBluetoothDeviceModel.DeviceAddressRole);
        deviceClassicSupport = bluetoothDeviceModel.data(idx, QfBluetoothDeviceModel.DeviceClassicSupportRole);
        deviceLowEnergySupport = bluetoothDeviceModel.data(idx, QfBluetoothDeviceModel.DeviceLowEnergySupportRole);
        deviceLowEnergyByDefault = bluetoothDeviceModel.data(idx, QfBluetoothDeviceModel.DeviceLowEnergyByDefaultRole);
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
        font: QfTheme.defaultFont
        wrapMode: Text.WordWrap
      }

      QfSwitch {
        id: preferBLESwitch
        Layout.preferredWidth: QfTheme.toolButtonSize
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
      font: QfTheme.defaultFont
      color: QfTheme.secondaryTextColor
      text: qsTr('Bluetooth device name:') + '\n ' + deviceName
      wrapMode: Text.WordWrap
    }

    Label {
      id: bluetoothDeviceAddress
      Layout.fillWidth: true
      Layout.leftMargin: 10
      visible: deviceAddress != ''
      font: QfTheme.defaultFont
      color: QfTheme.secondaryTextColor
      text: qsTr('Bluetooth device address:') + '\n ' + deviceAddress
      wrapMode: Text.WordWrap
    }

    Label {
      id: bluetoothDeviceCoreConfiguration
      Layout.fillWidth: true
      Layout.leftMargin: 10
      visible: deviceAddress != ''
      font: QfTheme.defaultFont
      color: QfTheme.secondaryTextColor
      text: qsTr('Bluetooth device configuration:') + '\n ' + (deviceBLE ? qsTr("Low Energy (BLE)") : qsTr("Classic (BT)"))
      wrapMode: Text.WordWrap
    }
  }
}
```


