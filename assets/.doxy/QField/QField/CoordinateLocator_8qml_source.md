

# File CoordinateLocator.qml

[**File List**](files.md) **>** [**qml**](dir_1d81f7f4362e4398c0a695c4e466fcb0.md) **>** [**CoordinateLocator.qml**](CoordinateLocator_8qml.md)

[Go to the documentation of this file](CoordinateLocator_8qml.md)


```C++
import QtQuick
import QtQuick.Shapes
import org.qgis
import org.qfield
import Theme

Item {
  id: locator

  property MapSettings mapSettings
  property color mainColor: "#CFD8DC"
  property color highlightColor: "#263238"

  property alias currentLayer: snappingUtils.currentLayer

  property var positionInformation: undefined
  property bool positionLocked: false
  property bool averagedPosition: false
  property int averagedPositionCount: 0

  property bool snapToCommonAngles: false
  property bool snappingIsRelative: false
  property real snappingAngleDegrees: 45.0
  property real snappingTolerance: 1

  property variant overrideLocation: undefined // QgsPoint

  property variant sourceLocation: undefined // Screen coordinate

  readonly property variant currentCoordinate: !!overrideLocation ? overrideLocation : snappingUtils.snappedCoordinate

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
    config: qgisProject ? qgisProject.snappingConfig : snappingUtils.emptySnappingConfig()
    // Get the current crosshair location in screen coordinates. If `undefined`, then we use the center of the screen as input point.
    inputCoordinate: sourceLocation === undefined ? Qt.point(locator.width / 2, locator.height / 2) : sourceLocation

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
      if (!locator.positionLocked && locator.snapToCommonAngles) {
        let backwardCommonAngleInDegrees = undefined;
        let backwardCoords = {};
        let backwardPoint = undefined;
        backwardCommonAngleInDegrees = getCommonAngleInDegrees(snappedPoint, locator.rubberbandModel, locator.snappingAngleDegrees, locator.snappingIsRelative);
        if (backwardCommonAngleInDegrees !== undefined) {
          backwardPoint = snapPointToCommonAngle(snappedPoint, locator.rubberbandModel, backwardCommonAngleInDegrees, locator.snappingIsRelative);
          backwardCoords = calculateSnapToAngleLineEndCoords(backwardPoint, backwardCommonAngleInDegrees, locator.snappingIsRelative, 1000);
        }
        let forwardCommonAngleInDegrees = undefined;
        let forwardCoords = {};
        let forwardPoint = undefined;
        if (locator.rubberbandModel && locator.rubberbandModel.vertexCount >= 4) {
          forwardCommonAngleInDegrees = getCommonAngleInDegrees(snappedPoint, locator.rubberbandModel, locator.snappingAngleDegrees, locator.snappingIsRelative, true);
          if (forwardCommonAngleInDegrees !== undefined) {
            forwardPoint = snapPointToCommonAngle(snappedPoint, locator.rubberbandModel, forwardCommonAngleInDegrees, locator.snappingIsRelative, true);
            forwardCoords = calculateSnapToAngleLineEndCoords(forwardPoint, forwardCommonAngleInDegrees, locator.snappingIsRelative, 1000, -1);
          }
        }
        snappingLinesModel.setProperty(0, "beginCoordX", backwardCoords.x1 || 0);
        snappingLinesModel.setProperty(0, "beginCoordY", backwardCoords.y1 || 0);
        snappingLinesModel.setProperty(0, "endCoordX", backwardCoords.x2 || 0);
        snappingLinesModel.setProperty(0, "endCoordY", backwardCoords.y2 || 0);
        snappingLinesModel.setProperty(0, "snappedToAngle", backwardCommonAngleInDegrees !== undefined);
        snappingLinesModel.setProperty(1, "beginCoordX", forwardCoords.x1 || 0);
        snappingLinesModel.setProperty(1, "beginCoordY", forwardCoords.y1 || 0);
        snappingLinesModel.setProperty(1, "endCoordX", forwardCoords.x2 || 0);
        snappingLinesModel.setProperty(1, "endCoordY", forwardCoords.y2 || 0);
        snappingLinesModel.setProperty(1, "snappedToAngle", forwardCommonAngleInDegrees !== undefined);
        if (backwardCommonAngleInDegrees !== undefined && forwardCommonAngleInDegrees !== undefined) {
          // Get the intersection of the two snapped lines
          const x1 = backwardCoords.x1;
          const y1 = backwardCoords.y1;
          const x2 = backwardCoords.x2;
          const y2 = backwardCoords.y2;
          const x3 = forwardCoords.x1;
          const y3 = forwardCoords.y1;
          const x4 = forwardCoords.x2;
          const y4 = forwardCoords.y2;
          const denominator = ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
          const ua = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / denominator;
          const ub = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / denominator;
          const intersectX = x1 + ua * (x2 - x1);
          const intersectY = y1 + ua * (y2 - y1);
          snappedPoint = Qt.point(intersectX, intersectY);
        } else if (backwardCommonAngleInDegrees !== undefined) {
          snappedPoint = backwardPoint;
        } else if (forwardCommonAngleInDegrees !== undefined) {
          snappedPoint = forwardPoint;
        }
        snappedCoordinate = mapSettings.screenToCoordinate(snappedPoint);
      } else {
        for (let i = 0; i < snappingLinesModel.count; ++i) {
          snappingLinesModel.setProperty(i, "beginCoordX", 0);
          snappingLinesModel.setProperty(i, "beginCoordY", 0);
          snappingLinesModel.setProperty(i, "endCoordX", 0);
          snappingLinesModel.setProperty(i, "endCoordY", 0);
          snappingLinesModel.setProperty(i, "snappedToAngle", false);
        }
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

    Behavior on x {
      enabled: !overrideLocation && !sourceLocation // It looks strange if the GPS position indicator and the crosshair are not synchronized
      NumberAnimation {
        duration: 100
      }
    }

    Behavior on y {
      enabled: !overrideLocation && !sourceLocation
      NumberAnimation {
        duration: 100
      }
    }

    Behavior on width {
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

  Repeater {
    id: snappingLines
    width: parent.width
    height: parent.height
    anchors.centerIn: parent

    model: ListModel {
      id: snappingLinesModel
      ListElement {
        beginCoordX: 0
        beginCoordY: 0
        endCoordX: 0
        endCoordY: 0
        snappedToAngle: false
      }
      ListElement {
        beginCoordX: 0
        beginCoordY: 0
        endCoordX: 0
        endCoordY: 0
        snappedToAngle: false
      }
    }

    delegate: Shape {
      visible: !!snappedToAngle || (endCoordX !== 0 && endCoordY !== 0)
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
        startX: beginCoordX
        startY: beginCoordY

        PathLine {
          x: endCoordX
          y: endCoordY
        }
      }

      // inner line
      ShapePath {
        strokeWidth: outerLine.strokeWidth / 2
        strokeColor: "#000"
        strokeStyle: ShapePath.DashLine
        dashPattern: outerLine.dashPattern.map(v => v * 2)
        startX: beginCoordX
        startY: beginCoordY

        PathLine {
          x: endCoordX
          y: endCoordY
        }
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
    if (positionLocked) {
      const outOfScreen = crosshairCircle.x + crosshairCircle.width <= 0 || crosshairCircle.x - crosshairCircle.width >= mainWindow.width || crosshairCircle.y + crosshairCircle.height <= 0 || crosshairCircle.y - crosshairCircle.height >= mainWindow.height;
      if (outOfScreen) {
        mapCanvasMap.jumpTo(positionSource.projectedPosition, -1, -1, true);
      }
    }
  }

  function getToleranceMultiplier() {
    switch (snappingTolerance) {
    case 0:
      return 0.5;
    case 2:
      return 4;
    case 1:
    default:
      return 1;
    }
  }

  function getCommonAngleInDegrees(currentPoint, rubberbandModel, commonAngleStepDeg, isRelativeAngle, forwardMode = false) {
    if (!rubberbandModel) {
      return;
    }
    const MINIMAL_PIXEL_DISTANCE_TRESHOLD = 20;
    const SOFT_CONSTRAINT_TOLERANCE_DEGREES = 20 * getToleranceMultiplier();
    const SOFT_CONSTRAINT_TOLERANCE_PIXEL = 40;
    const rubberbandPointsCount = rubberbandModel.vertexCount;
    const targetPoint = mapCanvas.mapSettings.coordinateToScreen(forwardMode ? rubberbandModel.firstCoordinate : rubberbandModel.lastCoordinate);
    const minimumDigitizedPoints = forwardMode ? 3 : 2;

    // we need at least [minimumDigitizedPoints - 1] alredy digitized point (and the other one is wanna be digitized)
    if (rubberbandPointsCount < minimumDigitizedPoints) {
      return;
    }
    const distanceFromLastPoint = Math.sqrt((currentPoint.x - targetPoint.x) ** 2 + (currentPoint.y - targetPoint.y) ** 2);
    if (distanceFromLastPoint < MINIMAL_PIXEL_DISTANCE_TRESHOLD) {
      return;
    }
    const commonAngle = commonAngleStepDeg * Math.PI / 180;
    // see if soft common angle constraint should be performed
    // only if not in HardLock mode
    let softAngle = Math.atan2(currentPoint.y - targetPoint.y, currentPoint.x - targetPoint.x);
    let deltaAngle = 0;
    if (isRelativeAngle && rubberbandPointsCount >= 3) {
      // compute the angle relative to the last segment (0° is aligned with last segment)
      const penultimatePoint = mapCanvas.mapSettings.coordinateToScreen(forwardMode ? rubberbandModel.vertices[1] : rubberbandModel.penultimateCoordinate);
      deltaAngle = Math.atan2(targetPoint.y - penultimatePoint.y, targetPoint.x - penultimatePoint.x);
      softAngle -= deltaAngle;
    }
    const quo = Math.round(softAngle / commonAngle);
    if (Math.abs(softAngle - quo * commonAngle) * 180.0 * Math.PI <= SOFT_CONSTRAINT_TOLERANCE_DEGREES) {
      // also check the distance in pixel to the line, otherwise it's too sticky at long ranges
      softAngle = quo * commonAngle;
      // http://mathworld.wolfram.com/Point-LineDistance2-Dimensional.html
      // use the direction vector (cos(a),sin(a)) from previous point. |x2-x1|=1 since sin2+cos2=1
      const dist = Math.abs(Math.cos(softAngle + deltaAngle) * (targetPoint.y - currentPoint.y) - Math.sin(softAngle + deltaAngle) * (targetPoint.x - currentPoint.x));
      if (dist < SOFT_CONSTRAINT_TOLERANCE_PIXEL) {
        return 180.0 / Math.PI * softAngle;
      }
    }
  }

  function snapPointToCommonAngle(currentPoint, rubberbandModel, commonAngleDegrees, isRelativeAngle, forwardMode = false) {
    if (!rubberbandModel) {
      return currentPoint;
    }
    if (commonAngleDegrees === null || commonAngleDegrees === undefined) {
      return currentPoint;
    }
    let angleValue = commonAngleDegrees * Math.PI / 180;
    const returnPoint = currentPoint;
    const rubberbandPointsCount = rubberbandModel.vertexCount;
    const targetPoint = mapCanvas.mapSettings.coordinateToScreen(forwardMode ? rubberbandModel.firstCoordinate : rubberbandModel.lastCoordinate);
    if (isRelativeAngle && rubberbandPointsCount >= 3) {
      // compute the angle relative to the last segment (0° is aligned with last segment)
      const penultimatePoint = mapCanvas.mapSettings.coordinateToScreen(forwardMode ? rubberbandModel.vertices[1] : rubberbandModel.penultimateCoordinate);
      angleValue += Math.atan2(targetPoint.y - penultimatePoint.y, targetPoint.x - penultimatePoint.x);
    }
    const cosa = Math.cos(angleValue);
    const sina = Math.sin(angleValue);
    const v = (currentPoint.x - targetPoint.x) * cosa + (currentPoint.y - targetPoint.y) * sina;
    returnPoint.x = (targetPoint.x + cosa * v);
    returnPoint.y = (targetPoint.y + sina * v);
    return returnPoint;
  }

  function calculateSnapToAngleLineEndCoords(currentPoint, angleDegrees, isRelativeAngle, screenSize, forwardMode = false) {
    if (rubberbandModel == null) {
      return {};
    }
    const rubberbandPointsCount = rubberbandModel.vertexCount;
    if (angleDegrees === null || angleDegrees === undefined) {
      return {};
    }
    let deltaAngle = 0;
    if (isRelativeAngle && rubberbandPointsCount >= 3) {
      // compute the angle relative to the last segment (0° is aligned with last segment)
      const targetPoint = mapCanvas.mapSettings.coordinateToScreen(forwardMode ? rubberbandModel.firstCoordinate : rubberbandModel.lastCoordinate);
      const penultimatePoint = mapCanvas.mapSettings.coordinateToScreen(forwardMode ? rubberbandModel.vertices[1] : rubberbandModel.penultimateCoordinate);
      deltaAngle = Math.atan2(targetPoint.y - penultimatePoint.y, targetPoint.x - penultimatePoint.x);
    }
    const xOrigin = currentPoint.x;
    const yOrigin = currentPoint.y;
    let angleRadians = angleDegrees * Math.PI / 180 + deltaAngle;
    const x1 = xOrigin + screenSize * Math.cos(angleRadians) * (!forwardMode ? 1 : -1);
    const y1 = yOrigin + screenSize * Math.sin(angleRadians) * (!forwardMode ? 1 : -1);
    angleRadians -= 180 * Math.PI / 180;
    const x2 = xOrigin + screenSize * Math.cos(angleRadians) * (!forwardMode ? 1 : -1);
    const y2 = yOrigin + screenSize * Math.sin(angleRadians) * (!forwardMode ? 1 : -1);
    return {
      "x1": x1,
      "y1": y1,
      "x2": x2,
      "y2": y2
    };
  }
}
```


