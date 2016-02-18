import QtQuick 2.0

Item {
  id: _button
  height: dp*48
  width: dp*48

  property alias icon: icon.source

  signal clicked
  signal pressAndHold

  Rectangle {
    anchors.fill: parent
    color: "#bb555555"
    border.color: "#dddddd"
    border.width: 1
  }

  MouseArea {
    anchors.fill: parent

    onClicked: _button.clicked()
    onPressAndHold: _button.pressAndHold()
  }

  Image {
    id: icon
    anchors.centerIn: parent
    width: 42
    height: 42
  }
}

