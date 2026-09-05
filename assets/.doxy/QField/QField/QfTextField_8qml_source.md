

# File QfTextField.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfTextField.qml**](QfTextField_8qml.md)

[Go to the documentation of this file](QfTextField_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import org.qfield.core
import org.qfield.gui

TextField {
  id: textField

  property string suffixText: ""

  echoMode: TextInput.Normal
  passwordMaskDelay: Qt.platform.os === "ios" || Qt.platform.os === "android" ? 1000 : 0
  font: QfTheme.defaultFont
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
    color: enabled ? QfTheme.secondaryTextColor : QfTheme.mainTextDisabledColor
  }

  QfToolButton {
    id: showPasswordButton
    property int originalEchoMode: TextInput.Normal
    z: 1
    visible: (!!textField.echoMode && textField.echoMode !== TextInput.Normal) || originalEchoMode !== TextInput.Normal
    iconSource: textField.echoMode === TextInput.Normal ? QfTheme.getThemeVectorIcon('ic_hide_green_48dp') : QfTheme.getThemeVectorIcon('ic_show_green_48dp')
    iconColor: textField.enabled ? QfTheme.mainTextColor : QfTheme.mainTextDisabledColor
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
    font: QfTheme.defaultFont
  }
}
```


