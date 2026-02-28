

# File TouchCameraController.qml

[**File List**](files.md) **>** [**3d**](dir_47cbdfc3401e26ec6b0f990982ff02ef.md) **>** [**TouchCameraController.qml**](TouchCameraController_8qml.md)

[Go to the documentation of this file](TouchCameraController_8qml.md)


```C++
import QtQuick
import QtQuick3D

Item {
  id: root

  required property PerspectiveCamera camera

  signal singleTapped(real x, real y)
  signal userInteractionStarted

  property vector3d target: Qt.vector3d(0, 100, 0)

  property real distance: root.defaultDistance
  property real yaw: 0
  property real pitch: 40

  property real defaultDistance: 2200
  property real defaultYaw: 0
  property real defaultPitch: 40

  property real minDistance: 100
  property real maxDistance: 10000
  property real minPitch: 5
  property real maxPitch: 89

  property real orbitSensitivity: 0.3

  property real targetX: 0
  property real targetY: 100
  property real targetZ: 0

  Component.onCompleted: {
    updateCameraPosition();
  }

  function updateCameraPosition() {
    if (!camera) {
      return;
    }

    target = Qt.vector3d(targetX, targetY, targetZ);

    const degToRad = Math.PI / 180.0;
    const elevationRad = pitch * degToRad;
    const yawRad = yaw * degToRad;
    const horizontalDist = distance * Math.cos(elevationRad);

    camera.position = Qt.vector3d(target.x + horizontalDist * Math.sin(yawRad), target.y + distance * Math.sin(elevationRad), target.z + horizontalDist * Math.cos(yawRad));
    camera.lookAt(target);
  }

  function resetView() {
    flyTo(Qt.vector3d(0, 100, 0), root.defaultDistance, root.defaultYaw, root.defaultPitch);
  }

  function zoomToFit(center, radius) {
    flyTo(center, Math.min(maxDistance, Math.max(minDistance, radius * 2.5)));
  }

  function lookAtPoint(point, zoomDistance) {
    flyTo(point, zoomDistance || Math.min(distance, 500));
  }

  function flyTo(newTarget, newDistance, newYaw, newPitch) {
    cameraAnimation.stop();
    cameraAnimation.toTargetX = newTarget.x;
    cameraAnimation.toTargetY = newTarget.y;
    cameraAnimation.toTargetZ = newTarget.z;
    cameraAnimation.toDistance = newDistance !== undefined ? newDistance : distance;
    cameraAnimation.toYaw = newYaw !== undefined ? newYaw : yaw;
    cameraAnimation.toPitch = newPitch !== undefined ? newPitch : pitch;
    cameraAnimation.start();
  }

  function clampPitch(value) {
    return Math.max(minPitch, Math.min(maxPitch, value));
  }

  function clampDistance(value) {
    return Math.max(minDistance, Math.min(maxDistance, value));
  }

  function applyPan(dx, dy) {
    const s = root.distance * 0.0025;
    const yawRad = -root.yaw * Math.PI / 180.0;
    root.targetX -= (dx * s * Math.cos(yawRad) - dy * s * Math.sin(yawRad));
    root.targetZ -= (dx * s * Math.sin(yawRad) + dy * s * Math.cos(yawRad));
  }

  ParallelAnimation {
    id: cameraAnimation

    property real toTargetX: 0
    property real toTargetY: 100
    property real toTargetZ: 0
    property real toDistance: root.defaultDistance
    property real toYaw: root.defaultYaw
    property real toPitch: root.defaultPitch

    NumberAnimation {
      target: root
      property: "targetX"
      to: cameraAnimation.toTargetX
      duration: 500
      easing.type: Easing.InOutCubic
    }
    NumberAnimation {
      target: root
      property: "targetY"
      to: cameraAnimation.toTargetY
      duration: 500
      easing.type: Easing.InOutCubic
    }
    NumberAnimation {
      target: root
      property: "targetZ"
      to: cameraAnimation.toTargetZ
      duration: 500
      easing.type: Easing.InOutCubic
    }
    NumberAnimation {
      target: root
      property: "distance"
      to: cameraAnimation.toDistance
      duration: 500
      easing.type: Easing.InOutCubic
    }
    NumberAnimation {
      target: root
      property: "yaw"
      to: cameraAnimation.toYaw
      duration: 500
      easing.type: Easing.InOutCubic
    }
    NumberAnimation {
      target: root
      property: "pitch"
      to: cameraAnimation.toPitch
      duration: 500
      easing.type: Easing.InOutCubic
    }
  }

  onTargetXChanged: updateCameraPosition()
  onTargetYChanged: updateCameraPosition()
  onTargetZChanged: updateCameraPosition()
  onYawChanged: updateCameraPosition()
  onPitchChanged: updateCameraPosition()
  onDistanceChanged: updateCameraPosition()

  TapHandler {
    id: tapHandler
    target: null
    acceptedButtons: Qt.LeftButton
    gesturePolicy: TapHandler.WithinBounds
    onDoubleTapped: resetView()
    onSingleTapped: function (eventPoint, button) {
      root.singleTapped(eventPoint.position.x, eventPoint.position.y);
    }
  }

  DragHandler {
    id: orbitDragHandler
    target: null
    acceptedButtons: Qt.LeftButton
    acceptedDevices: PointerDevice.Mouse | PointerDevice.Stylus | PointerDevice.TouchPad
    acceptedModifiers: Qt.NoModifier

    property var lastPoint

    onActiveChanged: {
      if (active) {
        lastPoint = centroid.position;
        root.userInteractionStarted();
      }
    }

    onCentroidChanged: {
      if (active) {
        const dx = centroid.position.x - lastPoint.x;
        const dy = centroid.position.y - lastPoint.y;

        root.yaw -= dx * orbitSensitivity;
        root.pitch = clampPitch(pitch + dy * orbitSensitivity);

        lastPoint = centroid.position;
      }
    }
  }

  DragHandler {
    id: panDragHandler
    target: null
    acceptedButtons: Qt.RightButton
    acceptedDevices: PointerDevice.Mouse | PointerDevice.Stylus | PointerDevice.TouchPad
    acceptedModifiers: Qt.NoModifier

    property var lastPoint

    onActiveChanged: {
      if (active) {
        lastPoint = centroid.position;
        root.userInteractionStarted();
      }
    }

    onCentroidChanged: {
      if (active) {
        applyPan(centroid.position.x - lastPoint.x, centroid.position.y - lastPoint.y);
        lastPoint = centroid.position;
      }
    }
  }

  DragHandler {
    id: touchOrbitHandler
    target: null
    acceptedButtons: Qt.NoButton | Qt.LeftButton
    acceptedDevices: PointerDevice.TouchScreen
    dragThreshold: 5
    grabPermissions: PointerHandler.CanTakeOverFromHandlersOfDifferentType | PointerHandler.ApprovesTakeOverByHandlersOfDifferentType

    property var lastPoint

    onActiveChanged: {
      if (active) {
        lastPoint = centroid.position;
        root.userInteractionStarted();
      }
    }

    onCentroidChanged: {
      if (active) {
        const dx = centroid.position.x - lastPoint.x;
        const dy = centroid.position.y - lastPoint.y;

        root.yaw -= dx * orbitSensitivity;
        root.pitch = clampPitch(pitch + dy * orbitSensitivity);

        lastPoint = centroid.position;
      }
    }
  }

  PinchHandler {
    id: pinchHandler
    target: null
    acceptedButtons: Qt.NoButton | Qt.LeftButton
    grabPermissions: PointerHandler.CanTakeOverFromHandlersOfDifferentType | PointerHandler.ApprovesTakeOverByHandlersOfDifferentType
    acceptedDevices: PointerDevice.TouchScreen
    dragThreshold: 5

    property real rotationThreshold: 5.0

    property var oldPos
    property real oldScale: 1.0
    property real oldRotation: 0.0
    property bool rotationThresholdReached: false

    onActiveChanged: {
      if (active) {
        oldScale = 1.0;
        oldRotation = 0.0;
        rotationThresholdReached = false;
        oldPos = centroid.position;
        root.userInteractionStarted();
      }
    }

    onCentroidChanged: {
      if (active) {
        const previousPos = oldPos;
        oldPos = centroid.position;
        applyPan(centroid.position.x - previousPos.x, centroid.position.y - previousPos.y);
      }
    }

    onActiveScaleChanged: {
      if (active) {
        root.distance = clampDistance(root.distance * (oldScale / pinchHandler.activeScale));
        oldScale = pinchHandler.activeScale;
      }
    }

    onRotationChanged: {
      if (active) {
        if (rotationThresholdReached) {
          root.yaw += (rotation - oldRotation);
          oldRotation = rotation;
        } else if (Math.abs(rotation - oldRotation) > rotationThreshold) {
          oldRotation = rotation;
          rotationThresholdReached = true;
        }
      }
    }
  }

  WheelHandler {
    id: wheelHandler
    target: null
    acceptedDevices: PointerDevice.AllDevices

    onWheel: function (event) {
      root.userInteractionStarted();
      root.distance = clampDistance(distance - event.angleDelta.y * 0.5);
    }
  }
}
```


