import QtQuick 2.0
import QtQuick.Controls 2.12

Dial {
  id: control

  property string source: "qrc:/images/qfield_logo.svg"
  property int rotationOffset: 0

  background: Rectangle {
    x: control.width / 2 - width / 2
    y: control.height / 2 - height / 2
    width: Math.max(64, Math.min(control.width, control.height))
    height: width
    color: "transparent"
    opacity: control.enabled ? 1 : 0.3
  }

  handle: Image {
    id: handleItem
    fillMode: Image.PreserveAspectFit
    smooth: true
    source: control.source
    sourceSize.width: parent.width
    sourceSize.height: parent.height

    x: control.background.x + control.background.width / 2 - width / 2
    y: control.background.y + control.background.height / 2 - height / 2

    opacity: control.enabled ? 1 : 0.3
    transform: [
      Rotation {
        angle: control.angle + control.rotationOffset
        origin.x: handleItem.width / 2
        origin.y: handleItem.height / 2
      }
    ]
  }
}
