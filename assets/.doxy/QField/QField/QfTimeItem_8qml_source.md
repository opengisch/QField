

# File QfTimeItem.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfTimeItem.qml**](QfTimeItem_8qml.md)

[Go to the documentation of this file](QfTimeItem_8qml.md)


```C++
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import org.qfield.core
import org.qfield.gui

Item {
  property alias hours: hoursSpinBox.value
  property alias minutes: minutesSpinBox.value
  property alias seconds: secondsSpinBox.value

  property bool __verticalView: false
  property real contentHeight: timeItemContent.height

  GridLayout {
    id: timeItemContent
    width: parent.width
    columns: __verticalView ? 1 : 3

    Label {
      text: qsTr("Hours")
      font: QfTheme.strongTipFont
      Layout.fillWidth: true
      horizontalAlignment: Text.AlignHCenter
    }

    Label {
      text: qsTr("Minutes")
      font: QfTheme.strongTipFont
      Layout.fillWidth: true
      horizontalAlignment: Text.AlignHCenter
      Layout.column: __verticalView ? 0 : 1
      Layout.row: __verticalView ? 2 : 0
    }

    Label {
      text: qsTr("Seconds")
      font: QfTheme.strongTipFont
      Layout.fillWidth: true
      horizontalAlignment: Text.AlignHCenter
      Layout.column: __verticalView ? 0 : 2
      Layout.row: __verticalView ? 4 : 0
    }

    SpinBox {
      id: hoursSpinBox
      editable: true
      from: 0
      to: 23
      value: 12
      inputMethodHints: Qt.ImhTime
      font: QfTheme.tipFont
      Layout.fillWidth: true
      Layout.column: 0
      Layout.row: 1
    }

    SpinBox {
      id: minutesSpinBox
      editable: true
      from: 0
      to: 59
      value: 30
      inputMethodHints: Qt.ImhTime
      font: QfTheme.tipFont
      Layout.fillWidth: true
      Layout.column: __verticalView ? 0 : 1
      Layout.row: __verticalView ? 3 : 1
    }

    SpinBox {
      id: secondsSpinBox
      editable: true
      from: 0
      to: 59
      value: 30
      inputMethodHints: Qt.ImhTime
      font: QfTheme.tipFont
      Layout.fillWidth: true
      Layout.column: __verticalView ? 0 : 2
      Layout.row: __verticalView ? 5 : 1
    }
  }
}
```


