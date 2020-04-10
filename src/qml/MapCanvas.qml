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

import QtQuick 2.12
import QtQuick.Controls 1.2
import QtQml 2.2
import org.qgis 1.0

Item {
  id: mapArea
  property alias mapSettings: mapCanvasWrapper.mapSettings
  property alias isRendering: mapCanvasWrapper.isRendering
  property alias incrementalRendering: mapCanvasWrapper.incrementalRendering

  // for signals, type can be "stylus" for any device click or "touch"

  //! This signal is emitted independently of an upcoming doubleClicked
  signal clicked(var point, var type)

  //! This signal is only emitted if there is no doubleClicked coming. It is emitted with a delay of mouseDoubleClickInterval
  signal confirmedClicked(var point)

  signal doubleClicked(var point)

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

  MapCanvasMap {
    id: mapCanvasWrapper

    anchors.fill: parent

    property var __freezecount: ({})

    freeze: false
  }

    TapHandler {
      grabPermissions: PointerHandler.ApprovesTakeOverByAnything

      property var timer: Timer {
          property var firstClickPoint
          interval: mouseDoubleClickInterval
          repeat: false

          onTriggered: {
              confirmedClicked(firstClickPoint)
          }
      }

      onTapCountChanged: {
          if (tapCount == 1) {
              timer.firstClickPoint = point.position
              timer.restart()
          }
          else if (tapCount == 2) {
              timer.stop()
              doubleClicked(point)
          }
      }

    }

    // stylus clicked
    TapHandler {
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

    // touch clicked
    TapHandler {
      acceptedDevices: PointerDevice.TouchScreen
      property bool longPressActive: false

      onSingleTapped: {
        mapArea.clicked(point.position, "touch")
      }

      onLongPressed: {
          mapArea.longPressed(point.position, "touch")
          longPressActive = true
      }

      onPressedChanged: {
          if (longPressActive)
              mapArea.longPressReleased("touch")
          longPressActive = false
      }
    }

    // zoom in/out on finger tap
    TapHandler {
        grabPermissions: PointerHandler.CanTakeOverFromItems
        acceptedDevices: PointerDevice.TouchScreen

        onSingleTapped: {
            var factor = point.modifiers === Qt.RightButton ? 1.25 : 0.8
            mapCanvasWrapper.zoom(point.position, factor)
        }
    }

    DragHandler {
        target: null
        grabPermissions: PointerHandler.TakeOverForbidden

        property var oldPos

        onActiveChanged: {
            if ( active )
                freeze('pan')
            else
                unfreeze('pan')
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
    }

    DragHandler {
        target: null
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
                mapCanvasWrapper.zoom(zoomCenter, Math.pow(0.8, (oldTranslationY - translation.y)/60))

            oldTranslationY = translation.y
        }
    }

    PinchHandler {
        id: pinch
        target: null
        grabPermissions: PointerHandler.TakeOverForbidden

        property var oldPos
        property real oldScale: 1.0

        onActiveChanged: {
            if ( active )
                freeze('pinch')
            else
                unfreeze('pinch')
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
            mapArea.panned()
            oldScale = pinch.activeScale
        }
    }

    // TODO add WheelHandler once we can expect Qt 5.14 on all platforms
}
