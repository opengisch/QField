/***************************************************************************
                            MapCanvas.qml
                              -------------------
              begin                : 10.12.2014
              copyright            : (C) 2014 by Matthias Kuhn
              email                : matthias.kuhn (at) opengis.ch
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
  anchors.fill: parent
  id: mapArea
  property MapCanvasMap mapCanvasWrapper: mapCanvasWrapper

  MapCanvasMap {
    id: mapCanvasWrapper
    anchors.fill: parent
  }

  PinchArea {
    id: pinchArea

    anchors.fill: parent

    onPinchUpdated: {
      mapCanvasWrapper.zoom( pinch.center, pinch.previousScale / pinch.scale )
      mapCanvasWrapper.pan( pinch.center, pinch.previousCenter )
    }

    onPinchFinished: {
      mapCanvasWrapper.refresh()
    }

    MouseArea {
      id: mouseArea

      property bool __isPanning: false
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
      }

      onPressAndHold: {
        if ( !__isPanning )
        {
          iface.identifyFeatures( Qt.point( mouse.x, mouse.y ) )
        }
      }

      onPressed: {
        __lastX = mouse.x
        __lastY = mouse.y
        __initDistance = 0
      }

      onReleased: {
        if ( __isPanning )
        {
          __isPanning = false
          mapCanvasWrapper.refresh()
        }
      }

      onPositionChanged: {
        if ( !__isPanning )
        {
          __initDistance = Math.abs( mouse.x - __lastX ) + Math.abs( mouse.y - __lastY )

          if ( __initDistance > 10*dp )
            __isPanning = true
        }

        if ( __isPanning ) {
          mapCanvasWrapper.pan( Qt.point( mouse.x, mouse.y ), Qt.point( __lastX, __lastY ) )
          __lastX = mouse.x
          __lastY = mouse.y

        }
      }

      onCanceled: {
        // Does that ever happen on a touch device?
        __isPanning = false;
      }
    }
  }
}
