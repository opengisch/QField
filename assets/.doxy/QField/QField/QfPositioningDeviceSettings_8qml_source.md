

# File QfPositioningDeviceSettings.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfPositioningDeviceSettings.qml**](QfPositioningDeviceSettings_8qml.md)

[Go to the documentation of this file](QfPositioningDeviceSettings_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qgis
import org.qfield.core
import org.qfield.gui

QfPopup {
  id: popup
  parent: mainWindow.contentItem

  signal apply

  width: mainWindow.width - QfTheme.popupScreenEdgeHorizontalMargin * 2
  height: mainWindow.height - Math.max(QfTheme.popupScreenEdgeVerticalMargin * 2, mainWindow.sceneTopMargin * 2 + 4, mainWindow.sceneBottomMargin * 2 + 4)
  x: (mainWindow.width - width) / 2
  y: (mainWindow.height - height) / 2
  focus: visible
  closePolicy: Popup.CloseOnEscape

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
        "value": QfPositioningDeviceModel.EgenioussDevice
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
    if (Qfield.hasBluetooth) {
      positioningDeviceTypeModel.insert(0, {
        "name": qsTr('Bluetooth BT + BLE (NMEA)'),
        "value": QfPositioningDeviceModel.BluetoothDevice
      });
    }
    if (Qfield.hasSerialPort) {
      positioningDeviceTypeModel.insert(positioningDeviceTypeModel.count, {
        "name": qsTr('Serial port (NMEA)'),
        "value": QfPositioningDeviceModel.SerialPortDevice
      });
    }
    if (Qt.platform.os !== "android" && Qt.platform.os !== "ios") {
      positioningDeviceTypeModel.insert(positioningDeviceTypeModel.count, {
        "name": qsTr('Logs file (NMEA)'),
        "value": QfPositioningDeviceModel.FileDevice
      });
    }
    if (positioningSettings.egenioussEnabled) {
      positioningDeviceTypeModel.insert(0, {
        "name": qsTr('Egeniouss'),
        "value": QfPositioningDeviceModel.EgenioussDevice
      });
    }
    positioningDeviceType.model = positioningDeviceTypeModel;
    positioningSettings.onEgenioussEnabledChanged.connect(handleEgenioussChange);
  }

  onAboutToHide: {
    if (positioningDeviceItem.item && positioningDeviceItem.item.close !== undefined) {
      positioningDeviceItem.item.close();
    }
  }

  Page {
    id: page
    width: parent.width
    height: parent.height
    padding: 5
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
        // To be on the safe side, call close now to insure no process that could interfere
        // with device connectivity is running priort o applying its configuration
        if (positioningDeviceItem.item && positioningDeviceItem.item.close !== undefined) {
          positioningDeviceItem.item.close();
        }
        popup.apply();
        popup.close();
      }
    }

    ScrollView {
      topPadding: 0
      leftPadding: 5
      rightPadding: 5
      bottomPadding: 0
      ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
      ScrollBar.vertical: QfScrollBar {}
      contentWidth: settingsLayout.childrenRect.width
      contentHeight: settingsLayout.childrenRect.height + positioningDeviceItem.item.childrenRect.height
      width: parent.width
      height: parent.height
      clip: true

      ColumnLayout {
        id: settingsLayout
        spacing: 10
        width: page.width - 20

        Label {
          text: qsTr("Connection type")
          font: QfTheme.strongFont
          color: QfTheme.mainTextColor
          wrapMode: Text.WordWrap
          Layout.fillWidth: true
          Layout.topMargin: 5
          Layout.columnSpan: 2
        }

        QfComboBox {
          id: positioningDeviceType
          Layout.fillWidth: true
          font: QfTheme.defaultFont

          popup.font: QfTheme.defaultFont
          popup.topMargin: mainWindow.sceneTopMargin
          popup.bottomMargin: mainWindow.sceneTopMargin

          textRole: "name"
          valueRole: "value"

          delegate: ItemDelegate {
            width: positioningDeviceType.width
            height: 36
            icon.source: {
              switch (value) {
              case QfPositioningDeviceModel.FileDevice:
                return QfTheme.getThemeVectorIcon("ic_file_black_24dp");
              case QfPositioningDeviceModel.BluetoothDevice:
                return QfTheme.getThemeVectorIcon('ic_bluetooth_receiver_black_24dp');
              case QfPositioningDeviceModel.TcpDevice:
                return QfTheme.getThemeVectorIcon('ic_tcp_receiver_black_24dp');
              case QfPositioningDeviceModel.UdpDevice:
                return QfTheme.getThemeVectorIcon('ic_udp_receiver_black_24dp');
              case QfPositioningDeviceModel.SerialPortDevice:
                return QfTheme.getThemeVectorIcon('ic_serial_port_receiver_black_24dp');
              case QfPositioningDeviceModel.EgenioussDevice:
                return QfTheme.getThemeVectorIcon('ic_egeniouss_receiver_black_24dp');
              }
              return '';
            }
            icon.width: 24
            icon.height: 24
            text: name
            font: QfTheme.defaultFont
            highlighted: positioningDeviceType.highlightedIndex === index
          }

          contentItem: MenuItem {
            width: positioningDeviceComboBox.width
            height: 36

            icon.source: {
              switch (positioningDeviceType.currentValue) {
              case QfPositioningDeviceModel.FileDevice:
                return QfTheme.getThemeVectorIcon("ic_file_black_24dp");
              case QfPositioningDeviceModel.BluetoothDevice:
                return QfTheme.getThemeVectorIcon('ic_bluetooth_receiver_black_24dp');
              case QfPositioningDeviceModel.TcpDevice:
                return QfTheme.getThemeVectorIcon('ic_tcp_receiver_black_24dp');
              case QfPositioningDeviceModel.UdpDevice:
                return QfTheme.getThemeVectorIcon('ic_udp_receiver_black_24dp');
              case QfPositioningDeviceModel.SerialPortDevice:
                return QfTheme.getThemeVectorIcon('ic_serial_port_receiver_black_24dp');
              case QfPositioningDeviceModel.EgenioussDevice:
                return QfTheme.getThemeVectorIcon('ic_egeniouss_receiver_black_24dp');
              }
              return '';
            }
            icon.width: 24
            icon.height: 24

            text: positioningDeviceType.currentText
            font: QfTheme.defaultFont

            onClicked: positioningDeviceType.popup.open()
          }
        }

        TextField {
          id: positioningDeviceName
          Layout.fillWidth: true
          font: QfTheme.defaultFont
          placeholderText: qsTr("Name") + (displayText === '' ? qsTr(' (leave empty to auto-fill)') : '')
        }

        Label {
          text: qsTr("Connection details")
          font: QfTheme.strongFont
          color: QfTheme.mainTextColor
          wrapMode: Text.WordWrap
          Layout.fillWidth: true
          Layout.topMargin: 5
          Layout.columnSpan: 2
        }

        ListModel {
          id: positioningDeviceTypeModel
          ListElement {
            name: qsTr('TCP (NMEA)')
            value: QfPositioningDeviceModel.TcpDevice
          }
          ListElement {
            name: qsTr('UDP (NMEA)')
            value: QfPositioningDeviceModel.UdpDevice
          }
        }

        Loader {
          id: positioningDeviceItem
          objectName: "positioningDeviceDetailsLoader"
          Layout.fillWidth: true
          Layout.fillHeight: true
          source: {
            switch (positioningDeviceType.currentValue) {
            case QfPositioningDeviceModel.FileDevice:
              return "QfFileDeviceChooser.qml";
            case QfPositioningDeviceModel.BluetoothDevice:
              return "QfBluetoothDeviceChooser.qml";
            case QfPositioningDeviceModel.TcpDevice:
              return "QfTcpDeviceChooser.qml";
            case QfPositioningDeviceModel.UdpDevice:
              return "QfUdpDeviceChooser.qml";
            case QfPositioningDeviceModel.SerialPortDevice:
              return "QfSerialPortDeviceChooser.qml";
            case QfPositioningDeviceModel.EgenioussDevice:
              return "QfEgenioussDeviceChooser.qml";
            }
            return '';
          }
        }
      }
    }
  }
}
```


