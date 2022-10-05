import QtQuick 2.14
import QtQuick.Controls 2.14

Item {
  id: nyukiContainer

  state: "hidden"
  states: [
    State {
      name: "shown"
      PropertyChanges {
        target: nyukiContainer
        anchors.bottomMargin: -80
      }
    },
    State {
      name: "hidden"
      PropertyChanges {
        target: nyukiContainer
        anchors.bottomMargin: -200
      }
    }
  ]
  transitions: Transition {
    from: "*"
    to: "*"
    NumberAnimation { target: nyukiContainer; property: "anchors.bottomMargin"; duration: 1000; easing.type: Easing.InOutQuad }
  }

  Image {
    anchors.fill: parent
    width: parent.width
    height: parent.height
    fillMode: Image.PreserveAspectFit
    smooth: true
    opacity: 1
    source: "qrc:/images/qfieldcloud_logo.svg"
    sourceSize.width: 1024
    sourceSize.height: 1024

    Rectangle {
        width: 12
        height: 12
        color: "white"
        x: 83
        y: 83
        radius: 5
    }
    Rectangle {
        width: 12
        height: 12
        color: "white"
        x: 105
        y: 83
        radius: 5
    }
    Rectangle {
      id: nyukiLeft
      width: 10
      height: 10
      color: "#4a6fae"
      x: 84
      y: 84
      radius: 5
      rotation: 0
      transformOrigin: Item.TopLeft
      SequentialAnimation {
        PauseAnimation { duration: 1000 }
        NumberAnimation {
          target:  nyukiLeft
          property: "rotation"
          to: 359
          duration: 2000
          easing.type: Easing.InOutQuad
        }
        running: nyuki.state == "shown"
        loops: Animation.Infinite
      }
    }
    Rectangle {
      id: nyukiRight
      width: 10
      height: 10
      color: "#4a6fae"
      x: 106
      y: 84
      radius: 5
      rotation: 0
      transformOrigin: Item.TopRight
      SequentialAnimation {
        PauseAnimation { duration: 1000 }
        NumberAnimation {
          target:  nyukiRight
          property: "rotation"
          to: -359
          duration: 2000
          easing.type: Easing.InOutQuad
        }
        running: nyuki.state == "shown"
        loops: Animation.Infinite
      }
    }
  }
}

