

# File QfTextField.qml

[**File List**](files.md) **>** [**imports**](dir_3be62dd4600925273911e91e0c7964f3.md) **>** [**Theme**](dir_1633596792308d5fdfbf00fb99c556ce.md) **>** [**QfTextField.qml**](QfTextField_8qml.md)

[Go to the documentation of this file](QfTextField_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import org.qfield

TextField {
  id: textField
  echoMode: TextInput.Normal
  passwordMaskDelay: Qt.platform.os === "ios" || Qt.platform.os === "android" ? 1000 : 0
  font: Theme.defaultFont
  rightPadding: showPasswordButton.visible ? showPasswordButton.width : leftPadding
  inputMethodHints: Qt.ImhNone

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
```


