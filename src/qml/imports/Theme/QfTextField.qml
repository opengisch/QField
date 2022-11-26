import QtQuick 2.14
import QtQuick.Controls 2.14

Item {
  id: textFieldWrapper

  property alias font: textField.font
  property alias text: textField.text
  property alias horizontalAlignment: textField.horizontalAlignment
  property alias inputMethodHints: textField.inputMethodHints
  property alias inputMask: textField.inputMask

  property var echoMode: TextInput.Normal

  signal textEdited
  signal editingFinished
  signal returnPressed

  width: textField.width
  height: textField.height

  FontMetrics {
    id: fontMetrics
    font: Theme.defaultFont
  }

  TextField {
    id: textField
    echoMode: textFieldWrapper.echoMode
    width: textFieldWrapper.width
    font: Theme.defaultFont
    rightPadding: showPasswordButton.visible ? 2 * showPasswordButton.width : 0
    inputMethodHints: Qt.ImhNone

    background: Rectangle {
      y: textField.height - height - textField.bottomPadding / 2
      implicitWidth: 120
      height: textField.activeFocus ? 2: 1
      color: textField.activeFocus ? Theme.accentColor : Theme.accentLightColor
    }

    onActiveFocusChanged: {
      if ( !activeFocus ) {
        echoMode = textFieldWrapper.echoMode
      }
    }

    onTextEdited: {
        textFieldWrapper.textEdited()
    }

    onEditingFinished: {
        textFieldWrapper.editingFinished();
    }

    onFocusChanged: {
        if (focus) {
            Qt.inputMethod.show()
        }
    }

    Keys.onReturnPressed: {
        textFieldWrapper.returnPressed()
    }
  }

  Image {
    id: showPasswordButton
    z: 1
    width: 20
    height: 20
    visible: !!textFieldWrapper.echoMode && textFieldWrapper.echoMode !== TextInput.Normal
    source: textField.echoMode === TextInput.Normal
            ? Theme.getThemeVectorIcon('ic_hide_green_48dp')
            : Theme.getThemeVectorIcon('ic_show_green_48dp')
    sourceSize.width: 20 * screen.devicePixelRatio
    sourceSize.height: 20 * screen.devicePixelRatio
    fillMode: Image.PreserveAspectFit
    anchors.right: textField.right
    anchors.rightMargin: height - 7
    anchors.verticalCenter: textField.verticalCenter
    opacity: textField.text.length > 0 ? 1 : 0.25

    MouseArea {
      anchors.fill: parent
      onClicked: {
        textField.echoMode = textField.echoMode === TextInput.Normal
            ? textFieldWrapper.echoMode
            : TextInput.Normal
      }
    }
  }
}
