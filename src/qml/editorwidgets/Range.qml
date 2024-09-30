import QtQuick
import QtQuick.Controls
import org.qfield
import Theme

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
      rightPadding: 0
      leftPadding: enabled ? 5 : 0
      width: parent.width - decreaseButton.width - increaseButton.width

      font: Theme.defaultFont
      color: value === undefined || !enabled ? Theme.mainTextDisabledColor : Theme.mainTextColor

      text: value !== undefined ? value : ''

      validator: {
        if (LayerUtils.fieldType(field) === 'double') {
          doubleValidator;
        } else {
          intValidator;
        }
      }

      inputMethodHints: Qt.ImhFormattedNumbersOnly

      background: Rectangle {
        implicitWidth: 120
        color: "transparent"

        Rectangle {
          y: textField.height - height - textField.bottomPadding / 2
          width: textField.width
          height: textField.activeFocus ? 2 : 1
          color: textField.activeFocus ? Theme.accentColor : Theme.accentLightColor
        }
      }

      onTextChanged: {
        if (text == '' || !isNaN(parseFloat(text))) {
          valueChangeRequested(text, text == '');
        }
      }
    }

    QfToolButton {
      id: decreaseButton
      width: enabled ? 48 : 0
      height: 48

      anchors.verticalCenter: textField.verticalCenter

      iconSource: Theme.getThemeVectorIcon("ic_remove_white_24dp")
      iconColor: Theme.mainTextColor
      bgcolor: "transparent"
      visible: enabled

      onClicked: {
        decreaseValue();
      }
      onDoubleClicked: {
        decreaseValue();
      }
      onPressAndHold: {
        changeValueTimer.increase = false;
        changeValueTimer.interval = 700;
        changeValueTimer.restart();
      }
      onReleased: {
        changeValueTimer.stop();
      }
      onCanceled: {
        changeValueTimer.stop();
      }
    }

    QfToolButton {
      id: increaseButton
      width: enabled ? 48 : 0
      height: 48

      anchors.verticalCenter: textField.verticalCenter

      iconSource: Theme.getThemeVectorIcon("ic_add_white_24dp")
      iconColor: Theme.mainTextColor
      bgcolor: "transparent"
      visible: enabled

      onClicked: {
        increaseValue();
      }
      onDoubleClicked: {
        increaseValue();
      }
      onPressAndHold: {
        changeValueTimer.increase = true;
        changeValueTimer.interval = 700;
        changeValueTimer.restart();
      }
      onReleased: {
        changeValueTimer.stop();
      }
      onCanceled: {
        changeValueTimer.stop();
      }
    }
  }

  Timer {
    id: changeValueTimer
    interval: 700
    repeat: true

    property bool increase: true

    onTriggered: {
      var hitBoundary = false;
      if (increase) {
        increaseValue();
        hitBoundary = textField.text == rangeItem.max;
      } else {
        decreaseValue();
        hitBoundary = textField.text == rangeItem.min;
      }
      if (!hitBoundary) {
        if (interval > 50)
          interval = interval * 0.7;
      } else {
        stop();
      }
    }
  }

  function decreaseValue() {
    var currentValue = Number.parseFloat(textField.text);
    var newValue;
    if (!isNaN(currentValue)) {
      newValue = currentValue - rangeItem.step;
      valueChangeRequested(Math.max(rangeItem.min, newValue), false);
    } else {
      newValue = 0;
      valueChangeRequested(newValue, false);
    }
  }

  function increaseValue() {
    var currentValue = Number.parseFloat(textField.text);
    var newValue;
    if (!isNaN(currentValue)) {
      newValue = currentValue + rangeItem.step;
      valueChangeRequested(Math.min(rangeItem.max, newValue), false);
    } else {
      newValue = 0;
      valueChangeRequested(newValue, false);
    }
  }

  Row {
    id: sliderRow
    anchors.left: parent.left
    anchors.top: parent.top
    width: parent.width
    height: childrenRect.height
    visible: widgetStyle === "Slider"

    Text {
      id: valueLabel
      width: sliderRow.width / 4
      height: fontMetrics.height + 20
      elide: Text.ElideRight
      text: value !== undefined && value != '' ? Number(slider.value).toFixed(rangeItem.precision).toLocaleString() + rangeItem.suffix : ''
      verticalAlignment: Text.AlignVCenter
      horizontalAlignment: Text.AlignLeft
      font: Theme.defaultFont
      color: value === undefined || !enabled ? Theme.mainTextDisabledColor : Theme.mainTextColor
    }

    QfSlider {
      id: slider
      showValueLabel: false
      width: sliderRow.width - valueLabel.width
      height: fontMetrics.height + 20
      implicitWidth: width
      from: !Number.isFinite(rangeItem.min) ? Number.MIN_VALUE : rangeItem.min
      to: !Number.isFinite(rangeItem.max) ? Number.MAX_VALUE : rangeItem.max
      stepSize: !Number.isFinite(rangeItem.step) ? 1 : rangeItem.step
      // TODO: using `rangeItem.parent.value` makes item reuseability harder.
      value: rangeItem.parent.value !== undefined && rangeItem.parent.value !== '' ? Number(rangeItem.parent.value) : from

      onValueChanged: {
        if (sliderRow.visible) {
          rangeItem.valueChangeRequested(slider.value, false);
        }
      }
    }
  }

  Rectangle {
    y: sliderRow.height - height
    visible: widgetStyle === "Slider"
    width: sliderRow.width
    implicitWidth: 120
    height: slider.activeFocus ? 2 : 1
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
