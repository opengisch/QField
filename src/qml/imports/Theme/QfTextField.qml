import QtQuick 2.0
import QtQuick.Controls 2.0

Item {
  id: textFieldWrapper

  property alias font: textField.font
  property var echoMode: TextInput.Normal

  width: textField.width
  height: textField.height

  TextField {
    id: textField
    echoMode: textFieldWrapper.echoMode
    height: font.height + 20
    width: textFieldWrapper.width
    font: Theme.defaultFont
    rightPadding: showPasswordButton.visible ? 2 * showPasswordButton.width : 0

    background: Rectangle {
      y: textField.height - height - textField.bottomPadding / 2
      implicitWidth: 120
      height: textField.activeFocus ? 2: 1
      color: textField.activeFocus ? '#4CAF50' : '#C8E6C9'
    }

    onActiveFocusChanged: {
      if ( !activeFocus ) {
        echoMode = textFieldWrapper.echoMode
      }
    }
  }

  Image {
    id: showPasswordButton
    z: 1
    visible: !!textFieldWrapper.echoMode && textFieldWrapper.echoMode !== TextInput.Normal
    source: textField.echoMode === TextInput.Normal
            ? Theme.getThemeVectorIcon('ic_hide_green_48dp')
            : Theme.getThemeVectorIcon('ic_show_green_48dp')
    sourceSize.width: 20 * screen.devicePixelRatio
    sourceSize.height: 20 * screen.devicePixelRatio
    fillMode: Image.PreserveAspectFit
    anchors.top: textField.top
    anchors.right: textField.right
    anchors.topMargin: height - 7
    anchors.rightMargin: height - 7
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
