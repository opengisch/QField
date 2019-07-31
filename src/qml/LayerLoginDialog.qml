import QtQuick 2.0
import QtQuick.Controls 1.4 as Controls
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.4

import org.qfield 1.0
import "js/style.js" as Style
import "." as QField

Item {
  signal enter( string usr, string pw )

    property var realm

  Rectangle {
    anchors.fill: parent
    color: "lightgray"
  }

  ColumnLayout{
      anchors.fill: parent
      spacing: 2
      anchors { margins: 4 * dp }

      Text {
        width: parent.width
        text: qsTr( "Login to "+realm )
        Layout.alignment: Qt.AlignHCenter
        font.pointSize: 25
      }

      Text {
        width: parent.width
        font.pointSize: 12
        text: qsTr( "Username" )
      }

      //Rectangle {
      //    height: username.height
      //    width: parent.width
      //    color: "white"
          TextField {
              id: username
              width: parent.width
              height: font.height + 20 * dp
              //text: "user_bsv0a"
              font.pointSize: 12
          }
      //}

      Text {
        width: parent.width
        text: qsTr( "Password" )
        font.pointSize: 12
      }

     // Rectangle {
     //     height: password.height
     //     width: parent.width
     //     color: "white"

          TextField {

            id: password
            width: parent.width
            height: font.height + 20 * dp
            //text:"2581qmtmq0t2schz"
            font.pointSize: 12
          }
      //}
  }

    QField.Button {
      id: enterButton
      anchors.left: parent.left
      anchors.bottom: parent.bottom

      width: parent.width
      height: 48 * dp

      text: "Aight"
      bgcolor: "steelblue"

      onClicked: {
        enter(username.text, password.text)
      }
    }
}
