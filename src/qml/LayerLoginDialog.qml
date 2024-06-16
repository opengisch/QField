import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import org.qfield 1.0
import Theme 1.0

Page {
  signal enter( string usr, string pw )
  signal cancel()

  property string credentialTitle
  property var inCancelation

    header: QfPageHeader {
        title: qsTr("Credentials Details")

        showBackButton: false
        showApplyButton: true
        showCancelButton: true

        onApply: {
          parent.enter(username.text, password.text)
          username.text=''
          password.text=''
        }
        onCancel: {
          parent.cancel()
        }
      }

  ColumnLayout{
    anchors.fill: parent
    Layout.fillWidth: true
    Layout.fillHeight: true

    spacing: 2
    anchors {
        margins: 4
        topMargin: 52// Leave space for the toolbar
    }

    Text {
      Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
      Layout.preferredHeight: font.height + 50
      horizontalAlignment: Text.AlignHCenter
      Layout.fillWidth: true
      wrapMode: Text.WordWrap
      text: credentialTitle
      font: Theme.strongFont
      color: Theme.mainTextColor
    }

    Item {
        // spacer item
        height: 35
    }

    Text {
      id: usernamelabel
      Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
      text: qsTr( "Username" )
      font: Theme.defaultFont
      color: Theme.mainTextColor
    }

    QfTextField {
      id: username
      Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
      Layout.preferredWidth: Math.max( parent.width / 2, usernamelabel.width )
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
      text: qsTr( "Password" )
      font: Theme.defaultFont
      color: Theme.mainTextColor
    }

    QfTextField {
      id: password
      Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
      Layout.preferredWidth: Math.max( parent.width / 2, usernamelabel.width )
      echoMode: TextInput.Password
      inputMethodHints: Qt.ImhHiddenText | Qt.ImhNoPredictiveText | Qt.ImhSensitiveData | Qt.ImhNoAutoUppercase | Qt.ImhPreferLowercase
      horizontalAlignment: Text.AlignHCenter
    }

    Item {
        // spacer item
        Layout.fillWidth: true
        Layout.fillHeight: true
    }
  }

  onVisibleChanged: {
      if (visible) {
          username.forceActiveFocus();
      }
  }
}
