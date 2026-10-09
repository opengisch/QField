import QtQuick
import QtQuick.Controls
import org.qfield.gui

/**
 * \ingroup qml_gui
 */
Item {
  width: parent.width
  height: childrenRect.height

  function generateName() {
    return qsTr("External accessory");
  }

  function setSettings(settings) {
  }

  function getSettings() {
    return {};
  }

  Label {
    width: parent.width
    text: qsTr("Pair the receiver in the iOS Bluetooth settings. QField connects to the first connected accessory with a supported GNSS protocol and reads its NMEA stream, including its accuracy values.")
    font: QfTheme.defaultFont
    wrapMode: Text.WordWrap
  }
}
