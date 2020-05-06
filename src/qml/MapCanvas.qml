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

import QtQml 2.2
import QtQuick 2.12
import QtQuick.Controls 2.12
import org.qgis 1.0

Item {
    // for signals, type can be "stylus" for any device click or "touch"
    // signal doubleClicked(var point)
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
    //    TapHandler {
    //      grabPermissions: PointerHandler.ApprovesTakeOverByAnything
    //      property var timer: Timer {
    //          property var firstClickPoint
    //          interval: mouseDoubleClickInterval
    //          repeat: false
    //          onTriggered: {
    //              confirmedClicked(firstClickPoint)
    //          }
    //      }
    //      onTapCountChanged: {
    //          if (tapCount == 1) {
    //              timer.firstClickPoint = point.position
    //              timer.restart()
    //          }
    //          else if (tapCount == 2) {
    //              timer.stop()
    //              doubleClicked(point)
    //          }
    //      }
    //    }
    // TODO add WheelHandler once we can expect Qt 5.14 on all platforms

    id: mapArea

    property alias mapSettings: mapCanvasWrapper.mapSettings
    property alias isRendering: mapCanvasWrapper.isRendering
    property alias incrementalRendering: mapCanvasWrapper.incrementalRendering
    property bool mouseAsTouchScreen: qfieldSettings.mouseAsTouchScreen

    //! This signal is emitted independently of an upcoming doubleClicked
    signal clicked(var point, var type)
    //! This signal is only emitted if there is no doubleClicked coming. It is emitted with a delay of mouseDoubleClickInterval
    signal confirmedClicked(var point)
    signal longPressed(var point, var type)
    //! Emitted when a release happens after a long press
    signal longPressReleased(var type)
    signal panned()

    function freeze(id) {
        mapCanvasWrapper.__freezecount[id] = true;
        mapCanvasWrapper.freeze = true;
    }

    function unfreeze(id) {
        delete mapCanvasWrapper.__freezecount[id];
        mapCanvasWrapper.freeze = Object.keys(mapCanvasWrapper.__freezecount).length !== 0;
    }

    MapCanvasMap {
        id: mapCanvasWrapper

        property var __freezecount: ({
        })

        width: mapArea.width
        height: mapArea.height
        freeze: false
    }

    // stylus clicked
    TapHandler {
        property bool longPressActive: false

        enabled: !mouseAsTouchScreen
        acceptedDevices: PointerDevice.AllDevices & ~PointerDevice.TouchScreen
        onSingleTapped: {
            mapArea.clicked(point.position, "stylus");
        }
        onLongPressed: {
            mapArea.longPressed(point.position, "stylus");
            longPressActive = true;
        }
        onPressedChanged: {
            if (longPressActive)
                mapArea.longPressReleased("stylus");

            longPressActive = false;
        }
    }

    // touch clicked
    TapHandler {
        property bool longPressActive: false

        acceptedDevices: mouseAsTouchScreen ? PointerDevice.AllDevices : PointerDevice.TouchScreen
        onSingleTapped: {
            mapArea.clicked(point.position, "touch");
        }
        onLongPressed: {
            mapArea.longPressed(point.position, "touch");
            longPressActive = true;
        }
        onPressedChanged: {
            if (longPressActive)
                mapArea.longPressReleased("touch");

            longPressActive = false;
        }
    }

    // zoom in/out on finger tap
    TapHandler {
        grabPermissions: PointerHandler.CanTakeOverFromItems
        acceptedDevices: mouseAsTouchScreen ? PointerDevice.AllDevices : PointerDevice.TouchScreen
        onSingleTapped: {
            if (point.modifiers === Qt.RightButton)
                mapCanvasWrapper.zoom(point.position, 1.25);

        }
        onDoubleTapped: {
            mapCanvasWrapper.zoom(point.position, 0.8);
        }
    }

    DragHandler {
        property var oldPos

        target: null
        grabPermissions: PointerHandler.TakeOverForbidden
        onActiveChanged: {
            if (active)
                freeze("pan");
            else
                unfreeze("pan");
        }
        onCentroidChanged: {
            var oldPos1 = oldPos;
            oldPos = centroid.position;
            if (active) {
                mapCanvasWrapper.pan(centroid.position, oldPos1);
                panned();
            }
        }
    }

    DragHandler {
        property real oldTranslationY
        property point zoomCenter

        target: null
        acceptedDevices: PointerDevice.Stylus | PointerDevice.Mouse
        grabPermissions: PointerHandler.TakeOverForbidden
        acceptedButtons: Qt.MiddleButton | Qt.RightButton
        onActiveChanged: {
            if (active) {
                oldTranslationY = 0;
                zoomCenter = centroid.position;
            }
            if (active)
                freeze("zoom");
            else
                unfreeze("zoom");
        }
        onTranslationChanged: {
            if (active)
                mapCanvasWrapper.zoom(zoomCenter, Math.pow(0.8, (oldTranslationY - translation.y) / 60));

            oldTranslationY = translation.y;
        }
    }

    PinchHandler {
        id: pinch

        property var oldPos
        property real oldScale: 1

        target: null
        acceptedDevices: PointerDevice.TouchScreen | PointerDevice.TouchPad
        grabPermissions: PointerHandler.TakeOverForbidden
        onActiveChanged: {
            if (active)
                freeze("pinch");
            else
                unfreeze("pinch");
        }
        onCentroidChanged: {
            var oldPos1 = oldPos;
            oldPos = centroid.position;
            if (active) {
                mapCanvasWrapper.pan(centroid.position, oldPos1);
                panned();
            }
        }
        onActiveScaleChanged: {
            mapCanvasWrapper.zoom(pinch.centroid.position, oldScale / pinch.activeScale);
            mapCanvasWrapper.pan(pinch.centroid.position, oldPos);
            mapArea.panned();
            oldScale = pinch.activeScale;
        }
    }

}
