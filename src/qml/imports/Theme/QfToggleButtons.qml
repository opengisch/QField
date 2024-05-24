import QtQuick

Rectangle {
  width: 200
  height: 200
  color: "green"

  Flow{
    anchors.fill: parent
    spacing: 8
    Repeater{
      model: 20

      Rectangle{
        width: index * 5
        height: 20
        color: "red"
      }
    }
  }
}
