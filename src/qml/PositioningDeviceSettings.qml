import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qgis
import org.qfield
import Theme

/**
 * \ingroup qml
 */
Popup {
  id: popup
  parent: mainWindow.contentItem

  signal apply

  width: mainWindow.width - Theme.popupScreenEdgeMargin * 2
  height: mainWindow.height - Math.max(Theme.popupScreenEdgeMargin * 2, mainWindow.sceneTopMargin * 2 + 4, mainWindow.sceneBottomMargin * 2 + 4)
  x: Theme.popupScreenEdgeMargin
  y: (mainWindow.height - height) / 2
  padding: 0
  modal: true
  focus: visible

  property alias name: positioningDeviceName.text
  property alias type: positioningDeviceType.currentValue

  function generateName() {
    return positioningDeviceItem.item.generateName();
  }

  function setType(type) {
    for (var i = 0; i < positioningDeviceType.model.count; i++) {
      if (positioningDeviceType.model.get(i)["value"] === type) {
        positioningDeviceType.currentIndex = i;
        break;
      }
    }
  }

  function setSettings(settings) {
    positioningDeviceItem.item.setSettings(settings);
  }

  function getSettings() {
    return positioningDeviceItem.item.getSettings();
  }

  function handleEgenioussChange() {
    if (positioningSettings.egenioussEnabled) {
      positioningDeviceTypeModel.insert(0, {
          "name": qsTr('Egeniouss'),
          "value": PositioningDeviceModel.EgenioussDevice
        });
    } else {
      positioningDeviceTypeModel.remove(0, 1);
      if (positioningDeviceModel.findIndexFromDeviceId("egeniouss:") !== -1) {
        positioningDeviceModel.removeDevice("Egeniouss");
        positioningDeviceComboBox.currentIndex = 0;
      }
    }
    positioningDeviceType.model = positioningDeviceTypeModel;
  }

  Component.onCompleted: {
    if (withBluetooth) {
      positioningDeviceTypeModel.insert(0, {
          "name": qsTr('Bluetooth (NMEA)'),
          "value": PositioningDeviceModel.BluetoothDevice
        });
    }
    if (withSerialPort) {
      positioningDeviceTypeModel.insert(positioningDeviceTypeModel.count, {
          "name": qsTr('Serial port (NMEA)'),
          "value": PositioningDeviceModel.SerialPortDevice
        });
    }
    if (positioningSettings.egenioussEnabled) {
      positioningDeviceTypeModel.insert(0, {
          "name": qsTr('Egeniouss'),
          "value": PositioningDeviceModel.EgenioussDevice
        });
    }
    positioningDeviceType.model = positioningDeviceTypeModel;
    positioningSettings.onEgenioussEnabledChanged.connect(handleEgenioussChange);
  }

  Page {
    id: page
    width: parent.width
    height: parent.height
    padding: 10
    header: QfPageHeader {
      id: pageHeader
      title: qsTr("Positioning Device Settings")

      showBackButton: false
      showApplyButton: true
      showCancelButton: true

      onCancel: {
        popup.close();
      }

      onApply: {
        popup.apply();
        popup.close();
      }
    }

    ColumnLayout {
      spacing: 10
      width: parent.width

      Label {
        Layout.fillWidth: true
        text: qsTr("Connection type")
        font: Theme.defaultFont
        wrapMode: Text.WordWrap
      }

      ComboBox {
        id: positioningDeviceType
        Layout.fillWidth: true
        font: Theme.defaultFont

        popup.font: Theme.defaultFont
        popup.topMargin: mainWindow.sceneTopMargin
        popup.bottomMargin: mainWindow.sceneTopMargin

        textRole: "name"
        valueRole: "value"

        delegate: ItemDelegate {
          width: positioningDeviceType.width
          height: 36
          icon.source: {
            switch (value) {
            case PositioningDeviceModel.BluetoothDevice:
              return Theme.getThemeVectorIcon('ic_bluetooth_receiver_black_24dp');
            case PositioningDeviceModel.TcpDevice:
              return Theme.getThemeVectorIcon('ic_tcp_receiver_black_24dp');
            case PositioningDeviceModel.UdpDevice:
              return Theme.getThemeVectorIcon('ic_udp_receiver_black_24dp');
            case PositioningDeviceModel.SerialPortDevice:
              return Theme.getThemeVectorIcon('ic_serial_port_receiver_black_24dp');
            case PositioningDeviceModel.EgenioussDevice:
              return Theme.getThemeVectorIcon('ic_egeniouss_receiver_black_24dp');
            }
            return '';
          }
          icon.width: 24
          icon.height: 24
          text: name
          font: Theme.defaultFont
          highlighted: positioningDeviceType.highlightedIndex === index
        }

        contentItem: MenuItem {
          width: positioningDeviceComboBox.width
          height: 36

          icon.source: {
            switch (positioningDeviceType.currentValue) {
            case PositioningDeviceModel.BluetoothDevice:
              return Theme.getThemeVectorIcon('ic_bluetooth_receiver_black_24dp');
            case PositioningDeviceModel.TcpDevice:
              return Theme.getThemeVectorIcon('ic_tcp_receiver_black_24dp');
            case PositioningDeviceModel.UdpDevice:
              return Theme.getThemeVectorIcon('ic_udp_receiver_black_24dp');
            case PositioningDeviceModel.SerialPortDevice:
              return Theme.getThemeVectorIcon('ic_serial_port_receiver_black_24dp');
            case PositioningDeviceModel.EgenioussDevice:
              return Theme.getThemeVectorIcon('ic_egeniouss_receiver_black_24dp');
            }
            return '';
          }
          icon.width: 24
          icon.height: 24

          text: positioningDeviceType.currentText
          font: Theme.defaultFont

          onClicked: positioningDeviceType.popup.open()
        }
      }

      TextField {
        id: positioningDeviceName
        Layout.fillWidth: true
        font: Theme.defaultFont
        placeholderText: qsTr("Name") + (displayText === '' ? qsTr(' (leave empty to auto-fill)') : '')
      }

      Label {
        Layout.fillWidth: true
        text: qsTr("Connection details")
        font: Theme.defaultFont
        wrapMode: Text.WordWrap
      }

      ListModel {
        id: positioningDeviceTypeModel
        ListElement {
          name: qsTr('TCP (NMEA)')
          value: PositioningDeviceModel.TcpDevice
        }
        ListElement {
          name: qsTr('UDP (NMEA)')
          value: PositioningDeviceModel.UdpDevice
        }
      }

      Loader {
        id: positioningDeviceItem
        Layout.fillWidth: true
        Layout.fillHeight: true
        source: {
          switch (positioningDeviceType.currentValue) {
          case PositioningDeviceModel.BluetoothDevice:
            return "qrc:/qml/BluetoothDeviceChooser.qml";
          case PositioningDeviceModel.TcpDevice:
            return "qrc:/qml/TcpDeviceChooser.qml";
          case PositioningDeviceModel.UdpDevice:
            return "qrc:/qml/UdpDeviceChooser.qml";
          case PositioningDeviceModel.SerialPortDevice:
            return "qrc:/qml/SerialPortDeviceChooser.qml";
          case PositioningDeviceModel.EgenioussDevice:
            return "qrc:/qml/EgenioussDeviceChooser.qml";
          }
          return '';
        }
      }
    }
  }
}
