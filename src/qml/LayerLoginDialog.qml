import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import org.qfield 1.0
import Theme 1.0

Page {
  signal enter( string usr, string pw )
  signal cancel()

  property string realm
  property var inCancelation

    header: PageHeader {
        title: qsTr("Login information")

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
      Layout.preferredHeight: font.height + 20
      text: realm
      font: Theme.strongFont
    }

    Text {
      id: usernamelabel
      Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
      text: qsTr( "Username" )
      font: Theme.defaultFont
    }

    QfTextField {
      id: username
      Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
      Layout.preferredWidth: Math.max( parent.width / 2, usernamelabel.width )
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
    }

    QfTextField {
      id: password
      Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
      Layout.preferredWidth: Math.max( parent.width / 2, usernamelabel.width )
      echoMode: TextInput.Password
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
