import QtQuick
import QtQuick.Controls

TextField {
  id: textField
  echoMode: TextInput.Normal
  passwordMaskDelay: Qt.platform.os === "ios" || Qt.platform.os === "android" ? 1000 : 0
  font: Theme.defaultFont
  placeholderTextColor: Theme.accentLightColor
  rightPadding: showPasswordButton.visible ? showPasswordButton.width : 0
  leftPadding: showPasswordButton.visible && horizontalAlignment !== Text.AlignLeft ? rightPadding : 0
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
      color: textField.activeFocus ? Theme.mainColor : textField.hovered ? textField.color : Theme.secondaryTextColor
    }
  }

  QfToolButton {
    id: showPasswordButton
    property int originalEchoMode: TextInput.Normal
    z: 1
    visible: (!!textField.echoMode && textField.echoMode !== TextInput.Normal) || originalEchoMode !== TextInput.Normal
    iconSource: textField.echoMode === TextInput.Normal ? Theme.getThemeVectorIcon('ic_hide_green_48dp') : Theme.getThemeVectorIcon('ic_show_green_48dp')
    iconColor: Theme.mainColor
    anchors.right: textField.right
    anchors.verticalCenter: textField.verticalCenter
    opacity: textField.text.length > 0 ? 1 : 0.25

    onClicked: {
      if (textField.echoMode !== TextInput.Normal) {
        originalEchoMode = textField.echoMode;
        textField.echoMode = TextInput.Normal;
      } else {
        textField.echoMode = originalEchoMode;
      }
    }
  }

  FontMetrics {
    id: fontMetrics
    font: Theme.defaultFont
  }
}
