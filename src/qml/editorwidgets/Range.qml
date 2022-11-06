import QtQuick 2.14
import QtQuick.Controls 2.14

import Theme 1.0
import org.qfield 1.0

import "."

EditorWidgetBase {
  id: rangeItem
  enabled: isEnabled

  property string widgetStyle: config["Style"] ? config["Style"] : "TextField"
  property int precision: config["Precision"] ? config["Precision"] : 1
  property real min: config["Min"] !== undefined ? config["Min"] : -Infinity
  property real max: config["Max"] !== undefined ? config["Max"] : Infinity
  property real step: config["Step"] !== undefined ? config["Step"] : 1
  property string suffix: config["Suffix"] ? config["Suffix"] : ""

  height: childrenRect.height

  Row {
      anchors.left: parent.left
      anchors.right: parent.right
      anchors.top: parent.top
      visible: widgetStyle != "Slider"

      TextField {
          id: textField
          height: fontMetrics.height + 20
          topPadding: 10
          bottomPadding: 10
          width: parent.width - decreaseButton.width - increaseButton.width

          font: Theme.defaultFont
          color: value === undefined || !enabled ? 'gray' : 'black'

          text: value !== undefined ? value : ''

          validator: {
              if (LayerUtils.fieldType( field ) === 'double')
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
              color: textField.activeFocus ? Theme.accentColor : Theme.accentLightColor
          }

          onTextChanged: {
              if (text == '' || !isNaN(parseFloat(text))) {
                  valueChangeRequested( text, text == '' )
              }
          }
      }

      QfToolButton {
          id: decreaseButton
          width: enabled ? 48 : 0
          height: 48

          anchors.verticalCenter: textField.verticalCenter

          bgcolor: "transparent"
          visible: enabled

          onClicked: {
              decreaseValue();
          }
          onDoubleClicked: {
              decreaseValue();
          }
          onPressAndHold: {
              changeValueTimer.increase = false
              changeValueTimer.interval = 700
              changeValueTimer.restart()
          }
          onReleased: {
              changeValueTimer.stop()
          }
          onCanceled: {
              changeValueTimer.stop()
          }

          iconSource: Theme.getThemeIcon("ic_remove_black_48dp")
      }

      QfToolButton {
          id: increaseButton
          width: enabled ? 48 : 0
          height: 48

          anchors.verticalCenter: textField.verticalCenter

          bgcolor: "transparent"
          visible: enabled

          onClicked: {
              increaseValue();
          }
          onDoubleClicked: {
              increaseValue();
          }
          onPressAndHold: {
              changeValueTimer.increase = true
              changeValueTimer.interval = 700
              changeValueTimer.restart()
          }
          onReleased: {
              changeValueTimer.stop()
          }
          onCanceled: {
              changeValueTimer.stop()
          }

          iconSource: Theme.getThemeIcon("ic_add_black_48dp")
      }
  }

  Timer {
      id: changeValueTimer
      interval: 700
      repeat: true

      property bool increase: true

      onTriggered: {
          var hitBoundary = false;
          if ( increase ) {
              increaseValue();
              hitBoundary = textField.text == rangeItem.max
          } else {
              decreaseValue();
              hitBoundary = textField.text == rangeItem.min;
          }

          if ( !hitBoundary ) {
              if ( interval > 50 ) interval = interval * 0.7;
          } else {
              stop();
          }
      }
  }

  function decreaseValue() {
      var currentValue = Number.parseFloat(textField.text)
      var newValue
      if (!isNaN(currentValue)) {
          newValue = currentValue - rangeItem.step;
          valueChangeRequested(Math.max(rangeItem.min, newValue), false)
      } else {
          newValue = 0;
          valueChangeRequested(newValue, false)
      }
  }

  function increaseValue() {
    var currentValue = Number.parseFloat(textField.text)
    var newValue
    if (!isNaN(currentValue)) {
          newValue = currentValue + rangeItem.step;
          valueChangeRequested(Math.min(rangeItem.max, newValue ), false)
      } else {
          newValue = 0;
          valueChangeRequested(newValue, false)
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
      from: !Number.isFinite(rangeItem.min) ? Number.MIN_VALUE : rangeItem.min
      to: !Number.isFinite(rangeItem.max) ? Number.MAX_VALUE : rangeItem.max
      stepSize: !Number.isFinite(rangeItem.step) ? 1 : rangeItem.step

      onValueChanged: {
        if (sliderRow.visible) {
          rangeItem.valueChangeRequested(slider.value, false)
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
    color: slider.activeFocus ? Theme.accentColor : Theme.accentLightColor
  }

  FontMetrics {
    id: fontMetrics
    font: textField.font
  }

  IntValidator {
    id: intValidator

    bottom: rangeItem.min
    top: rangeItem.max
  }

  DoubleValidator {
    id: doubleValidator

    locale: 'C'

    bottom: rangeItem.min
    top: rangeItem.max
  }
}
