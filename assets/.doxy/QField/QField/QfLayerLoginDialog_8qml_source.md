

# File QfLayerLoginDialog.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfLayerLoginDialog.qml**](QfLayerLoginDialog_8qml.md)

[Go to the documentation of this file](QfLayerLoginDialog_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield.core
import org.qfield.gui

Page {
  signal enter(string usr, string pw)
  signal cancel

  property string credentialTitle
  property var inCancelation

  padding: 5

  header: QfPageHeader {
    title: qsTr("Credentials Details")
    showBackButton: false
    showApplyButton: false
    showCancelButton: true
    onCancel: {
      parent.cancel();
    }
  }

  Flickable {
    id: flickable
    anchors.fill: parent
    anchors.margins: 20
    Layout.fillWidth: true
    Layout.fillHeight: true
    contentHeight: content.height
    clip: true
    ScrollBar.vertical: QfScrollBar {}

    ColumnLayout {
      id: content
      width: parent.width
      spacing: 10

      Image {
        Layout.alignment: Qt.AlignHCenter
        Layout.topMargin: 30
        Layout.bottomMargin: 10
        source: QfTheme.getThemeVectorIcon('ic_password_48dp')
        sourceSize.width: Math.min(64, parent.width / 5)
        sourceSize.height: Math.min(64, parent.width / 5)
      }

      Text {
        text: credentialTitle
        Layout.fillWidth: true
        Layout.bottomMargin: 10
        horizontalAlignment: Text.AlignHCenter
        wrapMode: Text.WordWrap
        font: QfTheme.defaultFont
        color: QfTheme.mainTextColor
        padding: 16
      }

      TextField {
        id: usernameField
        Layout.fillWidth: true
        inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase | Qt.ImhPreferLowercase
        horizontalAlignment: Text.AlignLeft
        placeholderText: qsTr("Username")
      }

      TextField {
        id: passwordField
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        Layout.fillWidth: true
        Layout.bottomMargin: 10
        rightPadding: 50
        echoMode: TextInput.Password
        passwordMaskDelay: Qt.platform.os === "ios" || Qt.platform.os === "android" ? 1000 : 0
        inputMethodHints: Qt.ImhHiddenText | Qt.ImhNoPredictiveText | Qt.ImhSensitiveData | Qt.ImhNoAutoUppercase | Qt.ImhPreferLowercase
        horizontalAlignment: Text.AlignLeft
        placeholderText: qsTr("Password")

        Keys.onReturnPressed: _processAuth()
        Keys.onEnterPressed: _processAuth()

        QfToolButton {
          id: showPasswordButton

          property var linkedField: passwordField
          property int originalEchoMode: TextInput.Normal

          visible: (!!linkedField.echoMode && linkedField.echoMode !== TextInput.Normal) || originalEchoMode !== TextInput.Normal
          iconSource: linkedField.echoMode === TextInput.Normal ? QfTheme.getThemeVectorIcon('ic_hide_green_48dp') : QfTheme.getThemeVectorIcon('ic_show_green_48dp')
          iconColor: QfTheme.mainColor
          anchors.right: linkedField.right
          anchors.verticalCenter: linkedField.verticalCenter
          opacity: linkedField.text.length > 0 ? 1 : 0.25

          onClicked: {
            if (linkedField.echoMode !== TextInput.Normal) {
              originalEchoMode = linkedField.echoMode;
              linkedField.echoMode = TextInput.Normal;
            } else {
              linkedField.echoMode = originalEchoMode;
            }
          }
        }
      }

      QfButton {
        id: submit
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        Layout.fillWidth: true
        text: qsTr("Submit")
        onClicked: _processAuth()
      }

      Item {
        // spacer item
        Layout.fillWidth: true
        Layout.fillHeight: true
      }
    }
  }

  onVisibleChanged: {
    if (visible) {
      usernameField.forceActiveFocus();
    }
  }

  function _processAuth() {
    enter(usernameField.text, passwordField.text);
    usernameField.text = '';
    passwordField.text = '';
  }
}
```


