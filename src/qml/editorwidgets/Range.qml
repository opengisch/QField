import QtQuick 2.14
import QtQuick.Controls 2.12

import Theme 1.0

import "."

EditorWidgetBase {
  height: childrenRect.height
  enabled: isEnabled

  id: rangeItem
  property string widgetStyle: config["Style"] ? config["Style"] : "TextField"
  property int precision: config["Precision"] ? config["Precision"] : 1
  property real from: config["Min"] ? config["Min"] : -Infinity
  property real to: config["Max"] ? config["Max"] : Infinity
  property real step: config["Step"] ? config["Step"] : 1
  property string suffix: config["Suffix"] ? config["Suffix"] : ""

  TextField {
    id: textField
    height: fontMetrics.height + 20
    topPadding: 10
    bottomPadding: 10
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
      implicitWidth: 120
      height: textField.activeFocus ? 2: 1
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
      height: fontMetrics.height + 20
      elide: Text.ElideRight
      text: value !== undefined && value != '' ? Number( slider.value ).toFixed( rangeItem.precision ).toLocaleString() + rangeItem.suffix : ''
      verticalAlignment: Text.AlignVCenter
      horizontalAlignment: Text.AlignLeft
      font: Theme.defaultFont
      color: value === undefined || !enabled ? 'gray' : 'black'
    }

    Slider {
      id: slider
      value: typeof rangeItem.parent.value === 'numeric' || typeof rangeItem.parent.value === 'number' ? rangeItem.parent.value : slider.value
      width: sliderRow.width - valueLabel.width
      height: fontMetrics.height + 20
      implicitWidth: width
      from: rangeItem.from === -Infinity || rangeItem.from == null ? Number.MIN_VALUE : rangeItem.from
      to: rangeItem.to === +Infinity || rangeItem.to == null ? Number.MAX_VALUE : rangeItem.to
      stepSize: rangeItem.step === +Infinity || rangeItem.step == null ? 1 : rangeItem.step

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
        radius: 4
        color:  Theme.lightGray
      }

      handle: Rectangle {
        x: slider.leftPadding + slider.visualPosition * (slider.availableWidth - width)
        y: slider.topPadding + slider.availableHeight / 2 - height / 2
        implicitWidth: 26
        implicitHeight: 26
        radius: 13
        color: slider.enabled ? Theme.mainColor : Theme.lightGray
        border.color: Theme.lightGray
      }
    }
  }

  Rectangle {
    y: textField.height - height - textField.bottomPadding / 2
    visible: widgetStyle === "Slider"
    width: sliderRow.width
    implicitWidth: 120
    height: slider.activeFocus ? 2: 1
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

    locale: 'C'

    bottom: from
    top: to
  }
}
