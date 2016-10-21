import QtQuick 2.5
import org.qgis 1.0
import org.qfield 1.0

import QtPositioning 5.3

Item {
  id: locator
  property MapSettings mapSettings
  property color color: "#263238"
  property color highlightColor: "#CFD8DC"

  property point sourcePoint: Qt.point( width / 2, height / 2 ) // In screen coordinates
  property alias currentLayer: snappingUtils.currentLayer

  readonly property alias snappingResult: snappingUtils.snappingResult
  readonly property alias snappedCoordinate: snappingUtils.snappedCoordinate // In map coordinates, derived from snappinResult
  readonly property alias snappedPoint: snappingUtils.snappedPoint // In screen coordinates, derived from snappinResult

  SnappingUtils {
    id: snappingUtils

    mapSettings: locator.mapSettings
    inputCoordinate: sourcePoint
    config: qgisProject.snappingConfig

    property point snappedCoordinate
    property point snappedPoint

    onSnappingResultChanged: {
      if ( snappingResult.isValid )
      {
        snappedCoordinate = Qt.point( snappingResult.point.x, snappingResult.point.y )
        snappedPoint = mapSettings.coordinateToScreen( snappedCoordinate )
      }
      else
      {
        snappedPoint = sourcePoint
        snappedCoordinate = mapSettings.screenToCoordinate( snappedPoint )
      }
    }
  }

  Rectangle {
    id: crosshairCircleInnerBuffer
    anchors.centerIn: crosshairCircle

    width: crosshairCircle.width - dp
    height: crosshairCircle.height - dp

    border.color: highlightColor
    border.width: 2 * dp
    color: "transparent"
    radius: width / 2
  }

  Rectangle {
    id: crosshairCircle

    x: snappedPoint.x - radius
    y: snappedPoint.y - radius

    Behavior on x {
      NumberAnimation { duration: 100 }
    }

    Behavior on y {
      NumberAnimation { duration: 100 }
    }

    border.color: parent.color

    Behavior on border.color {
      ColorAnimation {
        duration: 200
      }
    }

    Behavior on width {
      SmoothedAnimation { duration: 2000 }
    }

    Connections {
      target: snappingUtils
      onSnappingResultChanged: {
        crosshairCircle.border.color = snappingResult.isValid ? "#9b59b6" : locator.color
        crosshairCircle.width = snappingResult.isValid ? 32 * dp : 48 * dp
      }
    }

    border.width: 1.2 * dp
    color: "transparent"
    antialiasing: true

    width: 48 * dp
    height: width
    radius: width / 2

    Rectangle {
      anchors.centerIn: parent

      color: parent.border.color

      width: 1.2 * dp
      height: parent.height * 4 / 6
    }

    Rectangle {
      anchors.centerIn: parent

      color: parent.border.color

      width: parent.width * 4 / 6
      height: 1.2 * dp
    }
  }

  ParallelAnimation {
    id: flashAnimation

    SequentialAnimation {
      ScaleAnimator {
        target: crosshairCircle
        from: 1
        to: 0.7
        duration: 150

        easing.type: Easing.InOutQuad
      }
      ScaleAnimator {
        target: crosshairCircle
        from: 0.7
        to: 1
        duration: 150

        easing.type: Easing.InOutCubic
      }
    }

    SequentialAnimation {
      ScaleAnimator {
        target: crosshairCircleInnerBuffer
        from: 1
        to: 0.7
        duration: 150

        easing.type: Easing.InOutQuad
      }
      ScaleAnimator {
        target: crosshairCircleInnerBuffer
        from: 0.7
        to: 1
        duration: 150

        easing.type: Easing.InOutCubic
      }
    }
  }

  function flash()
  {
    flashAnimation.start()
  }
}
