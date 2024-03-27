import QtQuick 2.15

import Theme 1.0
import org.qfield 1.0

Item {
  id: sketcher

  DrawingCanvas {
    id: drawingCanvas
    anchors.fill: parent

    fillColor: "#cccccc"

    Behavior on zoomFactor {
      enabled: true
      NumberAnimation { duration: 200; easing.type: Easing.OutQuad; }
    }
  }

  DragHandler {
    id: dragHandler
    enabled: sketcher.visible
    target: null
    acceptedButtons: Qt.NoButton | Qt.LeftButton
    dragThreshold: 10

    property point oldPosition

    onCentroidChanged: {
      if (active) {
        drawingCanvas.pan(oldPosition, centroid.position)
      }
      oldPosition = centroid.position
    }
  }

  WheelHandler {
    enabled: sketcher.visible
    target: null

    onWheel: (event) => { imageCanvas.zoomFactor = imageCanvas.zoomFactor * (event.angleDelta.y > 0 ? 1.25 : 0.75) }
  }

  Component.onCompleted: {
    drawingCanvas.createBlankCanvas(800, 800, "#FFFFFF")
  }
}
