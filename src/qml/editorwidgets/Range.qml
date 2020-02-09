import QtQuick 2.11
import QtQuick.Controls 2.4
import Theme 1.0

Item {
  signal valueChanged(var value, bool isNull)
  height: childrenRect.height

  QtObject {
      id: fieldData
      property bool isNumeric: field ? field.isNumeric : false
  }

  TextField {
    id: textField
    height: fontMetrics.height + 20 * dp
    topPadding: 10 * dp
    bottomPadding: 10 * dp
    visible: height !== 0
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
      implicitWidth: 120 * dp
      height: textField.activeFocus ? 2 * dp : 1 * dp
      color: textField.activeFocus ? "#4CAF50" : "#C8E6C9"
    }

    onTextChanged: {
      valueChanged( text, text == '' )
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
  }
}
