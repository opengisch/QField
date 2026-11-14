

# File QfProcessingParameterDistance.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**processingparameterwidgets**](dir_0b91122ea4e9f423064291e4771a420d.md) **>** [**QfProcessingParameterDistance.qml**](QfProcessingParameterDistance_8qml.md)

[Go to the documentation of this file](QfProcessingParameterDistance_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import org.qfield.gui
import org.qfield.core
import org.qgis

QfProcessingParameterWidgetBase {
  id: distanceItem

  property real step: 1
  property real min: configuration['minimum']
  property real max: configuration['maximum']
  property int distanceUnit: configuration['distanceUnit']
  property bool distanceConvertible: distanceUnit !== Qgis.DistanceUnit.Degrees && distanceUnit !== Qgis.DistanceUnit.Unknown

  height: childrenRect.height

  property double currentValue: distanceConvertible ? value * UnitTypes.fromUnitToUnitFactor(distanceUnit, unitTypesComboBox.currentValue) : value

  Row {
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.top: parent.top
    spacing: 5

    QfTextField {
      id: textField
      height: fontMetrics.height + 20
      width: parent.width - decreaseButton.width - increaseButton.width - (distanceConvertible ? unitTypesComboBox.width : unitTypeLabel.width) - parent.spacing * 3

      font: QfTheme.defaultFont
      color: value === undefined || !enabled ? QfTheme.mainTextDisabledColor : QfTheme.mainTextColor

      text: currentValue !== undefined ? currentValue : ''

      validator: doubleValidator

      inputMethodHints: Qt.ImhFormattedNumbersOnly

      onTextChanged: {
        if (parseFloat(text) !== currentValue) {
          if (!isNaN(parseFloat(text))) {
            let numberValue = Math.max(distanceItem.min, Math.min(distanceItem.max, text));
            prepareValueChangeRequest(numberValue);
          }
        }
      }
    }

    Label {
      id: unitTypeLabel
      visible: !distanceConvertible
      width: distanceConvertible ? contentWidth : 0
      font: QfTheme.defaultFont
      color: QfTheme.mainTextColor

      text: distanceUnit === Qgis.DistanceUnit.Degrees ? qsTr('degrees') : qsTr('<unknown>')
    }

    QfComboBox {
      id: unitTypesComboBox

      visible: distanceConvertible
      implicitContentWidthPolicy: ComboBox.WidestTextWhenCompleted

      textRole: "display"
      valueRole: "value"
      model: ListModel {
        ListElement {
          display: qsTr("meters")
          value: Qgis.DistanceUnit.Meters
        }
        ListElement {
          display: qsTr("kilometers")
          value: Qgis.DistanceUnit.Kilometers
        }
        ListElement {
          display: qsTr("feet")
          value: Qgis.DistanceUnit.Feet
        }
        ListElement {
          display: qsTr("yards")
          value: Qgis.DistanceUnit.Yards
        }
        ListElement {
          display: qsTr("miles")
          value: Qgis.DistanceUnit.Miles
        }
        ListElement {
          display: qsTr("nautical miles")
          value: Qgis.DistanceUnit.NauticalMiles
        }
        ListElement {
          display: qsTr("centimeters")
          value: Qgis.DistanceUnit.Centimeters
        }
        ListElement {
          display: qsTr("millimeters")
          value: Qgis.DistanceUnit.Millimeters
        }
        ListElement {
          display: qsTr("inches")
          value: Qgis.DistanceUnit.Inches
        }
      }

      onCurrentIndexChanged: {
        if (!isNaN(parseFloat(textField.text))) {
          let numberValue = Math.max(distanceItem.min, Math.min(distanceItem.max, textField.text));
          prepareValueChangeRequest(numberValue);
        }
      }

      Component.onCompleted: {
        if (distanceConvertible) {
          for (let i = 0; i < model.count; i++) {
            if (model.get(i).value === distanceUnit) {
              currentIndex = distanceUnit;
              break;
            }
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
        hitBoundary = parseFloat(textField.text) === distanceItem.max;
      } else {
        adjustValue(-1);
        hitBoundary = parseFloat(textField.text) === distanceItem.min;
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
      newValue = currentValue + (distanceItem.step * direction);
      prepareValueChangeRequest(Math.min(distanceItem.max, Math.max(distanceItem.min, newValue)));
    } else {
      newValue = 0;
      prepareValueChangeRequest(newValue, false);
    }
  }

  FontMetrics {
    id: fontMetrics
    font: textField.font
  }

  DoubleValidator {
    id: doubleValidator
    locale: 'C'
    bottom: distanceItem.min
    top: distanceItem.max
  }

  function prepareValueChangeRequest(newValue) {
    if (distanceConvertible) {
      valueChangeRequested(newValue * UnitTypes.fromUnitToUnitFactor(unitTypesComboBox.currentValue, distanceUnit));
    } else {
      valueChangeRequested(newValue);
    }
  }
}
```


