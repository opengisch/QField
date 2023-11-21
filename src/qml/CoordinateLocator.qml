import QtQuick 2.14
import QtQuick.Shapes 1.14

import org.qgis 1.0
import org.qfield 1.0
import Theme 1.0

Item {
  id: locator

  property MapSettings mapSettings
  property color mainColor: "#CFD8DC"
  property color highlightColor: "#263238"

  /**
   * Set the current layer on which snapping should be performed.
   */
  property alias currentLayer: snappingUtils.currentLayer

  /**
   * Position-related information
   */
  property var positionInformation: undefined
  property bool positionLocked: false
  property bool averagedPosition: false
  property int averagedPositionCount: 0

  /**
   * Overrides any possibility for the user to modify the coordinate.
   * There will be no user interaction or snapping if this is set to a QgsPoint.
   * Set this to `undefined` to revert to the user and snapping controlled behavior.
   */
  property variant overrideLocation: undefined // QgsPoint

  //! Source location for snapping. If this is set to undefined, the center of the screen will be used.
  //! Overwritten by stylus / hoverHandler.
  property variant sourceLocation: undefined // Screen coordinate

  readonly property variant currentCoordinate: !!overrideLocation ? overrideLocation : snappingUtils.snappedCoordinate

  // some trickery here: the first part (!mapSettings.visibleExtent) is only there to get a signal when
  // the map canvas extent changes (user pans/zooms) and the calculation of the display position is retriggered
  readonly property point displayPosition: {
    // this property gets initially evaluated before the `currentCoordinate`
    if ( ! currentCoordinate )
      currentCoordinate = !!overrideLocation ? overrideLocation : snappingUtils.snappedCoordinate

    return !!mapSettings.visibleExtent || true ? mapSettings.coordinateToScreen(currentCoordinate) : 0
  }

  readonly property alias snappedCoordinate: snappingUtils.snappedCoordinate // In map coordinates, derived from snappinResult
  readonly property alias snappedPoint: snappingUtils.snappedPoint // In screen coordinates, derived from snappinResult
  readonly property alias topSnappingResult: snappingUtils.snappingResult // The snappingResult itself, only one (compared to QGIS where it's usually a list)
  SnappingUtils {
    id: snappingUtils

    enabled: locator.visible
    mapSettings: locator.mapSettings
    inputCoordinate: sourceLocation === undefined ? Qt.point( locator.width / 2, locator.height / 2 ) : sourceLocation // In screen coordinates
    config: qgisProject ? qgisProject.snappingConfig : snappingUtils.emptySnappingConfig()

    property variant snappedCoordinate
    property point snappedPoint

    onSnappingResultChanged: {
      if ( snappingResult.isValid )
      {
        snappedCoordinate = snappingResult.point
        snappedPoint = mapSettings.coordinateToScreen( snappedCoordinate )
      }
      else
      {
        snappedPoint = inputCoordinate
        snappedCoordinate = mapSettings.screenToCoordinate( snappedPoint )
      }
    }
  }

  Rectangle {
    id: averagedInfoShield
    visible: positionLocked && averagedPosition
    anchors.left: crosshairCircle.left
    anchors.leftMargin: 4
    anchors.bottom: crosshairCircle.top
    anchors.bottomMargin: 3
    width: crosshairCircle.width - 8
    height: averagedInfo.contentHeight
    color: "#44263238"
    radius: 3
    border.color: highlightColor
    border.width: 1.2
    clip: true

    Rectangle {
      id: averagedProgress
      anchors.top: parent.top
      anchors.topMargin: 1.2
      anchors.left: parent.left
      anchors.leftMargin: 1.2
      height: parent.height - 2.4
      width: (positioningSettings.averagedPositioning
              ? Math.min(parent.width,(parent.width * (averagedPositionCount / positioningSettings.averagedPositioningMinimumCount)))
              : parent.width) - 2.4
      color: positioningSettings.accuracyIndicator
             ? !positionSource.positionInformation
               || !positionSource.positionInformation.haccValid
               || positionSource.positionInformation.hacc > positioningSettings.accuracyBad
               ? Theme.accuracyBad
               : positionSource.positionInformation.hacc > positioningSettings.accuracyExcellent
                 ? Theme.accuracyTolerated
                 : Theme.accuracyExcellent
              : Theme.positionColor

      transitions: [ Transition { NumberAnimation { property: "width"; duration: 200 } } ]
    }

    Text {
      id: averagedInfo
      anchors.centerIn: parent
      text: averagedPositionCount
      color: mainColor
      font.pointSize: Theme.tinyFont.pointSize - 2
      style: Text.Outline
      styleColor: highlightColor
    }
  }

  Shape {
    id: crosshairCircle

    property bool isSnapped: false
    property real halfWidth: width / 2
    property real arcSpacing: isSnapped ? 0 : 20

    width: isSnapped ? 32 : 48
    height: width

    x: displayPosition.x - halfWidth
    y: displayPosition.y - halfWidth

    Behavior on x {
      enabled: !overrideLocation && !sourceLocation // It looks strange if the GPS position indicator and the crosshair are not synchronized
      NumberAnimation { duration: 100 }
    }

    Behavior on y {
      enabled: !overrideLocation && !sourceLocation
      NumberAnimation { duration: 100 }
    }

    Behavior on width {
      SmoothedAnimation { duration: 2000 }
    }

    ShapePath {
      id: crosshairPathBuffer
      strokeColor: "#FFFFFF"
      strokeWidth: crosshairPath.strokeWidth + 2
      fillColor: "transparent"
    }

    ShapePath {
      id: crosshairPath
      strokeColor: overrideLocation !== undefined ? Theme.positionColor : "#000000"
      strokeWidth: 2
      fillColor: "transparent"

      PathAngleArc {
        centerX: crosshairCircle.halfWidth; centerY: crosshairCircle.halfWidth
        radiusX: crosshairCircle.halfWidth; radiusY: crosshairCircle.halfWidth
        startAngle: 0 + crosshairCircle.arcSpacing; sweepAngle: 90 - crosshairCircle.arcSpacing * 2
      }
      PathAngleArc {
        centerX: crosshairCircle.halfWidth; centerY: crosshairCircle.halfWidth
        radiusX: crosshairCircle.halfWidth; radiusY: crosshairCircle.halfWidth
        startAngle: 90 + crosshairCircle.arcSpacing; sweepAngle: 90 - crosshairCircle.arcSpacing * 2
      }
      PathAngleArc {
        centerX: crosshairCircle.halfWidth; centerY: crosshairCircle.halfWidth
        radiusX: crosshairCircle.halfWidth; radiusY: crosshairCircle.halfWidth
        startAngle: 180 + crosshairCircle.arcSpacing; sweepAngle: 90 - crosshairCircle.arcSpacing * 2
      }
      PathAngleArc {
        centerX: crosshairCircle.halfWidth; centerY: crosshairCircle.halfWidth
        radiusX: crosshairCircle.halfWidth; radiusY: crosshairCircle.halfWidth
        startAngle: 270 + crosshairCircle.arcSpacing; sweepAngle: 90 - crosshairCircle.arcSpacing * 2
      }
      PathMove {
        x: crosshairCircle.halfWidth; y: crosshairCircle.halfWidth - 8
      }
      PathLine {
        x: crosshairCircle.halfWidth; y: crosshairCircle.halfWidth + 8
      }
      PathMove {
        x: crosshairCircle.halfWidth - 8; y: crosshairCircle.halfWidth
      }
      PathLine {
        x: crosshairCircle.halfWidth + 8; y: crosshairCircle.halfWidth
      }
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
  }

  Component.onCompleted: {
    crosshairPathBuffer.pathElements = crosshairPath.pathElements
  }

  Connections {
    target: snappingUtils

    function onSnappingResultChanged() {
      crosshairCircle.isSnapped = overrideLocation == undefined && snappingUtils.snappingResult.isValid
    }
  }

  function flash()
  {
    flashAnimation.start()
  }
}
