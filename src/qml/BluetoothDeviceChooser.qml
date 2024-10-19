import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield
import Theme

/**
 * \ingroup qml
 */
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
    return {
      "name": deviceName,
      "address": deviceAddress
    };
  }

  GridLayout {
    width: parent.width
    columns: 1
    columnSpacing: 0
    rowSpacing: 5

    Rectangle {
      color: "transparent"
      Layout.fillWidth: true
      Layout.preferredHeight: childrenRect.height
      Layout.alignment: Qt.AlignVCenter

      QfButton {
        id: scanButton
        width: parent.width
        text: qsTr('Scan for nearby devices')

        onClicked: {
          bluetoothDeviceModel.startServiceDiscovery(false);
        }
        onPressAndHold: {
          fullDiscoveryDialog.open();
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

    Label {
      Layout.fillWidth: true
      text: bluetoothDeviceComboBox.count > 0 ? qsTr("Select the Bluetooth device from the list below:") : qsTr("No Bluetooth devices detected, scan to populate nearby devices.")
      font: Theme.defaultFont

      wrapMode: Text.WordWrap
    }

    ComboBox {
      id: bluetoothDeviceComboBox
      Layout.fillWidth: true
      visible: bluetoothDeviceComboBox.count
      font: Theme.defaultFont

      popup.font: Theme.defaultFont
      popup.topMargin: mainWindow.sceneTopMargin
      popup.bottomMargin: mainWindow.sceneTopMargin

      enabled: bluetoothDeviceModel.scanningStatus !== BluetoothDeviceModel.Scanning

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
          if (scanningStatus === BluetoothDeviceModel.Scanning) {
            displayToast(qsTr('Scanning for paired devices'));
          }
          if (scanningStatus === BluetoothDeviceModel.Failed) {
            displayToast(qsTr('Scanning failed: %1').arg(bluetoothDeviceModel.lastError), 'error');
          }
          if (scanningStatus === BluetoothDeviceModel.Succeeded) {
            var message = qsTr('Scanning done');
            if (bluetoothDeviceModel.rowCount() > 1) {
              message += ': ' + qsTr('%n device(s) found', '', bluetoothDeviceModel.rowCount() - 1);
            }
            displayToast(message);
          }
          if (scanningStatus === BluetoothDeviceModel.Canceled) {
            displayToast(qsTr('Scanning canceled'));
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

  QfDialog {
    id: fullDiscoveryDialog
    parent: mainWindow.contentItem
    title: qsTr("Make a full service discovery")
    Label {
      width: parent.width
      wrapMode: Text.WordWrap
      text: qsTr('A full device scan can take longer. You really want to do it?\nCancel to make a minimal device scan instead.')
    }

    onAccepted: {
      bluetoothDeviceModel.startServiceDiscovery(true);
      visible = false;
    }
    onRejected: {
      bluetoothDeviceModel.startServiceDiscovery(false);
      visible = false;
    }
  }
}
