

# File area.qml

[**File List**](files.md) **>** [**processingparameterwidgets**](dir_845aa3fe93488bbf89a398aeca40ac68.md) **>** [**area.qml**](area_8qml.md)

[Go to the documentation of this file](area_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import Theme
import org.qfield
import org.qgis

ProcessingParameterWidgetBase {
  id: areaItem

  property real step: 1
  property real min: configuration['minimum']
  property real max: configuration['maximum']
  property int areaUnit: configuration['areaUnit']
  property bool areaConvertible: areaUnit !== Qgis.AreaUnit.SquareDegrees && areaUnit !== Qgis.AreaUnit.Unknown

  height: childrenRect.height

  property double currentValue: areaConvertible ? value * ProcessingUtils.fromAreaUnitToAreaUnitFactor(areaUnit, unitTypesComboBox.currentValue) : value

  Row {
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.top: parent.top
    spacing: 5

    QfTextField {
      id: textField
      height: fontMetrics.height + 20
      width: parent.width - decreaseButton.width - increaseButton.width - (areaConvertible ? unitTypesComboBox.width : unitTypeLabel.width) - parent.spacing * 3

      font: Theme.defaultFont
      color: value === undefined || !enabled ? Theme.mainTextDisabledColor : Theme.mainTextColor

      text: currentValue !== undefined ? currentValue : ''

      validator: doubleValidator

      inputMethodHints: Qt.ImhFormattedNumbersOnly

      onTextChanged: {
        if (parseFloat(text) !== currentValue) {
          if (!isNaN(parseFloat(text))) {
            let numberValue = Math.max(areaItem.min, Math.min(areaItem.max, text));
            prepareValueChangeRequest(numberValue);
          }
        }
      }
    }

    Label {
      id: unitTypeLabel
      visible: !areaConvertible
      width: areaConvertible ? contentWidth : 0
      font: Theme.defaultFont
      color: Theme.mainTextColor

      text: areaUnit === Qgis.AreaUnit.SquareDegrees ? qsTr('square degrees') : qsTr('<unknown>')
    }

    QfComboBox {
      id: unitTypesComboBox

      visible: areaConvertible
      implicitContentWidthPolicy: ComboBox.WidestTextWhenCompleted

      textRole: "display"
      valueRole: "value"
      model: ListModel {
        ListElement {
          display: qsTr("sqr. meters")
          value: Qgis.AreaUnit.SquareMeters
        }
        ListElement {
          display: qsTr("sqr. kilometers")
          value: Qgis.AreaUnit.SquareKilometers
        }
        ListElement {
          display: qsTr("sqr. feet")
          value: Qgis.AreaUnit.SquareFeet
        }
        ListElement {
          display: qsTr("sqr. yards")
          value: Qgis.AreaUnit.SquareYards
        }
        ListElement {
          display: qsTr("sqr. miles")
          value: Qgis.AreaUnit.SquareMiles
        }
        ListElement {
          display: qsTr("hectares")
          value: Qgis.AreaUnit.Hectares
        }
        ListElement {
          display: qsTr("acres")
          value: Qgis.AreaUnit.Acres
        }
        ListElement {
          display: qsTr("sqr. nautical miles")
          value: Qgis.AreaUnit.SquareNauticalMiles
        }
        ListElement {
          display: qsTr("sqr. centimeters")
          value: Qgis.AreaUnit.SquareCentimeters
        }
        ListElement {
          display: qsTr("sqr. millimeters")
          value: Qgis.AreaUnit.SquareMillimeters
        }
        ListElement {
          display: qsTr("sqr. inches")
          value: Qgis.AreaUnit.SquareInches
        }
      }

      onCurrentIndexChanged: {
        if (!isNaN(parseFloat(textField.text))) {
          let numberValue = Math.max(areaItem.min, Math.min(areaItem.max, textField.text));
          prepareValueChangeRequest(numberValue);
        }
      }

      Component.onCompleted: {
        if (areaConvertible) {
          for (let i = 0; i < model.count; i++) {
            if (model.get(i).value === areaUnit) {
              currentIndex = areaUnit;
              break;
            }
          }
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
        adjustValue(-1);
      }
      onDoubleClicked: {
        adjustValue(-1);
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
        adjustValue(1);
      }
      onDoubleClicked: {
        adjustValue(1);
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
        adjustValue(1);
        hitBoundary = parseFloat(textField.text) === areaItem.max;
      } else {
        adjustValue(-1);
        hitBoundary = parseFloat(textField.text) === areaItem.min;
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
      newValue = currentValue + (areaItem.step * direction);
      prepareValueChangeRequest(Math.min(areaItem.max, Math.max(areaItem.min, newValue)));
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
    bottom: areaItem.min
    top: areaItem.max
  }

  function prepareValueChangeRequest(newValue) {
    if (areaConvertible) {
      valueChangeRequested(newValue * ProcessingUtils.fromAreaUnitToAreaUnitFactor(unitTypesComboBox.currentValue, areaUnit));
    } else {
      valueChangeRequested(newValue);
    }
  }
}
```


