import QtQuick
import QtQuick.Controls
import org.qfield.core
import org.qfield.gui

QfEditorWidgetBase {
  id: rangeItem
  enabled: isEnabled

  property bool isDouble: field === undefined || (QfLayerUtils.fieldType(field) !== 'int' && QfLayerUtils.fieldType(field) !== 'qlonglong')
  property string widgetStyle: config["Style"] ? config["Style"] : "TextField"
  property int precision: config["Precision"] ? config["Precision"] : isDouble ? 2 : 0
  property real min: config["Min"] !== undefined ? config["Min"] : -Infinity
  property real max: config["Max"] !== undefined ? config["Max"] : Infinity
  property real step: config["Step"] !== undefined ? config["Step"] : 1
  property string suffix: config["Suffix"] ? config["Suffix"] : ""

  height: childrenRect.height

  function roundValue(value, precision) {
    const multiplier = Math.pow(10, precision || 0);
    return Math.round(value * multiplier) / multiplier;
  }

  Loader {
    id: widgetStyleLoader
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.top: parent.top

    sourceComponent: {
      if (widgetStyle === "Dial") {
        return dialComponent;
      } else if (widgetStyle === "Slider") {
        return sliderComponent;
      }
      return spinBoxComponent;
    }
  }

  Component {
    id: spinBoxComponent

    Row {
      spacing: 5

      Label {
        id: textReadonlyValue
        height: textField.height
        width: parent.width
        topPadding: 10
        bottomPadding: 10
        leftPadding: 0
        visible: !isEditing
        font: QfTheme.defaultFont
        color: (!isEditable && isEditing) || isNull || isEmpty ? QfTheme.mainTextDisabledColor : QfTheme.mainTextColor
        opacity: 1
        wrapMode: Text.Wrap
        text: {
          if (isEmpty) {
            return qsTr("Empty");
          } else if (isNull) {
            return qsTr("NULL");
          }
          return rangeItem.suffix ? value + " " + rangeItem.suffix : value;
        }
      }

      QfTextField {
        id: textField
        leftPadding: isEnabled || (!isEditable && isEditing) ? 10 : 0
        width: parent.width - decreaseButton.width - increaseButton.width - parent.spacing * 2
        visible: isEditing

        font: QfTheme.defaultFont
        color: (!isEditable && isEditing) ? QfTheme.mainTextDisabledColor : QfTheme.mainTextColor
        suffixText: rangeItem.suffix

        text: isNull ? '' : value

        validator: doubleValidator

        inputMethodHints: Qt.ImhFormattedNumbersOnly | Qt.ImhNoPredictiveText

        background.visible: isEnabled || (!isEditable && isEditing)

        onTextEdited: {
          if (text === "") {
            if (!isNull) {
              valueChangeRequested(text, true);
            }
          } else {
            const parsedValue = parseFloat(text);
            if (!isNaN(parsedValue)) {
              if (Number.isFinite(rangeItem.max) && parsedValue > rangeItem.max) {
                valueChangeRequested(rangeItem.max, false);
                text = Qt.binding(function () {
                  return isNull ? '' : value;
                });
              } else {
                valueChangeRequested(parsedValue, false);
              }
            }
          }
        }

        onActiveFocusChanged: {
          if (!activeFocus) {
            commitValue();
          }
        }

        onEditingFinished: commitValue()

        function commitValue() {
          if (text !== "") {
            const parsedValue = parseFloat(text);
            if (!isNaN(parsedValue)) {
              let clampedValue = parsedValue;
              if (Number.isFinite(rangeItem.min) && parsedValue < rangeItem.min) {
                clampedValue = rangeItem.min;
              } else if (Number.isFinite(rangeItem.max) && parsedValue > rangeItem.max) {
                clampedValue = rangeItem.max;
              }
              if (clampedValue !== value) {
                valueChangeRequested(clampedValue, false);
              }
              text = Qt.binding(function () {
                return isNull ? '' : value;
              });
            }
          }
        }
      }

      QfToolButton {
        id: decreaseButton
        width: enabled ? QfTheme.toolButtonSize : 0
        height: QfTheme.toolButtonSize

        anchors.verticalCenter: textField.verticalCenter

        iconSource: QfTheme.getThemeVectorIcon("ic_remove_white_24dp")
        iconColor: QfTheme.mainTextColor
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
          changeValueTimer.interval = changeValueTimer.initialInterval;
          changeValueTimer.triggered();
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
        width: enabled ? QfTheme.toolButtonSize : 0
        height: QfTheme.toolButtonSize

        anchors.verticalCenter: textField.verticalCenter

        iconSource: QfTheme.getThemeVectorIcon("ic_add_white_24dp")
        iconColor: QfTheme.mainTextColor
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
          changeValueTimer.interval = changeValueTimer.initialInterval;
          changeValueTimer.triggered();
          changeValueTimer.restart();
        }
        onReleased: {
          changeValueTimer.stop();
        }
        onCanceled: {
          changeValueTimer.stop();
        }
      }

      Timer {
        id: changeValueTimer
        property int initialInterval: 500
        interval: initialInterval
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
          newValue = rangeItem.roundValue(currentValue - rangeItem.step, rangeItem.precision);
          newValue = Math.max(rangeItem.min, Math.min(rangeItem.max, newValue));
          valueChangeRequested(newValue, false);
        } else {
          if (rangeItem.max <= 0) {
            newValue = Math.max(0, rangeItem.min);
          } else {
            newValue = rangeItem.min;
          }
          valueChangeRequested(newValue, false);
        }
      }

      function increaseValue() {
        const currentValue = Number.parseFloat(textField.text);
        let newValue;
        if (!isNaN(currentValue)) {
          newValue = rangeItem.roundValue(currentValue + rangeItem.step, rangeItem.precision);
          newValue = Math.min(rangeItem.max, Math.max(rangeItem.min, newValue));
          valueChangeRequested(newValue, false);
        } else {
          if (rangeItem.max >= 0) {
            newValue = Math.max(0, rangeItem.min);
          } else {
            newValue = rangeItem.min;
          }
          valueChangeRequested(newValue, false);
        }
      }

      DoubleValidator {
        id: doubleValidator

        locale: 'C'

        bottom: rangeItem.min
        top: rangeItem.max
        decimals: isDouble ? -1 : 0
      }
    }
  }

  Component {
    id: sliderComponent

    Row {
      id: sliderRow
      height: childrenRect.height

      Text {
        id: valueLabel
        width: sliderRow.width / 4
        height: Math.max(QfTheme.toolButtonSize, fontMetrics.height + 20)
        elide: Text.ElideRight
        text: {
          const formattedValue = Number(slider.value).toFixed(rangeItem.precision).toLocaleString() + rangeItem.suffix;
          if (isEmpty) {
            return qsTr("Empty");
          } else if (isNull) {
            return qsTr("NULL");
          }
          return formattedValue;
        }
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignLeft
        font: QfTheme.defaultFont
        color: (!isEditable && isEditing) || isNull || isEmpty ? QfTheme.mainTextDisabledColor : QfTheme.mainTextColor
      }

      QfSlider {
        id: slider
        showValueLabel: false
        width: sliderRow.width - valueLabel.width
        height: Math.max(QfTheme.toolButtonSize, fontMetrics.height + 20)
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

      FontMetrics {
        id: fontMetrics
        font: QfTheme.defaultFont
      }
    }
  }

  Component {
    id: dialComponent

    Item {
      id: dialItem

      readonly property var fieldValue: value

      height: dialRow.height + 12

      Row {
        id: dialRow
        anchors.horizontalCenter: parent.horizontalCenter
        y: 6
        spacing: 12

        QfToolButton {
          id: dialDecreaseButton
          anchors.verticalCenter: parent.verticalCenter
          width: QfTheme.toolButtonSize
          height: QfTheme.toolButtonSize
          visible: enabled
          autoRepeat: true

          iconSource: QfTheme.getThemeVectorIcon("ic_remove_white_24dp")
          iconColor: QfTheme.mainTextColor
          bgcolor: "transparent"

          onClicked: {
            dial.stepValue(false);
          }
        }

        QfDial {
          id: dial

          from: Number.isFinite(rangeItem.min) ? rangeItem.min : -Number.MAX_VALUE
          to: Number.isFinite(rangeItem.max) ? rangeItem.max : Number.MAX_VALUE
          stepSize: Number.isFinite(rangeItem.step) && rangeItem.step > 0 ? rangeItem.step : 1
          snapMode: Dial.SnapAlways
          value: hasValue ? Number(dialItem.fieldValue) : from

          hasValue: !isNull && !isEmpty
          placeholderText: isEmpty ? qsTr("Empty") : qsTr("NULL")
          precision: rangeItem.precision
          suffixText: rangeItem.suffix
          textColor: !isEditable && isEditing ? QfTheme.mainTextDisabledColor : QfTheme.mainTextColor

          onMoved: {
            rangeItem.valueChangeRequested(rangeItem.roundValue(value, rangeItem.precision), false);
          }

          function stepValue(increase) {
            const previousValue = value;
            if (increase) {
              dial.increase();
            } else {
              dial.decrease();
            }
            if (value !== previousValue || !hasValue) {
              rangeItem.valueChangeRequested(rangeItem.roundValue(value, rangeItem.precision), false);
            }
          }
        }

        QfToolButton {
          id: dialIncreaseButton
          anchors.verticalCenter: parent.verticalCenter
          width: QfTheme.toolButtonSize
          height: QfTheme.toolButtonSize
          visible: enabled
          autoRepeat: true

          iconSource: QfTheme.getThemeVectorIcon("ic_add_white_24dp")
          iconColor: QfTheme.mainTextColor
          bgcolor: "transparent"

          onClicked: {
            dial.stepValue(true);
          }
        }
      }
    }
  }
}
