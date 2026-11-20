import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield.core
import org.qfield.gui

/**
 * \ingroup qml_gui
 */
Item {
  width: parent.width
  height: childrenRect.height

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
    externalAccessoryModel.refresh();
  }

  ColumnLayout {
    width: parent.width
    spacing: 5

    Label {
      Layout.fillWidth: true
      text: externalAccessoryComboBox.count > 0 ? qsTr("Select the accessory from the list below:") : qsTr("No accessories detected, refresh the list once a receiver is connected.")
      font: QfTheme.defaultFont

      wrapMode: Text.WordWrap
    }

    QfComboBox {
      id: externalAccessoryComboBox
      Layout.fillWidth: true
      visible: externalAccessoryComboBox.count
      font: QfTheme.defaultFont

      popup.font: QfTheme.defaultFont
      popup.topMargin: mainWindow.sceneTopMargin
      popup.bottomMargin: mainWindow.sceneTopMargin

      textRole: 'display'
      model: QfExternalAccessoryModel {
        id: externalAccessoryModel
      }

      onCurrentIndexChanged: {
        if (currentIndex < 0) {
          deviceName = '';
          deviceAddress = '';
          return;
        }
        const modelIndex = externalAccessoryModel.index(currentIndex, 0);
        deviceName = externalAccessoryModel.data(modelIndex, QfExternalAccessoryModel.NameRole);
        deviceAddress = externalAccessoryModel.data(modelIndex, QfExternalAccessoryModel.SerialNumberRole);
      }
    }

    QfButton {
      id: refreshButton
      Layout.fillWidth: true
      text: qsTr('Refresh list')

      onClicked: {
        externalAccessoryModel.refresh();
      }
    }

    Label {
      id: externalAccessoryName
      Layout.fillWidth: true
      Layout.leftMargin: 10
      visible: deviceAddress != ''
      font: QfTheme.defaultFont
      color: QfTheme.secondaryTextColor
      text: qsTr('Accessory name:') + '\n ' + deviceName
      wrapMode: Text.WordWrap
    }

    Label {
      id: externalAccessorySerialNumber
      Layout.fillWidth: true
      Layout.leftMargin: 10
      visible: deviceAddress != ''
      font: QfTheme.defaultFont
      color: QfTheme.secondaryTextColor
      text: qsTr('Accessory serial number:') + '\n ' + deviceAddress
      wrapMode: Text.WordWrap
    }
  }
}
