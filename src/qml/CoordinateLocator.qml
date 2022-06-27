import QtQuick 2.14

import org.qgis 1.0
import org.qfield 1.0
import Theme 1.0

Item {
  id: locator
  property MapSettings mapSettings
  property color mainColor: "#263238"
  property color highlightColor: "#CFD8DC"

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

  Rectangle {
    id: crosshairCircleInnerBuffer
    anchors.centerIn: crosshairCircle

    width: crosshairCircle.width
    height: crosshairCircle.height

    border.color: highlightColor
    border.width: 2
    color: "transparent"
    radius: width / 2
  }

  Rectangle {
    id: crosshairCircle

    x: displayPosition.x - radius
    y: displayPosition.y - radius

    border.width: 1.2
    color: "transparent"
    antialiasing: true

    width: 48
    height: width
    radius: width / 2

    Behavior on x {
      enabled: !overrideLocation && !sourceLocation // It looks strange if the GPS position indicator and the crosshair are not synchronized
      NumberAnimation { duration: 100 }
    }

    Behavior on y {
      enabled: !overrideLocation && !sourceLocation
      NumberAnimation { duration: 100 }
    }

    border.color: mainColor

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

      function onSnappingResultChanged() {
        crosshairCircle.border.color = overrideLocation == undefined ? ( snappingUtils.snappingResult.isValid ? "#9b59b6" : locator.mainColor ) : "#AD1457"
        crosshairCircle.width = snappingUtils.snappingResult.isValid ? 32: 48
      }
    }

    Rectangle {
      anchors.centerIn: parent

      color: highlightColor

      width: 3
      height: parent.height * 4 / 6 + 3
    }
    Rectangle {
      anchors.centerIn: parent

      color: highlightColor

      width: parent.width * 4 / 6 + 3
      height: 3
    }

    Rectangle {
      anchors.centerIn: parent

      color: parent.border.color
      border.color: parent.color
      border.width: 1.2

      width: 1
      height: parent.height * 4 / 6
    }
    Rectangle {
      anchors.centerIn: parent

      color: parent.border.color

      width: parent.width * 4 / 6
      height: 1
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
