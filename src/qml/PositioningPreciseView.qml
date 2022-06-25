import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Shapes 1.12
import QtGraphicalEffects 1.12
import Qt.labs.calendar 1.0

import Theme 1.0
import org.qfield 1.0

import "."

Item {
  id: positioningPreciseView

  property double precision: 1
  property bool hasZ: !isNaN(navigation.verticalDistance)

  property double positionX: navigation.distance * Math.cos((navigation.bearing - 90) * Math.PI / 180) * (preciseTarget.width / 2) / precision
  property double positionY: navigation.distance * Math.sin((navigation.bearing - 90) * Math.PI / 180) * (preciseTarget.width / 2) / precision
  property double positionZ: hasZ ? navigation.verticalDistance * (preciseElevation.height / 2) / precision : 0.0

  property string negativeLabel: UnitTypes.formatDistance(-precision, 1, navigation.distanceUnits)
  property string positiveLabel: UnitTypes.formatDistance(precision, 1, navigation.distanceUnits)

  property color positionColor: Theme.positionColor
  SequentialAnimation on positionColor  {
    loops: Animation.Infinite
    ColorAnimation  { from: Theme.positionColor; to: Theme.darkPositionColor; duration: 2000; easing.type: Easing.InOutQuad }
    ColorAnimation  { from: Theme.darkPositionColor; to: Theme.positionColor; duration: 1000; easing.type: Easing.InOutQuad }
  }

  Rectangle {
    anchors.fill: parent
    color: "white"
  }

  Row {
    anchors.centerIn: parent
    anchors.margins: 10
    spacing: 5

    Rectangle {
      width: labelTarget.contentWidth
      height: preciseElevation.height
      Text {
        id: labelTarget
        anchors.centerIn: parent
        font: Theme.tinyFont
        color: Theme.navigationColor
        text: positiveLabel
      }
    }

    Shape {
      id: preciseTarget
      width: Math.min(positioningPreciseView.height - 10,
                      positioningPreciseView.width - preciseElevation.width - labelTarget.contentWidth - labelElevation.width - 20)
      height: width
      rotation: locationMarker.compassHasValue ? -locationMarker.compassOrientation : 0

      ShapePath {
        strokeWidth: 1
        strokeColor: Theme.navigationColorSemiOpaque
        strokeStyle: ShapePath.SolidLine
        fillColor: "transparent"
        startX: preciseTarget.width / 2
        startY: 0
        PathLine {
          x: preciseTarget.width / 2
          y: preciseTarget.height
        }
      }
      ShapePath {
        strokeWidth: 1
        strokeColor: Theme.navigationColorSemiOpaque
        strokeStyle: ShapePath.SolidLine
        fillColor: "transparent"
        startX: 0
        startY: preciseTarget.height / 2
        PathLine {
          x: preciseTarget.width
          y: preciseTarget.height / 2
        }
      }
      ShapePath {
        strokeWidth: 1
        strokeColor: Theme.navigationBackgroundColor
        strokeStyle: ShapePath.SolidLine
        fillColor: "transparent"
        startX: 0
        startY: 0
        PathAngleArc {
          centerX: preciseTarget.width / 2
          centerY: centerX
          radiusX: preciseTarget.width / 8
          radiusY:radiusX
          startAngle: 0
          sweepAngle: 360
        }
      }
      ShapePath {
        strokeWidth: 1
        strokeColor: Theme.navigationBackgroundColor
        strokeStyle: ShapePath.SolidLine
        fillColor: "transparent"
        startX: 0
        startY: 0
        PathAngleArc {
          centerX: preciseTarget.width / 2
          centerY: centerX
          radiusX: preciseTarget.width / 4
          radiusY:radiusX
          startAngle: 0
          sweepAngle: 360
        }
      }
      ShapePath {
        strokeWidth: 1
        strokeColor: Theme.navigationBackgroundColor
        strokeStyle: ShapePath.SolidLine
        fillColor: "transparent"
        startX: 0
        startY: 0
        PathAngleArc {
          centerX: preciseTarget.width / 2
          centerY: centerX
          radiusX: preciseTarget.width / 2.66
          radiusY:radiusX
          startAngle: 0
          sweepAngle: 360
        }
      }
      ShapePath {
        strokeWidth: 1
        strokeColor: "#000000"
        strokeStyle: ShapePath.SolidLine
        fillColor: "transparent"
        startX: 0
        startY: 0
        PathAngleArc {
          centerX: preciseTarget.width / 2
          centerY: centerX
          radiusX: preciseTarget.width / 2
          radiusY: radiusX
          startAngle: 0
          sweepAngle: 360
        }
      }

      Rectangle {
        id: preciseHorizontalPosition
        x: (preciseTarget.width - width) / 2 + positionX
        y: (preciseTarget.width - width)  / 2 + positionY
        width: 28
        height: width
        radius: width / 2
        color: positionColor
      }
    }

    Rectangle {
      id: labelElevation
      width: Math.max(labelElevationTop.contentWidth, labelElevationBottom.contentWidth)
      height: preciseElevation.height
      opacity: hasZ ? 1 : 0.25
      color: "transparent"

      Text {
        id: labelElevationTop
        anchors.top: parent.top
        anchors.right: parent.right
        font: Theme.tinyFont
        color: hasZ ? Theme.navigationColor : "#000000"
        text: positiveLabel
      }
      Text {
        id: labelElevationBottom
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        font: Theme.tinyFont
        color: hasZ ? Theme.navigationColor : "#000000"
        text: negativeLabel
      }
    }

    Rectangle {
      id: preciseElevation
      width: 14
      height: preciseTarget.height
      radius: 7
      opacity: hasZ ? 1 : 0.25
      color: "transparent"
      border.color: "#000000"
      border.width: 1

      Rectangle {
        x: 1
        y: parent.height / 2
        width: parent.width - 2
        height: 1
        color: hasZ ? Theme.navigationColorSemiOpaque : "#000000"
      }

      Rectangle {
        id: preciseVerticalPosition
        x: -1
        y: (preciseElevation.height - width) / 2 + positionZ
        width: preciseElevation.width + 2
        height: width
        radius: width / 2
        opacity: hasZ ? 1 : 0
        color: hasZ ? positionColor : Theme.gray
      }
    }
  }
}
