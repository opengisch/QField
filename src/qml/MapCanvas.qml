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

import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQml 2.2

import org.qgis 1.0

Item {
  id: mapArea
  property alias mapSettings: mapCanvasWrapper.mapSettings
  property alias isRendering: mapCanvasWrapper.isRendering
  property alias incrementalRendering: mapCanvasWrapper.incrementalRendering

  property bool mouseAsTouchScreen: qfieldSettings.mouseAsTouchScreen
  property bool freehandDigitizing: false

  // for signals, type can be "stylus" for any device click or "touch"

  //! This signal is emitted independently of double tap / click
  signal clicked(var point, var type)

  //! This signal is only emitted if there is no double tap/click coming after a short delay
  signal confirmedClicked(var point)

  signal longPressed(var point, var type)

  //! Emitted when a release happens after a long press
  signal longPressReleased(var type)

  signal panned

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

    // stylus clicks
    TapHandler {
      enabled: !mouseAsTouchScreen
      acceptedDevices: PointerDevice.AllDevices & ~PointerDevice.TouchScreen
      property bool longPressActive: false

      onSingleTapped: {
        mapArea.clicked(point.position, "stylus")
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

    // touch clicked & zoom in and out
    TapHandler {
        id: tapHandler
        acceptedDevices: mouseAsTouchScreen ? PointerDevice.AllDevices : PointerDevice.TouchScreen
        property bool longPressActive: false
        property bool doublePressed: false
        property var timer: Timer {
            property var tapPoint
            interval: 350
            repeat: false

            onTriggered: {
                confirmedClicked(tapPoint)
            }
        }

        onSingleTapped: {
            if( point.modifiers === Qt.RightButton)
            {
              mapCanvasWrapper.zoom(point.position, 1.25)
            }
            else
            {
              timer.tapPoint = point.position
              timer.restart()
            }
        }

        onDoubleTapped: {
            mapCanvasWrapper.zoom(point.position, 0.8)
        }

        onLongPressed: {
            mapArea.longPressed(point.position, "touch")
            longPressActive = true
        }

        onPressedChanged: {
            if ( pressed && timer.running )
            {
                timer.stop()
                doublePressed = true
                dragHandler.grabPermissions = PointerHandler.CanTakeOverFromItems | PointerHandler.CanTakeOverFromHandlersOfDifferentType | PointerHandler.ApprovesTakeOverByAnything
            }
            else
            {
                doublePressed = false
                dragHandler.grabPermissions = PointerHandler.CanTakeOverFromHandlersOfDifferentType | PointerHandler.ApprovesTakeOverByItems
            }

            if (longPressActive)
                mapArea.longPressReleased("touch")
            longPressActive = false
        }
    }

    DragHandler {
        id: dragHandler
        target: null
        enabled: !freehandDigitizing
        grabPermissions: PointerHandler.CanTakeOverFromHandlersOfDifferentType | PointerHandler.ApprovesTakeOverByItems

        property var oldPos
        property real oldTranslationY

        property bool isZooming: false
        property point zoomCenter

        onActiveChanged: {
            if ( active )
            {
                if ( tapHandler.doublePressed )
                {
                    oldTranslationY = 0;
                    zoomCenter = centroid.position;
                    isZooming = true;
                    freeze('zoom');
                }
                else
                {
                    freeze('pan');
                }
            }
            else
            {
                unfreeze(isZooming ? 'zoom' : 'pan');
                isZooming = false;
            }
        }

        onCentroidChanged: {
            var oldPos1 = oldPos;
            oldPos = centroid.position;
            if ( active )
            {
                if ( isZooming )
                {
                    mapCanvasWrapper.zoom(zoomCenter, Math.pow(0.8, (translation.y - oldTranslationY)/60))
                    oldTranslationY = translation.y
                }
                else
                {
                    mapCanvasWrapper.pan(centroid.position, oldPos1)
                    panned()
                }
            }
        }
    }

    DragHandler {
        target: null
        acceptedDevices: PointerDevice.Stylus | PointerDevice.Mouse
        grabPermissions: PointerHandler.TakeOverForbidden
        acceptedButtons: Qt.MiddleButton | Qt.RightButton

        property real oldTranslationY
        property point zoomCenter

        onActiveChanged: {
            if (active)
            {
                oldTranslationY = 0
                zoomCenter = centroid.position
            }

            if ( active )
                freeze('zoom')
            else
                unfreeze('zoom')
        }

        onTranslationChanged: {
            if (active)
            {
              mapCanvasWrapper.zoom(zoomCenter, Math.pow(0.8, (oldTranslationY - translation.y)/60))
            }

            oldTranslationY = translation.y
        }
    }

    PinchHandler {
        id: pinch
        target: null
        acceptedDevices: PointerDevice.TouchScreen | PointerDevice.TouchPad
        grabPermissions: PointerHandler.TakeOverForbidden

        property var oldPos
        property real oldScale: 1.0

        onActiveChanged: {
            if ( active ) {
                freeze('pinch')
                oldScale = 1.0
                oldPos = centroid.position
            } else {
                unfreeze('pinch')
            }
        }

        onCentroidChanged: {
            var oldPos1 = oldPos
            oldPos = centroid.position
            if ( active )
            {
                mapCanvasWrapper.pan(centroid.position, oldPos1)
                panned()
            }
        }

        onActiveScaleChanged: {
            mapCanvasWrapper.zoom( pinch.centroid.position, oldScale / pinch.activeScale )
            mapCanvasWrapper.pan( pinch.centroid.position, oldPos )
            panned()
            oldScale = pinch.activeScale
        }
    }


    WheelHandler {
        target: null
        grabPermissions: PointerHandler.CanTakeOverFromHandlersOfDifferentType | PointerHandler.ApprovesTakeOverByItems

        onWheel: {
            if ( event.angleDelta.y > 0 )
            {
                zoomIn(point.position)
            }
            else
            {
                zoomOut(point.position)
            }
        }
    }
}
