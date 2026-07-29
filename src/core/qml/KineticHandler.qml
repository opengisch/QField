import QtQuick

/**
 * Provides kinetic (inertia) scrolling for map pan and pinch-zoom gestures.
 *
 * Qt's pointer handlers only report instantaneous position, not velocity.
 * We sample finger positions over a short time window to compute release velocity.
 * After the gesture ends motion continues with exponential friction decay (~60fps timer).
 *
 * \ingroup qml
 */
Item {
  id: kineticHandler

  required property Item mapCanvas
  required property Item mapCanvasWrapper

  property var panSamples: []
  property var zoomSamples: []

  function addPanSample(x, y) {
    const now = Date.now();
    panSamples.push({
      x: x,
      y: y,
      time: now
    });
    while (panSamples.length > 1 && now - panSamples[0].time > 100) {
      panSamples.shift();
    }
  }

  function resetPanSamples() {
    panSamples = [];
  }

  function addZoomSample(scale) {
    const now = Date.now();
    zoomSamples.push({
      scale: scale,
      time: now
    });
    while (zoomSamples.length > 1 && now - zoomSamples[0].time > 150) {
      zoomSamples.shift();
    }
  }

  function resetZoomSamples() {
    zoomSamples = [];
  }

  property real panVx: 0
  property real panVy: 0
  property real panAccumX: 0
  property real panAccumY: 0
  property bool panRunning: false

  property real zoomVelocity: 1.0
  property point zoomCenter
  property bool zoomRunning: false

  function startPanInertia() {
    const now = Date.now();
    while (panSamples.length > 1 && now - panSamples[0].time > 100) {
      panSamples.shift();
    }

    if (panSamples.length < 2) {
      return;
    }

    const first = panSamples[0];
    const last = panSamples[panSamples.length - 1];
    const dt = last.time - first.time;
    if (dt <= 0) {
      return;
    }
    let vxMs = (last.x - first.x) / dt;
    let vyMs = (last.y - first.y) / dt;
    const speed = Math.sqrt(vxMs * vxMs + vyMs * vyMs);
    if (speed < 0.2) {
      return;
    }
    if (speed > 5.0) {
      const cap = 5.0 / speed;
      vxMs *= cap;
      vyMs *= cap;
    }
    panVx = vxMs * 16;
    panVy = vyMs * 16;
    panAccumX = 0;
    panAccumY = 0;
    panRunning = true;
    activate();
  }

  function startZoomInertia(center) {
    const now = Date.now();
    while (zoomSamples.length > 1 && now - zoomSamples[0].time > 150) {
      zoomSamples.shift();
    }

    if (zoomSamples.length < 2) {
      return;
    }

    const first = zoomSamples[0];
    const last = zoomSamples[zoomSamples.length - 1];
    const dt = last.time - first.time;
    if (dt <= 0) {
      return;
    }
    const ratio = last.scale / first.scale;
    const framesElapsed = dt / 16;
    const perFrameFactor = Math.pow(ratio, 1 / framesElapsed);
    if (Math.abs(perFrameFactor - 1.0) < 0.003) {
      return;
    }
    zoomVelocity = perFrameFactor;
    zoomCenter = center;
    zoomRunning = true;
    activate();
  }

  function activate() {
    if (!inertiaTimer.running) {
      mapCanvas.freeze('kinetic');
      inertiaTimer.start();
    }
  }

  function stopAll() {
    panRunning = false;
    zoomRunning = false;
    if (inertiaTimer.running) {
      inertiaTimer.stop();
      mapCanvas.unfreeze('kinetic');
    }
  }

  Timer {
    id: inertiaTimer
    interval: 16
    repeat: true

    onTriggered: {
      if (kineticHandler.panRunning) {
        kineticHandler.panAccumX += kineticHandler.panVx;
        kineticHandler.panAccumY += kineticHandler.panVy;
        const moveX = Math.round(kineticHandler.panAccumX);
        const moveY = Math.round(kineticHandler.panAccumY);
        if (moveX !== 0 || moveY !== 0) {
          const cx = kineticHandler.mapCanvas.width / 2;
          const cy = kineticHandler.mapCanvas.height / 2;
          kineticHandler.mapCanvasWrapper.pan(Qt.point(cx + moveX, cy + moveY), Qt.point(cx, cy));
          kineticHandler.panAccumX -= moveX;
          kineticHandler.panAccumY -= moveY;
        }
        kineticHandler.panVx *= 0.92;
        kineticHandler.panVy *= 0.92;
        if (kineticHandler.panVx * kineticHandler.panVx + kineticHandler.panVy * kineticHandler.panVy < 1.0) {
          kineticHandler.panRunning = false;
        }
      }

      if (kineticHandler.zoomRunning) {
        kineticHandler.mapCanvasWrapper.zoomByFactor(kineticHandler.zoomCenter, 1.0 / kineticHandler.zoomVelocity);
        kineticHandler.zoomVelocity = 1.0 + (kineticHandler.zoomVelocity - 1.0) * 0.92;
        if (Math.abs(kineticHandler.zoomVelocity - 1.0) < 0.001) {
          kineticHandler.zoomRunning = false;
        }
      }

      if (!kineticHandler.panRunning && !kineticHandler.zoomRunning) {
        inertiaTimer.stop();
        kineticHandler.mapCanvas.unfreeze('kinetic');
      }
    }
  }
}
