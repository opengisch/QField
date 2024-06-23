import QtQuick 2.14
import QtQuick.Controls 2.14

import Theme 1.0
import org.qfield 1.0

import "."

ProcessingParameterWidgetBase {
  id: numberItem

  property real step: 1
  property real min: -99999999999999999999
  property real max: 99999999999999999999

  height: childrenRect.height

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
      width: parent.width - decreaseButton.width - increaseButton.width

      font: Theme.defaultFont
      color: value === undefined || !enabled ? Theme.mainTextDisabledColor : Theme.mainTextColor

      text: value !== undefined ? value : ''

      validator: {
        if (true)
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
        if (text != value) {
          if (!isNaN(parseFloat(text))) {
            valueChangeRequested( text )
          }
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
        hitBoundary = textField.text == numberItem.max
      } else {
        decreaseValue();
        hitBoundary = textField.text == numberItem.min;
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
      newValue = currentValue - numberItem.step;
      valueChangeRequested(Math.max(numberItem.min, newValue))
    } else {
      newValue = 0;
      valueChangeRequested(newValue, false)
    }
  }

  function increaseValue() {
    var currentValue = Number.parseFloat(textField.text)
    var newValue
    if (!isNaN(currentValue)) {
      newValue = currentValue + numberItem.step;
      valueChangeRequested(Math.min(numberItem.max, newValue ))
    } else {
      newValue = 0;
      valueChangeRequested(newValue)
    }
  }

  FontMetrics {
    id: fontMetrics
    font: textField.font
  }

  IntValidator {
    id: intValidator
  }

  DoubleValidator {
    id: doubleValidator

    locale: 'C'
  }
}
