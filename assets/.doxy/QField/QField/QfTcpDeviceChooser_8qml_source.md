

# File QfTcpDeviceChooser.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfTcpDeviceChooser.qml**](QfTcpDeviceChooser_8qml.md)

[Go to the documentation of this file](QfTcpDeviceChooser_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield.core
import org.qfield.gui

Item {
  width: parent.width
  height: childrenRect.height

  property alias deviceAddress: tcpDeviceAddress.text
  property alias devicePort: tcpDevicePort.text

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
      id: tcpDeviceAddress
      Layout.fillWidth: true
      font: QfTheme.defaultFont
      placeholderText: qsTr("IP address")
      text: '127.0.0.1'
      inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase | Qt.ImhPreferLowercase
    }

    TextField {
      id: tcpDevicePort
      Layout.fillWidth: true
      font: QfTheme.defaultFont
      placeholderText: qsTr("Port")
      text: '9000'
      inputMethodHints: Qt.ImhFormattedNumbersOnly
    }
  }
}
```


