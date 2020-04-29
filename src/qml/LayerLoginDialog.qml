import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import org.qfield 1.0
import Theme 1.0
import "."

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
        margins: 4 * dp
        topMargin: 52 * dp // Leave space for the toolbar
    }

    Text {
      Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
      Layout.preferredHeight: font.height + 20 * dp
      text: realm
      font: Theme.strongFont
    }

    Text {
      id: usernamelabel
      Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
      text: qsTr( "Username" )
      font: Theme.defaultFont
    }

    TextField {
      id: username
      Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
      Layout.preferredWidth: Math.max( parent.width / 2, usernamelabel.width )
      font: Theme.defaultFont

      background: Rectangle {
        y: username.height - height - username.bottomPadding / 2
        implicitWidth: 120 * dp
        height: username.activeFocus ? 2 * dp : 1 * dp
        color: username.activeFocus ? "#4CAF50" : "#C8E6C9"
      }
    }

    Item {
        // spacer item
        height: 35 * dp
    }

    Text {
      id: passwordlabel
      Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
      text: qsTr( "Password" )
      font: Theme.defaultFont
    }

    TextField {
      id: password
      echoMode: TextInput.Password
      Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
      Layout.preferredWidth: Math.max( parent.width / 2, usernamelabel.width )
      Layout.preferredHeight: font.height + 20 * dp
      height: font.height + 20 * dp
      font: Theme.defaultFont

      background: Rectangle {
        y: password.height - height - password.bottomPadding / 2
        implicitWidth: 120 * dp
        height: password.activeFocus ? 2 * dp : 1 * dp
        color: password.activeFocus ? "#4CAF50" : "#C8E6C9"
      }
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

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
