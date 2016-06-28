import QtQuick.Controls 1.4 as Controls
import QtQuick.Controls.Styles 1.4
import QtQuick 2.5

Item {
  id: item

  property alias iconSource: button.iconSource
  property alias checked: button.checked
  property alias checkable: button.checkable
  property bool round: false
  property alias text: button.text

  property color bgcolor: "#212121"
  property color borderColor: "#EEEEEE"

  signal pressAndHold
  signal clicked

  height: 48 * dp
  width: 48 * dp

  Controls.Button {
    id: button
    anchors.fill: parent

    style: ButtonStyle {
      background: Rectangle {
        implicitWidth: 100
        implicitHeight: 25
        border.width: !round
        border.color: item.borderColor
        color: bgcolor
        radius: round ? height / 2 : 0
      }
    }

    MouseArea {
      anchors.fill: parent

      onClicked: item.clicked()
      onPressAndHold: item.pressAndHold()
    }
  }
}
