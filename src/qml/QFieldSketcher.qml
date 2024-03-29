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

  width: mainWindow.width
  height: mainWindow.height
  x: 0
  y: 0
  z: 10000 // 1000s are embedded feature forms, use a higher value to insure feature form popups always show above embedded feature formes
  padding: 0

  closePolicy: Popup.CloseOnEscape
  dim: true

  Settings {
    id: settings
    property color strokeColor: "#000000"
  }

  Page {
    width: parent.width
    height: parent.height
    padding: 0

    DrawingCanvas {
      id: drawingCanvas
      anchors.fill: parent

      fillColor: Theme.mainBackgroundColor
      frameColor: Theme.mainColor

      Behavior on zoomFactor {
        enabled: !pinchHandler.active
        NumberAnimation { duration: 100; easing.type: Easing.OutQuad; }
      }
    }

    PinchHandler {
      id: pinchHandler
      enabled: sketcher.visible
      target: null

      property point oldPosition

      onScaleChanged: (delta) => {
                        if (active) {
                          drawingCanvas.zoomFactor = drawingCanvas.zoomFactor * delta
                        }
                      }
      onTranslationChanged: (delta) => {
                              if (active) {
                                drawingCanvas.pan(Qt.point(0, 0), Qt.point(delta.x, delta.y))
                              }
                            }
    }

    DragHandler {
      id: dragHandler
      enabled: sketcher.visible
      target: null
      acceptedButtons: Qt.NoButton | Qt.LeftButton | Qt.RightButton
      dragThreshold: 0
      grabPermissions: PointerHandler.CanTakeOverFromHandlersOfDifferentType | PointerHandler.ApprovesTakeOverByAnything

      property point oldPosition

      onActiveChanged: {
        if (active) {
          if (centroid.pressedButtons !== Qt.RightButton) {
            drawingCanvas.strokeBegin(centroid.position, settings.strokeColor)
          } else {
            oldPosition = centroid.position
          }
        } else {
          drawingCanvas.strokeEnd(centroid.position)
        }
      }

      onCentroidChanged: {
        if (active) {
          if (centroid.pressedButtons === Qt.RightButton) {
            drawingCanvas.pan(oldPosition, centroid.position)
            oldPosition = centroid.position
          } else {
            drawingCanvas.strokeMove(centroid.position)
          }
        }
      }
    }

    WheelHandler {
      enabled: sketcher.visible
      target: null

      onWheel: (event) => { drawingCanvas.zoomFactor = drawingCanvas.zoomFactor * (event.angleDelta.y > 0 ? 1.25 : 0.75) }
    }

    RowLayout {
      anchors.horizontalCenter: parent.horizontalCenter
      anchors.bottom: parent.bottom
      anchors.bottomMargin: mainWindow.sceneBottomMargin + 5
      spacing: 3

      Repeater {
        model: [["#000000","#ffffff"], ["#ffffff","#000000"], ["#e41a1c","#e41a1c"], ["#377eb8","#377eb8"], ["#4daf4a","#4daf4a"]]

        QfToolButton {
          property color colorValue: modelData[0]

          width: 48
          height: 48
          round: true
          borderColor: modelData[1]
          scale: settings.strokeColor == colorValue ? 1 : 0.66
          opacity: settings.strokeColor == colorValue ? 1 : 0.66

          Behavior on scale {
            enabled: true
            NumberAnimation { duration: 200; easing.type: Easing.OutQuad; }
          }
          Behavior on opacity {
            enabled: true
            NumberAnimation { duration: 200; easing.type: Easing.OutQuad; }
          }

          bgcolor: colorValue

          onClicked: {
            settings.strokeColor = colorValue
          }
        }
      }
    }

    QfToolButton {
      id: backButton

      anchors.left: parent.left
      anchors.leftMargin: 5
      anchors.top: parent.top
      anchors.topMargin: mainWindow.sceneTopMargin + 5

      iconSource: Theme.getThemeIcon("ic_chevron_left_white_24dp")
      iconColor: "white"
      bgcolor: Theme.darkGraySemiOpaque
      round: true

      onClicked: {
        sketcher.cancelled()
        sketcher.close()
      }
    }

    QfToolButton {
      id: saveButton

      anchors.right: parent.right
      anchors.rightMargin: 5
      anchors.top: parent.top
      anchors.topMargin: mainWindow.sceneTopMargin + 5

      iconSource: Theme.getThemeIcon( 'ic_check_white_48dp' )
      iconColor: "white"
      bgcolor: drawingCanvas.isDirty ? Theme.mainColor : Theme.darkGraySemiOpaque
      round: true

      onClicked: {
        sketcher.finished(drawingCanvas.save())
        sketcher.close()
      }
    }
  }

  function loadImage(path) {
    drawingCanvas.createCanvasFromImage(path)
  }
}
