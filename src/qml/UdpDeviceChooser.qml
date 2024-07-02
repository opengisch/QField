import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import org.qfield 1.0
import Theme 1.0

Item {
  width: parent.width

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
    columnSpacing: 0
    rowSpacing: 5

    Label {
      Layout.fillWidth: true
      text: qsTr("Address:")
      font: Theme.defaultFont
      wrapMode: Text.WordWrap
    }

    QfTextField {
      id: udpDeviceAddress
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
      id: udpDevicePort
      Layout.fillWidth: true
      font: Theme.defaultFont
      text: '11111'
      inputMethodHints: Qt.ImhFormattedNumbersOnly
    }
  }
}
