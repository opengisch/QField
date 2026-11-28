

# File QfUdpDeviceChooser.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfUdpDeviceChooser.qml**](QfUdpDeviceChooser_8qml.md)

[Go to the documentation of this file](QfUdpDeviceChooser_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield.core
import org.qfield.gui

Item {
  width: parent.width
  height: childrenRect.height

  property alias deviceAddress: udpDeviceAddress.text
  property alias devicePort: udpDevicePort.text

  function generateName() {
    return deviceAddress + ' (' + devicePort + ')';
  }

  function setSettings(settings) {
    deviceAddress = settings['address'];
    devicePort = settings['port'];
  }

  function getSettings() {
    return {
      "address": deviceAddress.trim(),
      "port": parseInt(devicePort)
    };
  }

  GridLayout {
    width: parent.width
    columns: 1
    columnSpacing: 10
    rowSpacing: 10

    TextField {
      id: udpDeviceAddress
      Layout.fillWidth: true
      font: QfTheme.defaultFont
      placeholderText: qsTr("Address")
      text: '127.0.0.1'
      inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase | Qt.ImhPreferLowercase
    }

    TextField {
      id: udpDevicePort
      Layout.fillWidth: true
      font: QfTheme.defaultFont
      placeholderText: qsTr("Port")
      text: '11111'
      inputMethodHints: Qt.ImhFormattedNumbersOnly
    }
  }
}
```


