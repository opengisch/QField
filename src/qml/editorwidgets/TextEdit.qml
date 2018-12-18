import QtQuick 2.11
import QtQuick.Controls 2.4

Item {
  signal valueChanged(var value, bool isNull)
  height: childrenRect.height

  TextField {
    id: textField
    height: textArea.height == 0 ? fontMetrics.height + 20 * dp : 0
    topPadding: 10 * dp
    bottomPadding: 10 * dp
    visible: height !== 0
    anchors.left: parent.left
    anchors.right: parent.right
    font.pointSize: 14

    text: value !== undefined ? value : ''

    validator: {
      if (field.isNumeric || widget == 'Range') {
        if (platformUtilities.fieldType( field ) === 'double')
          doubleValidator;
        else
          intValidator;
      }
      else {
        null;
      }
    }

    inputMethodHints: field.isNumeric || widget == 'Range' ? Qt.ImhFormattedNumbersOnly : Qt.ImhNone

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

  TextArea {
    id: textArea
    height: config['IsMultiline'] === true ? undefined : 0
    visible: height !== 0
    anchors.left: parent.left
    anchors.right: parent.right
    wrapMode: Text.Wrap

    text: value !== undefined ? value : ''
    textFormat: config['UseHtml'] ? TextEdit.RichText : TextEdit.PlainText

    onEditingFinished: {
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
