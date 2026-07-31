import QtQuick
import QtQuick.Controls
import org.qfield

/**
 * \ingroup qml
 */
TextField {
  id: textField

  property string suffixText: ""

  echoMode: TextInput.Normal
  passwordMaskDelay: Qt.platform.os === "ios" || Qt.platform.os === "android" ? 1000 : 0
  font: Theme.defaultFont
  rightPadding: showPasswordButton.visible ? showPasswordButton.width : (suffixLabel.visible ? suffixLabel.implicitWidth + 16 : leftPadding)
  inputMethodHints: Qt.ImhNone

  Label {
    id: suffixLabel
    visible: textField.suffixText !== ""
    text: textField.suffixText
    anchors.right: parent.right
    anchors.rightMargin: 10
    anchors.verticalCenter: parent.verticalCenter
    font: parent.font
    color: enabled ? Theme.secondaryTextColor : Theme.mainTextDisabledColor
  }

  QfToolButton {
    id: showPasswordButton
    property int originalEchoMode: TextInput.Normal
    z: 1
    visible: (!!textField.echoMode && textField.echoMode !== TextInput.Normal) || originalEchoMode !== TextInput.Normal
    iconSource: textField.echoMode === TextInput.Normal ? Theme.getThemeVectorIcon('ic_hide_green_48dp') : Theme.getThemeVectorIcon('ic_show_green_48dp')
    iconColor: textField.enabled ? Theme.mainTextColor : Theme.mainTextDisabledColor
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
