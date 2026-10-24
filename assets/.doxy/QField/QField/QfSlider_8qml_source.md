

# File QfSlider.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfSlider.qml**](QfSlider_8qml.md)

[Go to the documentation of this file](QfSlider_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield.core
import org.qfield.gui

Item {
  id: wrapper

  property alias from: slider.from
  property alias to: slider.to
  property alias stepSize: slider.stepSize
  property alias snapMode: slider.snapMode
  property alias value: slider.value
  property string prefixText: ""
  property string suffixText: ""
  property bool showValueLabel: true

  signal moved

  implicitHeight: layout.implicitHeight

  RowLayout {
    id: layout
    anchors.fill: parent

    Slider {
      id: slider

      Layout.fillWidth: true
      opacity: wrapper.enabled ? 1 : 0.5

      value: 50
      from: 0
      to: 100
      stepSize: 1
      onMoved: wrapper.moved()
    }

    Label {
      id: suffix

      visible: showValueLabel
      Layout.maximumWidth: layout.width / 4
      // prevent the slider width to change as the number increases, if the number is up to three digits
      Layout.minimumWidth: suffixMetrics.width

      text: prefixText + value + suffixText

      font: QfTheme.tipFont
      color: QfTheme.secondaryTextColor
    }

    TextMetrics {
      id: suffixMetrics
      font: suffix.font
      text: prefixText + getSampleOfNumberOfLength(Math.min(slider.to, 999)) + suffixText

      function getSampleOfNumberOfLength(number) {
        return new Array(number.toString().length + 1).join(9);
      }
    }
  }
}
```


