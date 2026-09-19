

# File QfProgressRing.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfProgressRing.qml**](QfProgressRing_8qml.md)

[Go to the documentation of this file](QfProgressRing_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Shapes
import org.qfield.core
import org.qfield.gui

ProgressBar {
  id: control

  property int size: 56
  property real strokeWidth: 6
  property color color: QfTheme.mainColor
  property color backgroundColor: QfTheme.lightGray

  implicitWidth: control.size
  implicitHeight: control.size

  background: Shape {
    anchors.fill: parent
    ShapePath {
      strokeWidth: control.strokeWidth
      strokeColor: control.backgroundColor
      fillColor: "transparent"

      PathAngleArc {
        centerX: control.width / 2
        centerY: control.height / 2
        radiusX: control.width / 2 - control.strokeWidth / 2
        radiusY: radiusX
        startAngle: 0
        sweepAngle: 360
      }
    }
  }

  contentItem: Shape {
    anchors.fill: parent
    ShapePath {
      strokeWidth: control.strokeWidth
      strokeColor: control.color
      fillColor: "transparent"
      capStyle: ShapePath.RoundCap

      PathAngleArc {
        centerX: control.width / 2
        centerY: control.height / 2
        radiusX: control.width / 2 - control.strokeWidth / 2
        radiusY: radiusX
        startAngle: -90
        sweepAngle: control.visualPosition * 360

        Behavior on sweepAngle {
          PropertyAnimation {
            easing.type: Easing.OutQuart
            duration: 500
          }
        }
      }
    }
  }
}
```


