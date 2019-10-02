import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Controls 1.4 as Controls
import QtQuick.Layouts 1.3

import org.qfield 1.0
import Theme 1.0
import "."

Page {
  signal enter( string usr, string pw )
  signal cancel()

  property var realm
  property var inCancelation

  header: ToolBar {
    id: toolbar
    height: 48 * dp
    visible: true

    anchors {
      top: parent.top
      left: parent.left
      right: parent.right
    }

    background: Rectangle {
      color: Theme.mainColor
    }

    RowLayout {
      anchors.fill: parent
      Layout.margins: 0

      Button {
        id: enterButton

        Layout.alignment: Qt.AlignTop | Qt.AlignLeft

        visible: form.state === 'Add' || form.state === 'Edit'
        width: 48*dp
        height: 48*dp
        clip: true
        bgcolor: Theme.darkGray

        iconSource: Theme.getThemeIcon( 'ic_check_white_48dp' )

        onClicked: {
          enter(username.text, password.text)
          username.text=''
          password.text=''
        }
      }

      Label {
        id: titleLabel

        text: "Login information"
        font: Theme.strongFont
        color: "#FFFFFF"
        elide: Label.ElideRight
        horizontalAlignment: Qt.AlignHCenter
        verticalAlignment: Qt.AlignVCenter
        Layout.fillWidth: true
      }

      Button {
        id: cancelButton

        Layout.alignment: Qt.AlignTop | Qt.AlignRight

        width: 49*dp
        height: 48*dp
        clip: true
        bgcolor: form.state === 'Add' ? "#900000" : Theme.darkGray

        iconSource: Theme.getThemeIcon( 'ic_close_white_24dp' )

        onClicked: {
          cancel()
        }
      }
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
      Layout.preferredHeight: font.height
      text: qsTr( "Username" )
      font: Theme.defaultFont
    }

    TextField {
      id: username
      Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
      Layout.preferredWidth: Math.max( parent.width / 2, usernamelabel.width )
      Layout.preferredHeight: font.height + 20 * dp
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
      Layout.preferredHeight: font.height
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
