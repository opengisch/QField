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

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQml 2.15

import org.qgis 1.0

Item {
  id: mapArea
  property alias mapSettings: mapCanvasWrapper.mapSettings
  property alias isRendering: mapCanvasWrapper.isRendering
  property alias incrementalRendering: mapCanvasWrapper.incrementalRendering
  property alias quality: mapCanvasWrapper.quality
  property alias forceDeferredLayersRepaint: mapCanvasWrapper.forceDeferredLayersRepaint

  property bool interactive: true
  property bool hovered: false
  property bool pinched: pinchArea.isDragging
  property bool freehandDigitizing: false

  // for signals, type can be "stylus" for any device click or "touch"

  //! This signal is emitted independently of double tap / click
  signal clicked(var point, var type)

  //! This signal is emitted when a right mouse/stylus click occurs
  signal rightClicked(var point, var type)

  //! This signal is only emitted if there is no double tap/click coming after a short delay
  signal confirmedClicked(var point)

  signal longPressed(var point, var type)

  //! Emitted when a release happens after a long press
  signal longPressReleased(var type)

  /**
   * Freezes the map canvas refreshes.
   *
   * In case of repeated geometry changes (animated resizes, pinch, pan...)
   * triggering refreshes all the time can cause severe performance impacts.
   *
   * If freeze is called, an internal counter is incremented and only when the
   * counter is 0, refreshes will happen.
   * It is therefore important to call freeze() and unfreeze() exactly the same
   * number of times.
   */
  function freeze(id) {
    mapCanvasWrapper.__freezecount[id] = true
    mapCanvasWrapper.freeze = true
  }

  function unfreeze(id) {
    delete mapCanvasWrapper.__freezecount[id]
    mapCanvasWrapper.freeze = Object.keys(mapCanvasWrapper.__freezecount).length !== 0
  }

  function zoomIn(point) {
    mapCanvasWrapper.zoom(point, 0.67)
  }

  function zoomOut(point) {
    mapCanvasWrapper.zoom(point, 1.5)
  }

  MapCanvasMap {
    id: mapCanvasWrapper

    width: mapArea.width
    height: mapArea.height

    property var __freezecount: ({})

    freeze: false
  }

  PinchArea {
    id: pinchArea
    enabled: interactive
    anchors.fill: parent

    property bool isDragging: false

    property real rotationTreshold: 20.0
    property bool rotationTresholdReached: false
    property real previousRotation: 0

    onPinchStarted: {
      freeze('pinch')
      isDragging = true
      previousRotation = 0
      rotationTresholdReached = false
    }
    onPinchFinished: {
      isDragging = false
      unfreeze('pinch')
    }

    onPinchUpdated: (pinch) => {
                      mapCanvasWrapper.pan(pinch.center, pinch.previousCenter)
                      mapCanvasWrapper.zoom(pinch.center, pinch.previousScale / pinch.scale)
                      if (rotationTresholdReached) {
                        mapCanvasWrapper.rotate(pinch.rotation - previousRotation)
                        previousRotation = pinch.rotation
                      } else if (Math.abs(pinch.rotation - previousRotation) > rotationTreshold) {
                        rotationTresholdReached = true
                      }
                    }
  }

  TapHandler {
    id: stylusTapHandler
    enabled: interactive && !pinchArea.isDragging
    grabPermissions: PointerHandler.CanTakeOverFromHandlersOfDifferentType | PointerHandler.ApprovesTakeOverByAnything | PointerHandler.ApprovesCancellation
    acceptedDevices: !qfieldSettings.mouseAsTouchScreen ? PointerDevice.Stylus | PointerDevice.Mouse : PointerDevice.Stylus
    acceptedButtons: Qt.LeftButton | Qt.RightButton

    property bool longPressActive: false

    onSingleTapped: (eventPoint, button) => {
                      if (button === undefined) {
                        button = eventPoint.event.button
                      }

                      if (button === Qt.RightButton) {
                        mapArea.rightClicked(point.position, "stylus")
                      } else {
                        mapArea.clicked(point.position, "stylus")
                      }
                    }

    onLongPressed: {
      mapArea.longPressed(point.position, "stylus")
      longPressActive = true
    }

    onPressedChanged: {
      if (longPressActive)
        mapArea.longPressReleased("stylus")
      longPressActive = false
    }
  }

  DragHandler {
    id: stylusDragHandler
    enabled: interactive && !freehandDigitizing && !pinchArea.isDragging
    target: null
    grabPermissions: PointerHandler.CanTakeOverFromHandlersOfDifferentType | PointerHandler.ApprovesTakeOverByHandlersOfDifferentType
    acceptedDevices: !qfieldSettings.mouseAsTouchScreen ? PointerDevice.Stylus | PointerDevice.Mouse : PointerDevice.Stylus
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
          oldTranslationY = 0
          zoomCenter = centroid.position
          isZooming = true
          freeze('zoom')
        } else {
          oldPos = centroid.position
          isPanning = true
          freeze('pan')
        }
      } else {
        if (isZooming || isPanning) {
          unfreeze(isZooming ? 'zoom' : 'pan')
        }
        isZooming = false
        isPanning = false
      }
    }

    onCentroidChanged: {
      if (active) {
        if (isZooming) {
          mapCanvasWrapper.zoom(zoomCenter, Math.pow(0.8, (translation.y - oldTranslationY) / 60))
          oldTranslationY = translation.y
        } else if (isPanning) {
          mapCanvasWrapper.pan(centroid.position, oldPos)
          oldPos = centroid.position
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
      mainTapHandler.doublePressed = false
      confirmedClicked(tapPoint)
    }
  }

  TapHandler {
    id: mainTapHandler
    enabled: interactive && !hovered && !pinchArea.isDragging
    acceptedButtons: Qt.NoButton | Qt.LeftButton | Qt.RightButton
    grabPermissions: PointerHandler.CanTakeOverFromHandlersOfDifferentType | PointerHandler.ApprovesTakeOverByHandlersOfDifferentType
    acceptedDevices: !qfieldSettings.mouseAsTouchScreen ? PointerDevice.TouchScreen : PointerDevice.TouchScreen | PointerDevice.Mouse

    property bool longPressActive: false
    property bool doublePressed: false

    onLongPressed: {
                     mapArea.longPressed(Qt.point(point.position.x, point.position.y), "touch")
                     longPressActive = true
                   }

    onPressedChanged: {
                 if (pressed) {
                   if (point.pressedButtons !== Qt.RightButton) {
                     if (timer.running) {
                       timer.stop()
                       doublePressed = true
                     } else {
                       doublePressed = false
                     }
                   }
                 }
               }

    onTapped: (eventPoint, button) => {
                if (button === Qt.RightButton) {
                  mapArea.rightClicked(Qt.point(mouse.x, mouse.y), "touch")
                } else {
                  if (!doublePressed) {
                    timer.tapPoint = Qt.point(eventPoint.position.x, eventPoint.position.y)
                    timer.restart()
                  } else {
                    mapCanvasWrapper.zoom(Qt.point(eventPoint.position.x, eventPoint.position.y), 0.8)
                  }
                }
              }

    onCanceled: {
      timer.stop()
    }
  }

  DragHandler {
    id: mainDragHandler
    enabled: interactive && !hovered && !freehandDigitizing && !pinchArea.isDragging
    target: null
    acceptedButtons: Qt.NoButton | Qt.LeftButton
    grabPermissions: PointerHandler.CanTakeOverFromHandlersOfDifferentType | PointerHandler.ApprovesTakeOverByHandlersOfDifferentType
    acceptedDevices: !qfieldSettings.mouseAsTouchScreen ? PointerDevice.TouchScreen : PointerDevice.TouchScreen | PointerDevice.Mouse
    dragThreshold: 5

    property var oldPos
    property real oldTranslationY

    property bool isZooming: false
    property bool isPanning: false
    property point zoomCenter

    onActiveChanged: {
      if (active) {
        if (mainTapHandler.doublePressed) {
          oldTranslationY = 0
          zoomCenter = centroid.position
          isZooming = true
          freeze('zoom')
        } else {
          oldPos = centroid.position
          isPanning = true
          freeze('pan')
        }
      } else {
        if (isZooming || isPanning) {
          unfreeze(isZooming ? 'zoom' : 'pan')
        }
        isZooming = false
        isPanning = false
      }
    }

    onCentroidChanged: {
      if (active) {
        if (isZooming) {
          mapCanvasWrapper.zoom(zoomCenter, Math.pow(0.8, (translation.y - oldTranslationY) / 60))
          oldTranslationY = translation.y
        } else if (isPanning) {
          mapCanvasWrapper.pan(centroid.position, oldPos)
          oldPos = centroid.position
        }
      }
    }
  }

  DragHandler {
    id: secondaryDragHandler
    target: null
    enabled: interactive && !pinchArea.isDragging
    grabPermissions: PointerHandler.CanTakeOverFromItems | PointerHandler.CanTakeOverFromHandlersOfDifferentType | PointerHandler.ApprovesTakeOverByAnything;
    acceptedDevices: PointerDevice.Stylus | PointerDevice.Mouse
    acceptedButtons: Qt.RightButton
    dragThreshold: 10

    property real oldTranslationY
    property point zoomCenter

    onActiveChanged: {
      if (active) {
        oldTranslationY = 0
        zoomCenter = centroid.position
        freeze('zoom')
      } else {
        grabPermissions: PointerHandler.TakeOverForbidden
        unfreeze('zoom')
      }
    }

    onTranslationChanged: {
      if (active) {
        mapCanvasWrapper.zoom(zoomCenter, Math.pow(0.8, (oldTranslationY - translation.y)/60))
      }

      oldTranslationY = translation.y
    }
  }

  DragHandler {
    target: null
    enabled: interactive && !pinchArea.isDragging
    acceptedDevices: PointerDevice.Stylus | PointerDevice.Mouse
    acceptedModifiers: Qt.NoModifier
    grabPermissions: PointerHandler.TakeOverForbidden
    acceptedButtons: Qt.MiddleButton

    property var oldPos

    onActiveChanged: {
      if (active) {
        freeze('pan')
        oldPos = centroid.position
      } else {
        unfreeze('pan')
      }
    }

    onCentroidChanged: {
      if (active) {
        var oldPos1 = oldPos
        oldPos = centroid.position
        mapCanvasWrapper.pan(centroid.position, oldPos1)
      }
    }
  }

  DragHandler {
    target: null
    enabled: interactive && !pinchArea.isDragging
    acceptedDevices: PointerDevice.Stylus | PointerDevice.Mouse
    acceptedModifiers: Qt.ShiftModifier
    grabPermissions: PointerHandler.TakeOverForbidden
    acceptedButtons: Qt.MiddleButton

    property real oldTranslationY: 0
    property bool translationThresholdReached: false

    onActiveChanged: {
      if (active) {
        freeze('rotate')
        oldTranslationY = 0
        translationThresholdReached = false
      } else {
        unfreeze('rotate')
      }
    }

    onTranslationChanged: {
      if (active) {
        if (translationThresholdReached) {
          if (oldTranslationY != 0) {
            mapCanvasWrapper.rotate(oldTranslationY - translation.y)
          }
          oldTranslationY = translation.y
          translationThresholdReached = true
        } else if (Math.abs(oldTranslationY - translation.y) > pinchArea.rotationTreshold) {
          oldTranslationY = translation.y
          translationThresholdReached = true
        }
      }
    }
  }

  WheelHandler {
    enabled: interactive && !pinchArea.isDragging
    target: null
    grabPermissions: PointerHandler.CanTakeOverFromHandlersOfDifferentType | PointerHandler.ApprovesTakeOverByItems

    onWheel: (event) => {
               if (event.angleDelta.y > 0) {
                 zoomIn(point.position)
               } else {
                 zoomOut(point.position)
               }
             }
  }
}
