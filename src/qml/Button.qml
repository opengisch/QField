import QtQuick.Controls 1.4 as Controls
import QtQuick.Controls.Styles 1.4
import QtQuick 2.5
import QtGraphicalEffects 1.0

Item {
  id: item

  property alias iconSource: button.iconSource
  property alias checked: button.checked
  property alias checkable: button.checkable
  property bool round: false
  property alias text: button.text

  property color bgcolor: "#212121"
  property color borderColor: bgcolor

  signal pressAndHold
  signal clicked

  height: 48 * dp
  width: 48 * dp

  Controls.Button {
    id: button
    anchors.fill: parent

    style: ButtonStyle {
      background: Rectangle {
        border.width: !round
        border.color: borderColor
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

  DropShadow {
    visible: round
    anchors.fill: source
    horizontalOffset: 1 * dp
    verticalOffset: 1 * dp
    radius: 4 * dp
    samples: 17
    color: "#80000000"
    source: button
  }
}
