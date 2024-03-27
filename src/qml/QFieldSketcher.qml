import QtQuick 2.15
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import Qt.labs.settings 1.0

import Theme 1.0
import org.qfield 1.0

Popup {
  id: sketcher

  signal finished(var path)
  signal cancelled()

  width: mainWindow.width - Theme.popupScreenEdgeMargin
  height: mainWindow.height - Theme.popupScreenEdgeMargin * 2
  x: (parent.width - width) / 2
  y: (parent.height - height) / 2
  z: 10000 // 1000s are embedded feature forms, use a higher value to insure feature form popups always show above embedded feature formes
  padding: 0

  closePolicy: Popup.CloseOnEscape
  dim: true

  Page {
    width: parent.width
    height: parent.height
    padding: 0
    header: PageHeader {
      id: pageHeader
      title: qsTr("Sketcher")

      showBackButton: false
      showApplyButton: true
      showCancelButton: true

      onCancel: {
        sketcher.cancelled()
        sketcher.close()
      }

      onApply: {
        sketcher.finished(drawingCanvas.save())
        sketcher.close()
      }
    }

    ColumnLayout {
      width: parent.width
      height: parent.height

      DrawingCanvas {
        id: drawingCanvas
        Layout.fillWidth: true
        Layout.fillHeight: true

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
            drawingCanvas.strokeBegin(centroid.position, "#ffffff")
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
    }
  }

  function loadImage(path) {
    drawingCanvas.createCanvasFromImage(path)
  }
}
