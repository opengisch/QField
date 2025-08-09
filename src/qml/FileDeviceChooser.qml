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

  property alias deviceFilePath: filePath.text
  property alias deviceInterval: interval.text

  function generateName() {
    return deviceFilePath + ' (' + deviceInterval + ' ms)';
  }

  function setSettings(settings) {
    deviceFilePath = settings['filePath'];
    deviceInterval = settings['interval'];
  }

  function getSettings() {
    return {
      "filePath": deviceFilePath.trim(),
      "interval": parseInt(deviceInterval)
    };
  }

  GridLayout {
    width: parent.width
    columns: 1
    columnSpacing: 10
    rowSpacing: 10

    TextField {
      id: filePath
      Layout.fillWidth: true
      font: Theme.defaultFont
      placeholderText: qsTr("File path")
      text: ''
      inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase | Qt.ImhPreferLowercase
    }

    TextField {
      id: interval
      Layout.fillWidth: true
      font: Theme.defaultFont
      placeholderText: qsTr("Interval (in milliseconds)")
      text: '100'
      inputMethodHints: Qt.ImhFormattedNumbersOnly
    }
  }
}
