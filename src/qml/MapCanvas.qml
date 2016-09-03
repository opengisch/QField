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

import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQml 2.2
import org.qgis 1.0

Item {
  id: mapArea
  property alias mapSettings: mapCanvasWrapper.mapSettings

  property int __freezecount: 0

  signal clicked(var mouse)

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
  function freeze() {
    __freezecount += 1
  }

  function unfreeze() {
    __freezecount -= 1
  }

  MapCanvasMap {
    id: mapCanvasWrapper

    anchors.fill: parent

    freeze: __freezecount != 0
  }

  PinchArea {
    id: pinchArea

    anchors.fill: parent

    onPinchStarted: {
      freeze()
    }

    onPinchUpdated: {
      mapCanvasWrapper.zoom( pinch.center, pinch.previousScale / pinch.scale )
      mapCanvasWrapper.pan( pinch.center, pinch.previousCenter )
    }

    onPinchFinished: {
      unfreeze()
      mapCanvasWrapper.refresh()
    }

    MouseArea {
      id: mouseArea

      property int __lastX: -1
      property int __lastY: -1
      property int __initDistance: -1

      anchors.fill : parent

      onDoubleClicked: {
        var center = Qt.point( mouse.x, mouse.y )
        mapCanvasWrapper.zoom( center, 0.8 )
        // mapCanvasWrapper.pan( pinch.center, pinch.previousCenter )
      }

      onClicked: {
        if ( mouse.button === Qt.RightButton )
        {
          var center = Qt.point( mouse.x, mouse.y )
          mapCanvasWrapper.zoom( center, 1.2 )
        }
        else
        {
          if ( __initDistance < 5 * dp)
            mapArea.clicked( mouse )
        }
      }

      onPressed: {
        __lastX = mouse.x
        __lastY = mouse.y
        __initDistance = 0
        freeze()
      }

      onReleased: {
        unfreeze()
      }

      onPositionChanged: {
        if ( !mapCanvasWrapper.freeze )
        {
          __initDistance = Math.abs( mouse.x - __lastX ) + Math.abs( mouse.y - __lastY )
        }

        if ( mapCanvasWrapper.freeze ) {
          mapCanvasWrapper.pan( Qt.point( mouse.x, mouse.y ), Qt.point( __lastX, __lastY ) )
          __lastX = mouse.x
          __lastY = mouse.y

        }
      }

      onCanceled: {
        unfreeze()
      }

      onWheel: {
        mapCanvasWrapper.zoom( Qt.point( wheel.x, wheel.y ), Math.pow( 0.8, wheel.angleDelta.y / 60 ) )
      }
    }
  }
}
