

# File Range.qml

[**File List**](files.md) **>** [**editorwidgets**](dir_7621275d8427768f32eeef555af1c8d1.md) **>** [**Range.qml**](Range_8qml.md)

[Go to the documentation of this file](Range_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import org.qfield
import Theme

EditorWidgetBase {
  id: rangeItem
  enabled: isEnabled

  property bool isDouble: field === undefined || (LayerUtils.fieldType(field) !== 'int' && LayerUtils.fieldType(field) !== 'qlonglong')
  property string widgetStyle: config["Style"] ? config["Style"] : "TextField"
  property int precision: config["Precision"] ? config["Precision"] : isDouble ? 2 : 0
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
    spacing: 5

    Label {
      id: textReadonlyValue
      height: textField.height
      width: parent.width
      topPadding: 10
      bottomPadding: 10
      leftPadding: 0
      visible: !isEditing
      font: Theme.defaultFont
      color: (!isEditable && isEditing) || isNull || isEmpty ? Theme.mainTextDisabledColor : Theme.mainTextColor
      opacity: 1
      wrapMode: Text.Wrap
      text: {
        if (isEmpty) {
          return qsTr("Empty");
        } else if (isNull) {
          return qsTr("NULL");
        }
        return value;
      }
    }

    TextField {
      id: textField
      leftPadding: isEnabled || (!isEditable && isEditing) ? 10 : 0
      width: parent.width - decreaseButton.width - increaseButton.width - parent.spacing * 2
      visible: isEditing

      font: Theme.defaultFont
      color: (!isEditable && isEditing) ? Theme.mainTextDisabledColor : Theme.mainTextColor

      text: isNull ? '' : value

      validator: doubleValidator

      inputMethodHints: Qt.ImhFormattedNumbersOnly

      background.visible: isEnabled || (!isEditable && isEditing)

      onTextChanged: {
        if (text !== "") {
          if (!isNaN(parseFloat(text))) {
            valueChangeRequested(text, false);
          }
        } else if (!isNull) {
          valueChangeRequested(text, true);
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
      let hitBoundary = false;
      if (increase) {
        increaseValue();
        hitBoundary = textField.text === rangeItem.max;
      } else {
        decreaseValue();
        hitBoundary = textField.text === rangeItem.min;
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
    const currentValue = Number.parseFloat(textField.text);
    let newValue;
    if (!isNaN(currentValue)) {
      newValue = roundValue(currentValue - rangeItem.step, rangeItem.precision);
      valueChangeRequested(Math.max(rangeItem.min, newValue), false);
    } else {
      newValue = 0;
      valueChangeRequested(newValue, false);
    }
  }

  function increaseValue() {
    const currentValue = Number.parseFloat(textField.text);
    let newValue;
    if (!isNaN(currentValue)) {
      newValue = roundValue(currentValue + rangeItem.step, rangeItem.precision);
      valueChangeRequested(Math.min(rangeItem.max, newValue), false);
    } else {
      newValue = 0;
      valueChangeRequested(newValue, false);
    }
  }

  function roundValue(value, precision) {
    const multiplier = Math.pow(10, precision || 0);
    return Math.round(value * multiplier) / multiplier;
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
      text: {
        const formattedValue = Number(slider.value).toFixed(rangeItem.precision).toLocaleString() + rangeItem.suffix;
        if (isEditing) {
          return (!isNull && !isEmpty) ? formattedValue : '';
        }
        if (isEmpty) {
          return qsTr("Empty");
        } else if (isNull) {
          return qsTr("NULL");
        }
        return formattedValue;
      }
      verticalAlignment: Text.AlignVCenter
      horizontalAlignment: Text.AlignLeft
      font: Theme.defaultFont
      color: (!isEditable && isEditing) || isNull || isEmpty ? Theme.mainTextDisabledColor : Theme.mainTextColor
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
    decimals: isDouble ? -1 : 0
  }
}
```


