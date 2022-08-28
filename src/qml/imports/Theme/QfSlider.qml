import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

Item {
  property alias from: slider.from;
  property alias to: slider.to
  property alias stepSize: slider.stepSize
  property alias value: slider.value
  property string prefixText: ""
  property string suffixText: ""

  signal moved

  id: wrapper

  implicitHeight: childrenRect.height

  RowLayout {
    id: layout
    anchors.fill: parent

    Slider {
      Layout.fillWidth: true

      id: slider
      value: 50
      from: 0
      to: 100
      stepSize: 1
      onMoved: wrapper.moved()
    }

    Label {
      Layout.maximumWidth: layout.width / 4
      // prevent the slider width to change as the number increases, if the number is up to three digits
      Layout.minimumWidth: suffixMetrics.width

      id: suffix
      text: prefixText + value + suffixText

      font: Theme.tipFont
      color: Theme.gray
    }

    TextMetrics {
      id: suffixMetrics
      font: suffix.font
      text: prefixText + getSampleOfNumberOfLength(Math.min(slider.to, 999)) + suffixText


      function getSampleOfNumberOfLength(number) {
        return new Array(number.toString().length + 1).join(9)
      }
    }
  }
}
