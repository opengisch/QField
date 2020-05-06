import QtPositioning 5.3
import QtQuick 2.12
import org.qfield 1.0
import org.qgis 1.0

Item {
    /**
   * Set the current layer on which snapping should be performed
   */
    /**
   * Overrides any possibility for the user to modify the coordinate.
   * There will be no user interaction or snapping if this is set to a QgsPoint.
   * Set this to `undefined` to revert to the user and snapping controlled behavior.
   */
    // Screen coordinate

    id: locator

    property MapSettings mapSettings
    property color color: "#263238"
    property color highlightColor: "#CFD8DC"
    property alias currentLayer: snappingUtils.currentLayer
    property variant overrideLocation: undefined // QgsPoint
    //! Source location for snapping. If this is set to undefined, the center of the screen will be used.
    //! Overwritten by stylus / hoverHandler.
    property variant sourceLocation: undefined
    readonly property variant currentCoordinate: !!overrideLocation ? overrideLocation : snappingUtils.snappedCoordinate
    // some trickery here: the first part (!mapSettings.visibleExtent) is only there to get a signal when
    // the map canvas extent changes (user pans/zooms) and the calculation of the display position is retriggered
    readonly property point displayPosition: !!mapSettings.visibleExtent || true ? mapSettings.coordinateToScreen(currentCoordinate) : 0
    readonly property alias snappedCoordinate: snappingUtils.snappedCoordinate // In map coordinates, derived from snappinResult
    readonly property alias snappedPoint: snappingUtils.snappedPoint // In screen coordinates, derived from snappinResult
    readonly property alias topSnappingResult: snappingUtils.snappingResult // The snappingResult itself, only one (compared to QGIS where it's usually a list)

    function flash() {
        flashAnimation.start();
    }

    SnappingUtils {
        id: snappingUtils

        property variant snappedCoordinate
        property point snappedPoint

        mapSettings: locator.mapSettings
        inputCoordinate: sourceLocation === undefined ? Qt.point(locator.width / 2, locator.height / 2) : sourceLocation // In screen coordinates
        config: qgisProject.snappingConfig
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
        border.color: parent.color

        Connections {
            target: snappingUtils
            onSnappingResultChanged: {
                crosshairCircle.border.color = overrideLocation == undefined ? (snappingUtils.snappingResult.isValid ? "#9b59b6" : locator.color) : "#AD1457";
                crosshairCircle.width = snappingUtils.snappingResult.isValid ? 32 : 48;
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

        Behavior on border.color {
            ColorAnimation {
                duration: 200
            }

        }

        Behavior on width {
            SmoothedAnimation {
                duration: 2000
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

}
