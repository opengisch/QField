import QtQuick 2.15

import Theme 1.0
import org.qfield 1.0

Item {
  id: sketcher

  DrawingCanvas {
    id: drawingCanvas
    anchors.fill: parent

    fillColor: "#cccccc"
    frameColor: Theme.mainColor

    Behavior on zoomFactor {
      enabled: true
      NumberAnimation { duration: 200; easing.type: Easing.OutQuad; }
    }
  }

  DragHandler {
    id: dragHandler
    enabled: sketcher.visible
    target: null
    acceptedButtons: Qt.NoButton | Qt.LeftButton | Qt.RightButton
    dragThreshold: 10

    property point oldPosition

    onActiveChanged: {
      if (active && centroid.pressedButtons === Qt.LeftButton) {
        drawingCanvas.strokeBegin(centroid.position)
      } else {
        drawingCanvas.strokeEnd(centroid.position)
      }
    }

    onCentroidChanged: {
      if (active) {
        if (centroid.pressedButtons === Qt.RightButton) {
          drawingCanvas.pan(oldPosition, centroid.position)
        } else {
          drawingCanvas.strokeMove(centroid.position)
        }
      }
      oldPosition = centroid.position
    }
  }

  WheelHandler {
    enabled: sketcher.visible
    target: null

    onWheel: (event) => { drawingCanvas.zoomFactor = drawingCanvas.zoomFactor * (event.angleDelta.y > 0 ? 1.25 : 0.75) }
  }

  Component.onCompleted: {
    drawingCanvas.createBlankCanvas(800, 800, "#FFFFFF")
  }
}
