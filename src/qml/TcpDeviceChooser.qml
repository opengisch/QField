import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield
import Theme

Item {
  width: parent.width

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
    columnSpacing: 0
    rowSpacing: 5

    Label {
      Layout.fillWidth: true
      text: qsTr("Address:")
      font: Theme.defaultFont
      wrapMode: Text.WordWrap
    }

    QfTextField {
      id: tcpDeviceAddress
      Layout.fillWidth: true
      font: Theme.defaultFont
      text: '127.0.0.1'
      inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase | Qt.ImhPreferLowercase
    }

    Label {
      Layout.fillWidth: true
      text: qsTr("Port:")
      font: Theme.defaultFont
      wrapMode: Text.WordWrap
    }

    QfTextField {
      id: tcpDevicePort
      Layout.fillWidth: true
      font: Theme.defaultFont
      text: '9000'
      inputMethodHints: Qt.ImhFormattedNumbersOnly
    }
  }
}
