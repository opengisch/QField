

# File TcpDeviceChooser.qml

[**File List**](files.md) **>** [**qml**](dir_1d81f7f4362e4398c0a695c4e466fcb0.md) **>** [**TcpDeviceChooser.qml**](TcpDeviceChooser_8qml.md)

[Go to the documentation of this file](TcpDeviceChooser_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield
import Theme

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
      font: Theme.defaultFont
      placeholderText: qsTr("IP address")
      text: '127.0.0.1'
      inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase | Qt.ImhPreferLowercase
    }

    TextField {
      id: tcpDevicePort
      Layout.fillWidth: true
      font: Theme.defaultFont
      placeholderText: qsTr("Port")
      text: '9000'
      inputMethodHints: Qt.ImhFormattedNumbersOnly
    }
  }
}
```


