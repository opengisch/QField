import QtQuick
import QtQuick.Controls
import QtQuick.Shapes
import org.qfield.core
import org.qfield.gui

/**
 * \ingroup qml_gui
 */
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
    preferredRendererType: Shape.CurveRenderer
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
    preferredRendererType: Shape.CurveRenderer
    ShapePath {
      id: progressPath

      property real indeterminateRotation: 0

      strokeWidth: control.strokeWidth
      strokeColor: control.color
      fillColor: "transparent"
      capStyle: ShapePath.RoundCap

      NumberAnimation on indeterminateRotation {
        running: control.indeterminate && control.visible
        from: 0
        to: 360
        duration: 1000
        loops: Animation.Infinite
      }

      PathAngleArc {
        centerX: control.width / 2
        centerY: control.height / 2
        radiusX: control.width / 2 - control.strokeWidth / 2
        radiusY: radiusX
        startAngle: control.indeterminate ? progressPath.indeterminateRotation - 90 : -90
        sweepAngle: control.indeterminate ? 90 : control.visualPosition * 360

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
