import QtQuick 2.0
import QtQuick.Controls 1.4 as Controls
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.4

import org.qfield 1.0
import "js/style.js" as Style
import "." as QField

Item {
  signal enter( string usr, string pw )
  signal cancel()

  property var realm
  property var inCancelation

  Rectangle {
    id: rectangle
    anchors.fill: parent
    color: "lightgray"

    ColumnLayout{
      anchors.fill: parent
      Layout.fillWidth: true
      Layout.fillHeight: true

      spacing: 2
      anchors { margins: 4 * dp }

      Text {
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        Layout.preferredHeight: font.height + 20 * dp
        text: realm
        font.pointSize: 25
        font.bold: true
      }

      Text {
        id: usernamelabel
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        Layout.preferredHeight: font.height + 20 * dp
        text: qsTr( "Username" )
        font.pointSize: 12
      }

      TextField {
        id: username
        background: Rectangle {
            color: 'whitesmoke'
            border.color: 'grey'
            width: parent.width + 20 * dp
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
        }
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        Layout.preferredWidth: Math.max( parent.width / 2, usernamelabel.width )
        Layout.preferredHeight: font.height + 20 * dp
        font.pointSize: 12
      }

      Text {
        id: passwordlabel
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        Layout.preferredHeight: font.height + 20 * dp
        text: qsTr( "Password" )
        font.pointSize: 12
      }

      TextField {
        id: password
        background: Rectangle {
            color: 'whitesmoke'
            border.color: 'grey'
            width: parent.width + 20 * dp
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
        }
        echoMode: TextInput.Password
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        Layout.preferredWidth: Math.max( parent.width / 2, usernamelabel.width )
        Layout.preferredHeight: font.height + 20 * dp
        height: font.height + 20 * dp
        font.pointSize: 12
      }

      RowLayout{
        spacing: 2
        anchors { margins: 4 * dp }
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignVCenter

        QField.Button {
          id: cancelButton

          Layout.fillWidth: true
          height: 48 * dp

          text: "Cancel"
          bgcolor: "steelblue"
          borderColor: "white"

          onClicked: {
            cancel()
          }
        }

        QField.Button {
          id: enterButton

          Layout.fillWidth: true
          height: 48 * dp

          text: "Ok"
          bgcolor: "steelblue"
          borderColor: "white"

          onClicked: {
            enter(username.text, password.text)
            username.text=''
            password.text=''
          }
        }
      }
    }
  }
}
