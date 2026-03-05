

# File QfSlider.qml

[**File List**](files.md) **>** [**imports**](dir_3be62dd4600925273911e91e0c7964f3.md) **>** [**Theme**](dir_1633596792308d5fdfbf00fb99c556ce.md) **>** [**QfSlider.qml**](QfSlider_8qml.md)

[Go to the documentation of this file](QfSlider_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield

Item {
  id: wrapper

  property alias from: slider.from
  property alias to: slider.to
  property alias stepSize: slider.stepSize
  property alias value: slider.value
  property string prefixText: ""
  property string suffixText: ""
  property bool showValueLabel: true

  signal moved

  implicitHeight: childrenRect.height

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

      font: Theme.tipFont
      color: Theme.secondaryTextColor
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


