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

  signal clicked(var point)
  signal doubleClicked(var point)
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
              clicked(firstClickPoint)
          }
      }

      onTapCountChanged: {
          if (tapCount == 1) {
              timer.firstClickPoint = point
              timer.running = true
          }
          else if (tapCount == 2) {
              timer.running = false
              doubleClicked(point)
          }
      }
    }

    TapHandler {
        grabPermissions: PointerHandler.CanTakeOverFromItems
        acceptedButtons: Qt.RightButton

        onSingleTapped: {
            mapCanvasWrapper.zoom(point.position, 1.2)
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
        target: null
        grabPermissions: PointerHandler.TakeOverForbidden

        property var oldPos

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
            mapCanvasWrapper.zoom( pinch.center, pinch.previousScale / pinch.scale )
            mapCanvasWrapper.pan( pinch.center, pinch.previousCenter )
            mapArea.panned()

        }
    }

    // TODO add WheelHandler once we can expect Qt 5.14 on all platforms
}
