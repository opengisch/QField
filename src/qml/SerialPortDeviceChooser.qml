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
      font: Theme.defaultFont

      wrapMode: Text.WordWrap
    }

    QfComboBox {
      id: serialPortComboBox
      Layout.fillWidth: true
      visible: serialPortComboBox.count
      font: Theme.defaultFont

      popup.font: Theme.defaultFont
      popup.topMargin: mainWindow.sceneTopMargin
      popup.bottomMargin: mainWindow.sceneTopMargin

      textRole: 'display'
      model: SerialPortModel {
        id: serialPortModel
      }

      property string selectedSerialPort

      onCurrentIndexChanged: {
        var modelIndex = serialPortModel.index(currentIndex, 0);
        deviceName = serialPortModel.data(modelIndex, Qt.DisplayRole);
        deviceAddress = serialPortModel.data(modelIndex, SerialPortModel.PortNameRole);
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
      visible: deviceAddress != ''
      font: Theme.defaultFont
      color: Theme.secondaryTextColor
      text: qsTr('Serial port display name:') + '\n ' + deviceName
      wrapMode: Text.WordWrap
    }

    Label {
      id: serialPortAddress
      Layout.fillWidth: true
      visible: deviceAddress != ''
      font: Theme.defaultFont
      color: Theme.secondaryTextColor
      text: qsTr('Serial port address:') + '\n ' + deviceAddress
      wrapMode: Text.WordWrap
    }
  }
}
