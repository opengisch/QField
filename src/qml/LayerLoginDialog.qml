import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield
import Theme

/**
 * \ingroup qml
 */
Page {
  signal enter(string usr, string pw)
  signal cancel

  property string credentialTitle
  property var inCancelation

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
    ScrollBar.vertical: QfScrollBar {
    }

    ColumnLayout {
      id: content
      width: parent.width
      spacing: 10

      Image {
        Layout.alignment: Qt.AlignHCenter
        Layout.topMargin: 30
        Layout.bottomMargin: 10
        source: Theme.getThemeVectorIcon('ic_password_48dp')
        sourceSize.width: Math.min(64, parent.width / 5)
        sourceSize.height: Math.min(64, parent.width / 5)
      }

      Text {
        text: credentialTitle
        Layout.fillWidth: true
        Layout.bottomMargin: 10
        horizontalAlignment: Text.AlignHCenter
        wrapMode: Text.WordWrap
        font: Theme.defaultFont
        color: Theme.mainTextColor
        padding: 16
      }

      TextField {
        id: usernameField
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        Layout.preferredWidth: parent.width - showPasswordButton.width * 2
        inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase | Qt.ImhPreferLowercase
        horizontalAlignment: Text.AlignHCenter
        placeholderText: qsTr("Username")
      }

      TextField {
        id: passwordField
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        Layout.preferredWidth: parent.width - showPasswordButton.width * 2
        Layout.bottomMargin: 10
        echoMode: TextInput.Password
        passwordMaskDelay: 1000
        inputMethodHints: Qt.ImhHiddenText | Qt.ImhNoPredictiveText | Qt.ImhSensitiveData | Qt.ImhNoAutoUppercase | Qt.ImhPreferLowercase
        horizontalAlignment: Text.AlignHCenter
        placeholderText: qsTr("Password")

        Keys.onReturnPressed: _processAuth()
        Keys.onEnterPressed: _processAuth()

        QfToolButton {
          id: showPasswordButton

          property var linkedField: passwordField
          property int originalEchoMode: TextInput.Normal

          visible: (!!linkedField.echoMode && linkedField.echoMode !== TextInput.Normal) || originalEchoMode !== TextInput.Normal
          iconSource: linkedField.echoMode === TextInput.Normal ? Theme.getThemeVectorIcon('ic_hide_green_48dp') : Theme.getThemeVectorIcon('ic_show_green_48dp')
          iconColor: Theme.mainColor
          anchors.left: linkedField.right
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
        text: "Submit"
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
