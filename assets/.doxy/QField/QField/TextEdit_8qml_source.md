

# File TextEdit.qml

[**File List**](files.md) **>** [**editorwidgets**](dir_7621275d8427768f32eeef555af1c8d1.md) **>** [**TextEdit.qml**](TextEdit_8qml.md)

[Go to the documentation of this file](TextEdit_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material.impl
import org.qfield
import Theme

EditorWidgetBase {
  id: topItem

  readonly property bool isEditable: isEnabled && LayerUtils.fieldType(field) !== 'QStringList' && LayerUtils.fieldType(field) !== 'QVariantList' && LayerUtils.fieldType(field) !== 'QVariantMap'

  height: childrenRect.height

  // Due to QTextEdit::onLinkActivated does not work on Android & iOS, we need a separate `Text` element to support links https://bugreports.qt.io/browse/QTBUG-38487
  Label {
    id: textReadonlyValue
    height: !textArea.visible ? textField.height : 0
    topPadding: 10
    bottomPadding: 10
    leftPadding: 0
    visible: height !== 0 && !isEditing
    anchors.left: parent.left
    anchors.right: parent.right
    font: Theme.defaultFont
    color: isNull || isEmpty ? Theme.mainTextDisabledColor : Theme.mainTextColor
    opacity: 1
    wrapMode: Text.Wrap
    textFormat: (config['IsMultiline'] === true && config['UseHtml'] === true) || StringUtils.hasLinks(value) ? TextEdit.RichText : TextEdit.AutoText

    text: {
      if (isEmpty) {
        return qsTr("Empty");
      } else if (isNull) {
        return qsTr("NULL");
      } else if (config['IsMultiline'] === true) {
        if (config['UseHtml'] === true) {
          return value;
        }
        return StringUtils.hasLinks(value) ? StringUtils.insertLinks(value).replace(/\n/g, '<br>') : value;
      } else {
        return StringUtils.insertLinks(value).replace(/\n/g, '');
      }
    }

    onLinkActivated: link => {
      Qt.openUrlExternally(link);
    }
  }

  TextField {
    id: textField
    leftPadding: isEditing ? 10 : 0
    visible: (config['IsMultiline'] === undefined || config['IsMultiline'] == false) && isEditing
    enabled: isEditable
    anchors.left: parent.left
    anchors.right: parent.right
    font: Theme.defaultFont
    color: (!isEditable && isEditing) ? Theme.mainTextDisabledColor : Theme.mainTextColor
    maximumLength: field != undefined && field.length > 0 ? field.length : -1
    wrapMode: TextInput.Wrap
    background.visible: enabled || (!isEditable && isEditing)

    text: isNull ? '' : value

    validator: {
      if (field && field.isNumeric)
        if (LayerUtils.fieldType(field) === 'double') {
          doubleValidator;
        } else {
          intValidator;
        }
      else {
        null;
      }
    }

    IntValidator {
      id: intValidator
    }

    DoubleValidator {
      id: doubleValidator
      locale: 'C'
    }

    inputMethodHints: field && field.isNumeric ? Qt.ImhFormattedNumbersOnly : Qt.ImhNone

    onTextChanged: {
      if (text !== '') {
        if (field.isNumeric) {
          let value = parseFloat(text);
          // Only trigger value change for valid numerical values to insure we do not
          // interfere with 'Autogenerate' value
          if (!isNaN(value)) {
            valueChangeRequested(value, false);
          }
        } else {
          valueChangeRequested(text, false);
        }
      } else if (!isNull) {
        valueChangeRequested(text, true);
      }
    }
  }

  TextArea {
    id: textArea
    leftPadding: isEditing ? 10 : 0
    height: config['IsMultiline'] === true ? undefined : 0
    visible: config['IsMultiline'] === true && isEditing
    enabled: isEditable
    anchors.left: parent.left
    anchors.right: parent.right
    wrapMode: Text.Wrap
    font: Theme.defaultFont
    color: (!isEditable && isEditing) ? Theme.mainTextDisabledColor : Theme.mainTextColor

    text: isNull ? '' : value
    textFormat: config['UseHtml'] ? TextEdit.RichText : TextEdit.PlainText

    onTextChanged: {
      if (text !== '') {
        valueChangeRequested(text, false);
      } else if (!isNull) {
        valueChangeRequested(text, true);
      }
    }
    background.visible: enabled || (!isEditable && isEditing)
  }

  FontMetrics {
    id: fontMetrics
    font: textField.font
  }

  Component.onCompleted: {
    menu.addItem(copyTextItem);
    menu.addItem(pasteTextItem);
    menu.addItem(separatorItem);
    menu.addItem(scanCodeItem);
    hasMenu = true;
  }

  Item {
    visible: false

    MenuItem {
      id: copyTextItem
      text: qsTr('Copy Text')

      font: Theme.defaultFont
      icon.source: Theme.getThemeVectorIcon("ic_copy_black_24dp")
      height: 48
      leftPadding: Theme.menuItemLeftPadding

      onTriggered: {
        platformUtilities.copyTextToClipboard(value);
      }
    }

    MenuItem {
      id: pasteTextItem
      text: qsTr('Paste Text')

      font: Theme.defaultFont
      icon.source: Theme.getThemeVectorIcon("ic_paste_black_24dp")
      height: 48
      leftPadding: Theme.menuItemLeftPadding

      onTriggered: {
        var text = platformUtilities.getTextFromClipboard();
        text = text.trim();
        valueChangeRequested(text, text == '');
      }
    }

    MenuSeparator {
      id: separatorItem
      width: parent.width
    }

    MenuItem {
      id: scanCodeItem
      text: qsTr('Scan Code')

      font: Theme.defaultFont
      icon.source: withNfc ? Theme.getThemeVectorIcon("ic_qr_nfc_code_black_24dp") : Theme.getThemeVectorIcon("ic_qr_code_black_24dp")
      height: 48
      leftPadding: Theme.menuItemLeftPadding

      onTriggered: {
        requestBarcode(topItem);
      }
    }
  }

  function requestedBarcodeReceived(string) {
    valueChangeRequested(string, false);
  }
}
```


