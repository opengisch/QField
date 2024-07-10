import QtQuick 2.14
import QtQuick.Controls 2.14

TextField {
  id: textField
  echoMode: TextInput.Normal
  font: Theme.defaultFont
  placeholderTextColor: Theme.accentLightColor
  rightPadding: showPasswordButton.visible ? showPasswordButton.width : 0
  leftPadding: rightPadding
  topPadding: 10
  bottomPadding: 10
  inputMethodHints: Qt.ImhNone

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

  onFocusChanged: {
    if (focus) {
      Qt.inputMethod.show();
    }
  }

  QfToolButton {
    id: showPasswordButton
    z: 1
    visible: !!textField.echoMode && textField.echoMode !== TextInput.Normal
    iconSource: textField.echoMode === TextInput.Normal ? Theme.getThemeVectorIcon('ic_hide_green_48dp') : Theme.getThemeVectorIcon('ic_show_green_48dp')
    anchors.right: textField.right
    anchors.verticalCenter: textField.verticalCenter
    opacity: textField.text.length > 0 ? 1 : 0.25

    onClicked: {
      textField.echoMode = textField.echoMode === TextInput.Normal ? textField.echoMode : TextInput.Normal;
    }
  }

  FontMetrics {
    id: fontMetrics
    font: Theme.defaultFont
  }
}
