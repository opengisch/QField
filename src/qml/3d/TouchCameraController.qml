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

  // Animated target for smooth camera movement
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

    // Update target from animated values
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
    acceptedButtons: Qt.LeftButton
    gesturePolicy: TapHandler.WithinBounds
    onDoubleTapped: resetView()
    onSingleTapped: function (eventPoint, button) {
      root.singleTapped(eventPoint.position.x, eventPoint.position.y);
    }
  }

  DragHandler {
    acceptedButtons: Qt.LeftButton
    acceptedDevices: PointerDevice.AllDevices
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
        updateCameraPosition();
      }
    }
  }

  DragHandler {
    acceptedButtons: Qt.RightButton
    acceptedDevices: PointerDevice.AllDevices
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
        const panScale = root.distance * 0.0025;
        let dx = (centroid.position.x - lastPoint.x) * panScale;
        let dy = (centroid.position.y - lastPoint.y) * panScale;

        const yawRad = -root.yaw * Math.PI / 180.0;
        root.targetX -= (dx * Math.cos(yawRad) - dy * Math.sin(yawRad));
        root.targetZ -= (dx * Math.sin(yawRad) + dy * Math.cos(yawRad));

        lastPoint = centroid.position;
        updateCameraPosition();
      }
    }
  }

  // Pinch to zoom (touch devices)
  PinchHandler {
    id: pinchHandler
    target: null

    property real lastScale: 1.0
    property real lastRotation: 0.0

    onActiveChanged: {
      if (active) {
        lastScale = 1.0;
        root.userInteractionStarted();
      }
    }

    onActiveScaleChanged: {
      if (active) {
        const scaleDelta = scale / lastScale;
        root.distance = clampDistance(root.distance / scaleDelta);
        lastScale = scale;
        updateCameraPosition();
      }
    }

    onRotationChanged: {
      if (active) {
        root.yaw -= (rotation - oldRotation) * orbitSensitivity;
        lastRotation = rotation;
      }
    }

    // TODO: fix jankiness
    /*onTranslationChanged: function (delta) {
      // Pan with translation
      const panScale = root.distance * 0.0005;
      const yawRad = root.yaw * Math.PI / 180.0;

      // Right vector
      const rightX = Math.cos(yawRad);
      const rightZ = -Math.sin(yawRad);

      root.targetX -= delta.x * panScale * rightX;
      root.targetZ -= delta.x * panScale * rightZ;
      root.targetY += delta.y * panScale;

      updateCameraPosition();
    }*/
  }

  WheelHandler {
    acceptedDevices: PointerDevice.AllDevices

    onWheel: function (event) {
      root.userInteractionStarted();
      root.distance = clampDistance(distance - event.angleDelta.y * 0.5);
      updateCameraPosition();
    }
  }
}
