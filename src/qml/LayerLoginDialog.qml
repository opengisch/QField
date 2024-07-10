import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import org.qfield 1.0
import Theme 1.0

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
    Layout.fillWidth: true
    Layout.fillHeight: true
    contentHeight: content.height
    clip: true

    ScrollBar.vertical: ScrollBar {
      width: 8
      policy: height < flickable.contentHeight ? ScrollBar.AlwaysOn : ScrollBar.AsNeeded
    }

    ColumnLayout {
      id: content
      width: parent.width
      spacing: 2
      anchors {
        margins: 4
        topMargin: 52 // Leave space for the toolbar
      }

      Item {
        // spacer item
        height: 24
      }

      Image {
        Layout.alignment: Qt.AlignHCenter
        source: Theme.getThemeVectorIcon('ic_password_48dp')
        sourceSize.width: Math.min(64, parent.width / 5)
        sourceSize.height: Math.min(64, parent.width / 5)
      }

      Item {
        // spacer item
        height: 8
      }

      Text {
        text: credentialTitle
        horizontalAlignment: Text.AlignHCenter
        Layout.fillWidth: true
        wrapMode: Text.WordWrap
        font: Theme.defaultFont
        color: Theme.mainTextColor
        padding: 16
      }

      Item {
        // spacer item
        height: 35
      }

      Text {
        id: usernamelabel
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        text: qsTr("Username")
        font: Theme.defaultFont
        color: Theme.mainTextColor
      }

      QfTextField {
        id: username
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        Layout.preferredWidth: Math.max(parent.width / 2, usernamelabel.width)
        inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase | Qt.ImhPreferLowercase
        horizontalAlignment: Text.AlignHCenter
      }

      Item {
        // spacer item
        height: 35
      }

      Text {
        id: passwordlabel
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        text: qsTr("Password")
        font: Theme.defaultFont
        color: Theme.mainTextColor
      }

      QfTextField {
        id: password
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        Layout.preferredWidth: Math.max(parent.width / 2, usernamelabel.width)
        echoMode: TextInput.Password
        inputMethodHints: Qt.ImhHiddenText | Qt.ImhNoPredictiveText | Qt.ImhSensitiveData | Qt.ImhNoAutoUppercase | Qt.ImhPreferLowercase
        horizontalAlignment: Text.AlignHCenter

        Keys.onReturnPressed: _processAuth()
        Keys.onEnterPressed: _processAuth()
      }

      Item {
        // spacer item
        height: 35
      }

      QfButton {
        id: submit
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        Layout.preferredWidth: Math.max(parent.width / 2, usernamelabel.width)
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
      username.forceActiveFocus();
    }
  }

  function _processAuth() {
    enter(username.text, password.text);
    username.text = '';
    password.text = '';
  }
}
