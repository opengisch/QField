import QtQuick.Controls 1.4 as Controls
import QtQuick.Controls.Styles 1.4
import QtQuick 2.5

Item {
  id: item

  property alias iconSource: button.iconSource
  property alias checked: button.checked
  property alias checkable: button.checkable

  signal pressAndHold
  signal clicked

  height: dp*48
  width: dp*48

  Controls.Button {
    id: button
    anchors.fill: parent

    style: ButtonStyle {
      background: Rectangle {
        implicitWidth: 100
        implicitHeight: 25
        border.width: control.activeFocus ? 2 : 1
        border.color: "#888"
        color: "#cc555555"
      }
    }

    MouseArea {
      anchors.fill: parent

      onClicked: item.clicked()
      onPressAndHold: item.pressAndHold()
    }
  }
}
