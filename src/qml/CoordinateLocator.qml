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
  property var commonAngleInDegrees: null

  // some trickery here: the first part (!mapSettings.visibleExtent) is only there to get a signal when
  // the map canvas extent changes (user pans/zooms) and the calculation of the display position is retriggered
  readonly property point displayPosition: {
    // this property gets initially evaluated before the `currentCoordinate`
    const coordinate = currentCoordinate ? currentCoordinate : !!overrideLocation ? overrideLocation : snappingUtils.snappedCoordinate;
    return !!mapSettings.visibleExtent && coordinate !== undefined ? mapSettings.coordinateToScreen(coordinate) : Qt.point();
  }

  property variant rubberbandModel: undefined

  readonly property alias snappedCoordinate: snappingUtils.snappedCoordinate // In map coordinates, derived from snappinResult
  readonly property alias snappedPoint: snappingUtils.snappedPoint // In screen coordinates, derived from snappinResult
  readonly property alias topSnappingResult: snappingUtils.snappingResult // The snappingResult itself, only one (compared to QGIS where it's usually a list)

  SnappingUtils {
    id: snappingUtils

    enabled: locator.visible
    mapSettings: locator.mapSettings
    inputCoordinate: {
      // Get the current crosshair location in screen coordinates. If `undefined`, then we use the center of the screen as input point.
      const location = sourceLocation === undefined ? Qt.point(locator.width / 2, locator.height / 2) : sourceLocation;
      if (snapToCommonAngleButton.isSnapToCommonAngleEnabled) {
        locator.commonAngleInDegrees = getCommonAngleInDegrees(location, locator.rubberbandModel, snapToCommonAngleButton.snapToCommonAngleDegrees, snapToCommonAngleButton.isSnapToCommonAngleRelative);
        const coords = calculateSnapToAngleLineEndCoords(snappedPoint, locator.commonAngleInDegrees, snapToCommonAngleButton.isSnapToCommonAngleRelative, 1000);
        snapToCommonAngleLines.endCoordX = coords.x || 0;
        snapToCommonAngleLines.endCoordY = coords.y || 0;
        return snapPointToCommonAngle(location, locator.rubberbandModel, locator.commonAngleInDegrees, snapToCommonAngleButton.isSnapToCommonAngleRelative);
      } else {
        locator.commonAngleInDegrees = null;
        snapToCommonAngleLines.endCoordX = 0;
        snapToCommonAngleLines.endCoordY = 0;
      }
      return location;
    }
    config: qgisProject ? qgisProject.snappingConfig : snappingUtils.emptySnappingConfig()

    property variant snappedCoordinate
    property point snappedPoint

    onSnappingResultChanged: {
      if (snappingResult.isValid) {
        snappedCoordinate = snappingResult.point;
        snappedPoint = mapSettings.coordinateToScreen(snappedCoordinate);
      } else {
        snappedPoint = inputCoordinate;
        snappedCoordinate = mapSettings.screenToCoordinate(snappedPoint);
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
      width: (positioningSettings.averagedPositioning ? Math.min(parent.width, (parent.width * (averagedPositionCount / positioningSettings.averagedPositioningMinimumCount))) : parent.width) - 2.4
      color: positioningSettings.accuracyIndicator ? !positionSource.positionInformation || !positionSource.positionInformation.haccValid || positionSource.positionInformation.hacc > positioningSettings.accuracyBad ? Theme.accuracyBad : positionSource.positionInformation.hacc > positioningSettings.accuracyExcellent ? Theme.accuracyTolerated : Theme.accuracyExcellent : Theme.positionColor

      transitions: [
        Transition {
          NumberAnimation {
            property: "width"
            duration: 200
          }
        }
      ]
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

    Behavior on x  {
      enabled: !overrideLocation && !sourceLocation // It looks strange if the GPS position indicator and the crosshair are not synchronized
      NumberAnimation {
        duration: 100
      }
    }

    Behavior on y  {
      enabled: !overrideLocation && !sourceLocation
      NumberAnimation {
        duration: 100
      }
    }

    Behavior on width  {
      SmoothedAnimation {
        duration: 2000
      }
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
        centerX: crosshairCircle.halfWidth
        centerY: crosshairCircle.halfWidth
        radiusX: crosshairCircle.halfWidth
        radiusY: crosshairCircle.halfWidth
        startAngle: 0 + crosshairCircle.arcSpacing
        sweepAngle: 90 - crosshairCircle.arcSpacing * 2
      }
      PathAngleArc {
        centerX: crosshairCircle.halfWidth
        centerY: crosshairCircle.halfWidth
        radiusX: crosshairCircle.halfWidth
        radiusY: crosshairCircle.halfWidth
        startAngle: 90 + crosshairCircle.arcSpacing
        sweepAngle: 90 - crosshairCircle.arcSpacing * 2
      }
      PathAngleArc {
        centerX: crosshairCircle.halfWidth
        centerY: crosshairCircle.halfWidth
        radiusX: crosshairCircle.halfWidth
        radiusY: crosshairCircle.halfWidth
        startAngle: 180 + crosshairCircle.arcSpacing
        sweepAngle: 90 - crosshairCircle.arcSpacing * 2
      }
      PathAngleArc {
        centerX: crosshairCircle.halfWidth
        centerY: crosshairCircle.halfWidth
        radiusX: crosshairCircle.halfWidth
        radiusY: crosshairCircle.halfWidth
        startAngle: 270 + crosshairCircle.arcSpacing
        sweepAngle: 90 - crosshairCircle.arcSpacing * 2
      }
      PathMove {
        x: crosshairCircle.halfWidth
        y: crosshairCircle.halfWidth - 8
      }
      PathLine {
        x: crosshairCircle.halfWidth
        y: crosshairCircle.halfWidth + 8
      }
      PathMove {
        x: crosshairCircle.halfWidth - 8
        y: crosshairCircle.halfWidth
      }
      PathLine {
        x: crosshairCircle.halfWidth + 8
        y: crosshairCircle.halfWidth
      }
    }
  }

  Shape {
    id: snapToCommonAngleLines

    property double endCoordX: 0
    property double endCoordY: 0

    visible: !!locator.commonAngleInDegrees || (endCoordX !== 0 && endCoordY !== 0)
    width: parent.width
    height: parent.height
    anchors.centerIn: parent
    opacity: 0.5

    // outer line
    ShapePath {
      id: outerLine

      strokeWidth: 4
      strokeColor: "#fff"
      strokeStyle: ShapePath.DashLine
      dashPattern: [5, 3]
      startX: snappedPoint.x
      startY: snappedPoint.y

      PathLine {
        x: snapToCommonAngleLines.endCoordX
        y: snapToCommonAngleLines.endCoordY
      }
    }

    // inner line
    ShapePath {
      strokeWidth: outerLine.strokeWidth / 2
      strokeColor: "#000"
      strokeStyle: ShapePath.DashLine
      dashPattern: outerLine.dashPattern.map(v => v * 2)
      startX: snappedPoint.x
      startY: snappedPoint.y

      PathLine {
        x: snapToCommonAngleLines.endCoordX
        y: snapToCommonAngleLines.endCoordY
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
    crosshairPathBuffer.pathElements = crosshairPath.pathElements;
  }

  Connections {
    target: snappingUtils

    function onSnappingResultChanged() {
      crosshairCircle.isSnapped = overrideLocation == undefined && snappingUtils.snappingResult.isValid;
    }
  }

  function flash() {
    flashAnimation.start();
  }

  /**
   * Computes the possible common angle
   *
   * @param {QPointF} currentPoint - the current point being proposed
   * @param {Rubberband} rubberbandModel - holds all previously added points in the current digitizing session
   * @param {number} commonAngleStepDeg - the common angle in degrees we are trying to snap to
   * @param {boolean} isRelativeAngle - whether the angle should be calculated relative to the last geometry segment
   * @returns {number|undefined} - the closest common angle. Returns`undefined` if there is no nearby point that forms a common angle.
   */
  function getCommonAngleInDegrees(currentPoint, rubberbandModel, commonAngleStepDeg, isRelativeAngle) {
    if (!rubberbandModel) {
      return;
    }
    const MINIMAL_PIXEL_DISTANCE_TRESHOLD = 20;
    const SOFT_CONSTRAINT_TOLERANCE_DEGREES = 20;
    const SOFT_CONSTRAINT_TOLERANCE_PIXEL = 40;
    const previousPoint = mapCanvas.mapSettings.coordinateToScreen(rubberbandModel.lastCoordinate);
    const rubberbandPointsCount = rubberbandModel.vertexCount;

    // we need at least 1 alredy digitized point (and the other one is wanna be digitized)
    if (rubberbandPointsCount < 2) {
      return;
    }
    const distanceFromLastPoint = Math.sqrt((currentPoint.x - previousPoint.x) ** 2 + (currentPoint.y - previousPoint.y) ** 2);
    if (distanceFromLastPoint < MINIMAL_PIXEL_DISTANCE_TRESHOLD) {
      return;
    }
    const commonAngle = commonAngleStepDeg * Math.PI / 180;
    // see if soft common angle constraint should be performed
    // only if not in HardLock mode
    let softAngle = Math.atan2(currentPoint.y - previousPoint.y, currentPoint.x - previousPoint.x);
    let deltaAngle = 0;
    if (isRelativeAngle && rubberbandPointsCount >= 3) {
      // compute the angle relative to the last segment (0° is aligned with last segment)
      const penultimatePoint = mapCanvas.mapSettings.coordinateToScreen(rubberbandModel.penultimateCoordinate);
      deltaAngle = Math.atan2(previousPoint.y - penultimatePoint.y, previousPoint.x - penultimatePoint.x);
      softAngle -= deltaAngle;
    }
    const quo = Math.round(softAngle / commonAngle);
    if (Math.abs(softAngle - quo * commonAngle) * 180.0 * Math.PI <= SOFT_CONSTRAINT_TOLERANCE_DEGREES) {
      // also check the distance in pixel to the line, otherwise it's too sticky at long ranges
      softAngle = quo * commonAngle;
      // http://mathworld.wolfram.com/Point-LineDistance2-Dimensional.html
      // use the direction vector (cos(a),sin(a)) from previous point. |x2-x1|=1 since sin2+cos2=1
      const dist = Math.abs(Math.cos(softAngle + deltaAngle) * (previousPoint.y - currentPoint.y) - Math.sin(softAngle + deltaAngle) * (previousPoint.x - currentPoint.x));
      if (dist < SOFT_CONSTRAINT_TOLERANCE_PIXEL) {
        return 180.0 / Math.PI * softAngle;
      }
    }
  }

  /**
   * Computes a new point that forms a common angle
   *
   * @param {QPointF} currentPoint - the current point being proposed
   * @param {Rubberband} rubberbandModel - holds all previously added points in the current digitizing session
   * @param {number} commonAngleDegrees - the common angle in degrees we are snapping to
   * @param {boolean} isRelativeAngle - whether the angle should be calculated relative to the last geometry segment
   * @returns {QPointF} - the resulting snapped to common angle point. If no snapping was possible, then `currentPoint` is returned.
   */
  function snapPointToCommonAngle(currentPoint, rubberbandModel, commonAngleDegrees, isRelativeAngle) {
    if (!rubberbandModel) {
      return currentPoint;
    }

    // if null or undefined, no common angle is determined
    if (commonAngleDegrees == null) {
      return currentPoint;
    }
    let angleValue = commonAngleDegrees * Math.PI / 180;
    const returnPoint = currentPoint;
    const previousPoint = mapCanvas.mapSettings.coordinateToScreen(rubberbandModel.lastCoordinate);
    const rubberbandPointsCount = rubberbandModel.vertexCount;
    if (isRelativeAngle && rubberbandPointsCount >= 3) {
      // compute the angle relative to the last segment (0° is aligned with last segment)
      const penultimatePoint = mapCanvas.mapSettings.coordinateToScreen(rubberbandModel.penultimateCoordinate);
      angleValue += Math.atan2(previousPoint.y - penultimatePoint.y, previousPoint.x - penultimatePoint.x);
    }
    const cosa = Math.cos(angleValue);
    const sina = Math.sin(angleValue);
    const v = (currentPoint.x - previousPoint.x) * cosa + (currentPoint.y - previousPoint.y) * sina;
    returnPoint.x = (previousPoint.x + cosa * v);
    returnPoint.y = (previousPoint.y + sina * v);
    return returnPoint;
  }

  /**
   * Calculates snap to angle guide line end coordinate.
   * @param {QPointF} currentPoint - the current point being proposed
   * @param {number} angleDegrees - angle of the line in degrees.
   * @param {boolean} isRelativeAngle - whether the angle should be calculated relative to the last geometry segment
   * @param {number} screenSize - size of the screen. Used to make sure the end of the line is outside the screen.
   */
  function calculateSnapToAngleLineEndCoords(currentPoint, angleDegrees, isRelativeAngle, screenSize) {
    if (rubberbandModel == null) {
      return {};
    }
    const rubberbandPointsCount = rubberbandModel.vertexCount;

    // if the angle is null or undefined, return empty coordinate map
    if (angleDegrees == null) {
      return {};
    }
    let deltaAngle = 0;
    if (isRelativeAngle && rubberbandPointsCount >= 3) {
      // compute the angle relative to the last segment (0° is aligned with last segment)
      const previousPoint = mapCanvas.mapSettings.coordinateToScreen(rubberbandModel.lastCoordinate);
      const penultimatePoint = mapCanvas.mapSettings.coordinateToScreen(rubberbandModel.penultimateCoordinate);
      deltaAngle = Math.atan2(previousPoint.y - penultimatePoint.y, previousPoint.x - penultimatePoint.x);
    }
    const x1 = currentPoint.x;
    const y1 = currentPoint.y;
    const angleRadians = angleDegrees * Math.PI / 180 + deltaAngle;
    const x2 = x1 + screenSize * Math.cos(angleRadians);
    const y2 = y1 + screenSize * Math.sin(angleRadians);
    return {
      "x": x2,
      "y": y2
    };
  }
}
