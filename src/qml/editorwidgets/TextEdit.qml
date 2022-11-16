import QtQuick 2.14
import QtQuick.Controls 2.14

import Theme 1.0
import org.qfield 1.0

import "."

EditorWidgetBase {
  id: topItem

  property bool isEditable: isEnabled &&
                            LayerUtils.fieldType( field ) !== 'QStringList' &&
                            LayerUtils.fieldType( field ) !== 'QVariantList' &&
                            LayerUtils.fieldType( field ) !== 'QVariantMap'

  height: childrenRect.height

  // Due to QTextEdit::onLinkActivated does not work on Android & iOS, we need a separate `Text` element to support links https://bugreports.qt.io/browse/QTBUG-38487
  Label {
    id: textReadonlyValue
    height: !textArea.visible ? textField.height : 0
    topPadding: 10
    bottomPadding: 10
    visible: height !== 0 && !isEditable
    anchors.left: parent.left
    anchors.right: parent.right
    font: Theme.defaultFont
    color: 'gray'
    wrapMode: Text.Wrap
    textFormat: config['IsMultiline'] === true && config['UseHtml'] ? TextEdit.RichText : TextEdit.AutoText

    text: value == null ? '' : config['IsMultiline'] === true ? stringUtilities.insertLinks(value) : stringUtilities.insertLinks(value).replace('\n','')

    onLinkActivated: Qt.openUrlExternally(link)
  }

  TextField {
    id: textField
    topPadding: 10
    bottomPadding: 10
    visible: (config['IsMultiline'] === undefined || config['IsMultiline'] == false) && isEditable
    anchors.left: parent.left
    anchors.right: parent.right
    font: Theme.defaultFont
    color: 'black'
    maximumLength: field != undefined && field.length > 0 ? field.length : -1
    wrapMode: TextInput.Wrap

    text: value == null ? '' : value

    validator: {
      if (field && field.isNumeric)
          if ( LayerUtils.fieldType( field ) === 'double')
          {
            doubleValidator;
          }
          else
          {
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

    background: Rectangle {
        width: parent.width
        height: parent.height
        color: "transparent";
    }

    onTextChanged: {
      valueChangeRequested( text, text == '' )
    }
  }

  TextArea {
    id: textArea
    height: config['IsMultiline'] === true ? undefined : 0
    visible: config['IsMultiline'] === true && isEditable
    enabled: isEditable
    anchors.left: parent.left
    anchors.right: parent.right
    wrapMode: Text.Wrap
    font: Theme.defaultFont

    text: value !== undefined ? value : ''
    textFormat: config['UseHtml'] ? TextEdit.RichText : TextEdit.PlainText

    background: Rectangle {
        width: parent.width
        height: parent.height
        color: "transparent";
    }

    onTextChanged: {
      valueChangeRequested( text, text == '' )
    }
  }

  Rectangle {
      anchors.left: parent.left
      anchors.right: parent.right
      y: Math.max( textField.height, textArea.height ) - height - textField.bottomPadding / 2
      implicitWidth: 120
      height: textField.activeFocus || textArea.activeFocus ? 2: 1
      color: textField.activeFocus || textArea.activeFocus ? Theme.accentColor : Theme.accentLightColor
  }

  FontMetrics {
    id: fontMetrics
    font: textField.font
  }

  Component.onCompleted: {
    menu.addItem( copyTextItem );
    menu.addItem( pasteTextItem );
    menu.addItem( separatorItem );
    menu.addItem( scanCodeItem );

    hasMenu = true;
  }


  MenuItem {
    id: copyTextItem
    text: qsTr( 'Copy Text' )

    font: Theme.defaultFont
    icon.source: Theme.getThemeVectorIcon( "ic_copy_black_24dp" )
    height: 48
    leftPadding: 10

    onTriggered: {
      platformUtilities.copyTextToClipboard(value)
    }
  }
  MenuItem {
    id: pasteTextItem
    text: qsTr( 'Paste Text' )

    font: Theme.defaultFont
    icon.source: Theme.getThemeVectorIcon( "ic_paste_black_24dp" )
    height: 48
    leftPadding: 10

    onTriggered: {
      var text = platformUtilities.getTextFromClipboard();
      text = text.trim()
      valueChangeRequested(text, text == '')
    }
  }

  MenuSeparator { id: separatorItem; width: parent.width }

  MenuItem {
    id: scanCodeItem
    text: qsTr( 'Scan Code' )

    font: Theme.defaultFont
    icon.source: Theme.getThemeVectorIcon( "ic_qrcode_black_24dp" )
    height: 48
    leftPadding: 10

    onTriggered: {
      requestBarcode(topItem)
    }
  }

  function requestedBarcodeReceived(string) {
    valueChangeRequested(string, false)
  }
}
