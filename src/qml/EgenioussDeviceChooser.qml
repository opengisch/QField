import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield
import Theme

Item {
  width: parent.width

  property alias deviceAddress: egenioussDeviceAddress.text
  property alias devicePort: egenioussDevicePort.text

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
      id: egenioussDeviceAddress
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
      id: egenioussDevicePort
      Layout.fillWidth: true
      font: Theme.defaultFont
      text: '1235'
      inputMethodHints: Qt.ImhFormattedNumbersOnly
    }
  }
}
