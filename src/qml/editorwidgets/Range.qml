import QtQuick 2.11
import QtQuick.Controls 2.4
import Theme 1.0

Item {
  signal valueChanged(var value, bool isNull)
  height: childrenRect.height

  id: rangeItem
  property string widgetStyle: config["Style"] ? config["Style"] : "TextField"
  property int precision: config["Precision"]
  property real from: config["Min"]
  property real to: config["Max"]
  property real step: config["Step"] ? config["Step"] : 1
  property string suffix: config["Suffix"] ? config["Suffix"] : ""

  TextField {
    id: textField
    height: fontMetrics.height + 20 * dp
    topPadding: 10 * dp
    bottomPadding: 10 * dp
    visible: widgetStyle != "Slider"
    anchors.left: parent.left
    anchors.right: parent.right
    font: Theme.defaultFont
    color: value === undefined || !enabled ? 'gray' : 'black'

    text: value !== undefined ? value : ''

    validator: {
      if (platformUtilities.fieldType( field ) === 'double')
      {
        doubleValidator;
      }
      else
      {
        intValidator;
      }
    }

    inputMethodHints: Qt.ImhFormattedNumbersOnly

    background: Rectangle {
      y: textField.height - height - textField.bottomPadding / 2
      implicitWidth: 120 * dp
      height: textField.activeFocus ? 2 * dp : 1 * dp
      color: textField.activeFocus ? "#4CAF50" : "#C8E6C9"
    }

    onTextChanged: {
      valueChanged( text, text == '' )
    }
  }

  Row {
    id: sliderRow
    anchors.fill: parent
    visible: widgetStyle === "Slider"

    Text {
      id: valueLabel
      width: sliderRow.width / 4
      height: fontMetrics.height + 20 * dp
      elide: Text.ElideRight
      text: value !== undefined && value != '' ? Number( slider.value ).toFixed( rangeItem.precision ).toLocaleString() + rangeItem.suffix : ''
      verticalAlignment: Text.AlignVCenter
      horizontalAlignment: Text.AlignLeft
      font: Theme.defaultFont
      color: value === undefined || !enabled ? 'gray' : 'black'
    }

    Slider {
      id: slider
      value: rangeItem.parent.value
      width: sliderRow.width - valueLabel.width
      height: fontMetrics.height + 20 * dp
      implicitWidth: width
      from: rangeItem.from
      to: rangeItem.to
      stepSize: rangeItem.step

      onValueChanged: {
        if (sliderRow.visible) {
          rangeItem.valueChanged(slider.value, false)
        }
      }

      background: Rectangle {
        x: slider.leftPadding
        y: slider.topPadding + slider.availableHeight / 2 - height / 2
        implicitWidth: slider.width
        implicitHeight: slider.height * 0.1
        width: slider.availableWidth
        height: implicitHeight
        radius: 4 * dp
        color:  Theme.lightGray
      }
    }
  }

  Rectangle {
    y: textField.height - height - textField.bottomPadding / 2
    visible: widgetStyle === "Slider"
    width: sliderRow.width
    implicitWidth: 120 * dp
    height: slider.activeFocus ? 2 * dp : 1 * dp
    color: slider.activeFocus ? "#4CAF50" : "#C8E6C9"
  }

  FontMetrics {
    id: fontMetrics
    font: textField.font
  }

  IntValidator {
    id: intValidator

    bottom: from
    top: to
  }

  DoubleValidator {
    id: doubleValidator

    bottom: from
    top: to
  }
}
