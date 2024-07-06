import QtQuick 2.14
import QtQuick.Controls 2.14
import Theme 1.0
import org.qfield 1.0
import org.qgis 1.0
import "."

ProcessingParameterWidgetBase {
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

    TextField {
      id: textField
      height: fontMetrics.height + 20
      topPadding: 10
      bottomPadding: 10
      rightPadding: 0
      leftPadding: enabled ? 5 : 0
      width: parent.width - decreaseButton.width - increaseButton.width - 10 - (distanceConvertible ? unitTypesComboBox.width : unitTypeLabel.width)

      font: Theme.defaultFont
      color: value === undefined || !enabled ? Theme.mainTextDisabledColor : Theme.mainTextColor

      text: currentValue !== undefined ? currentValue : ''

      validator: doubleValidator

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
        if (text != currentValue) {
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
      width: distanceConvertible ? 100 : 0
      font: Theme.defaultFont
      color: Theme.mainTextColor

      text: distanceUnit === Qgis.DistanceUnit.Degrees ? qsTr('degrees') : qsTr('<unknown>')
    }

    ComboBox {
      id: unitTypesComboBox

      property bool initialized: false

      visible: distanceConvertible
      implicitContentWidthPolicy: ComboBox.WidestTextWhenCompleted

      textRole: "display"
      valueRole: "value"
      model: ListModel {
        ListElement {
          display: "meters"
          value: Qgis.DistanceUnit.Meters
        }
        ListElement {
          display: "kilometers"
          value: Qgis.DistanceUnit.Kilometers
        }
        ListElement {
          display: "feet"
          value: Qgis.DistanceUnit.Feet
        }
        ListElement {
          display: "yards"
          value: Qgis.DistanceUnit.Yards
        }
        ListElement {
          display: "miles"
          value: Qgis.DistanceUnit.Miles
        }
        ListElement {
          display: "nautical miles"
          value: Qgis.DistanceUnit.NauticalMiles
        }
        ListElement {
          display: "centimeters"
          value: Qgis.DistanceUnit.Centimeters
        }
        ListElement {
          display: "millimeters"
          value: Qgis.DistanceUnit.Millimeters
        }
        ListElement {
          display: "inches"
          value: Qgis.DistanceUnit.Inches
        }
      }

      onCurrentIndexChanged: {
        if (textField.text != value) {
          if (!isNaN(parseFloat(textField.text))) {
            let numberValue = Math.max(distanceItem.min, Math.min(distanceItem.max, textField.text));
            prepareValueChangeRequest(numberValue);
          }
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
          initialized = true;
        }
      }
    }

    QfToolButton {
      id: decreaseButton
      width: enabled ? 48 : 0
      height: 48

      anchors.verticalCenter: textField.verticalCenter

      iconSource: Theme.getThemeVectorIcon("ic_remove_black_24dp")
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

      iconSource: Theme.getThemeVectorIcon("ic_add_black_24dp")
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
        hitBoundary = textField.text == distanceItem.max;
      } else {
        decreaseValue();
        hitBoundary = textField.text == distanceItem.min;
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
      newValue = currentValue - distanceItem.step;
      prepareValueChangeRequest(Math.max(distanceItem.min, newValue));
    } else {
      newValue = 0;
      prepareValueChangeRequest(newValue, false);
    }
  }

  function increaseValue() {
    var currentValue = Number.parseFloat(textField.text);
    var newValue;
    if (!isNaN(currentValue)) {
      newValue = currentValue + distanceItem.step;
      prepareValueChangeRequest(Math.min(distanceItem.max, newValue));
    } else {
      newValue = 0;
      prepareValueChangeRequest(newValue);
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
