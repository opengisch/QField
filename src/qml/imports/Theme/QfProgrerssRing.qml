import QtQuick
import QtQuick.Controls
import QtQuick.Shapes

/**
 * \ingroup qml
 */
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
