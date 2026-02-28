

# File MapCanvas.qml

[**File List**](files.md) **>** [**qml**](dir_1d81f7f4362e4398c0a695c4e466fcb0.md) **>** [**MapCanvas.qml**](MapCanvas_8qml.md)

[Go to the documentation of this file](MapCanvas_8qml.md)


```C++
/***************************************************************************
                            MapCanvas.qml
                              -------------------
              begin                : 10.12.2014
              copyright            : (C) 2014 by Matthias Kuhn
              email                : matthias (at) opengis.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
import QtQuick
import QtQuick.Controls
import QtQml
import org.qgis

Item {
  id: mapArea

  property alias mapCanvasWrapper: mapCanvasWrapper
  property alias mapSettings: mapCanvasWrapper.mapSettings
  property alias bottomMargin: mapCanvasWrapper.bottomMargin
  property alias rightMargin: mapCanvasWrapper.rightMargin
  property alias isRendering: mapCanvasWrapper.isRendering
  property alias incrementalRendering: mapCanvasWrapper.incrementalRendering
  property alias quality: mapCanvasWrapper.quality
  property alias smooth: mapCanvasWrapper.smooth
  property alias previewJobsEnabled: mapCanvasWrapper.previewJobsEnabled
  property alias previewJobsQuadrants: mapCanvasWrapper.previewJobsQuadrants
  property alias forceDeferredLayersRepaint: mapCanvasWrapper.forceDeferredLayersRepaint

  property bool interactive: true
  property bool hovered: false
  property bool freehandDigitizing: false
  property bool isMapRotationEnabled: false

  readonly property bool pinched: pinchHandler.active
  readonly property bool jumping: jumpDetails.enabled

  // for signals, type can be "stylus" for any device click or "touch"

  signal clicked(var point, var type)

  signal rightClicked(var point, var type)

  signal confirmedClicked(var point)

  signal longPressed(var point, var type)

  signal longPressReleased(var type)

  signal aboutToWheelZoom

  signal doubleClicked(var point, var type)

  function freeze(id) {
    mapCanvasWrapper.__freezecount[id] = true;
    mapCanvasWrapper.freeze = true;
    if (id !== 'follow') {
      mapCanvasWrapper.stopRendering();
    }
  }

  function unfreeze(id) {
    delete mapCanvasWrapper.__freezecount[id];
    mapCanvasWrapper.freeze = Object.keys(mapCanvasWrapper.__freezecount).length !== 0;
  }

  function zoomIn(point) {
    mapCanvasWrapper.zoomByFactor(point, 0.67);
  }

  function zoomOut(point) {
    mapCanvasWrapper.zoomByFactor(point, 1.5);
  }

  function refresh(ignoreFreeze) {
    mapCanvasWrapper.refresh(ignoreFreeze === undefined ? false : ignoreFreeze);
  }

  function stopRendering() {
    mapCanvasWrapper.stopRendering();
  }

  function _setupJump(scale, rotation, handleMargins, callback) {
    const currentCenter = mapCanvasWrapper.mapSettings.getCenter(true);
    const currentRotation = mapCanvasWrapper.mapSettings.rotation;
    jumpDetails.fromScale = mapCanvasWrapper.mapSettings.scale;
    jumpDetails.fromRotation = currentRotation;
    jumpDetails.fromX = currentCenter.x;
    jumpDetails.fromY = currentCenter.y;
    jumpDetails.toScale = !isNaN(scale) ? scale : -1;
    jumpDetails.completedCallback = callback;
    jumpDetails.toRotation = !isNaN(rotation) ? rotation : -1;
    jumpDetails.position = 0.0;
    jumpDetails.handleMargins = handleMargins;
    freeze('jumping');
    jumpDetails.enabled = true;
    jumpDetails.position = 1.0;
  }

  function jumpTo(point, scale = -1, rotation = -1, handleMargins = false, callback = null) {
    jumpDetails.positionSource = null;
    jumpDetails.toX = "x" in point ? point.x : 0;
    jumpDetails.toY = "y" in point ? point.y : 0;
    _setupJump(scale, rotation, handleMargins, callback);
  }

  function jumpToPosition(positionSource, scale = -1, rotation = -1, handleMargins = false, callback = null) {
    jumpDetails.positionSource = positionSource;
    _setupJump(scale, rotation, handleMargins, callback);
  }

  Item {
    id: jumpDetails

    property bool enabled: false
    property double fromRotation: 0
    property double fromScale: 0
    property double fromX: 0
    property double fromY: 0
    property double toRotation: -1
    property double toScale: -1
    property double toX: 0
    property double toY: 0
    property var positionSource: null
    property double position: 0
    property bool handleMargins: false
    property var completedCallback: null

    onPositionChanged: {
      if (!enabled) {
        return;
      }
      const targetX = positionSource ? positionSource.projectedPosition.x : toX;
      const targetY = positionSource ? positionSource.projectedPosition.y : toY;
      const dx = targetX - fromX;
      const dy = targetY - fromY;
      const distance = Math.sqrt(dx * dx + dy * dy);
      if (distance === 0 && toRotation === -1 && toScale === -1) {
        enabled = false;
        positionSource = null;
        mapArea.unfreeze('jumping');
        return;
      }
      if (toRotation !== -1) {
        const progressRotation = fromRotation + (toRotation - fromRotation) * position;
        mapCanvasWrapper.mapSettings.rotation = progressRotation;
      }
      const progressX = fromX + dx * position;
      const progressY = fromY + dy * position;
      if (toScale !== -1) {
        const progressScale = fromScale + (toScale - fromScale) * position;
        mapCanvasWrapper.zoomScale(Qt.point(progressX, progressY), progressScale, jumpDetails.handleMargins);
      } else {
        mapCanvasWrapper.mapSettings.setCenter(Qt.point(progressX, progressY), jumpDetails.handleMargins);
      }
      if (position >= 1.0) {
        enabled = false;
        positionSource = null;
        mapArea.unfreeze('jumping');
        if (completedCallback && typeof completedCallback === 'function') {
          completedCallback();
          completedCallback = null;
        }
      }
    }

    Behavior on position {
      enabled: jumpDetails.enabled
      NumberAnimation {
        easing.type: Easing.InOutQuart
        duration: 500
      }
    }
  }

  MapCanvasMap {
    id: mapCanvasWrapper

    width: mapArea.width
    height: mapArea.height

    property var __freezecount: ({})
    property int animationDuration: 50

    freeze: false

    Behavior on x {
      NumberAnimation {
        duration: mapCanvasWrapper.animationDuration
      }
    }
    Behavior on y {
      NumberAnimation {
        duration: mapCanvasWrapper.animationDuration
      }
    }
    Behavior on scale {
      NumberAnimation {
        duration: mapCanvasWrapper.animationDuration
      }
    }
    Behavior on rotation {
      NumberAnimation {
        duration: mapCanvasWrapper.animationDuration
      }
    }
  }

  TapHandler {
    id: stylusTapHandler
    enabled: interactive
    grabPermissions: PointerHandler.CanTakeOverFromHandlersOfDifferentType | PointerHandler.ApprovesTakeOverByAnything | PointerHandler.ApprovesCancellation
    acceptedDevices: !qfieldSettings.mouseAsTouchScreen ? PointerDevice.Stylus | PointerDevice.Mouse | PointerDevice.TouchPad : PointerDevice.Stylus
    acceptedButtons: Qt.LeftButton | Qt.RightButton

    property bool longPressActive: false

    onSingleTapped: (eventPoint, button) => {
      if (button === undefined) {
        button = eventPoint.event.button;
      }
      if (button === Qt.RightButton) {
        mapArea.rightClicked(point.position, "stylus");
      } else {
        mapArea.clicked(point.position, "stylus");
      }
    }

    onLongPressed: {
      mapArea.longPressed(point.position, "stylus");
      longPressActive = true;
    }

    onPressedChanged: {
      if (longPressActive)
        mapArea.longPressReleased("stylus");
      longPressActive = false;
    }
  }

  DragHandler {
    id: stylusDragHandler
    enabled: interactive && !freehandDigitizing
    target: null
    grabPermissions: PointerHandler.CanTakeOverFromHandlersOfDifferentType | PointerHandler.ApprovesTakeOverByHandlersOfDifferentType
    acceptedDevices: !qfieldSettings.mouseAsTouchScreen ? PointerDevice.Stylus | PointerDevice.Mouse | PointerDevice.TouchPad : PointerDevice.Stylus | PointerDevice.TouchPad
    acceptedButtons: Qt.NoButton | Qt.LeftButton
    dragThreshold: 5

    property var oldPos
    property real oldTranslationY

    property bool isZooming: false
    property bool isPanning: false
    property point zoomCenter

    onActiveChanged: {
      if (active) {
        if (mainTapHandler.doublePressed) {
          oldTranslationY = 0;
          zoomCenter = centroid.position;
          isZooming = true;
          freeze('zoom');
        } else {
          oldPos = centroid.position;
          isPanning = true;
          freeze('pan');
        }
      } else {
        if (isZooming || isPanning) {
          unfreeze(isZooming ? 'zoom' : 'pan');
        }
        isZooming = false;
        isPanning = false;
      }
    }

    onCentroidChanged: {
      if (active) {
        if (isZooming) {
          mapCanvasWrapper.zoomByFactor(zoomCenter, Math.pow(0.8, (translation.y - oldTranslationY) / 60));
          oldTranslationY = translation.y;
        } else if (isPanning) {
          mapCanvasWrapper.pan(centroid.position, oldPos);
          oldPos = centroid.position;
        }
      }
    }
  }

  Timer {
    id: timer
    interval: 350
    repeat: false

    property var tapPoint

    onTriggered: {
      mainTapHandler.doublePressed = false;
      confirmedClicked(tapPoint);
    }
  }

  TapHandler {
    id: mainTapHandler
    enabled: interactive && !hovered
    acceptedButtons: Qt.NoButton | Qt.LeftButton | Qt.RightButton
    grabPermissions: PointerHandler.CanTakeOverFromHandlersOfDifferentType | PointerHandler.ApprovesTakeOverByHandlersOfDifferentType
    acceptedDevices: !qfieldSettings.mouseAsTouchScreen ? PointerDevice.TouchScreen : PointerDevice.TouchScreen | PointerDevice.Mouse | PointerDevice.TouchPad

    property bool longPressActive: false
    property bool doublePressed: false

    onLongPressed: {
      mapArea.longPressed(Qt.point(point.position.x, point.position.y), "touch");
      longPressActive = true;
    }

    onPressedChanged: {
      if (pressed) {
        if (point.pressedButtons !== Qt.RightButton) {
          if (timer.running) {
            timer.stop();
            doublePressed = true;
          } else {
            doublePressed = false;
          }
        }
      }
    }

    onTapped: (eventPoint, button) => {
      if (button === Qt.RightButton) {
        mapArea.rightClicked(Qt.point(eventPoint.position.x, eventPoint.position.y), "touch");
      } else {
        if (!doublePressed) {
          timer.tapPoint = Qt.point(eventPoint.position.x, eventPoint.position.y);
          timer.restart();
        } else {
          mapArea.doubleClicked(point.position, "touch");
          doublePressed = false;
        }
      }
    }

    onCanceled: {
      timer.stop();
    }
  }

  DragHandler {
    id: mainDragHandler
    enabled: interactive && !hovered && !freehandDigitizing
    target: null
    acceptedButtons: Qt.NoButton | Qt.LeftButton
    grabPermissions: PointerHandler.CanTakeOverFromHandlersOfDifferentType | PointerHandler.ApprovesTakeOverByHandlersOfDifferentType
    acceptedDevices: !qfieldSettings.mouseAsTouchScreen ? PointerDevice.TouchScreen : PointerDevice.TouchScreen | PointerDevice.Mouse | PointerDevice.TouchPad
    dragThreshold: 5

    property var oldPos
    property real oldTranslationY

    property bool isZooming: false
    property bool isPanning: false
    property point zoomCenter

    onActiveChanged: {
      if (active) {
        if (mainTapHandler.doublePressed) {
          oldTranslationY = 0;
          zoomCenter = centroid.position;
          isZooming = true;
          freeze('zoom');
        } else {
          oldPos = centroid.position;
          isPanning = true;
          freeze('pan');
        }
      } else {
        if (isZooming || isPanning) {
          unfreeze(isZooming ? 'zoom' : 'pan');
        }
        isZooming = false;
        isPanning = false;
      }
    }

    onCentroidChanged: {
      if (active) {
        if (isZooming) {
          mapCanvasWrapper.zoomByFactor(zoomCenter, Math.pow(0.8, (translation.y - oldTranslationY) / 60));
          oldTranslationY = translation.y;
        } else if (isPanning) {
          mapCanvasWrapper.pan(centroid.position, oldPos);
          oldPos = centroid.position;
        }
      }
    }
  }

  DragHandler {
    id: secondaryDragHandler
    target: null
    enabled: interactive
    grabPermissions: PointerHandler.CanTakeOverFromItems | PointerHandler.CanTakeOverFromHandlersOfDifferentType | PointerHandler.ApprovesTakeOverByAnything
    acceptedDevices: PointerDevice.Stylus | PointerDevice.Mouse
    acceptedButtons: Qt.RightButton
    dragThreshold: 10

    property real oldTranslationY
    property point zoomCenter

    onActiveChanged: {
      if (active) {
        oldTranslationY = 0;
        zoomCenter = centroid.position;
        freeze('zoom');
      } else {
        grabPermissions: PointerHandler.TakeOverForbidden;
        unfreeze('zoom');
      }
    }

    onTranslationChanged: {
      if (active) {
        mapCanvasWrapper.zoomByFactor(zoomCenter, Math.pow(0.8, (oldTranslationY - translation.y) / 60));
      }
      oldTranslationY = translation.y;
    }
  }

  DragHandler {
    target: null
    enabled: interactive
    acceptedDevices: PointerDevice.Stylus | PointerDevice.Mouse
    acceptedModifiers: Qt.NoModifier
    grabPermissions: PointerHandler.TakeOverForbidden
    acceptedButtons: Qt.MiddleButton

    property var oldPos

    onActiveChanged: {
      if (active) {
        freeze('pan');
        oldPos = centroid.position;
      } else {
        unfreeze('pan');
      }
    }

    onCentroidChanged: {
      if (active) {
        var oldPos1 = oldPos;
        oldPos = centroid.position;
        mapCanvasWrapper.pan(centroid.position, oldPos1);
      }
    }
  }

  DragHandler {
    target: null
    enabled: interactive && isMapRotationEnabled
    acceptedDevices: PointerDevice.Stylus | PointerDevice.Mouse
    acceptedModifiers: Qt.ShiftModifier
    grabPermissions: PointerHandler.TakeOverForbidden
    acceptedButtons: Qt.MiddleButton

    property real pressClickX: 0
    property real pressClickY: 0
    property real screenCenterX: 0
    property real screenCenterY: 0
    property real lastRotateAngle: 0

    onActiveChanged: {
      if (active) {
        freeze('rotate');
        pressClickX = centroid.position.x;
        pressClickY = centroid.position.y;
        screenCenterX = width / 2;
        screenCenterY = height / 2;
        lastRotateAngle = 0;
      } else {
        unfreeze('rotate');
      }
    }

    onTranslationChanged: {
      if (active) {
        if (lastRotateAngle != 0) {
          let newPositionX = pressClickX + translation.x;
          let newPositionY = pressClickY + translation.y;
          let angle = Math.atan2(newPositionY - screenCenterY, newPositionX - screenCenterX) - Math.atan2(pressClickY - screenCenterY, pressClickX - screenCenterX);
          if (angle != 0) {
            mapCanvasWrapper.rotate(angle * 180 / Math.PI - lastRotateAngle);
          }
          lastRotateAngle = angle * 180 / Math.PI;
        } else {
          let newPositionX = pressClickX + translation.x;
          let newPositionY = pressClickY + translation.y;
          let angle = Math.atan2(newPositionY - screenCenterY, newPositionX - screenCenterX) - Math.atan2(pressClickY - screenCenterY, pressClickX - screenCenterX);
          lastRotateAngle = angle * 180 / Math.PI;
        }
      }
    }
  }

  PinchHandler {
    id: pinchHandler
    enabled: interactive
    target: null
    acceptedButtons: Qt.NoButton | Qt.LeftButton
    grabPermissions: PointerHandler.CanTakeOverFromHandlersOfDifferentType | PointerHandler.ApprovesTakeOverByHandlersOfDifferentType
    acceptedDevices: PointerDevice.TouchScreen
    dragThreshold: 5

    property real rotationTreshold: 20.0

    property var oldPos
    property real oldScale: 1.0
    property real oldRotation: 0.0

    property bool rotationActive: false
    property bool rotationTresholdReached: false

    onActiveChanged: {
      if (active) {
        freeze('pinch');
        oldScale = 1.0;
        oldRotation = 0.0;
        rotationTresholdReached = false;
        oldPos = centroid.position;
      } else {
        unfreeze('pinch');
      }
    }

    onCentroidChanged: {
      var oldPos1 = oldPos;
      oldPos = centroid.position;
      if (active) {
        mapCanvasWrapper.pan(centroid.position, oldPos1);
      }
    }

    onRotationChanged: {
      if (active && isMapRotationEnabled) {
        if (rotationTresholdReached) {
          mapCanvasWrapper.rotate(rotation - oldRotation);
          oldRotation = rotation;
        } else if (Math.abs(rotation - oldRotation) > pinchHandler.rotationTreshold) {
          oldRotation = rotation;
          rotationTresholdReached = true;
        }
      }
    }

    onActiveScaleChanged: {
      mapCanvasWrapper.zoomByFactor(pinchHandler.centroid.position, oldScale / pinchHandler.activeScale);
      mapCanvasWrapper.pan(pinchHandler.centroid.position, oldPos);
      oldScale = pinchHandler.activeScale;
    }
  }

  WheelHandler {
    enabled: interactive
    target: null
    acceptedDevices: PointerDevice.AllDevices
    grabPermissions: PointerHandler.CanTakeOverFromHandlersOfDifferentType | PointerHandler.ApprovesTakeOverByItems

    onWheel: event => {
      aboutToWheelZoom();
      if (event.angleDelta.y > 0) {
        zoomIn(point.position);
      } else {
        zoomOut(point.position);
      }
    }
  }
}
```


