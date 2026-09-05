

# File QfImageDial.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfImageDial.qml**](QfImageDial_8qml.md)

[Go to the documentation of this file](QfImageDial_8qml.md)


```C++
import QtQuick
import QtQuick.Controls

Dial {
  id: control

  property string source: "qrc:/images/app_logo.svg"
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
```


