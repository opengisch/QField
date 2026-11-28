

# File QfSerialPortDeviceChooser.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfSerialPortDeviceChooser.qml**](QfSerialPortDeviceChooser_8qml.md)

[Go to the documentation of this file](QfSerialPortDeviceChooser_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield.core
import org.qfield.gui

Item {
  width: parent.width
  height: childrenRect.height

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

  Component.onCompleted: {
    serialPortModel.refresh();
  }

  GridLayout {
    width: parent.width
    columns: 1
    columnSpacing: 0
    rowSpacing: 5

    Label {
      Layout.fillWidth: true
      text: serialPortComboBox.count > 0 ? qsTr("Select the serial port from the list below:") : qsTr("No serial ports detected, refresh the list once a device is connected.")
      font: QfTheme.defaultFont

      wrapMode: Text.WordWrap
    }

    QfComboBox {
      id: serialPortComboBox
      Layout.fillWidth: true
      visible: serialPortComboBox.count
      font: QfTheme.defaultFont

      popup.font: QfTheme.defaultFont
      popup.topMargin: mainWindow.sceneTopMargin
      popup.bottomMargin: mainWindow.sceneTopMargin

      textRole: 'display'
      model: QfSerialPortModel {
        id: serialPortModel
      }

      property string selectedSerialPort

      onCurrentIndexChanged: {
        var modelIndex = serialPortModel.index(currentIndex, 0);
        deviceName = serialPortModel.data(modelIndex, Qt.DisplayRole);
        deviceAddress = serialPortModel.data(modelIndex, QfSerialPortModel.PortNameRole);
        selectedSerialPort = serialPortAddress.text;
      }
    }

    QfButton {
      id: refreshButton
      Layout.fillWidth: true
      text: qsTr('Refresh list')

      onClicked: {
        serialPortModel.refresh();
      }
    }

    Label {
      id: serialPortName
      Layout.fillWidth: true
      Layout.leftMargin: 10
      visible: deviceAddress != ''
      font: QfTheme.defaultFont
      color: QfTheme.secondaryTextColor
      text: qsTr('Serial port display name:') + '\n ' + deviceName
      wrapMode: Text.WordWrap
    }

    Label {
      id: serialPortAddress
      Layout.fillWidth: true
      Layout.leftMargin: 10
      visible: deviceAddress != ''
      font: QfTheme.defaultFont
      color: QfTheme.secondaryTextColor
      text: qsTr('Serial port address:') + '\n ' + deviceAddress
      wrapMode: Text.WordWrap
    }
  }
}
```


