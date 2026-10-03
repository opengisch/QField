

# File QfFileDeviceChooser.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfFileDeviceChooser.qml**](QfFileDeviceChooser_8qml.md)

[Go to the documentation of this file](QfFileDeviceChooser_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import org.qfield.core
import org.qfield.gui

Item {
  width: parent.width
  height: childrenRect.height

  property alias deviceFilePath: filePath.text
  property alias deviceInterval: interval.text

  function generateName() {
    return QfFileUtils.fileName(deviceFilePath) + ' (' + deviceInterval + ' ms)';
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

    RowLayout {
      Layout.fillWidth: true

      TextField {
        id: filePath
        Layout.fillWidth: true
        font: QfTheme.defaultFont
        placeholderText: qsTr("File path")
        text: ''
        inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase | Qt.ImhPreferLowercase
      }
      QfToolButton {
        width: QfTheme.toolButtonSize
        height: QfTheme.toolButtonSize

        iconSource: QfTheme.getThemeVectorIcon("ic_folder_open_black_24dp")
        iconColor: QfTheme.mainTextColor
        bgcolor: "transparent"

        onClicked: {
          fileDialog.open();
        }
      }
    }

    TextField {
      id: interval
      Layout.fillWidth: true
      font: QfTheme.defaultFont
      placeholderText: qsTr("Interval (in milliseconds)")
      text: '100'
      inputMethodHints: Qt.ImhFormattedNumbersOnly
    }
  }

  FileDialog {
    id: fileDialog
    onAccepted: filePath.text = QfUrlUtils.toLocalFile(selectedFile)
  }
}
```


