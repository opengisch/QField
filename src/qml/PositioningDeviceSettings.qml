import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import org.qgis 1.0
import org.qfield 1.0

import Theme 1.0

Popup {
    id: popup
    parent: ApplicationWindow.overlay

    signal apply

    width: mainWindow.width - Theme.popupScreenEdgeMargin * 2
    height: mainWindow.height - Theme.popupScreenEdgeMargin * 2
    x: Theme.popupScreenEdgeMargin
    y: Theme.popupScreenEdgeMargin
    padding: 0

    property alias name: positioningDeviceName.text
    property alias type: positioningDeviceType.currentValue

    function generateName() {
      return positioningDeviceItem.item.generateName();
    }

    function setType(type) {
      for(var i = 0; i < positioningDeviceType.model.count; i++) {
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

    Page {
        id: page
        width: parent.width
        height: parent.height
        padding: 10
        header: PageHeader {
          id: pageHeader
          title: qsTr("Positioning Device Settings")

          showBackButton: false
          showApplyButton: true
          showCancelButton: true

          onCancel: {
            popup.close()
          }

          onApply: {
            popup.apply()
            popup.close()
          }
        }

        ColumnLayout {
            spacing: 5
            width: parent.width

            Label {
                Layout.fillWidth: true
                text: qsTr("Name:")
                font: Theme.defaultFont
                wrapMode: Text.WordWrap
            }

            QfTextField {
                id: positioningDeviceName
                Layout.fillWidth: true
                font: Theme.defaultFont
                placeholderText: qsTr('Leave empty to auto-fill')
            }

            Label {
                Layout.fillWidth: true
                text: qsTr("Connection type:")
                font: Theme.defaultFont
                wrapMode: Text.WordWrap
            }

            ComboBox {
                id: positioningDeviceType
                Layout.fillWidth: true
                font: Theme.defaultFont
                popup.font: Theme.defaultFont

                textRole: "name"
                valueRole: "value"
                model: ListModel {
                  ListElement { name: qsTr('Bluetooth (NMEA)'); value: PositioningDeviceModel.BluetoothDevice }
                  ListElement { name: qsTr('TCP (NMEA)'); value: PositioningDeviceModel.TcpDevice }
                  ListElement { name: qsTr('UDP (NMEA)'); value: PositioningDeviceModel.UdpDevice }
                }
            }

            Loader {
                id: positioningDeviceItem
                Layout.fillWidth: true
                Layout.fillHeight: true
                source: {
                  switch(positioningDeviceType.currentValue) {
                    case PositioningDeviceModel.BluetoothDevice:
                      return "qrc:/qml/BluetoothDeviceChooser.qml";
                    case PositioningDeviceModel.TcpDevice:
                      return "qrc:/qml/TcpDeviceChooser.qml";
                    case PositioningDeviceModel.UdpDevice:
                      return "qrc:/qml/UdpDeviceChooser.qml";
                  }
                  return '';
                }
            }
        }
    }
}
