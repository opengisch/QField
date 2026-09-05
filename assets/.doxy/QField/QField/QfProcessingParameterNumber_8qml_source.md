

# File QfProcessingParameterNumber.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**processingparameterwidgets**](dir_0b91122ea4e9f423064291e4771a420d.md) **>** [**QfProcessingParameterNumber.qml**](QfProcessingParameterNumber_8qml.md)

[Go to the documentation of this file](QfProcessingParameterNumber_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import org.qfield.gui
import org.qfield.core
import org.qgis

QfProcessingParameterWidgetBase {
  id: numberItem

  property real step: 1
  property real min: configuration['minimum']
  property real max: configuration['maximum']
  property bool isDouble: configuration['dataType'] === Qgis.ProcessingNumberParameterType.Double

  height: childrenRect.height

  Row {
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.top: parent.top
    spacing: 5

    QfTextField {
      id: textField
      height: fontMetrics.height + 20
      width: parent.width - decreaseButton.width - increaseButton.width - parent.spacing * 2

      font: QfTheme.defaultFont
      color: value === undefined || !enabled ? QfTheme.mainTextDisabledColor : QfTheme.mainTextColor

      text: value !== undefined ? value : ''

      validator: {
        if (isDouble) {
          doubleValidator;
        } else {
          intValidator;
        }
      }

      inputMethodHints: Qt.ImhFormattedNumbersOnly

      onTextChanged: {
        if (text != value) {
          if (!isNaN(parseFloat(text))) {
            let numberValue = Math.max(numberItem.min, Math.min(numberItem.max, text));
            valueChangeRequested(numberValue);
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
        adjustValue(-1);
      }
      onDoubleClicked: {
        adjustValue(-1);
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
        adjustValue(1);
      }
      onDoubleClicked: {
        adjustValue(1);
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
  }

  Timer {
    id: changeValueTimer
    property int initialInterval: 500
    interval: initialInterval
    repeat: true

    property bool increase: true

    onTriggered: {
      var hitBoundary = false;
      if (increase) {
        adjustValue(1);
        hitBoundary = parseFloat(textField.text) === numberItem.max;
      } else {
        adjustValue(-1);
        hitBoundary = parseFloat(textField.text) === numberItem.min;
      }
      if (!hitBoundary) {
        if (interval > 50)
          interval = interval * 0.7;
      } else {
        stop();
      }
    }
  }

  function adjustValue(direction) {
    var currentValue = parseFloat(textField.text);
    var newValue;
    if (!isNaN(currentValue)) {
      newValue = currentValue + (numberItem.step * direction);
      valueChangeRequested(Math.min(numberItem.max, Math.max(numberItem.min, newValue)));
    } else {
      newValue = 0;
      valueChangeRequested(newValue, false);
    }
  }

  FontMetrics {
    id: fontMetrics
    font: textField.font
  }

  IntValidator {
    id: intValidator
    bottom: numberItem.min
    top: numberItem.max
  }

  DoubleValidator {
    id: doubleValidator
    locale: 'C'
    bottom: numberItem.min
    top: numberItem.max
  }
}
```


