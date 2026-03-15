

# File QfProgrerssRing.qml

[**File List**](files.md) **>** [**imports**](dir_3be62dd4600925273911e91e0c7964f3.md) **>** [**Theme**](dir_1633596792308d5fdfbf00fb99c556ce.md) **>** [**QfProgrerssRing.qml**](QfProgrerssRing_8qml.md)

[Go to the documentation of this file](QfProgrerssRing_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Shapes
import org.qfield

ProgressBar {
  id: control

  property int size: 56
  property real strokeWidth: 6
  property color color: Theme.mainColor
  property color backgroundColor: Theme.lightGray

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
      }
    }
  }
}
```


