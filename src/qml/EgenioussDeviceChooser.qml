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

  property string deviceAddress
  property string devicePort

  function generateName() {
    return "Egeniouss";
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
}
