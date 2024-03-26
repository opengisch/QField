import QtQuick 2.15

import Theme 1.0
import org.qfield 1.0

Item {
  id: sketcher

  ImageCanvas {
    id: imageCanvas
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
        imageCanvas.pan(oldPosition, centroid.position)
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
    //imageCanvas.createCanvasFromImage('/home/webmaster/Desktop/2024-03-21-00 00_2024-03-21-23 59_Landsat_8-9_L1_True_color.jpg')
    imageCanvas.createBlankCanvas(800, 800, "#FFFFFF")
  }
}
