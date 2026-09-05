

# File QgisMobileapp.qml

[**File List**](files.md) **>** [**app**](dir_5194e5fea318fda12687127c23f8aba1.md) **>** [**qml**](dir_408f7dc2ccecf14179dc3d37678eaa5b.md) **>** [**QgisMobileapp.qml**](QgisMobileapp_8qml.md)

[Go to the documentation of this file](QgisMobileapp_8qml.md)


```C++
/***************************************************************************
                            QgisMobileapp.qml
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
import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Controls.impl
import QtQuick.Controls.Material
import QtQuick.Controls.Material.impl
import QtQuick.Effects
import QtQuick.Shapes
import QtQuick.Window
import QtQml
import QtSensors
import org.qgis
import org.qfield.core
import org.qfield.gui
import org.qfield.app

ApplicationWindow {
  id: mainWindow
  objectName: 'mainWindow'
  visible: true
  flags: Qt.platform.os === "ios" || Qt.platform.os === "android" ? Qt.ExpandedClientAreaHint | Qt.NoTitleBarBackgroundHint : Qt.Window | Qt.WindowTitleHint | Qt.WindowSystemMenuHint | Qt.WindowMinMaxButtonsHint | Qt.WindowCloseButtonHint | (sceneBorderless ? Qt.FramelessWindowHint : 0)

  topPadding: 0
  bottomPadding: 0
  leftPadding: 0
  rightPadding: 0

  Material.theme: QfTheme.darkTheme ? Material.Dark : Material.Light
  Material.accent: QfTheme.mainColor

  property bool sceneLoaded: false
  property bool sceneBorderless: false
  property double sceneTopMargin: SafeArea.margins.top
  property double sceneBottomMargin: SafeArea.margins.bottom
  property double sceneLeftMargin: SafeArea.margins.left
  property double sceneRightMargin: SafeArea.margins.right

  onSceneLoadedChanged: {
    // This requires the scene to be fully loaded not to crash due to possibility of
    // a thread blocking permission request being thrown
    if (positioningSettings.positioningActivated) {
      positionSource.active = true;
    }
  }

  Settings {
    id: mainWindowSettings

    property real x: 0
    property real y: 0
    property real width: 300
    property real height: 300

    property string screenConfiguration: ''

    Component.onCompleted: {
      if (Qt.platform.os !== "ios" && Qt.platform.os !== "android") {
        let currentScreensConfiguration = `${Qt.application.screens.length}`;
        for (let screen of Qt.application.screens) {
          currentScreensConfiguration += `:${screen.width}x${screen.height}-${screen.virtualX}-${screen.virtualY}`;
        }
        const minimumSize = 300;
        if (screenConfiguration == '') {
          screenConfiguration = currentScreensConfiguration;
          x = mainWindow.x;
          y = mainWindow.y;
          width = Math.max(mainWindow.width, minimumSize);
          height = Math.max(mainWindow.height, minimumSize);
          if (mainWindow.width !== width) {
            mainWindow.width = width;
          }
          if (mainWindow.height !== height) {
            mainWindow.height = height;
          }
        } else if (screenConfiguration != currentScreensConfiguration) {
          screenConfiguration = currentScreensConfiguration;
          mainWindow.x = Math.min(x, mainWindow.screen.width - width);
          mainWindow.y = Math.min(y, mainWindow.screen.height - height);
          mainWindow.width = Math.max(width, minimumSize);
          mainWindow.height = Math.max(height, minimumSize);
        } else {
          mainWindow.x = x;
          mainWindow.y = y;
          mainWindow.width = Math.max(width, minimumSize);
          mainWindow.height = Math.max(height, minimumSize);
        }
      }
    }

    Component.onDestruction: {
      if (Qt.platform.os !== "ios" && Qt.platform.os !== "android") {
        mainWindowSettings.x = mainWindow.x;
        mainWindowSettings.y = mainWindow.y;
        mainWindowSettings.width = mainWindow.width;
        mainWindowSettings.height = mainWindow.height;
      }
    }
  }

  palette {
    link: QfTheme.mainColor
    linkVisited: QfTheme.mainColor
  }

  Connections {
    target: QfTheme

    function onDarkThemeChanged() {
      Application.styleHints.colorScheme = QfTheme.darkTheme ? Qt.ColorScheme.Dark : Qt.ColorScheme.Light;
    }
  }

  QfLocatorModelSuperBridge {
    id: locatorBridge
    objectName: "locatorBridge"

    activeLayer: dashBoard.activeLayer
    bookmarks: bookmarkModel
    featureListController: featureListForm.extentController
    mapSettings: mapCanvas.mapSettings
    navigation: navigation
    geometryHighlighter: geometryHighlighter.geometryWrapper
    keepScale: qfieldSettings.locatorKeepScale

    onMessageEmitted: {
      displayToast(text);
    }

    onRequestJumpToPoint: function (center, scale, handleMargins) {
      mapCanvasMap.jumpTo(center, scale, -1, handleMargins);
    }
  }

  //this keyHandler is because otherwise the back-key is not handled in the mainWindow. Probably this could be solved cuter.
  Item {
    id: keyHandler
    objectName: "keyHandler"

    visible: true
    focus: true

    Keys.onReleased: event => {
      if (event.modifiers === Qt.NoModifier) {
        if (event.key === Qt.Key_Back || event.key === Qt.Key_Escape) {
          if (featureListForm.visible) {
            featureListForm.hide();
          } else if (stateMachine.state === 'measure') {
            mainWindow.closeMeasureTool();
          } else {
            mainWindow.close();
          }
          event.accepted = true;
        }
      }
    }

    Component.onCompleted: focusstack.addFocusTaker(this)
  }

  Shortcut {
    property int previousVisibilityState: Window.Windowed
    enabled: Qt.platform.os !== "android" && Qt.platform.os !== "ios"
    sequence: "F11"
    onActivated: {
      if (mainWindow.visibility !== Window.FullScreen) {
        previousVisibilityState = mainWindow.visibility;
        mainWindow.visibility = Window.FullScreen;
      } else {
        mainWindow.visibility = Window.Windowed;
        if (previousVisibilityState === Window.Maximized) {
          mainWindow.showMaximized();
        }
      }
    }
  }

  Shortcut {
    enabled: Qt.platform.os !== "android" && Qt.platform.os !== "ios"
    sequence: "F12"
    onActivated: {
      mainWindow.sceneBorderless = !mainWindow.sceneBorderless;
      if (mainWindow.sceneBorderless) {
        displayToast(qsTr("Borderless mode activated, use the top left and botom right corner to move and resize the window"));
      }
    }
  }

  Shortcut {
    enabled: keyHandler.focus
    sequence: "Ctrl+K"
    onActivated: {
      locatorItem.state = "on";
    }
  }

  Shortcut {
    enabled: true
    sequence: "Ctrl+M"
    onActivated: {
      activateMeasurementMode();
    }
  }

  Shortcut {
    enabled: keyHandler.focus || welcomeScreen.focus
    sequence: "Ctrl+O"
    onActivated: {
      welcomeScreen.showLocalDataPicker();
    }
  }

  Shortcut {
    enabled: projectInfo.insertRights
    sequence: "Ctrl++"
    onActivated: {
      mainWindow.toggleDigitizeMode();
    }
  }

  Shortcut {
    enabled: keyHandler.focus && stateMachine.state === "digitize"
    sequence: "Ctrl+Space"
    onActivated: {
      digitizingToolbar.triggerAddVertex();
    }
  }

  //currentRubberband provides the rubberband depending on the current state (digitize or measure)
  property QfRubberband currentRubberband
  property QfLayerObserver layerObserverAlias: layerObserver
  property QgsGpkgFlusher gpkgFlusherAlias: gpkgFlusher

  signal closeMeasureTool
  signal close3DView

  signal changeMode(string mode)
  signal toggleDigitizeMode

  Item {
    id: stateMachine
    objectName: "stateMachine"

    property string lastState

    states: [
      State {
        name: "browse"
        PropertyChanges {
          target: identifyTool
          deactivated: false
        }
      },
      State {
        name: "digitize"
        PropertyChanges {
          target: identifyTool
          deactivated: false
        }
        PropertyChanges {
          target: mainWindow
          currentRubberband: digitizingRubberband
        }
      },
      State {
        name: 'measure'
        PropertyChanges {
          target: identifyTool
          deactivated: true
        }
        PropertyChanges {
          target: mainWindow
          currentRubberband: measuringTool.measuringRubberband
        }
        PropertyChanges {
          target: featureListForm
          state: "Hidden"
        }
      },
      State {
        name: '3d'
      }
    ]
    state: "browse"
  }

  onToggleDigitizeMode: {
    if (stateMachine.state === "digitize") {
      if (digitizingToolbar.rubberbandModel && digitizingToolbar.rubberbandModel.vertexCount > 1) {
        displayToast(qsTr("Finish or dimiss the digitizing feature before toggling to browse mode"));
      } else {
        changeMode("browse");
      }
    } else {
      changeMode("digitize");
    }
  }

  onChangeMode: mode => {
    if (stateMachine.state === mode) {
      return;
    }
    if (stateMachine.state !== 'measure' && stateMachine.state !== '3d') {
      stateMachine.lastState = stateMachine.state;
    }
    stateMachine.state = mode;
    switch (stateMachine.state) {
    case 'browse':
      projectInfo.stateMode = mode;
      platformUtilities.setHandleVolumeKeys(false);
      displayToast(qsTr('You are now in browse mode'));
      break;
    case 'digitize':
      projectInfo.stateMode = mode;
      platformUtilities.setHandleVolumeKeys(qfieldSettings.digitizingVolumeKeys);
      dashBoard.ensureEditableLayerSelected();
      if (dashBoard.activeLayer) {
        displayToast(qsTr('You are now in digitize mode on layer %1').arg(dashBoard.activeLayer.name));
      } else {
        displayToast(qsTr('You are now in digitize mode'));
      }
      break;
    case 'measure':
      platformUtilities.setHandleVolumeKeys(qfieldSettings.digitizingVolumeKeys);
      informationDrawer.elevationProfile.populateLayersFromProject();
      displayToast(qsTr('You are now in measure mode'));
      break;
    case '3d':
      break;
    }
  }

  onCloseMeasureTool: {
    changeMode(stateMachine.lastState);
  }

  onClose3DView: {
    // Sync 2D map to the 3D extent so we land on the same area
    if (mapCanvas3DLoader.item) {
      const ext = mapCanvas3DLoader.item.terrainExtent;
      if (ext && ext.width > 0 && ext.height > 0) {
        mapCanvas.mapSettings.extent = ext;
      }
    }
    changeMode(stateMachine.lastState);
  }

  QfPositioning {
    id: positionSource
    objectName: "positionSource"

    serviceMode: trackings.count > 0// && (platformUtilities.capabilities & QfPlatformUtilities.PositioningService)
    deviceId: positioningSettings.positioningDevice
    badAccuracyThreshold: positioningSettings.accuracyBad
    excellentAccuracyThreshold: positioningSettings.accuracyExcellent
    averagedPositionFilterAccuracy: positioningSettings.accuracyIndicator && positioningSettings.accuracyRequirement

    property bool currentness: false
    property bool ntripCurrentness: false

    property bool jumpToPosition: false
    property alias destinationCrs: positionSource.coordinateTransformer.destinationCrs
    property real bearingTrueNorth: 0.0

    coordinateTransformer: CoordinateTransformer {
      destinationCrs: mapCanvas.mapSettings.destinationCrs
      transformContext: qgisProject ? qgisProject.transformContext : QfCoordinateReferenceSystemUtils.emptyTransformContext()
      deltaZ: 0
      skipAltitudeTransformation: positioningSettings.skipAltitudeCorrection
      verticalGrid: positioningSettings.verticalGrid
    }

    elevationCorrectionMode: positioningSettings.elevationCorrectionMode
    antennaHeight: positioningSettings.antennaHeightActivated ? positioningSettings.antennaHeight : 0

    loggingPath: platformUtilities.appDataDirs()[0] + "/logs"
    logging: positioningSettings.logging

    enableNtrip: positioningSettings.enableNtrip
    ntripSettings: QfPositioningUtils.createNtripSettings(positioningSettings.ntripSettings)

    onPositionInformationChanged: {
      if (active) {
        if (jumpToPosition && positionSource.projectedPosition.x) {
          jumpToPosition = false;
          gnssButton.jumpToLocation();
        }
        bearingTrueNorth = QfPositioningUtils.bearingTrueNorth(positionSource.projectedPosition, mapCanvas.mapSettings.destinationCrs);
        if (gnssButton.followActive) {
          if (stateMachine.state === '3d') {
            if (mapCanvas3DLoader.item) {
              const pos3d = mapCanvas3DLoader.item.geoTo3D(positionSource.projectedPosition.x, positionSource.projectedPosition.y);
              if (pos3d !== null) {
                mapCanvas3DLoader.item.lookAtPoint(pos3d, 1000);
              } else {
                // We're now out of the 3D map extent, unfollow
                mapCanvasMap.unfreeze('follow');
                gnssButton.followActive = false;
              }
            }
          } else {
            gnssButton.followLocation(false);
            // Call followOrientation for movement direction mode
            if (positioningSettings.positionFollowMode === QfPositioningSettings.FollowMode.PositionAndDirection) {
              gnssButton.followOrientation();
            }
          }
        }
      }
    }

    onOrientationChanged: {
      if (active && gnssButton.followActive) {
        // Call followOrientation for compass mode
        if (positioningSettings.positionFollowMode === QfPositioningSettings.FollowMode.PositionAndCompass) {
          gnssButton.followOrientation();
        }
      }
    }

    onDeviceLastErrorChanged: {
      displayToast(qsTr('Positioning device error: %1').arg(positionSource.deviceLastError), 'error');
    }

    onBackgroundModeChanged: {
      if (trackings.count > 0) {
        if (backgroundMode) {
          trackingModel.suspendUntilReplay();
        } else {
          busyOverlay.text = qsTr("Replaying collected positions, hold on");
          busyOverlay.state = "visible";
          replayTimer.restart();
        }
      }
    }
  }

  Timer {
    id: replayTimer

    interval: 250
    repeat: false
    onTriggered: {
      mapCanvasMap.freeze('trackerreplay');
      let list = positionSource.getBackgroundPositionInformation();
      // Qt bug weirdly returns an empty list on first invokation to source, call twice to insure we've got the actual list
      list = positionSource.getBackgroundPositionInformation();
      trackingModel.replayPositionInformationList(list, positionSource.coordinateTransformer);
      mapCanvasMap.unfreeze('trackerreplay');
      busyOverlay.state = "hidden";
    }
  }

  QfPositioningSettings {
    id: positioningSettings
    objectName: "positioningSettings"

    onPositioningActivatedChanged: {
      if (mainWindow.sceneLoaded) {
        if (positioningActivated) {
          displayToast(qsTr("Activating positioning service"));
          positionSource.active = true;
        } else {
          gnssButton.followActive = false;
          gnssButton.autoRefollow = false;
          positionSource.active = false;
        }
      }
    }

    onPositionFollowModeChanged: {
      // When mode changes while locked, apply orientation immediately
      if (gnssButton.followActive && gnssButton.followOrientationActive) {
        gnssButton.followOrientation();
      }
    }
  }

  Timer {
    id: positionTimer

    property bool geocoderLocatorFiltersChecked: false

    interval: 2500
    repeat: true
    running: positionSource.active
    triggeredOnStart: true
    onTriggered: {
      if (positionSource.positionInformation) {
        positionSource.currentness = ((Date.now() - positionSource.positionInformation.utcDateTime.getTime()) / 1000) < 30;
        if (!geocoderLocatorFiltersChecked && positionSource.valid) {
          locatorItem.locatorFiltersModel.setGeocoderLocatorFiltersDefaulByPosition(positionSource.positionInformation);
          geocoderLocatorFiltersChecked = true;
        }
      }
      if (positionSource.ntripState === QfPositioning.NtripState.Connected) {
        positionSource.ntripCurrentness = ((Date.now() - positionSource.ntripLastBytesReceivedUtcDateTime.getTime()) / 1000) < 10;
      }
    }
  }

  Item {
    id: mapCanvas
    objectName: "mapCanvasContainer"
    clip: true

    DragHandler {
      id: freehandHandler
      property bool isDigitizing: false
      property int freehandStartVertexIndex: -1
      enabled: freehandButton.visible && freehandButton.freehandDigitizing && !digitizingToolbar.cogoEnabled && !digitizingToolbar.rubberbandModel.frozen && ((!featureListForm.visible && digitizingToolbar.digitizingAllowed) || digitizingToolbar.geometryRequested)
      acceptedDevices: !qfieldSettings.mouseAsTouchScreen ? PointerDevice.Stylus | PointerDevice.Mouse : PointerDevice.Stylus
      grabPermissions: PointerHandler.CanTakeOverFromHandlersOfSameType | PointerHandler.CanTakeOverFromHandlersOfDifferentType | PointerHandler.ApprovesTakeOverByAnything
      dragThreshold: 0

      onActiveChanged: {
        if (active) {
          freehandStartVertexIndex = digitizingToolbar.rubberbandModel.currentCoordinateIndex + 1;
          geometryEditorsToolbar.canvasFreehandBegin();
        } else {
          digitizingToolbar.rubberbandModel.smoothSegment(freehandStartVertexIndex, digitizingToolbar.rubberbandModel.currentCoordinateIndex, mapCanvas.mapSettings.mapUnitsPerPoint);
          geometryEditorsToolbar.canvasFreehandEnd();
          var screenLocation = centroid.position;
          var screenFraction = settings.value("/QField/Digitizing/FreehandRecenterScreenFraction", 5);
          var threshold = Math.min(mainWindow.width, mainWindow.height) / screenFraction;
          if (screenLocation.x < threshold || screenLocation.x > mainWindow.width - threshold || screenLocation.y < threshold || screenLocation.y > mainWindow.height - threshold) {
            mapCanvas.mapSettings.setCenter(mapCanvas.mapSettings.screenToCoordinate(screenLocation));
          }
        }
      }

      onCentroidChanged: {
        if (active) {
          if (centroid.position !== Qt.point(0, 0)) {
            coordinateLocator.sourceLocation = centroid.position;
            if (!geometryEditorsToolbar.canvasClicked(centroid.position, "stylus")) {
              digitizingToolbar.addVertex();
            }
          }
        }
      }
    }

    DragHandler {
      id: rotateDragHandler
      enabled: rotateFeaturesToolbar.rotateFeaturesRequested == true
      acceptedDevices: !qfieldSettings.mouseAsTouchScreen ? PointerDevice.TouchScreen | PointerDevice.Mouse : PointerDevice.TouchScreen | PointerDevice.Mouse | PointerDevice.Stylus
      grabPermissions: PointerHandler.CanTakeOverFromHandlersOfSameType | PointerHandler.CanTakeOverFromHandlersOfDifferentType | PointerHandler.ApprovesTakeOverByAnything

      property real pressClickX: 0
      property real pressClickY: 0
      property real screenCenterX: 0
      property real screenCenterY: 0

      onActiveChanged: {
        if (active) {
          pressClickX = centroid.position.x;
          pressClickY = centroid.position.y;
          screenCenterX = width / 2;
          screenCenterY = height / 2;
        }
      }

      onTranslationChanged: {
        if (active) {
          let newPositionX = pressClickX + translation.x;
          let newPositionY = pressClickY + translation.y;
          screenCenterX = mapCanvas.mapSettings.coordinateToScreen(featureListForm.extentController.getCentroidFromSelected()).x;
          screenCenterY = mapCanvas.mapSettings.coordinateToScreen(featureListForm.extentController.getCentroidFromSelected()).y;
          let angle = Math.atan2(newPositionY - screenCenterY, newPositionX - screenCenterX) - Math.atan2(pressClickY - screenCenterY, pressClickX - screenCenterX);
          if (angle != 0) {
            moveAndRotateFeaturesHighlight.originX = screenCenterX;
            moveAndRotateFeaturesHighlight.originY = screenCenterY;
            moveAndRotateFeaturesHighlight.rotationDegrees = angle * 180 / Math.PI;
          }
        }
      }
    }

    HoverHandler {
      id: hoverHandler
      enabled: !digitizingToolbar.rubberbandModel || !digitizingToolbar.rubberbandModel.frozen
      acceptedDevices: !qfieldSettings.mouseAsTouchScreen ? PointerDevice.TouchPad | PointerDevice.Stylus | PointerDevice.Mouse : PointerDevice.Stylus

      grabPermissions: PointerHandler.TakeOverForbidden

      property bool hasBeenHovered: false
      property bool skipHover: false

      function isPointOverFloatingComponents(pt) {
        return pointHandler.pointInItem(pt, digitizingToolbarContainer) || pointHandler.pointInItem(pt, zoomToolbar) || pointHandler.pointInItem(pt, mainToolbar) || pointHandler.pointInItem(pt, mainMenuBar) || pointHandler.pointInItem(pt, locationToolbar) || pointHandler.pointInItem(pt, locatorItem) || pointHandler.pointInItem(pt, informationDrawer.cogoOperationSettings) || pointHandler.pointInItem(pt, featureListForm) || overlayFeatureFormDrawer.opened;
      }

      onPointChanged: {
        if (skipHover || !mapCanvasMap.hovered) {
          return;
        }

        // Park the crosshair at the last digitized vertex when hovering the digitizing / geometry-editor toolbars
        if (!freehandHandler.active && (pointHandler.pointInItem(point.position, digitizingToolbar) || pointHandler.pointInItem(point.position, elevationProfileButton))) {
          coordinateLocator.sourceLocation = mapCanvas.mapSettings.coordinateToScreen(digitizingToolbar.rubberbandModel.lastCoordinate);
          return;
        }
        if (!freehandHandler.active && pointHandler.pointInItem(point.position, geometryEditorsToolbar)) {
          coordinateLocator.sourceLocation = mapCanvas.mapSettings.coordinateToScreen(geometryEditorsToolbar.editorRubberbandModel.lastCoordinate);
          return;
        }

        // Over any other floating component, do not move the crosshair at all
        // freeze it wherever it last was on the map.
        if (isPointOverFloatingComponents(point.position)) {
          return;
        }

        if (!freehandHandler.active && point.position !== Qt.point(0, 0)) {
          coordinateLocator.sourceLocation = point.position;
        }
      }

      onActiveChanged: {
        if (!active) {
          coordinateLocator.sourceLocation = undefined;
        }
      }

      onHoveredChanged: {
        if (mapCanvasMap.pinched) {
          return;
        }
        if (skipHover) {
          if (!hovered) {
            mapCanvasMap.hovered = false;
            dummyHoverTimer.restart();
          }
          return;
        }
        mapCanvasMap.hovered = hovered;
        if (hovered) {
          hasBeenHovered = true;
        } else {
          if (currentRubberband && currentRubberband.model.vertexCount > 1) {
            coordinateLocator.sourceLocation = mapCanvas.mapSettings.coordinateToScreen(currentRubberband.model.lastCoordinate);
          } else if (geometryEditorsToolbar.editorRubberbandModel && geometryEditorsToolbar.editorRubberbandModel.vertexCount > 1) {
            coordinateLocator.sourceLocation = mapCanvas.mapSettings.coordinateToScreen(geometryEditorsToolbar.editorRubberbandModel.lastCoordinate);
          } else {
            if (digitizingToolbar.rubberbandModel == undefined || !digitizingToolbar.rubberbandModel.frozen) {
              coordinateLocator.sourceLocation = undefined;
            }
          }
        }
      }
    }

    /* The second hover handler is a workaround what appears to be an issue with
     * Qt whereas synthesized mouse event would trigger the first HoverHandler even though
     * PointerDevice.TouchScreen was explicitly taken out of the accepted devices.
     * The timer is needed as adding additional fingers onto a device re-triggers hovered
     * changes in unpredictable order.
     *
     * Known issue: Switching between finger and stylus input within 500 milliseconds may break
     * the stylus binding to the QfCoordinateLocator.
     */
    Timer {
      id: dummyHoverTimer
      interval: 500
      repeat: false

      onTriggered: {
        hoverHandler.skipHover = false;
      }
    }

    HoverHandler {
      id: dummyHoverHandler
      enabled: !qfieldSettings.mouseAsTouchScreen && hoverHandler.enabled
      acceptedDevices: PointerDevice.TouchScreen
      grabPermissions: PointerHandler.TakeOverForbidden

      onHoveredChanged: {
        if (hovered) {
          dummyHoverTimer.stop();
          hoverHandler.skipHover = true;

          // Unfortunately, Qt fails to set the hovered property to false when stylus leaves proximity
          // of the screen, we've got to compensate for that
          mapCanvasMap.hovered = false;
          if (!qfieldSettings.fingerTapDigitizing) {
            coordinateLocator.sourceLocation = undefined;
          }
        }
      }
    }

    /* Initialize a MapSettings object. This will contain information about
     * the current canvas extent. It is shared between the base map and all
     * map canvas items and is used to transform map coordinates to pixel
     * coordinates.
     * It may change any time and items that hold a reference to this property
     * are responsible to handle this properly.
     */
    property MapSettings mapSettings: mapCanvasMap.mapSettings

    /* Placement and size. Share right anchor with featureForm */
    anchors.top: parent.top
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.bottom: parent.bottom

    Rectangle {
      id: mapCanvasBackground
      anchors.fill: parent
      color: mapCanvas.mapSettings.backgroundColor
    }

    Item {
      id: mapCanvas3DPluginContainer
      objectName: 'mapCanvas3DPluginContainer'
      visible: false
    }

    Loader {
      id: mapCanvas3DLoader
      anchors.fill: parent
      active: stateMachine.state === '3d'
      visible: active ? true : false
      z: 100
      opacity: active ? 1.0 : 0.0

      Behavior on opacity {
        NumberAnimation {
          duration: 300
          easing.type: Easing.Linear
        }
      }

      source: "qrc:/qt/qml/org/qfield/_3d/QfMapCanvas3D.qml"

      onActiveChanged: {
        if (active) {
          mapCanvasMap.freeze('3d');
        } else {
          mapCanvasMap.unfreeze('3d');
        }
      }

      onLoaded: {
        item.objectName = 'mapCanvas3D';
        item.pluginContainer = mapCanvas3DPluginContainer;
        item.mapSettings = mapCanvas.mapSettings;
        item.trackingModel = trackingModel;
        item.eyeDomeLightingMode = settings.valueBool('3d/eyeDomeLightingMode', false);
        item.selectionColor = Qt.binding(() => QfTheme.mainColor);

        // Bind GNSS position updates
        item.gnssActive = Qt.binding(() => positionSource.active && positionSource.positionInformation && positionSource.positionInformation.latitudeValid);
        item.gnssPosition = Qt.binding(() => positionSource.projectedPosition);
        item.gnssSpeed = Qt.binding(() => positionSource.positionInformation && positionSource.positionInformation.speedValid ? positionSource.positionInformation.speed : -1);
        item.gnssDirection = Qt.binding(() => positionSource.positionInformation && positionSource.positionInformation.directionValid ? positionSource.positionInformation.direction : -1);
        item.gnssMarkerColor = Qt.binding(() => locationMarker.color);

        // Connect camera interaction signal to deactivate soft lock
        item.cameraInteractionDetected.connect(function () {
          if (gnssButton.followActive) {
            mapCanvasMap.unfreeze('follow');
            gnssButton.followActive = false;
          }
        });

        item.featureIdentifyRequested.connect(function (screenPoint) {
          if (!featureListForm.canvasOperationRequested && !overlayFeatureFormDrawer.opened && featureListForm.state !== "FeatureFormEdit") {
            identifyTool.isMenuRequest = false;
            identifyTool.identify(screenPoint);
          }
        });
        item.selectionModel = featureListForm.selection;
      }

      onStatusChanged: {
        if (status === Loader.Error) {
          mainWindow.close3DView();
          displayToast(qsTr("Failed to load 3D view"));
        }
      }
    }

    Rectangle {
      id: loadingOverlay
      anchors.fill: parent
      color: "#80000000"
      visible: stateMachine.state === '3d' && mapCanvas3DLoader.item && mapCanvas3DLoader.item.isLoading && mapCanvas3DLoader.item.isFirstLoad
      z: 1000

      Column {
        anchors.centerIn: parent
        spacing: 20

        BusyIndicator {
          anchors.horizontalCenter: parent.horizontalCenter
          running: stateMachine.state === '3d' && mapCanvas3DLoader.item && mapCanvas3DLoader.item.isLoading && mapCanvas3DLoader.item.isFirstLoad
          width: 64
          height: 64
        }

        Text {
          anchors.horizontalCenter: parent.horizontalCenter
          text: qsTr("Loading terrain...")
          color: "white"
          font.pixelSize: 16
          font.bold: true
        }
      }
    }

    QfGridRenderer {
      mapSettings: mapCanvas.mapSettings
      enabled: !gridDecoration.enabled
      indeterminate: true
      prepareLines: true
      autoColor: true
      annotationFont: QfTheme.tinyFont
    }

    /* The map canvas */
    QfMapCanvas {
      id: mapCanvasMap
      objectName: "mapCanvas"

      property bool isEnabled: !mapCanvas3DLoader.active && !dashBoard.opened && !aboutDialog.visible && !welcomeScreen.visible && !qfieldSettings.visible && !qfieldLocalDataPickerScreen.visible && !qfieldCloudScreen.visible && !qfieldCloudPopup.visible && !codeReader.visible && !sketcher.visible && !overlayFeatureFormDrawer.opened && !rotateFeaturesToolbar.rotateFeaturesRequested

      interactive: isEnabled && !screenLocker.enabled && !snapToCommonAngleMenu.visible
      isMapRotationEnabled: qfieldSettings.enableMapRotation
      mouseAsTouchScreen: qfieldSettings.mouseAsTouchScreen
      incrementalRendering: true
      quality: qfieldSettings.quality
      smooth: gnssButton.followActive && !mapCanvasMap.jumping
      previewJobsEnabled: qfieldSettings.previewJobsEnabled
      forceDeferredLayersRepaint: trackings.count > 0
      freehandDigitizing: freehandButton.freehandDigitizing && freehandHandler.active

      property bool allowMargins: !gnssButton.followActive || !gnssButton.followOrientationActive
      rightMargin: allowMargins ? !featureListForm.isFullscreen && !featureListForm.canvasOperationRequested && featureListForm.x > 0 ? featureListForm.width : 0 : 0
      bottomMargin: allowMargins ? Math.max(informationDrawer.height > mainWindow.sceneBottomMargin ? informationDrawer.height : 0, !featureListForm.isFullscreen && !featureListForm.canvasOperationRequested && featureListForm.y > 0 ? featureListForm.height : 0) : 0

      anchors.fill: parent

      onClicked: (point, type) => {
        // Check if any registered handlers want to handle this click
        if (pointHandler.clicked(point, type)) {
          return;
        }
        if (type === "stylus" && (overlayFeatureFormDrawer.opened || (featureListForm.visible && pointHandler.pointInItem(point, featureListForm)))) {
          return;
        }
        if (!digitizingToolbar.geometryRequested && featureListForm.state == "FeatureFormEdit") {
          return;
        }
        if (locatorItem.state === "on") {
          locatorItem.state = "off";
          return;
        }

        if (type === "stylus") {
          if (pointHandler.pointInItem(point, digitizingToolbar) || pointHandler.pointInItem(point, zoomToolbar) || pointHandler.pointInItem(point, mainToolbar) || pointHandler.pointInItem(point, mainMenuBar) || pointHandler.pointInItem(point, geometryEditorsToolbar) || pointHandler.pointInItem(point, locationToolbar) || pointHandler.pointInItem(point, digitizingToolbarContainer) || pointHandler.pointInItem(point, locatorItem)) {
            return;
          }

          // Check if geometry editor is taking over
          const positionLocked = positionSource.active && coordinateLocator.positionLocked;
          if (geometryEditorsToolbar.stateVisible) {
            if (!positionLocked) {
              geometryEditorsToolbar.canvasClicked(point, type);
            }
            return;
          }
          // Check if a feature movement can be confirmed
          if (moveFeaturesToolbar.moveFeaturesRequested) {
            moveFeaturesToolbar.confirm();
            return;
          }
          if ((stateMachine.state === "digitize" && digitizingFeature.currentLayer && digitizingToolbar.digitizingAllowed) || stateMachine.state === "measure") {
            if (digitizingToolbar.cogoEnabled) {
              return;
            }
            if (!positionLocked && (!featureListForm.visible || digitizingToolbar.geometryRequested)) {
              if (Number(currentRubberband.model.geometryType) === Qgis.GeometryType.Point || Number(currentRubberband.model.geometryType) === Qgis.GeometryType.Null) {
                digitizingToolbar.confirm();
              } else {
                digitizingToolbar.addVertex();
              }
            }
          } else {
            if (!featureListForm.canvasOperationRequested && !overlayFeatureFormDrawer.opened && featureListForm.state !== "FeatureFormEdit") {
              identifyTool.isMenuRequest = false;
              identifyTool.identify(point);
            }
          }
        }
      }

      onConfirmedClicked: point => {
        // Check if any registered handlers want to handle this tap
        if (pointHandler.clicked(point, "touch")) {
          return;
        }
        // Check if geometry editor is taking over
        const positionLocked = positionSource.active && coordinateLocator.positionLocked;
        if (geometryEditorsToolbar.stateVisible) {
          if (!positionLocked) {
            geometryEditorsToolbar.canvasClicked(point, '');
          }
          return;
        }
        if (qfieldSettings.fingerTapDigitizing && ((stateMachine.state === "digitize" && digitizingFeature.currentLayer && digitizingToolbar.digitizingAllowed) || stateMachine.state === "measure")) {
          if (!positionLocked && (!featureListForm.visible || digitizingToolbar.geometryRequested)) {
            coordinateLocator.sourceLocation = point;
          }
        } else if (!featureListForm.canvasOperationRequested && !overlayFeatureFormDrawer.opened && featureListForm.state !== "FeatureFormEdit") {
          identifyTool.isMenuRequest = false;
          identifyTool.identify(point);
        }
      }

      onLongPressed: (point, type) => {
        // Check if any registered handlers want to handle this press and hold
        if (pointHandler.pressAndHold(point, type)) {
          return;
        }
        if (type === "stylus") {
          if (overlayFeatureFormDrawer.opened || (featureListForm.visible && pointHandler.pointInItem(point, featureListForm))) {
            return;
          }

          // Check if geometry editor is taking over
          if (geometryEditorsToolbar.canvasLongPressed(point, type)) {
            return;
          }
          if (stateMachine.state === "digitize" && digitizingFeature.currentLayer && digitizingToolbar.digitizingAllowed) {
            if (digitizingToolbar.cogoEnabled) {
              return;
            }
            // the sourceLocation test checks if a (stylus) hover is active
            if ((currentRubberband.model.geometryType === Qgis.GeometryType.Line && currentRubberband.model.vertexCount >= 2) || (currentRubberband.model.geometryType === Qgis.GeometryType.Polygon && currentRubberband.model.vertexCount >= 3)) {
              digitizingToolbar.addVertex();

              // When it's released, it will normally cause a release event to close the attribute form.
              // We get around this by temporarily switching the closePolicy.
              overlayFeatureFormDrawer.closePolicy = Popup.CloseOnEscape;
              digitizingToolbar.confirm();
              return;
            }
          }

          // do not use else, as if it was catch it has return before
          identifyTool.isMenuRequest = false;
          identifyTool.identify(point);
        } else {
          // Check if geometry editor is taking over
          if (geometryEditorsToolbar.canvasLongPressed(point)) {
            return;
          }
          canvasMenu.point = mapCanvas.mapSettings.screenToCoordinate(point);
          canvasMenu.popup(point.x, point.y);
          identifyTool.isMenuRequest = true;
          identifyTool.identify(point);
        }
      }

      onRightClicked: (point, type) => {
        canvasMenu.point = mapCanvas.mapSettings.screenToCoordinate(point);
        canvasMenu.popup(point.x, point.y);
        identifyTool.isMenuRequest = true;
        identifyTool.identify(point);
      }

      onLongPressReleased: type => {
        if (type === "stylus") {
          // The user has released the long press. We can re-enable the default close behavior for the feature form.
          // The next press will be intentional to close the form.
          overlayFeatureFormDrawer.closePolicy = Popup.CloseOnEscape | Popup.CloseOnPressOutside;
        }
      }

      onDoubleClicked: (point, type) => {
        // Check if any registered handlers want to handle this double click
        if (pointHandler.doubleClicked(point, type)) {
          return;
        }
        if (type === "touch") {
          if (qfieldSettings.fingerTapDigitizing && !digitizingToolbar.cogoEnabled && ((stateMachine.state === "digitize" && digitizingFeature.currentLayer && digitizingToolbar.digitizingAllowed) || stateMachine.state === "measure")) {
            const positionLocked = positionSource.active && coordinateLocator.positionLocked;
            if (!positionLocked && (!featureListForm.visible || digitizingToolbar.geometryRequested)) {
              coordinateLocator.sourceLocation = point;
              if (Number(currentRubberband.model.geometryType) === Qgis.GeometryType.Point || Number(currentRubberband.model.geometryType) === Qgis.GeometryType.Null) {
                digitizingToolbar.confirm();
              } else {
                digitizingToolbar.addVertex();
              }
              return;
            }
          }
          mapCanvasWrapper.zoomByFactor(Qt.point(point.x, point.y), 0.8);
        }
      }

      onInteractiveChanged: {
        platformUtilities.setHandleVolumeKeys(qfieldSettings.digitizingVolumeKeys && interactive);
      }

      QfGridRenderer {
        id: gridDecoration
        mapSettings: mapCanvas.mapSettings
        annotationFont: QfTheme.tinyFont
      }

      QfMapCanvasPointHandler {
        id: pointHandler
        objectName: "pointHandler"
        coordinateSpace: mainWindow.contentItem
      }
    }

    /**************************************************
   * Overlays, including:
   * - Coordinate Locator
   * - Location Marker
   * - Identify Highlight
   * - Digitizing QfRubberband
   **************************************************/

    // Model for pie menu and context menu feature identification
    QfMultiFeatureListModel {
      id: menuFeatureListModel
    }

    QfIdentifyTool {
      id: identifyTool

      property bool isMenuRequest: false
      property bool isPieMenuRequest: false

      mapSettings: mapCanvas.mapSettings
      model: isPieMenuRequest || isMenuRequest ? menuFeatureListModel : featureListForm.model
      searchRadiusMm: 3

      onIdentifyFinished: {
        if (isPieMenuRequest) {
          actionsPieMenu.identifiedCount = menuFeatureListModel.count;
          isPieMenuRequest = false;
        } else if (isMenuRequest) {
          canvasMenuFeatureListInstantiator.active = true;
        } else {
          if (qfieldSettings.autoOpenFormSingleIdentify && !featureListForm.multiSelection && featureListForm.model.count === 1) {
            featureListForm.state = "FeatureForm";
            featureListForm.selection.focusedItem = 0;
          }
          if (qfieldSettings.autoZoomToIdentifiedFeature && featureListForm.model.count > 0) {
            featureListForm.extentController.zoomToAllFeatures();
          }
        }
      }
    }

    QfMeasuringTool {
      id: measuringTool
      visible: stateMachine.state === 'measure'
      anchors.fill: parent

      measuringRubberband.model.currentCoordinate: coordinateLocator.currentCoordinate
      measuringRubberband.mapSettings: mapCanvas.mapSettings
    }

    Repeater {
      id: trackings

      model: trackingModel

      onCountChanged: {
        if (count > 0) {
          // Start positioning if not yet active
          if (!positionSource.active) {
            positioningSettings.positioningActivated = true;
          }
        }
      }

      QfTrackingSession {
        positioning: positionSource
        filterAccuracy: positioningSettings.accuracyIndicator && positioningSettings.accuracyRequirement
        project: qgisProject
        mapSettings: mapCanvas.mapSettings
        cloudUserInformation: appScopesGenerator.cloudUserInformation

        onFeatureCreated: layer => projectInfo.saveTracker(layer)
      }
    }

    QfCogoOperationPreview {
      id: cogoOperationPreview
      visible: digitizingToolbar.cogoEnabled
      visualGuides: digitizingToolbar.cogoExecutor.visualGuides
    }

    QfRubberband {
      id: digitizingRubberband

      mapSettings: mapCanvas.mapSettings
      showVertices: digitizingToolbar.cogoEnabled

      model: QfRubberbandModel {
        frozen: false
        currentCoordinate: digitizingToolbar.cogoEnabled ? QfGeometryUtils.emptyPoint() : coordinateLocator.currentCoordinate
        measureValue: {
          let pi;
          if (digitizingToolbar.cogoEnabled) {
            pi = digitizingToolbar.cogoOperationSettings.positionInformation;
          } else if (coordinateLocator.positionLocked) {
            pi = coordinateLocator.positionInformation;
          } else {
            return Number.NaN;
          }
          switch (positioningSettings.digitizingMeasureType) {
          case QfTracker.Timestamp:
            return pi.utcDateTime.getTime();
          case QfTracker.GroundSpeed:
            return pi.speed;
          case QfTracker.Bearing:
            return pi.direction;
          case QfTracker.HorizontalAccuracy:
            return pi.hacc;
          case QfTracker.VerticalAccuracy:
            return pi.vacc;
          case QfTracker.PDOP:
            return pi.pdop;
          case QfTracker.HDOP:
            return pi.hdop;
          case QfTracker.VDOP:
            return pi.vdop;
          }
          return Number.NaN;
        }
        vectorLayer: digitizingToolbar.geometryRequested ? digitizingToolbar.geometryRequestedLayer : dashBoard.activeLayer
        crs: mapCanvas.mapSettings.destinationCrs
      }

      visible: stateMachine.state === "digitize"
    }

    QfGeometryRenderer {
      id: geometryEditorRenderer
    }

    QfRubberband {
      id: geometryEditorsRubberband
      color: '#96000000'

      mapSettings: mapCanvas.mapSettings

      model: QfRubberbandModel {
        frozen: false
        currentCoordinate: coordinateLocator.currentCoordinate
        crs: mapCanvas.mapSettings.destinationCrs
        geometryType: Qgis.GeometryType.Line
      }
    }

    QfBookmarkHighlight {
      id: bookmarkHighlight
      mapSettings: mapCanvas.mapSettings
    }

    QfNavigation {
      id: navigation
      objectName: "navigation"

      mapSettings: mapCanvas.mapSettings
      location: positionSource.active ? positionSource.projectedPosition : QfGeometryUtils.emptyPoint()

      proximityAlarm: positioningSettings.preciseViewProximityAlarm && informationDrawer.positioningPreciseView.visible && informationDrawer.positioningPreciseView.hasAcceptableAccuracy && !informationDrawer.positioningPreciseView.hasAlarmSnoozed
      proximityAlarmThreshold: positioningSettings.preciseViewPrecision
    }

    QfNavigationHighlight {
      id: navigationHighlight
      navigation: navigation
    }

    QfLinePolygon {
      id: elevationProfileHighlight

      visible: informationDrawer.elevationProfile.visible
      mapSettings: mapCanvas.mapSettings
      geometry: QfGeometryWrapper {
        crs: informationDrawer.elevationProfile.crs
        qgsGeometry: informationDrawer.elevationProfile.profileCurve
      }
      color: "#FFFFFF"
      lineWidth: 4
    }

    QfCoordinateLocator {
      id: coordinateLocator
      objectName: "coordinateLocator"
      anchors.fill: parent
      anchors.bottomMargin: mapCanvasMap.allowMargins ? informationDrawer.height > mainWindow.sceneBottomMargin ? informationDrawer.height : 0 : 0
      visible: (stateMachine.state === "digitize" || stateMachine.state === 'measure')
      highlightColor: digitizingToolbar.isDigitizing ? currentRubberband.color : "#CFD8DC"
      mapSettings: mapCanvas.mapSettings
      currentLayer: dashBoard.activeLayer
      positionInformation: positionSource.positionInformation
      positionLocked: !digitizingToolbar.cogoEnabled && positionSource.active && (positioningSettings.positioningCoordinateLock || gnssButton.followActive)
      rubberbandModel: geometryEditorsToolbar.stateVisible ? geometryEditorsToolbar.editorRubberbandModel : digitizingToolbar.rubberbandModel
      averagedPosition: positionSource.averagedPosition
      averagedPositionCount: positionSource.averagedPositionCount
      overrideLocation: positionLocked ? positionSource.projectedPosition : undefined

      snapToCommonAngles: qfieldSettings.snapToCommonAngleIsEnabled && (dashBoard.activeLayer && (dashBoard.activeLayer.geometryType() === Qgis.GeometryType.Polygon || dashBoard.activeLayer.geometryType() === Qgis.GeometryType.Line))
      snappingIsRelative: qfieldSettings.snapToCommonAngleIsRelative
      snappingAngleDegrees: qfieldSettings.snapToCommonAngleDegrees
      snappingTolerance: qfieldSettings.snapToCommonAngleTolerance
    }

    /* Location marker reflecting the current GNSS position */
    QfLocationMarker {
      id: locationMarker
      visible: positionSource.active && positionSource.positionInformation && positionSource.positionInformation.latitudeValid

      mapSettings: mapCanvas.mapSettings

      location: positionSource.projectedPosition
      accuracy: positionSource.projectedHorizontalAccuracy
      direction: positionSource.positionInformation && positionSource.positionInformation.directionValid ? positionSource.positionInformation.direction : -1
      speed: positionSource.positionInformation && positionSource.positionInformation.speedValid ? positionSource.positionInformation.speed : -1
      orientation: !isNaN(positionSource.orientation) ? positionSource.orientation + positionSource.bearingTrueNorth < 0 ? 360 + positionSource.orientation + positionSource.bearingTrueNorth : positionSource.orientation + positionSource.bearingTrueNorth : -1

      Component.onCompleted: {
        pointHandler.registerHandler("LocationMarker", (point, type, interactionType) => {
          if (!locationMarker.visible || !locationMarker.isOnMapCanvas || (interactionType !== "clicked" && interactionType !== "pressedAndHold")) {
            return false;
          }
          const dx = point.x - locationMarker.screenLocation.x;
          const dy = point.y - locationMarker.screenLocation.y;
          const distance = Math.sqrt(dx * dx + dy * dy);
          const markerHit = distance <= 25;
          if (markerHit) {
            if (type === "stylus" && interactionType === "clicked") {
              mainWindow.displayToast(qsTr("Long press on your location marker to show actions"));
            } else if ((type !== "stylus" && interactionType === "clicked") || (type === "stylus" && interactionType === "pressedAndHold")) {
              openPieMenu(point);
              return true;
            }
          }
          return false;
        }, QfMapCanvasPointHandler.Priority.High);
        if (!settings.valueBool("/QField/pieMenuOpenedOnce", false)) {
          bubbleText = qsTr("Tap on your location marker\nto show actions");
          bubbleVisible = Qt.binding(() => locationMarker.isOnMapCanvas && locationMarker.visible && !mapCanvasTour.visible);
          bubbleAction = () => {
            openPieMenu(locationMarker.screenLocation);
          };
        }
      }

      function openPieMenu(point) {
        if (actionsPieMenu.tooCloseToLeft) {
          actionsPieMenu.x = actionsPieMenu.minimumDistanceToScreenEdge;
        } else if (actionsPieMenu.tooCloseToRight) {
          actionsPieMenu.x = mainWindow.width - actionsPieMenu.width - actionsPieMenu.minimumDistanceToScreenEdge;
        } else {
          actionsPieMenu.x = locationMarker.screenLocation.x - actionsPieMenu.menuHalfSize;
        }
        if (actionsPieMenu.tooCloseToTop) {
          actionsPieMenu.y = actionsPieMenu.minimumDistanceToScreenEdge;
        } else if (actionsPieMenu.tooCloseToBottom) {
          actionsPieMenu.y = mainWindow.height - actionsPieMenu.height - informationDrawer.height - actionsPieMenu.minimumDistanceToScreenEdge;
        } else {
          actionsPieMenu.y = locationMarker.screenLocation.y - actionsPieMenu.menuHalfSize;
        }
        actionsPieMenu.open();
        identifyTool.isPieMenuRequest = true;
        identifyTool.identify(locationMarker.screenLocation);
      }
    }

    QfToolButtonPie {
      id: actionsPieMenu

      readonly property int minimumDistanceToScreenEdge: 80
      readonly property real menuHalfSize: actionsPieMenu.width / 2

      readonly property bool tooCloseToLeft: locationMarker.screenLocation.x - menuHalfSize - minimumDistanceToScreenEdge < 0
      readonly property bool tooCloseToRight: locationMarker.screenLocation.x + menuHalfSize + minimumDistanceToScreenEdge > mainWindow.width
      readonly property bool tooCloseToTop: locationMarker.screenLocation.y - menuHalfSize - minimumDistanceToScreenEdge < 0
      readonly property bool tooCloseToBottom: locationMarker.screenLocation.y + menuHalfSize + minimumDistanceToScreenEdge + informationDrawer.height > mainWindow.height
      readonly property bool nearToEdge: tooCloseToLeft || tooCloseToRight || tooCloseToTop || tooCloseToBottom

      readonly property bool locationMarkerOutSidePieMenu: {
        if (!visible)
          return true;
        const dx = actionsPieMenu.x + (actionsPieMenu.width / 2) - locationMarker.screenLocation.x;
        const dy = actionsPieMenu.y + (actionsPieMenu.height / 2) - locationMarker.screenLocation.y;
        const distance = Math.sqrt(dx * dx + dy * dy);
        return distance > 20;
      }

      readonly property int segmentAngle: 360 / actionsPieMenu.numberOfButtons
      property int identifiedCount: 0

      onClosed: {
        menuFeatureListModel.clear(false);
        identifiedCount = 0;
      }

      width: Math.min(150, mapCanvasMap.width / 3)
      height: width

      targetPoint: locationMarker.screenLocation
      showConnectionLine: visible && (nearToEdge || locationMarkerOutSidePieMenu)

      centralActionVisible: identifiedCount > 0
      centralActionComponent: Component {
        QfToolButton {
          id: identifyFeaturesButton
          width: actionsPieMenu.bandWidth - 8
          height: width
          padding: 2
          round: true
          iconSource: QfTheme.getThemeVectorIcon("ic_geometry_mixed_24dp")
          iconColor: QfTheme.toolButtonColor
          bgcolor: QfTheme.toolButtonBackgroundColor

          QfBadge {
            width: identifyFeaturesButton.width / 2.5
            topMargin: identifyFeaturesButton.width / 20
            rightMargin: identifyFeaturesButton.width / 20
            color: QfTheme.toolButtonColor
            border.width: 1
            border.color: QfTheme.toolButtonColor
            badgeText.text: actionsPieMenu.identifiedCount > 9 ? "+" : actionsPieMenu.identifiedCount
            badgeText.color: QfTheme.toolButtonBackgroundColor
            z: 2
          }

          onClicked: {
            identifyTool.isMenuRequest = false;
            identifyTool.isPieMenuRequest = false;
            identifyTool.identify(locationMarker.screenLocation);
            actionsPieMenu.close();
          }
        }
      }

      QfToolButton {
        id: gnssCursorLockButton
        width: actionsPieMenu.bandWidth - 8
        height: width
        padding: 2
        round: true
        checkable: true
        checked: positioningSettings.positioningCoordinateLock
        state: checked ? "On" : "Off"
        visible: actionsPieMenu.openingAngle >= actionsPieMenu.segmentAngle
        iconSource: QfTheme.getThemeVectorIcon("ic_location_cursor_lock_white_24dp")
        opacity: enabled ? 1 : 0.4

        states: [
          State {
            name: "Off"
            PropertyChanges {
              target: gnssCursorLockButton
              iconColor: QfTheme.light
              bgcolor: QfTheme.toolButtonBackgroundSemiOpaqueColor
            }
          },
          State {
            name: "On"
            PropertyChanges {
              target: gnssCursorLockButton
              iconColor: QfTheme.positionColor
              bgcolor: QfTheme.toolButtonBackgroundColor
            }
          }
        ]

        onClicked: {
          if (positioningSettings.positioningCoordinateLock) {
            positioningSettings.positioningCoordinateLock = false;
            // deactivate any active averaged position collection
            positionSource.averagedPosition = false;
            displayToast(qsTr("Coordinate cursor unlocked"));
          } else {
            if (stateMachine.state === "browse") {
              mainWindow.changeMode("digitize");
            }
            if (freehandButton.freehandDigitizing) {
              // deactivate freehand digitizing when cursor locked is on
              freehandButton.clicked();
            }
            positioningSettings.positioningCoordinateLock = true;
            displayToast(qsTr("Coordinate cursor locked to location"));
          }
          actionsPieMenu.close();
        }
      }

      QfToolButton {
        id: gnssCanvasLockButton
        width: actionsPieMenu.bandWidth - 8
        height: width
        padding: 2
        round: true
        checkable: true
        checked: gnssButton.autoRefollow
        state: checked ? "On" : "Off"
        visible: actionsPieMenu.openingAngle >= actionsPieMenu.segmentAngle * 2
        iconSource: QfTheme.getThemeVectorIcon("ic_location_canvas_lock_white_24dp")

        states: [
          State {
            name: "Off"
            PropertyChanges {
              target: gnssCanvasLockButton
              iconColor: QfTheme.light
              bgcolor: QfTheme.toolButtonBackgroundSemiOpaqueColor
            }
          },
          State {
            name: "On"
            PropertyChanges {
              target: gnssCanvasLockButton
              iconColor: QfTheme.positionColor
              bgcolor: QfTheme.toolButtonBackgroundColor
            }
          }
        ]

        onClicked: {
          if (gnssButton.autoRefollow) {
            mapCanvasMap.unfreeze('follow');
            gnssButton.autoRefollow = false;
            gnssButton.followActive = false;
            displayToast(qsTr("Map canvas unlocked"));
          } else {
            mapCanvasMap.freeze('follow');
            gnssButton.autoRefollow = true;
            gnssButton.followActive = true;
            gnssButton.followLocation(true);
            if (positioningSettings.positionFollowMode === QfPositioningSettings.FollowMode.PositionAndCompass) {
              displayToast(qsTr("Map canvas locked to location and compass orientation"));
            } else if (positioningSettings.positionFollowMode === QfPositioningSettings.FollowMode.PositionAndDirection) {
              displayToast(qsTr("Map canvas locked to location and movement direction"));
            } else {
              displayToast(qsTr("Map canvas locked to location"));
            }
            // Apply orientation immediately if enabled
            if (gnssButton.followOrientationActive) {
              gnssButton.followOrientation();
            }
          }
          actionsPieMenu.close();
        }
      }

      QfToolButton {
        id: addBookmarkAtCurrentLocationButton
        width: actionsPieMenu.bandWidth - 8
        height: width
        padding: 2
        iconSource: QfTheme.getThemeVectorIcon("ic_add_bookmark_black_24dp")
        round: true
        checkable: false
        checked: false
        enabled: true
        iconColor: QfTheme.light
        bgcolor: QfTheme.toolButtonBackgroundColor
        visible: actionsPieMenu.openingAngle >= actionsPieMenu.segmentAngle * 3
        onClicked: {
          var name = qsTr('My location') + ' (' + new Date().toLocaleString() + ')';
          var group = 'blue';
          var id = bookmarkModel.addBookmarkAtPoint(positionSource.projectedPosition, name, group);
          if (id !== '') {
            bookmarkProperties.bookmarkId = id;
            bookmarkProperties.bookmarkName = name;
            bookmarkProperties.bookmarkGroup = group;
            bookmarkProperties.open();
          }
          actionsPieMenu.close();
        }
      }

      QfToolButton {
        id: copyCurrentLocationButton
        width: actionsPieMenu.bandWidth - 8
        height: width
        padding: 2
        iconSource: QfTheme.getThemeVectorIcon("ic_copy_black_24dp")
        round: true
        checkable: false
        checked: false
        enabled: true
        iconColor: QfTheme.light
        bgcolor: QfTheme.toolButtonBackgroundColor
        visible: actionsPieMenu.openingAngle >= actionsPieMenu.segmentAngle * 4
        onClicked: {
          if (!positionSource.positionInformation) {
            return;
          }

          const point = QfGeometryUtils.reprojectPoint(positionSource.sourcePosition, QfCoordinateReferenceSystemUtils.wgs84Crs(), projectInfo.coordinateDisplayCrs);
          let coordinates = QfStringUtils.pointInformation(point, projectInfo.coordinateDisplayCrs);

          let appendAccuracy = projectInfo.coordinateDisplayCrs.authid !== "EPSG:4326";
          if (projectInfo.coordinateDisplayCrs.authid === "EPSG:4326") {
            if (!positionSource.positionInformation.haccValid || positionSource.positionInformation.hacc > 10) {
              appendAccuracy = true;
            }
          }
          if (appendAccuracy) {
            coordinates += ' (' + qsTr('Accuracy') + ' ' + (positionSource.positionInformation.haccValid ? positionSource.positionInformation.hacc.toLocaleString(Qt.locale(), 'f', 3) + " " + qsTr("meters") : qsTr("N/A")) + ')';
          }

          platformUtilities.copyTextToClipboard(coordinates);
          displayToast(qsTr('Current location copied to clipboard'));
          actionsPieMenu.close();
        }
      }

      QfToolButton {
        id: showGnssInformation
        width: actionsPieMenu.bandWidth - 8
        height: width
        padding: 2
        iconSource: QfTheme.getThemeVectorIcon("ic_info_white_24dp")
        round: true
        checkable: true
        checked: positioningSettings.showPositionInformation
        enabled: true
        iconColor: QfTheme.light
        bgcolor: QfTheme.toolButtonBackgroundColor
        state: checked ? "On" : "Off"
        visible: actionsPieMenu.openingAngle >= actionsPieMenu.segmentAngle * 5

        states: [
          State {
            name: "Off"
            PropertyChanges {
              target: showGnssInformation
              iconColor: QfTheme.light
              bgcolor: QfTheme.toolButtonBackgroundSemiOpaqueColor
            }
          },
          State {
            name: "On"
            PropertyChanges {
              target: showGnssInformation
              iconColor: QfTheme.positionColor
              bgcolor: QfTheme.toolButtonBackgroundColor
            }
          }
        ]

        onClicked: {
          positioningSettings.showPositionInformation = checked;
          actionsPieMenu.close();
        }
      }

      QfToolButton {
        id: trackingButton
        width: actionsPieMenu.bandWidth - 8
        height: width
        padding: 2
        iconSource: QfTheme.getThemeVectorIcon("directions_walk_24dp")
        round: true
        checkable: false
        checked: false
        enabled: true
        iconColor: QfTheme.light
        bgcolor: QfTheme.toolButtonBackgroundColor
        state: trackings.count ? "On" : "Off"
        visible: actionsPieMenu.openingAngle >= actionsPieMenu.segmentAngle * 6

        states: [
          State {
            name: "Off"
            PropertyChanges {
              target: trackingButton
              iconColor: QfTheme.light
              bgcolor: QfTheme.toolButtonBackgroundSemiOpaqueColor
            }
          },
          State {
            name: "On"
            PropertyChanges {
              target: trackingButton
              iconColor: QfTheme.positionColor
              bgcolor: QfTheme.toolButtonBackgroundColor
            }
          }
        ]

        onClicked: {
          if (trackings.count > 0) {
            displayToast(qsTr('Tracking active on %n layer(s)', '', trackings.count), 'info', qsTr('Stop all'), function () {
              displayToast(qsTr('Tracking on %n layer(s) stopped', '', trackings.count));
              trackingModel.stopTrackers();
            });
          } else {
            trackerSettings.prepareSettings();
            if (trackerSettings.availableLayersCount > 0) {
              trackerSettings.open();
            } else {
              displayToast(qsTr('No compatible layers available to launch tracking'), 'info', qsTr('Learn more'), function () {
                Qt.openUrlExternally('https://docs.qfield.org/how-to/navigation-and-positioning/tracking/');
              });
            }
          }
          actionsPieMenu.close();
        }
      }

      Component.onCompleted: {
        onOpened.connect(() => {
          settings.setValue("/QField/pieMenuOpenedOnce", true);
          locationMarker.bubbleVisible = false;
        });
      }
    }

    QfToolButtonPie {
      id: destinationActionsPieMenu

      readonly property int minimumDistanceToScreenEdge: 80
      readonly property real menuHalfSize: destinationActionsPieMenu.width / 2

      readonly property bool tooCloseToLeft: targetPoint && targetPoint.x - menuHalfSize - minimumDistanceToScreenEdge < 0
      readonly property bool tooCloseToRight: targetPoint && targetPoint.x + menuHalfSize + minimumDistanceToScreenEdge > mainWindow.width
      readonly property bool tooCloseToTop: targetPoint && targetPoint.y - menuHalfSize - minimumDistanceToScreenEdge < 0
      readonly property bool tooCloseToBottom: targetPoint && targetPoint.y + menuHalfSize + minimumDistanceToScreenEdge + informationDrawer.height > mainWindow.height
      readonly property bool nearToEdge: tooCloseToLeft || tooCloseToRight || tooCloseToTop || tooCloseToBottom

      readonly property bool destinationOutsidePieMenu: {
        if (!visible || !targetPoint)
          return true;
        const dx = destinationActionsPieMenu.x + (destinationActionsPieMenu.width / 2) - targetPoint.x;
        const dy = destinationActionsPieMenu.y + (destinationActionsPieMenu.height / 2) - targetPoint.y;
        const distance = Math.sqrt(dx * dx + dy * dy);
        return distance > 20;
      }

      readonly property int segmentAngle: 360 / destinationActionsPieMenu.numberOfButtons

      width: Math.min(150, mapCanvasMap.width / 3)
      height: width

      showConnectionLine: visible && (nearToEdge || destinationOutsidePieMenu)
      linkColor: QfTheme.navigationColor

      function openPieMenu(screenLocation) {
        targetPoint = screenLocation;
        if (tooCloseToLeft) {
          destinationActionsPieMenu.x = minimumDistanceToScreenEdge;
        } else if (tooCloseToRight) {
          destinationActionsPieMenu.x = mainWindow.width - destinationActionsPieMenu.width - minimumDistanceToScreenEdge;
        } else {
          destinationActionsPieMenu.x = screenLocation.x - menuHalfSize;
        }
        if (tooCloseToTop) {
          destinationActionsPieMenu.y = minimumDistanceToScreenEdge;
        } else if (tooCloseToBottom) {
          destinationActionsPieMenu.y = mainWindow.height - destinationActionsPieMenu.height - informationDrawer.height - minimumDistanceToScreenEdge;
        } else {
          destinationActionsPieMenu.y = screenLocation.y - menuHalfSize;
        }
        destinationActionsPieMenu.open();
      }

      Component.onCompleted: {
        pointHandler.registerHandler("DestinationMarker", (point, type, interactionType) => {
          if (!navigation.isActive || (interactionType !== "clicked" && interactionType !== "pressedAndHold")) {
            return false;
          }
          const destinationScreenLocation = mapCanvas.mapSettings.coordinateToScreen(navigation.destination);
          const dx = point.x - destinationScreenLocation.x;
          const dy = point.y - destinationScreenLocation.y;
          const distance = Math.sqrt(dx * dx + dy * dy);
          const markerHit = distance <= 25;
          if (markerHit) {
            if (type === "stylus" && interactionType === "clicked") {
              mainWindow.displayToast(qsTr("Long press on the destination marker to show actions"));
            } else if ((type !== "stylus" && interactionType === "clicked") || (type === "stylus" && interactionType === "pressedAndHold")) {
              openPieMenu(destinationScreenLocation);
              return true;
            }
          }
          return false;
        }, QfMapCanvasPointHandler.Priority.High);
      }

      QfToolButton {
        id: clearDestinationButton
        width: destinationActionsPieMenu.bandWidth - 8
        height: width
        padding: 2
        round: true
        iconSource: QfTheme.getThemeVectorIcon("ic_delete_forever_white_24dp")
        iconColor: QfTheme.light
        bgcolor: QfTheme.toolButtonBackgroundColor
        visible: destinationActionsPieMenu.openingAngle >= destinationActionsPieMenu.segmentAngle

        onClicked: {
          navigation.clear();
          destinationActionsPieMenu.close();
        }
      }

      QfToolButton {
        id: alwaysShowPreciseViewButton
        width: destinationActionsPieMenu.bandWidth - 8
        height: width
        padding: 2
        round: true
        checkable: true
        checked: positioningSettings.alwaysShowPreciseView
        state: checked ? "On" : "Off"
        visible: destinationActionsPieMenu.openingAngle >= destinationActionsPieMenu.segmentAngle * 2
        iconSource: QfTheme.getThemeVectorIcon("ic_radar_grey_24dp")

        states: [
          State {
            name: "Off"
            PropertyChanges {
              target: alwaysShowPreciseViewButton
              iconColor: QfTheme.light
              bgcolor: QfTheme.toolButtonBackgroundSemiOpaqueColor
            }
          },
          State {
            name: "On"
            PropertyChanges {
              target: alwaysShowPreciseViewButton
              iconColor: QfTheme.navigationColor
              bgcolor: QfTheme.toolButtonBackgroundColor
            }
          }
        ]

        onClicked: {
          positioningSettings.alwaysShowPreciseView = !positioningSettings.alwaysShowPreciseView;
          destinationActionsPieMenu.close();
        }
      }
    }

    /* QfRubberband for vertices  */
    Item {
      // highlighting geometry (point, line, surface)
      QfRubberband {
        id: editingRubberband
        vertexModel: geometryEditingVertexModel
        mapSettings: mapCanvas.mapSettings
      }

      // highlighting vertices
      QfVertexRubberband {
        id: vertexRubberband
        model: geometryEditingVertexModel
        mapSettings: mapCanvas.mapSettings
        vertexColor: QfTheme.vertexColorSemiOpaque
        vertexBorderColor: QfTheme.vertexColor
        selectedVertexColor: QfTheme.vertexSelectedColorSemiOpaque
        selectedVertexBorderColor: QfTheme.vertexSelectedColor
        newVertexColor: QfTheme.vertexNewColorSemiOpaque
        newVertexBorderColor: QfTheme.vertexNewColor
      }
    }

    /* Highlight the currently selected item on the feature list */
    QfFeatureListSelectionHighlight {
      id: featureListHighlight
      visible: !moveFeaturesToolbar.moveFeaturesRequested && !rotateFeaturesToolbar.rotateFeaturesRequested

      selectionModel: featureListForm.selection
      mapSettings: mapCanvas.mapSettings

      color: "yellow"
      focusedColor: "#ff7777"
      selectedColor: QfTheme.mainColor
      width: 5
    }

    /* Highlight the currently selected item being moved or rotate */
    QfFeatureListSelectionHighlight {
      id: moveAndRotateFeaturesHighlight
      visible: moveFeaturesToolbar.moveFeaturesRequested || rotateFeaturesToolbar.rotateFeaturesRequested
      showSelectedOnly: true

      selectionModel: featureListForm.selection
      mapSettings: mapCanvas.mapSettings

      // take rotation into account
      property double rotationRadians: -mapSettings.rotation * Math.PI / 180
      property bool hasTranslation: moveFeaturesToolbar.moveFeaturesRequested && moveFeaturesToolbar.startPoint !== undefined
      translateX: hasTranslation ? mapToScreenTranslateX.screenDistance * Math.cos(rotationRadians) - mapToScreenTranslateY.screenDistance * Math.sin(rotationRadians) : 0
      translateY: hasTranslation ? mapToScreenTranslateY.screenDistance * Math.cos(rotationRadians) + mapToScreenTranslateX.screenDistance * Math.sin(rotationRadians) : 0
      rotationDegrees: 0

      color: "yellow"
      focusedColor: "#ff7777"
      selectedColor: QfTheme.mainColor
      width: 5
    }

    /* Highlight features identified by locator or relation editor widgets */
    QfGeometryHighlighter {
      id: geometryHighlighter
      objectName: "geometryHighlighter"
    }

    QfMapToScreen {
      id: mapToScreenTranslateX
      mapSettings: mapCanvas.mapSettings
      mapDistance: {
        if (moveFeaturesToolbar.moveFeaturesRequested && moveFeaturesToolbar.startPoint !== undefined && mapCanvas.mapSettings.center) {
          if (stateMachine.state === "digitize") {
            return coordinateLocator.currentCoordinate.x - moveFeaturesToolbar.startPoint.x;
          } else {
            return mapCanvas.mapSettings.getCenter(true).x - moveFeaturesToolbar.startPoint.x;
          }
        }
        return 0;
      }
    }
    QfMapToScreen {
      id: mapToScreenTranslateY
      mapSettings: mapCanvas.mapSettings
      mapDistance: {
        if (moveFeaturesToolbar.moveFeaturesRequested && moveFeaturesToolbar.startPoint !== undefined && mapCanvas.mapSettings.center) {
          if (stateMachine.state === "digitize") {
            return coordinateLocator.currentCoordinate.y - moveFeaturesToolbar.startPoint.y;
          } else {
            return mapCanvas.mapSettings.getCenter(true).y - moveFeaturesToolbar.startPoint.y;
          }
        }
        return 0;
      }
    }

    QfProcessingAlgorithmPreview {
      id: processingAlgorithmPreview
      algorithm: featureListForm.algorithm
      mapSettings: mapCanvas.mapSettings
    }
  }

  QfGeofencer {
    id: geofencer

    position: positionSource.projectedPosition
    positionCrs: mapCanvas.mapSettings.destinationCrs

    readonly property int longVibration: 1000
    readonly property int shortVibration: 500

    onIsWithinChanged: {
      if (behavior == QfGeofencer.AlertWhenInsideGeofencedArea && geofencer.isWithin) {
        platformUtilities.vibrate(longVibration);
        displayToast(qsTr("Position has trespassed into ‘%1’").arg(isWithinAreaName), 'error');
      } else if (behavior == QfGeofencer.AlertWhenOutsideGeofencedArea && !geofencer.isWithin) {
        platformUtilities.vibrate(longVibration);
        displayToast(qsTr("Position outside areas after leaving ‘%1’").arg(lastWithinAreaName), 'error');
      } else if (behavior == QfGeofencer.InformWhenEnteringLeavingGeofencedArea) {
        if (isWithin) {
          platformUtilities.vibrate(shortVibration);
          displayToast(qsTr("Position entered into ‘%1’").arg(isWithinAreaName));
        } else if (lastWithinAreaName != '') {
          platformUtilities.vibrate(shortVibration);
          displayToast(qsTr("Position left from ‘%1’").arg(lastWithinAreaName));
        }
      }
    }
  }

  MultiEffect {
    id: geofencerFeedback
    anchors.fill: geofencerFeedbackSource
    source: geofencerFeedbackSource
    visible: true
    blurEnabled: true
    blurMax: 64
    blur: 2.0
    opacity: 0

    SequentialAnimation {
      id: geofencerFeedbackAnimation
      running: geofencer.isAlerting
      loops: Animation.Infinite

      onRunningChanged: {
        if (!running) {
          geofencerFeedback.opacity = 0;
        }
      }

      OpacityAnimator {
        target: geofencerFeedback
        from: 0
        to: 0.75
        duration: 1000
      }
      OpacityAnimator {
        target: geofencerFeedback
        from: 0.75
        to: 0
        duration: 1000
      }
    }
  }

  Rectangle {
    id: geofencerFeedbackSource
    width: Math.min(250, mainWindow.width / 2)
    height: width
    radius: width / 2
    visible: false

    x: parent.width - width / 2
    y: locationToolbar.y + gnssButton.y + (gnssButton.height / 2) - height / 2

    color: QfTheme.errorColor
  }

  QfInformationDrawer {
    id: informationDrawer

    navigation: navigation
    positionSource: positionSource
    positioningSettings: positioningSettings

    cogoOperationSettings.mapSettings: mapCanvas.mapSettings
  }

  /**************************************************
   * Map Canvas Overlays
   * - Decorations
   * - Scale Bar
   * - UI elements such as QfToolButtons
   **************************************************/
  Item {
    id: mapCanvasOverlays
    anchors.fill: mapCanvas
    anchors.bottomMargin: informationDrawer.height

    QfExpressionEvaluator {
      id: decorationExpressionEvaluator
      mode: QfExpressionEvaluator.ExpressionTemplateMode
      mapSettings: mapCanvas.mapSettings
      project: qgisProject
      appExpressionContextScopesGenerator: appScopesGenerator
    }

    Connections {
      target: mapCanvasMap
      enabled: titleDecoration.isExpressionTemplate || copyrightDecoration.isExpressionTemplate

      function onIsRenderingChanged() {
        if (mapCanvasMap.isRendering) {
          if (titleDecoration.isExpressionTemplate) {
            decorationExpressionEvaluator.expressionText = titleDecoration.decorationText;
            titleDecoration.text = decorationExpressionEvaluator.evaluate();
          }
          if (copyrightDecoration.isExpressionTemplate) {
            decorationExpressionEvaluator.expressionText = copyrightDecoration.decorationText;
            copyrightDecoration.text = decorationExpressionEvaluator.evaluate();
          }
        }
      }
    }

    Connections {
      target: positionSource
      enabled: titleDecoration.isExpressionPositioning || copyrightDecoration.isExpressionPositioning

      function onPositionInformationChanged() {
        if (titleDecoration.isExpressionPositioning) {
          decorationExpressionEvaluator.expressionText = titleDecoration.decorationText;
          titleDecoration.text = decorationExpressionEvaluator.evaluate();
        }
        if (copyrightDecoration.isExpressionPositioning) {
          decorationExpressionEvaluator.expressionText = copyrightDecoration.decorationText;
          copyrightDecoration.text = decorationExpressionEvaluator.evaluate();
        }
      }
    }

    Rectangle {
      id: titleDecorationBackground

      visible: titleDecoration.text != ''
      anchors.top: parent.top
      anchors.topMargin: mainWindow.sceneTopMargin + 4
      anchors.left: parent.left
      anchors.leftMargin: 56 + mainWindow.sceneLeftMargin
      anchors.right: parent.right
      anchors.rightMargin: 56 + mainWindow.sceneRightMargin

      height: 48
      radius: 8

      color: '#55000000'

      Text {
        id: titleDecoration

        property string decorationText: ''
        property bool isExpressionTemplate: decorationText.match('\[%.*%\]')
        property bool isExpressionPositioning: isExpressionTemplate && decorationText.match('\[%.*(@gnss_|@position_).*%\]')

        width: parent.width - 4
        height: parent.height
        topPadding: 2
        leftPadding: 2
        rightPadding: 2
        bottomPadding: 2

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        wrapMode: Text.WordWrap
        elide: Text.ElideRight

        font: QfTheme.strongFont
        fontSizeMode: Text.Fit

        text: ''
      }
    }

    Rectangle {
      id: copyrightDecorationBackground

      visible: copyrightDecoration.text != ''
      anchors.left: parent.left
      anchors.leftMargin: 56 + mainWindow.sceneLeftMargin
      anchors.right: parent.right
      anchors.rightMargin: 56 + mainWindow.sceneRightMargin
      anchors.bottom: parent.bottom
      anchors.bottomMargin: parent.height > 400 || stateMachine.state !== "browse" ? 56 : 6

      height: visible ? Math.min(copyrightDecoration.height, 48) : 0
      radius: 4
      clip: true

      color: '#55000000'

      Text {
        id: copyrightDecoration

        property string decorationText: ''
        property bool isExpressionTemplate: decorationText.match('\[%.*%\]')
        property bool isExpressionPositioning: isExpressionTemplate && decorationText.match('\[%.*(@gnss_|@position_).*%\]')

        anchors.bottom: parent.bottom

        width: parent.width - 4
        leftPadding: 2
        rightPadding: 2

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignBottom
        wrapMode: Text.WordWrap
        elide: Text.ElideRight

        font: QfTheme.tinyFont

        text: ''
      }
    }

    QfParameterizedImage {
      id: imageDecoration

      visible: source != ''
      anchors.left: parent.left
      anchors.leftMargin: 56 + mainWindow.sceneLeftMargin
      anchors.right: parent.right
      anchors.rightMargin: 56 + mainWindow.sceneRightMargin
      anchors.bottom: copyrightDecorationBackground.top
      anchors.bottomMargin: 4

      width: parent.width - anchors.leftMargin * 2
      height: 48

      source: ""
    }

    Text {
      id: coordinateLocatorInformationOverlay

      property bool coordinatesIsXY: QfCoordinateReferenceSystemUtils.defaultCoordinateOrderForCrsIsXY(projectInfo.coordinateDisplayCrs)
      property bool coordinatesIsGeographic: projectInfo.coordinateDisplayCrs.isGeographic

      QfDistanceArea {
        id: digitizingGeometryMeasure

        property VectorLayer currentLayer: dashBoard.activeLayer

        rubberbandModel: currentRubberband ? currentRubberband.model : null
        project: qgisProject
        crs: qgisProject ? qgisProject.crs : QfCoordinateReferenceSystemUtils.invalidCrs()
      }

      // The position is dynamically calculated to follow the coordinate locator
      x: {
        var newX = coordinateLocator.displayPosition.x + 20;
        if (newX + width > mapCanvas.x + mapCanvas.width)
          newX -= width + 40;
        return newX;
      }
      y: {
        var newY = coordinateLocator.displayPosition.y + 14;
        if (newY + height > mapCanvas.y + mapCanvas.height)
          newY -= height - 28;
        return newY;
      }

      textFormat: Text.PlainText
      text: {
        if ((qfieldSettings.numericalDigitizingInformation && stateMachine.state === "digitize") || stateMachine.state === 'measure') {
          var point = QfGeometryUtils.reprojectPoint(coordinateLocator.currentCoordinate, coordinateLocator.mapSettings.destinationCrs, projectInfo.coordinateDisplayCrs);
          var coordinates;
          if (coordinatesIsXY) {
            coordinates = '%1: %2\n%3: %4\n'.arg(coordinatesIsGeographic ? qsTr('Lon') : 'X').arg(point.x.toLocaleString(Qt.locale(), 'f', coordinatesIsGeographic ? 5 : 2)).arg(coordinatesIsGeographic ? qsTr('Lat') : 'Y').arg(point.y.toLocaleString(Qt.locale(), 'f', coordinatesIsGeographic ? 5 : 2));
          } else {
            coordinates = '%1: %2\n%3: %4\n'.arg(coordinatesIsGeographic ? qsTr('Lat') : 'Y').arg(point.y.toLocaleString(Qt.locale(), 'f', coordinatesIsGeographic ? 5 : 2)).arg(coordinatesIsGeographic ? qsTr('Lon') : 'X').arg(point.x.toLocaleString(Qt.locale(), 'f', coordinatesIsGeographic ? 5 : 2));
          }
          return '%1%2%3%4%5%6'.arg(stateMachine.state === 'digitize' || !digitizingToolbar.isDigitizing ? coordinates : '').arg(digitizingGeometryMeasure.lengthValid && digitizingGeometryMeasure.segmentLength != 0.0 ? '%1: %2\n'.arg(digitizingGeometryMeasure.segmentLength != digitizingGeometryMeasure.length ? qsTr('Segment') : qsTr('Length')).arg(UnitTypes.formatDistance(digitizingGeometryMeasure.convertLengthMeansurement(digitizingGeometryMeasure.segmentLength, projectInfo.distanceUnits), 3, projectInfo.distanceUnits)) : '').arg(digitizingGeometryMeasure.lengthValid && digitizingGeometryMeasure.segmentLength != 0.0 ? '%1: %2\n'.arg(qsTr('Azimuth')).arg(UnitTypes.formatAngle(digitizingGeometryMeasure.azimuth < 0 ? digitizingGeometryMeasure.azimuth + 360 : digitizingGeometryMeasure.azimuth, 2, Qgis.AngleUnit.Degrees)) : '').arg(currentRubberband && currentRubberband.model && currentRubberband.model.geometryType === Qgis.GeometryType.Polygon ? digitizingGeometryMeasure.perimeterValid ? '%1: %2\n'.arg(qsTr('Perimeter')).arg(UnitTypes.formatDistance(digitizingGeometryMeasure.convertLengthMeansurement(digitizingGeometryMeasure.perimeter, projectInfo.distanceUnits), 3, projectInfo.distanceUnits)) : '' : digitizingGeometryMeasure.lengthValid && digitizingGeometryMeasure.segmentLength != digitizingGeometryMeasure.length ? '%1: %2\n'.arg(qsTr('Length')).arg(UnitTypes.formatDistance(digitizingGeometryMeasure.convertLengthMeansurement(digitizingGeometryMeasure.length, projectInfo.distanceUnits), 3, projectInfo.distanceUnits)) : '').arg(digitizingGeometryMeasure.areaValid ? '%1: %2\n'.arg(qsTr('Area')).arg(UnitTypes.formatArea(digitizingGeometryMeasure.convertAreaMeansurement(digitizingGeometryMeasure.area, projectInfo.areaUnits), 3, projectInfo.areaUnits)) : '').arg(stateMachine.state === 'measure' && digitizingToolbar.isDigitizing ? coordinates : '');
        } else {
          return '';
        }
      }

      font: QfTheme.strongTipFont
      style: Text.Outline
      styleColor: QfTheme.light
    }

    QfToolButton {
      id: compassArrow
      rotation: mapCanvas.mapSettings.rotation
      visible: rotation !== 0 && stateMachine.state !== '3d'
      anchors.left: parent.left
      anchors.bottom: parent.bottom
      anchors.leftMargin: mainWindow.sceneLeftMargin + 4
      anchors.bottomMargin: 54
      round: true
      bgcolor: QfTheme.toolButtonBackgroundSemiOpaqueColor

      Shape {
        width: compassArrow.width
        height: compassArrow.height

        ShapePath {
          strokeWidth: 3
          strokeColor: "transparent"
          strokeStyle: ShapePath.SolidLine
          fillColor: QfTheme.mainColor
          joinStyle: ShapePath.MiterJoin
          startX: compassArrow.width / 2
          startY: 8
          PathLine {
            x: compassArrow.width / 2 + 6
            y: compassArrow.height / 2
          }
          PathLine {
            x: compassArrow.width / 2
            y: compassArrow.height / 2 - 2
          }
          PathLine {
            x: compassArrow.width / 2 - 6
            y: compassArrow.height / 2
          }
          PathLine {
            x: compassArrow.width / 2
            y: 8
          }
        }

        ShapePath {
          strokeWidth: 3
          strokeColor: "transparent"
          strokeStyle: ShapePath.SolidLine
          fillColor: QfTheme.toolButtonColor
          joinStyle: ShapePath.MiterJoin
          startX: compassArrow.width / 2
          startY: compassArrow.height - 8
          PathLine {
            x: compassArrow.width / 2 + 6
            y: compassArrow.height / 2
          }
          PathLine {
            x: compassArrow.width / 2
            y: compassArrow.height / 2 + 2
          }
          PathLine {
            x: compassArrow.width / 2 - 6
            y: compassArrow.height / 2
          }
          PathLine {
            x: compassArrow.width / 2
            y: compassArrow.height - 8
          }
        }
      }

      onClicked: {
        if (gnssButton.followActive) {
          mapCanvasMap.unfreeze('follow');
          gnssButton.followActive = false;
          if (gnssButton.autoRefollow) {
            showAutoLockToast();
          }
        } else if (gnssButton.autoRefollow) {
          showAutoLockToast();
        }

        mapCanvas.mapSettings.rotation = 0;
      }
    }

    QfScaleBar {
      visible: qfieldSettings.showScaleBar && stateMachine.state !== '3d'
      mapSettings: mapCanvas.mapSettings
      anchors.left: parent.left
      anchors.bottom: parent.bottom
      anchors.leftMargin: mainWindow.sceneLeftMargin + 8
      anchors.bottomMargin: 10
    }

    Column {
      id: pluginsToolbar
      objectName: "pluginsToolbar"

      anchors.right: locatorItem.right
      anchors.top: locatorItem.top
      anchors.topMargin: QfTheme.toolButtonSize + 4
      spacing: 4
    }

    Column {
      id: zoomToolbar
      anchors.right: parent.right
      anchors.rightMargin: mainWindow.sceneRightMargin + 10
      anchors.bottom: parent.bottom
      anchors.bottomMargin: (parent.height - zoomToolbar.height / 2) / 2
      spacing: 8
      visible: qfieldSettings.showZoomControls && (!screenLocker.enabled && (locationToolbar.height + digitizingToolbarContainer.height) / (digitizingToolbarContainer.y) < 0.41)

      QfToolButton {
        id: zoomInButton
        round: true
        anchors.right: parent.right

        bgcolor: QfTheme.toolButtonBackgroundColor
        iconSource: QfTheme.getThemeVectorIcon("ic_add_white_24dp")
        iconColor: QfTheme.toolButtonColor

        width: 36
        height: 36

        onClicked: {
          if (stateMachine.state === '3d') {
            if (mapCanvas3DLoader.item) {
              mapCanvas3DLoader.item.zoomIn();
            }
          } else {
            if (gnssButton.followActive) {
              gnssButton.followActiveSkipExtentChanged = true;
            }
            mapCanvasMap.zoomIn(Qt.point(mapCanvas.x + (mapCanvas.width - mapCanvasMap.rightMargin) / 2, mapCanvas.y + (mapCanvas.height - mapCanvasMap.bottomMargin) / 2));
            if (gnssButton.followActive) {
              // Trigger a map redraw
              gnssButton.followLocation(true);
            }
          }
        }
      }
      QfToolButton {
        id: zoomOutButton
        round: true
        anchors.right: parent.right

        bgcolor: QfTheme.toolButtonBackgroundColor
        iconSource: QfTheme.getThemeVectorIcon("ic_remove_white_24dp")
        iconColor: QfTheme.toolButtonColor

        width: 36
        height: 36

        onClicked: {
          if (stateMachine.state === '3d') {
            if (mapCanvas3DLoader.item) {
              mapCanvas3DLoader.item.zoomOut();
            }
          } else {
            if (gnssButton.followActive) {
              gnssButton.followActiveSkipExtentChanged = true;
            }
            mapCanvasMap.zoomOut(Qt.point(mapCanvas.x + (mapCanvas.width - mapCanvasMap.rightMargin) / 2, mapCanvas.y + (mapCanvas.height - mapCanvasMap.bottomMargin) / 2));
            if (gnssButton.followActive) {
              // Trigger a map redraw
              gnssButton.followLocation(true);
            }
          }
        }
      }
    }

    QfLocatorItem {
      id: locatorItem
      objectName: "locatorItem"

      locatorBridge: locatorBridge

      anchors.right: parent.right
      anchors.top: parent.top
      anchors.topMargin: mainWindow.sceneTopMargin + 4
      anchors.rightMargin: mainWindow.sceneRightMargin + 4

      visible: !screenLocker.enabled && stateMachine.state !== 'measure'

      onStateChanged: {
        if (state == "off") {
          focus = false;
          if (featureListForm.visible) {
            featureListForm.focus = true;
          } else {
            keyHandler.focus = true;
          }
        }
      }
    }

    QfDropShadow {
      anchors.fill: locatorItem
      visible: locatorItem.searchFieldVisible
      verticalOffset: 2
      radius: 10
      color: "#66212121"
      source: locatorItem
    }

    /* The main menu */
    Row {
      id: mainMenuBar
      visible: !screenLocker.enabled
      width: childrenRect.width
      height: childrenRect.height
      topPadding: mainWindow.sceneTopMargin + 4
      leftPadding: mainWindow.sceneLeftMargin + 4
      spacing: 4

      QfToolButton {
        id: menuButton
        round: true
        iconSource: QfTheme.getThemeVectorIcon("ic_menu_white_24dp")
        iconColor: QfTheme.toolButtonColor
        bgcolor: dashBoard.opened ? QfTheme.mainColor : QfTheme.toolButtonBackgroundColor

        onClicked: dashBoard.opened ? dashBoard.close() : dashBoard.open()

        onPressAndHold: {
          mainMenu.popup(menuButton.x, menuButton.y);
        }

        QfBadge {
          alignment: QfBadge.Alignment.TopRight
          width: menuButton.width / 4
          topMargin: menuButton.width / 24
          rightMargin: menuButton.width / 24
          visible: showSync || showPush
          color: showSync ? QfTheme.mainColor : QfTheme.cloudColor
          enableGradient: showSync && showPush

          readonly property bool showSync: cloudProjectsModel.currentProject ? cloudProjectsModel.currentProject.isOutdated : false
          readonly property bool showPush: cloudProjectsModel.layerObserver.deltaFileWrapper && cloudProjectsModel.layerObserver.deltaFileWrapper.count > 0
        }
      }

      QfActionButton {
        id: closeMeasureTool
        visible: stateMachine.state === 'measure'
        toolImage: QfTheme.getThemeVectorIcon("ic_measurement_black_24dp")
        toolText: qsTr('Close measure tool')

        onClicked: mainWindow.closeMeasureTool()
      }

      QfActionButton {
        id: close3DView
        visible: stateMachine.state === '3d'
        toolImage: QfTheme.getThemeVectorIcon("ic_3d_white_24dp")
        toolText: qsTr('Close 3D view')

        onClicked: {
          if (mapCanvas3DLoader.item && mapCanvas3DLoader.item.playClosingAnimation) {
            mapCanvas3DLoader.item.playClosingAnimation(function () {
              mainWindow.close3DView();
            });
          } else {
            mainWindow.close3DView();
          }
        }
      }

      QfActionButton {
        id: closeGeometryEditorsTool
        visible: (stateMachine.state === "digitize" && geometryEditingVertexModel.vertexCount > 0)
        toolImage: geometryEditorsToolbar.image
        toolText: qsTr('Stop editing')

        onClicked: geometryEditorsToolbar.cancelEditors()
      }

      QfActionButton {
        id: abortRequestGeometry
        visible: digitizingToolbar.geometryRequested
        toolImage: QfTheme.getThemeVectorIcon("ic_edit_geometry_white_24dp")
        toolText: qsTr('Cancel addition')

        onClicked: digitizingToolbar.cancel()
      }
    }

    Column {
      id: mainToolbar
      visible: !screenLocker.enabled
      anchors.left: mainMenuBar.left
      anchors.top: mainMenuBar.bottom
      anchors.leftMargin: mainWindow.sceneLeftMargin + 4
      anchors.topMargin: 4
      spacing: 4

      QfToolButtonDrawer {
        objectName: "digitizingDrawer"
        name: "digitizingDrawer"
        round: true
        bgcolor: QfTheme.toolButtonBackgroundColor
        iconSource: QfTheme.getThemeVectorIcon('ic_digitizing_settings_black_24dp')
        iconColor: QfTheme.toolButtonColor
        spacing: 4
        visible: stateMachine.state === "digitize" && dashBoard.activeLayer && dashBoard.activeLayer.isValid && (dashBoard.activeLayer.geometryType() === Qgis.GeometryType.Polygon || dashBoard.activeLayer.geometryType() === Qgis.GeometryType.Line || dashBoard.activeLayer.geometryType() === Qgis.GeometryType.Point)

        QfToolButton {
          id: cogoButton
          width: QfTheme.toolButtonSize * 5 / 6
          height: QfTheme.toolButtonSize * 5 / 6
          padding: 2
          round: true
          state: digitizingToolbar.cogoEnabled ? "On" : "Off"
          iconSource: QfTheme.getThemeVectorIcon("ic_cogo_white_24dp")
          iconColor: QfTheme.toolButtonColor
          bgcolor: QfTheme.toolButtonBackgroundSemiOpaqueColor

          states: [
            State {

              name: "Off"
              PropertyChanges {
                target: cogoButton
                iconColor: QfTheme.toolButtonColor
                bgcolor: QfTheme.toolButtonBackgroundSemiOpaqueColor
              }
            },
            State {
              name: "On"
              PropertyChanges {
                target: cogoButton
                iconColor: QfTheme.mainColor
                bgcolor: QfTheme.toolButtonBackgroundColor
              }
            }
          ]

          onClicked: {
            digitizingToolbar.cogoEnabled = !digitizingToolbar.cogoEnabled;
            displayToast(digitizingToolbar.cogoEnabled ? qsTr("COGO digitizing turned on") : qsTr("COGO digitizing turned off"));
          }
        }

        QfToolButton {
          id: snappingButton
          width: QfTheme.toolButtonSize * 5 / 6
          height: QfTheme.toolButtonSize * 5 / 6
          padding: 2
          round: true
          state: qgisProject && qgisProject.snappingConfig.enabled ? "On" : "Off"
          iconSource: QfTheme.getThemeVectorIcon("ic_snapping_white_24dp")
          iconColor: QfTheme.toolButtonColor
          bgcolor: QfTheme.toolButtonBackgroundSemiOpaqueColor

          states: [
            State {

              name: "Off"
              PropertyChanges {
                target: snappingButton
                iconColor: QfTheme.toolButtonColor
                bgcolor: QfTheme.toolButtonBackgroundSemiOpaqueColor
              }
            },
            State {
              name: "On"
              PropertyChanges {
                target: snappingButton
                iconColor: QfTheme.mainColor
                bgcolor: QfTheme.toolButtonBackgroundColor
              }
            }
          ]

          onClicked: {
            var snappingConfig = qgisProject.snappingConfig;
            snappingConfig.enabled = !snappingConfig.enabled;
            qgisProject.snappingConfig = snappingConfig;
            projectInfo.snappingEnabled = snappingConfig.enabled;
            displayToast(snappingConfig.enabled ? qsTr("Snapping turned on") : qsTr("Snapping turned off"));
          }
        }

        QfToolButton {
          id: topologyButton
          width: QfTheme.toolButtonSize * 5 / 6
          height: QfTheme.toolButtonSize * 5 / 6
          padding: 2
          round: true
          state: qgisProject && qgisProject.topologicalEditing ? "On" : "Off"
          iconSource: QfTheme.getThemeVectorIcon("ic_topology_white_24dp")
          iconColor: QfTheme.toolButtonColor
          bgcolor: QfTheme.toolButtonBackgroundSemiOpaqueColor

          states: [
            State {

              name: "Off"
              PropertyChanges {
                target: topologyButton
                iconColor: QfTheme.toolButtonColor
                bgcolor: QfTheme.toolButtonBackgroundSemiOpaqueColor
              }
            },
            State {
              name: "On"
              PropertyChanges {
                target: topologyButton
                iconColor: QfTheme.mainColor
                bgcolor: QfTheme.toolButtonBackgroundColor
              }
            }
          ]

          onClicked: {
            qgisProject.topologicalEditing = !qgisProject.topologicalEditing;
            displayToast(qgisProject.topologicalEditing ? qsTr("Topological editing turned on") : qsTr("Topological editing turned off"));
          }
        }

        QfToolButton {
          id: freehandButton
          width: visible ? QfTheme.toolButtonSize * 5 / 6 : 0
          height: visible ? QfTheme.toolButtonSize * 5 / 6 : 0
          padding: 2
          round: true
          visible: hoverHandler.hasBeenHovered && !(positionSource.active && coordinateLocator.positionLocked) && stateMachine.state === "digitize" && ((digitizingToolbar.geometryRequested && digitizingToolbar.geometryRequestedLayer && digitizingToolbar.geometryRequestedLayer.isValid && (digitizingToolbar.geometryRequestedLayer.geometryType() === Qgis.GeometryType.Polygon || digitizingToolbar.geometryRequestedLayer.geometryType() === Qgis.GeometryType.Line)) || (!digitizingToolbar.geometryRequested && dashBoard.activeLayer && dashBoard.activeLayer.isValid && (dashBoard.activeLayer.geometryType() === Qgis.GeometryType.Polygon || dashBoard.activeLayer.geometryType() === Qgis.GeometryType.Line)))
          iconSource: QfTheme.getThemeVectorIcon("ic_freehand_white_24dp")
          iconColor: QfTheme.toolButtonColor
          bgcolor: QfTheme.toolButtonBackgroundSemiOpaqueColor

          property bool freehandDigitizing: false
          state: freehandDigitizing ? "On" : "Off"

          states: [
            State {
              name: "Off"
              PropertyChanges {
                target: freehandButton
                iconColor: QfTheme.toolButtonColor
                bgcolor: QfTheme.toolButtonBackgroundSemiOpaqueColor
              }
            },
            State {
              name: "On"
              PropertyChanges {
                target: freehandButton
                iconColor: QfTheme.mainColor
                bgcolor: QfTheme.toolButtonBackgroundColor
              }
            }
          ]

          onClicked: {
            freehandDigitizing = !freehandDigitizing;
            if (freehandDigitizing && positioningSettings.positioningCoordinateLock) {
              positioningSettings.positioningCoordinateLock = false;
            }
            displayToast(freehandDigitizing ? qsTr("Freehand digitizing turned on") : qsTr("Freehand digitizing turned off"));
            settings.setValue("/QField/Digitizing/FreehandActive", freehandDigitizing);
          }

          Component.onCompleted: {
            freehandDigitizing = settings.valueBool("/QField/Digitizing/FreehandActive", false);
          }
        }

        QfToolButton {
          id: snapToCommonAngleButton

          width: visible ? QfTheme.toolButtonSize * 5 / 6 : 0
          height: visible ? QfTheme.toolButtonSize * 5 / 6 : 0
          round: true
          visible: dashBoard.activeLayer && (dashBoard.activeLayer.geometryType() === Qgis.GeometryType.Polygon || dashBoard.activeLayer.geometryType() === Qgis.GeometryType.Line)
          iconSource: QfTheme.getThemeVectorIcon("ic_common_angle_white_24dp")
          iconColor: QfTheme.toolButtonColor
          bgcolor: QfTheme.toolButtonBackgroundSemiOpaqueColor

          state: qfieldSettings.snapToCommonAngleIsEnabled ? "On" : "Off"

          states: [
            State {
              name: "Off"
              PropertyChanges {
                target: snapToCommonAngleButton
                iconColor: QfTheme.toolButtonColor
                bgcolor: QfTheme.toolButtonBackgroundSemiOpaqueColor
              }
            },
            State {
              name: "On"
              PropertyChanges {
                target: snapToCommonAngleButton
                iconColor: QfTheme.mainColor
                bgcolor: QfTheme.toolButtonBackgroundColor
              }
            }
          ]

          onClicked: {
            qfieldSettings.snapToCommonAngleIsEnabled = !qfieldSettings.snapToCommonAngleIsEnabled;
            displayToast(qfieldSettings.snapToCommonAngleIsEnabled ? qsTr("Snap to %1° angle turned on").arg(qfieldSettings.snapToCommonAngleDegrees) : qsTr("Snap to common angle turned off"));
          }

          onPressAndHold: {
            snapToCommonAngleMenu.popup(parent.x, parent.y);
          }

          Menu {
            id: snapToCommonAngleMenu
            width: QfTheme.menuItemIconlessLeftPadding + Math.max(angles.count * 35, tolorences.count * 55) + 24

            MenuItem {
              text: qsTr("Relative angle")
              font: QfTheme.defaultFont
              height: 48
              leftPadding: QfTheme.menuItemCheckLeftPadding

              checkable: true
              checked: qfieldSettings.snapToCommonAngleIsRelative

              onTriggered: {
                qfieldSettings.snapToCommonAngleIsRelative = !qfieldSettings.snapToCommonAngleIsRelative;
              }
            }

            MenuSeparator {
              width: parent.width
            }

            Text {
              text: qsTr("Snapping to every")
              color: QfTheme.mainTextColor
              font: QfTheme.defaultFont
              leftPadding: QfTheme.menuItemIconlessLeftPadding
            }

            Item {
              width: 1
              height: 8
            }

            ListView {
              id: angles
              height: 35
              anchors {
                left: parent.left
                leftMargin: QfTheme.menuItemIconlessLeftPadding
                rightMargin: 4
              }
              spacing: 3
              orientation: ListView.Horizontal
              model: [10, 15, 30, 45, 90]
              currentIndex: Math.max(model.findIndex(q => q === qfieldSettings.snapToCommonAngleDegrees), 0)
              highlightFollowsCurrentItem: true

              highlight: Rectangle {
                width: 35
                height: parent.height
                color: QfTheme.mainColor
                radius: width / 2
              }

              delegate: Item {
                width: 35
                height: width
                enabled: !selected

                property bool selected: modelData === qfieldSettings.snapToCommonAngleDegrees

                Text {
                  text: qsTr("%1°").arg(modelData)
                  font: parent.selected ? QfTheme.strongTipFont : QfTheme.tipFont
                  anchors.centerIn: parent
                  color: parent.selected ? QfTheme.buttonColor : QfTheme.mainTextColor
                }

                Ripple {
                  clip: true
                  anchors.fill: parent
                  clipRadius: width / 2
                  pressed: angleMouseArea.pressed
                  anchor: parent
                  active: angleMouseArea.pressed
                  color: "#22aaaaaa"
                }

                MouseArea {
                  id: angleMouseArea
                  anchors.fill: parent
                  onClicked: {
                    if (parent.selected) {
                      return;
                    }
                    qfieldSettings.snapToCommonAngleIsEnabled = true;
                    qfieldSettings.snapToCommonAngleDegrees = modelData;
                    displayToast(qsTr("Snap to %1° angle turned on").arg(modelData));
                  }
                }
              }
            }

            Item {
              width: 1
              height: 8
            }

            Text {
              text: qsTr("Snapping tolerance")
              color: QfTheme.mainTextColor
              font: QfTheme.defaultFont
              leftPadding: QfTheme.menuItemIconlessLeftPadding
            }

            Item {
              width: 1
              height: 8
            }

            ListView {
              id: tolorences
              height: 35
              anchors {
                left: parent.left
                leftMargin: QfTheme.menuItemIconlessLeftPadding
                rightMargin: 4
              }
              spacing: 3
              orientation: ListView.Horizontal
              model: [qsTr("Narrow"), qsTr("Normal"), qsTr("Large")]
              highlight: Rectangle {
                width: 35
                height: parent.height
                color: QfTheme.mainColor
                radius: 4
              }
              currentIndex: qfieldSettings.snapToCommonAngleTolerance
              highlightFollowsCurrentItem: true
              delegate: Item {
                id: tolorenceDelegate
                width: (angles.contentWidth) / 3
                height: 35
                enabled: !selected

                property bool selected: index === qfieldSettings.snapToCommonAngleTolerance

                Text {
                  id: tolorenceText
                  text: modelData
                  font: parent.selected ? QfTheme.strongTipFont : QfTheme.tipFont
                  anchors.centerIn: parent
                  color: tolorenceDelegate.selected ? QfTheme.buttonColor : QfTheme.mainTextColor
                  elide: Text.ElideRight
                  width: parent.width
                  horizontalAlignment: Text.AlignHCenter
                }

                Ripple {
                  clip: true
                  anchors.fill: parent
                  clipRadius: 4
                  pressed: tolerancesMouseArea.pressed
                  anchor: parent
                  active: tolerancesMouseArea.pressed
                  color: "#22aaaaaa"
                }

                MouseArea {
                  id: tolerancesMouseArea
                  anchors.fill: parent
                  onClicked: {
                    if (parent.selected) {
                      return;
                    }
                    qfieldSettings.snapToCommonAngleIsEnabled = true;
                    qfieldSettings.snapToCommonAngleTolerance = index;
                    displayToast(qsTr("Snapping tolerance set to %1").arg(modelData));
                  }
                }
              }
            }
          }
        }
      }

      QfToolButton {
        id: elevationProfileButton
        round: true
        visible: stateMachine.state === 'measure'
        iconSource: QfTheme.getThemeVectorIcon("ic_elevation_white_24dp")
        iconColor: QfTheme.toolButtonColor
        bgcolor: QfTheme.toolButtonBackgroundSemiOpaqueColor

        property bool elevationProfileActive: false
        state: elevationProfileActive ? "On" : "Off"

        states: [
          State {
            name: "Off"
            PropertyChanges {
              target: elevationProfileButton
              iconColor: QfTheme.toolButtonColor
              bgcolor: QfTheme.toolButtonBackgroundSemiOpaqueColor
            }
          },
          State {
            name: "On"
            PropertyChanges {
              target: elevationProfileButton
              iconColor: QfTheme.mainColor
              bgcolor: QfTheme.toolButtonBackgroundColor
            }
          }
        ]

        onClicked: {
          elevationProfileActive = !elevationProfileActive;

          // Draw an elevation profile if we have enough points to do so
          if (digitizingToolbar.rubberbandModel.vertexCount > 2) {
            // Clear the pre-existing profile to trigger a zoom to full updated profile curve
            informationDrawer.elevationProfile.clear();
            informationDrawer.elevationProfile.profileCurve = QfGeometryUtils.lineFromRubberband(digitizingToolbar.rubberbandModel, informationDrawer.elevationProfile.crs);
            informationDrawer.elevationProfile.refresh();
          }
          settings.setValue("/QField/Measuring/ElevationProfile", elevationProfileActive);
        }

        Component.onCompleted: {
          elevationProfileActive = settings.valueBool("/QField/Measuring/ElevationProfile", false);
        }
      }

      QfToolButtonDrawer {
        name: "3dDrawer"
        round: true
        collapsed: false
        bgcolor: QfTheme.toolButtonBackgroundColor
        iconSource: QfTheme.getThemeVectorIcon('ic_3d_settings_white_24dp')
        iconColor: QfTheme.toolButtonColor
        spacing: 4
        visible: stateMachine.state === '3d' && mapCanvas3DLoader.item

        QfToolButton {
          id: extentModeButton
          width: 40
          height: 40
          padding: 2
          round: true
          iconSource: QfTheme.getThemeVectorIcon("ic_move_white_24dp")
          iconColor: checked ? QfTheme.mainColor : QfTheme.toolButtonColor
          bgcolor: checked ? QfTheme.toolButtonBackgroundColor : QfTheme.toolButtonBackgroundSemiOpaqueColor
          checkable: true
          checked: mapCanvas3DLoader.item ? mapCanvas3DLoader.item.extentMode : false

          onClicked: {
            if (mapCanvas3DLoader.item) {
              mapCanvas3DLoader.item.extentMode = !mapCanvas3DLoader.item.extentMode;
            }
          }
        }

        QfToolButton {
          id: eyeDomeLightingModeButton
          width: 40
          height: 40
          padding: 2
          round: true
          iconSource: QfTheme.getThemeVectorIcon("ic_eye_dome_lighting_white_24dp")
          iconColor: checked ? QfTheme.mainColor : QfTheme.toolButtonColor
          bgcolor: checked ? QfTheme.toolButtonBackgroundColor : QfTheme.toolButtonBackgroundSemiOpaqueColor
          checkable: true
          checked: mapCanvas3DLoader.item ? mapCanvas3DLoader.item.eyeDomeLightingMode : false

          onClicked: {
            if (mapCanvas3DLoader.item) {
              mapCanvas3DLoader.item.eyeDomeLightingMode = !mapCanvas3DLoader.item.eyeDomeLightingMode;
              settings.setValue('3d/eyeDomeLightingMode', mapCanvas3DLoader.item.eyeDomeLightingMode);
            }
          }
        }
      }
    }

    BusyIndicator {
      id: busyIndicator
      anchors.left: mainMenuBar.left
      anchors.leftMargin: mainWindow.sceneLeftMargin
      anchors.top: mainToolbar.bottom
      width: menuButton.width + 10
      height: width
      running: mapCanvasMap.isRendering || (stateMachine.state === '3d' && mapCanvas3DLoader.item && mapCanvas3DLoader.item.isLoading && !mapCanvas3DLoader.item.isFirstLoad)
    }

    Column {
      id: locationToolbar
      anchors.right: parent.right
      anchors.rightMargin: mainWindow.sceneRightMargin + 4
      anchors.bottom: digitizingToolbarContainer.top
      anchors.bottomMargin: 4

      spacing: 4

      QfToolButton {
        id: navigationButton
        visible: navigation.isActive
        round: true
        anchors.right: parent.right

        property bool isFollowLocationActive: positionSource.active && gnssButton.followActive && followIncludeDestination
        iconSource: QfTheme.getThemeVectorIcon("ic_navigation_flag_purple_24dp")
        iconColor: isFollowLocationActive ? QfTheme.toolButtonColor : QfTheme.navigationColor
        bgcolor: isFollowLocationActive ? QfTheme.navigationColor : QfTheme.toolButtonBackgroundColor

        /*
        / When set to true, when the map follows the device's current position, the extent
        / will always include the destination marker.
        */
        property bool followIncludeDestination: true

        onClicked: {
          if (positionSource.active && gnssButton.followActive) {
            followIncludeDestination = !followIncludeDestination;
            settings.setValue("/QField/Navigation/FollowIncludeDestination", followIncludeDestination);
            gnssButton.followLocation(true);
          } else {
            mapCanvasMap.jumpTo(navigation.destination, -1, -1, true);
          }
        }

        onPressAndHold: {
          navigationMenu.popup(locationToolbar.x + locationToolbar.width - navigationMenu.width, locationToolbar.y + navigationButton.height - navigationMenu.height);
        }

        Component.onCompleted: {
          followIncludeDestination = settings.valueBool("/QField/Navigation/FollowIncludeDestination", true);
        }
      }

      QfToolButton {
        id: gnssButton
        state: positionSource.active ? "On" : "Off"
        visible: positionSource.valid
        round: true

        anchors.right: parent.right

        /*
        / When set to true, the map will follow the device's current position; the map
        / will stop following the position whe the user manually drag the map.
        */
        property bool followActive: false
        /*
        / When set to true, map canvas extent changes will not result in the
        / deactivation of the above followActive mode.
        */
        property bool followActiveSkipExtentChanged: false
        /*
        / When set to true, the map will rotate to match the device's orientation (compass or movement direction based on setting).
        */
        property bool followOrientationActive: followActive && qfieldSettings.enableMapRotation && positioningSettings.positionFollowMode !== QfPositioningSettings.FollowMode.PositionOnly
        /*
        / When set to true, map canvas rotation changes will not result in the
        / deactivation of the above followOrientationActive mode.
        */
        property bool followActiveSkipRotationChanged: false
        /*
        / Used to have hard lock on positions
        / when we are in locked mode, and user pans the map, it will be true and after 5 seconds it will relock on position.
        */
        property bool autoRefollow: false
        states: [
          State {
            name: "Off"
            PropertyChanges {
              target: gnssButton
              iconSource: QfTheme.getThemeVectorIcon("ic_location_disabled_white_24dp")
              iconColor: QfTheme.toolButtonColor
              bgcolor: QfTheme.toolButtonBackgroundSemiOpaqueColor
            }
          },
          State {
            name: "On"
            PropertyChanges {
              target: gnssButton
              iconSource: trackings.count > 0 ? QfTheme.getThemeVectorIcon("ic_location_tracking_white_24dp") : positionSource.positionInformation && positionSource.positionInformation.latitudeValid ? QfTheme.getThemeVectorIcon("ic_location_valid_white_24dp") : QfTheme.getThemeVectorIcon("ic_location_white_24dp")
              iconColor: followActive ? QfTheme.toolButtonColor : QfTheme.positionColor
              bgcolor: followActive ? QfTheme.positionColor : QfTheme.toolButtonBackgroundColor
            }
          }
        ]

        onClicked: {
          if (!positionSource.active) {
            positionSource.jumpToPosition = true;
            positioningSettings.positioningActivated = true;
          } else {
            if (positionSource.projectedPosition.x) {
              jumpToLocation();
            } else {
              displayToast(qsTr("Waiting for location"));
            }
          }
        }

        onPressAndHold: {
          gnssMenu.popup(locationToolbar.x + locationToolbar.width - gnssMenu.width, locationToolbar.y + locationToolbar.height - gnssMenu.height);
        }

        property bool jumpedOnce: false

        function jumpToLocation() {
          const is3D = stateMachine.state === '3d';
          if (is3D) {
            const pos3d = mapCanvas3DLoader.item.geoTo3D(positionSource.projectedPosition.x, positionSource.projectedPosition.y);
            if (pos3d === null) {
              return;
            }
          }

          let targetScale = -1;
          if (!jumpedOnce && !is3D) {
            // The scale range and speed range aims at providing an adequate default
            // value for a range of scenarios from people walking to people being driven
            // in trains
            const scaleMin = 9028;
            const scaleMax = 144448;
            const speedMin = 2.57; // meters per second
            const speedMax = 140; // meters per second
            targetScale = scaleMin;
            if (positionSource.positionInformation.speedValid) {
              const speed = positionSource.positionInformation.speed;
              if (speed > speedMax) {
                targetScale = scaleMax;
              } else if (speed < speedMin) {
                targetScale = scaleMin;
              } else {
                const exp = 2;
                const ratio = (Math.pow(speed - speedMin, exp) - 1) / (Math.pow(speedMax - speedMin, exp) - 1);
                targetScale = (scaleMax - scaleMin) * ratio + scaleMin;
              }
            }
            jumpedOnce = true;
          }
          mapCanvasMap.jumpToPosition(positionSource, targetScale, -1, true, () => {
            gnssButton.followLocation(true);
            // Apply orientation immediately when follow mode includes orientation
            if (gnssButton.followOrientationActive) {
              gnssButton.followOrientation();
            }
          });
          if (!gnssButton.followActive) {
            mapCanvasMap.freeze('follow');
            gnssButton.followActive = true;
            if (positioningSettings.positionFollowMode === QfPositioningSettings.FollowMode.PositionAndCompass) {
              displayToast(qsTr("Map canvas follows location and compass orientation"));
            } else if (positioningSettings.positionFollowMode === QfPositioningSettings.FollowMode.PositionAndDirection) {
              displayToast(qsTr("Map canvas follows location and movement direction"));
            } else {
              displayToast(qsTr("Map canvas follows location"));
            }
          }
        }

        property int followLocationMinScale: 125
        property int followLocationMinMargin: 60
        property int followLocationScreenFraction: settings ? settings.value("/QField/Positioning/FollowScreenFraction", 5) : 5

        function followLocation(forceRecenter) {
          if (navigation.isActive && navigationButton.followIncludeDestination) {
            if (mapCanvas.mapSettings.scale > followLocationMinScale) {
              gnssButton.followActiveSkipExtentChanged = true;
              const points = [positionSource.projectedPosition, navigation.destination];
              mapCanvas.mapSettings.setExtentFromPoints(points, followLocationMinScale, true);
            }
          } else {
            gnssButton.followActiveSkipExtentChanged = true;
            mapCanvas.mapSettings.setCenter(positionSource.projectedPosition, true);
          }
          const triggerRecenter = Math.abs(Math.abs(mapCanvasMap.mapCanvasWrapper.scale) - 1) > 0.25 || Math.abs(mapCanvasMap.mapCanvasWrapper.x) > (mainWindow.width / 2) || Math.abs(mapCanvasMap.mapCanvasWrapper.y) > (mainWindow.height / 2);
          if (triggerRecenter || forceRecenter) {
            if (positionSource.positionInformation.directionValid) {
              // Prioritize preview quadrants based on movement direction
              const direction = (positionSource.positionInformation.direction + (mapCanvas.mapSettings.rotation + 360)) % 360;
              if (direction >= 337.5 || direction < 22.5) {
                // moving ~north
                mapCanvasMap.previewJobsQuadrants = [1, 2, 0, 5, 3, 8, 6, 7];
              } else if (direction >= 22.5 && direction < 67.5) {
                // moving ~northeast
                mapCanvasMap.previewJobsQuadrants = [2, 5, 1, 8, 0, 7, 3, 6];
              } else if (direction >= 67.5 && direction < 112.5) {
                // moving ~east
                mapCanvasMap.previewJobsQuadrants = [5, 8, 2, 7, 1, 6, 0, 3];
              } else if (direction >= 112.5 && direction < 157.5) {
                // moving ~southeast
                mapCanvasMap.previewJobsQuadrants = [8, 7, 5, 6, 2, 3, 1, 0];
              } else if (direction >= 157.5 && direction < 202.5) {
                // moving ~south
                mapCanvasMap.previewJobsQuadrants = [7, 8, 6, 5, 3, 2, 0, 1];
              } else if (direction >= 202.5 && direction < 247.5) {
                // moving ~southwest
                mapCanvasMap.previewJobsQuadrants = [6, 7, 3, 8, 0, 5, 1, 2];
              } else if (direction >= 247.5 && direction < 292.5) {
                // moving ~west
                mapCanvasMap.previewJobsQuadrants = [3, 6, 0, 7, 1, 8, 2, 5];
              } else if (direction >= 292.5 && direction < 337.5) {
                // moving ~northwest
                mapCanvasMap.previewJobsQuadrants = [0, 1, 3, 2, 6, 5, 7, 8];
              }
            }
            mapCanvasMap.refresh(true);
          }
        }

        function followOrientation() {
          if (!gnssButton.followOrientationActive) {
            return;
          }
          let targetRotation;
          if (positioningSettings.positionFollowMode === QfPositioningSettings.FollowMode.PositionAndCompass) {
            // Follow compass orientation
            if (isNaN(positionSource.orientation)) {
              return;
            }
            targetRotation = -positionSource.orientation;
          } else if (positioningSettings.positionFollowMode === QfPositioningSettings.FollowMode.PositionAndDirection) {
            // Follow movement direction
            if (!positionSource.positionInformation || !positionSource.positionInformation.directionValid) {
              return;
            }
            targetRotation = -positionSource.positionInformation.direction;
          } else {
            return;
          }
          if (Math.abs(targetRotation - mapCanvas.mapSettings.rotation) >= 2) {
            mapCanvas.mapSettings.rotation = targetRotation;
            gnssButton.followActiveSkipRotationChanged = true;
            const triggerRefresh = Math.abs(mapCanvasMap.mapCanvasWrapper.rotation) > 60;
            if (triggerRefresh) {
              mapCanvasMap.refresh(true);
            }
          }
        }

        QfBadge {
          alignment: QfBadge.Alignment.TopRight
          width: gnssButton.width / 4
          topMargin: gnssButton.width / 24
          rightMargin: gnssButton.width / 24
          visible: positioningSettings.accuracyIndicator && gnssButton.state === "On" && positionSource.positionInformation.accuracyQuality != QfGnssPositionInformation.AccuracyUndetermined
          color: {
            if (!positionSource.positionInformation || !positionSource.positionInformation.haccValid)
              return QfTheme.accuracyBad;
            switch (positionSource.positionInformation.accuracyQuality) {
            case QfGnssPositionInformation.AccuracyExcellent:
              return QfTheme.accuracyExcellent;
            case QfGnssPositionInformation.AccuracyOk:
              return QfTheme.accuracyTolerated;
            case QfGnssPositionInformation.AccuracyBad:
            case QfGnssPositionInformation.AccuracyUndetermined:
            default:
              return QfTheme.accuracyBad;
            }
          }
        }
      }

      Connections {
        target: mapCanvas.mapSettings

        function onExtentChanged() {
          if (mapCanvasMap.jumping) {
            return;
          }
          if (gnssButton.followActive) {
            if (gnssButton.followActiveSkipExtentChanged) {
              gnssButton.followActiveSkipExtentChanged = false;
              return;
            }
            mapCanvasMap.unfreeze('follow');
            gnssButton.followActive = false;
            if (gnssButton.autoRefollow) {
              showAutoLockToast();
            }
          } else if (gnssButton.autoRefollow) {
            showAutoLockToast();
          }
        }

        function onRotationChanged() {
          if (mapCanvasMap.jumping) {
            return;
          }
          if (gnssButton.followActive) {
            if (gnssButton.followOrientationActive && gnssButton.followActiveSkipRotationChanged) {
              gnssButton.followActiveSkipRotationChanged = false;
              return;
            }
            mapCanvasMap.unfreeze('follow');
            gnssButton.followActive = false;
            if (gnssButton.autoRefollow) {
              showAutoLockToast();
            }
          } else if (gnssButton.autoRefollow) {
            showAutoLockToast();
          }
        }

        function onOutputSizeChanged() {
          if (gnssButton.followActive) {
            mapCanvasMap.refresh(true);
          }
        }
      }
    }

    Column {
      id: digitizingToolbarContainer
      anchors.right: parent.right
      anchors.rightMargin: mainWindow.sceneRightMargin + 4
      anchors.bottom: parent.bottom
      anchors.bottomMargin: 4

      spacing: 4

      QfDigitizingToolbar {
        id: digitizingToolbar

        property bool digitizingAllowed: dashBoard.activeLayer && !dashBoard.activeLayer.readOnly && !QfLayerUtils.isFeatureAdditionLocked(dashBoard.activeLayer) && (projectInfo.editRights || projectInfo.insertRights)
        property string previousStateMachineState: ''

        stateVisible: !screenLocker.enabled && (!positioningSettings.geofencingPreventDigitizingDuringAlert || !geofencer.isAlerting) && ((stateMachine.state === "digitize" && digitizingAllowed && !geometryEditorsToolbar.stateVisible && !moveFeaturesToolbar.stateVisible && !rotateFeaturesToolbar.stateVisible) || stateMachine.state === 'measure' || (stateMachine.state === "digitize" && digitizingToolbar.geometryRequested))
        rubberbandModel: currentRubberband ? currentRubberband.model : null
        mapSettings: mapCanvas.mapSettings
        showConfirmButton: stateMachine.state === "digitize"
        screenHovering: mapCanvasMap.hovered

        cogoOperationSettings: informationDrawer.cogoOperationSettings

        digitizingLogger.type: stateMachine.state === 'measure' ? '' : 'add'

        QfFeatureModel {
          id: digitizingFeature
          project: qgisProject
          currentLayer: digitizingToolbar.geometryRequested ? digitizingToolbar.geometryRequestedLayer : dashBoard.activeLayer
          appExpressionContextScopesGenerator: appScopesGenerator
          topSnappingResult: coordinateLocator.topSnappingResult
          geometry: QfGeometry {
            id: digitizingGeometry
            rubberbandModel: digitizingRubberband.model
            vectorLayer: digitizingToolbar.geometryRequested ? digitizingToolbar.geometryRequestedLayer : dashBoard.activeLayer
          }
        }

        onVertexCountChanged: {
          if (stateMachine.state === 'measure' && elevationProfileButton.elevationProfileActive) {
            if (rubberbandModel.vertexCount > 2) {
              // Clear the pre-existing profile to trigger a zoom to full updated profile curve
              informationDrawer.elevationProfile.clear();
              informationDrawer.elevationProfile.profileCurve = QfGeometryUtils.lineFromRubberband(rubberbandModel, informationDrawer.elevationProfile.crs);
              informationDrawer.elevationProfile.refresh();
            }
          } else if (qfieldSettings.autoSave && stateMachine.state === "digitize") {
            if (digitizingToolbar.geometryValid) {
              if (digitizingRubberband.model.geometryType === Qgis.GeometryType.Null) {
                digitizingRubberband.model.reset();
              } else {
                digitizingFeature.geometry.applyRubberband();
                digitizingFeature.applyGeometry();
              }
              if (!overlayFeatureFormDrawer.featureForm.featureCreated) {
                overlayFeatureFormDrawer.featureModel.geometry = digitizingFeature.geometry;
                overlayFeatureFormDrawer.featureModel.applyGeometry();
                overlayFeatureFormDrawer.featureModel.resetAttributes();
                if (overlayFeatureFormDrawer.featureForm.model.constraintsHardValid && !overlayFeatureFormDrawer.featureForm.featureAdditionLocked) {
                  // when the constrainst are fulfilled
                  // indirect action, no need to check for success and display a toast, the log is enough
                  overlayFeatureFormDrawer.featureModel.featureCreated = overlayFeatureFormDrawer.featureModel.create();
                }
              } else {
                // indirect action, no need to check for success and display a toast, the log is enough
                overlayFeatureFormDrawer.featureModel.geometry = digitizingFeature.geometry;
                overlayFeatureFormDrawer.featureModel.applyGeometry();
                overlayFeatureFormDrawer.featureForm.save();
              }
            } else {
              if (overlayFeatureFormDrawer.featureForm.featureCreated) {
                // delete the feature when the geometry gets invalid again
                // indirect action, no need to check for success and display a toast, the log is enough
                overlayFeatureFormDrawer.featureForm.featureCreated = !overlayFeatureFormDrawer.featureForm.deleteFeature();
              }
            }
          }
        }

        onCancel: {
          coordinateLocator.sourceLocation = undefined;
          if (stateMachine.state === 'measure' && elevationProfileButton.elevationProfileActive) {
            informationDrawer.elevationProfile.clear();
            informationDrawer.elevationProfile.refresh();
          } else {
            if (geometryRequested) {
              if (overlayFeatureFormDrawer.isAdding) {
                overlayFeatureFormDrawer.open();
              }
              geometryRequested = false;
            }
          }
          if (dashBoard.shouldReturnHome) {
            openWelcomeScreen();
          }
        }

        onConfirmed: {
          if (geometryRequested) {
            if (overlayFeatureFormDrawer.isAdding) {
              overlayFeatureFormDrawer.open();
            }
            coordinateLocator.flash();
            digitizingFeature.geometry.applyRubberband();
            geometryRequestedItem.requestedGeometryReceived(digitizingFeature.geometry);
            digitizingRubberband.model.reset();
            geometryRequested = false;
            coordinateLocator.sourceLocation = undefined;
            return;
          }
          if (digitizingRubberband.model.geometryType === Qgis.GeometryType.Null) {
            digitizingRubberband.model.reset();
            digitizingRubberband.model.frozen = digitizingToolbar.cogoEnabled;
          } else {
            coordinateLocator.flash();
            digitizingFeature.geometry.applyRubberband();
            digitizingFeature.applyGeometry();
            digitizingRubberband.model.frozen = true;
            digitizingFeature.updateRubberband();
          }
          if (!digitizingFeature.suppressFeatureForm()) {
            overlayFeatureFormDrawer.featureModel.geometry = digitizingFeature.geometry;
            overlayFeatureFormDrawer.featureModel.applyGeometry();
            overlayFeatureFormDrawer.featureModel.resetAttributes();
            overlayFeatureFormDrawer.open();
            overlayFeatureFormDrawer.state = "Add";
          } else {
            if (!overlayFeatureFormDrawer.featureForm.featureCreated) {
              overlayFeatureFormDrawer.featureModel.geometry = digitizingFeature.geometry;
              overlayFeatureFormDrawer.featureModel.applyGeometry();
              overlayFeatureFormDrawer.featureModel.resetAttributes();
              if (!overlayFeatureFormDrawer.featureForm.featureAdditionLocked) {
                if (!overlayFeatureFormDrawer.featureModel.create()) {
                  displayToast(qsTr("Failed to create feature"), 'error');
                }
              } else {
                displayToast(qsTr("Failed to create feature due to feature addition permission disabled"), 'warning');
              }
            } else {
              if (!overlayFeatureFormDrawer.featureModel.save()) {
                displayToast(qsTr("Failed to save feature"), 'error');
              }
            }
            digitizingRubberband.model.reset();
            digitizingRubberband.model.frozen = digitizingToolbar.cogoEnabled;
            digitizingFeature.resetFeature();
          }
          coordinateLocator.sourceLocation = undefined;
        }

        onGeometryRequestedChanged: {
          if (geometryRequested) {
            digitizingRubberband.model.reset();
            previousStateMachineState = stateMachine.state;
            stateMachine.state = "digitize";
          } else {
            stateMachine.state = previousStateMachineState;
          }
        }

        onRequestJumpToPoint: function (center, scale, handleMargins) {
          mapCanvasMap.jumpTo(center, scale, -1, handleMargins);
        }

        onRequestPosition: function (item, fromCoordinateLocator) {
          if (fromCoordinateLocator) {
            item.requestedPositionReceived(coordinateLocator.currentCoordinate, undefined);
          } else {
            if (!positionSource.active) {
              displayToast(qsTr("Enable positioning service to get points at your location"));
              item.requestedPositionReceived(QfGeometryUtils.emptyPoint(), undefined);
            } else if (!positionSource.positionInformation.latitudeValid) {
              displayToast(qsTr("Positioning service has not yet received a valid location"));
              item.requestedPositionReceived(QfGeometryUtils.emptyPoint(), undefined);
            } else {
              item.requestedPositionReceived(positionSource.projectedPosition, positionSource.positionInformation);
            }
          }
        }
      }

      QfGeometryEditorsToolbar {
        id: geometryEditorsToolbar

        featureModel: geometryEditingFeature
        mapSettings: mapCanvas.mapSettings
        editorRubberbandModel: geometryEditorsRubberband.model
        editorRenderer: geometryEditorRenderer
        screenHovering: mapCanvasMap.hovered

        stateVisible: !screenLocker.enabled && (stateMachine.state === "digitize" && geometryEditingVertexModel.vertexCount > 0)

        onRequestJumpToPoint: function (center, scale, handleMargins) {
          mapCanvasMap.jumpTo(center, scale, -1, handleMargins);
        }
      }

      QfConfirmationToolbar {
        id: moveFeaturesToolbar

        property bool moveFeaturesRequested: false
        property var startPoint: undefined // QgsPoint or undefined
        property var endPoint: undefined // QgsPoint or undefined
        signal moveConfirmed
        signal moveCanceled

        stateVisible: moveFeaturesRequested

        onConfirm: {
          endPoint = stateMachine.state === "digitize" ? coordinateLocator.currentCoordinate : mapCanvas.mapSettings.getCenter(true);
          moveFeaturesRequested = false;
          moveConfirmed();
        }

        onCancel: {
          startPoint = undefined;
          endPoint = undefined;
          moveFeaturesRequested = false;
          moveCanceled();
        }

        function initializeMoveFeatures() {
          moveFeaturesRequested = true;
          if (featureListForm && featureListForm.selection.model.selectedCount === 1) {
            featureListForm.extentController.zoomToSelected();
            let centroid = QfGeometryUtils.reprojectPoint(QfGeometryUtils.boundingBox(featureListForm.selection.model.selectedFeatures[0].geometry).center, featureListForm.selection.model.selectedLayer.crs, mapCanvas.mapSettings.destinationCrs);
            centroid = QfGeometryUtils.point(centroid.x, centroid.y);
            startPoint = centroid;
          } else {
            startPoint = mapCanvas.mapSettings.getCenter(true);
          }
          moveAndRotateFeaturesHighlight.rotationDegrees = 0;
        }
      }

      QfConfirmationToolbar {
        id: rotateFeaturesToolbar

        property bool rotateFeaturesRequested: false
        property var angle: 0.0

        signal rotateConfirmed
        signal rotateCanceled

        stateVisible: rotateFeaturesRequested

        onConfirm: {
          rotateFeaturesRequested = false;
          angle = moveAndRotateFeaturesHighlight.rotationDegrees;
          rotateConfirmed();
        }
        onCancel: {
          rotateFeaturesRequested = false;
          rotateCanceled();
        }

        function initializeRotateFeatures() {
          rotateFeaturesRequested = true;
          if (featureListForm && featureListForm.selection.model.selectedCount === 1) {
            featureListForm.extentController.zoomToSelected();
          }
          moveAndRotateFeaturesHighlight.rotationDegrees = 0;
        }
      }
    }
  }

  QfLocatorSettings {
    id: locatorSettings
    locatorFiltersModel: locatorItem.locatorFiltersModel

    Component.onCompleted: focusstack.addFocusTaker(this)
  }

  QfPluginManagerSettings {
    id: pluginManagerSettings

    Component.onCompleted: focusstack.addFocusTaker(this)
  }

  QfDashBoard {
    id: dashBoard
    objectName: "dashBoard"

    allowActiveLayerChange: !digitizingToolbar.isDigitizing
    cloudProjectsModel: cloudProjectsModel
    allowInteractive: !welcomeScreen.visible && !qfieldSettings.visible && !qfieldCloudScreen.visible && !qfieldLocalDataPickerScreen.visible && !codeReader.visible && !screenLocker.enabled
    mapSettings: mapCanvas.mapSettings

    Component.onCompleted: focusstack.addFocusTaker(this)

    onOpened: {
      if (settings.valueBool("/QField/showDashboardGuide", true)) {
        dashboardTour.index = 0;
        dashboardTour.runTour();
        settings.setValue("/QField/showDashboardGuide", false);
        return;
      }
      if (cloudConnection.isReachable && settings.valueBool("/QField/showCloudButtonGuide", true)) {
        const deltaFileWrapper = cloudProjectsModel.layerObserver.deltaFileWrapper;
        if (deltaFileWrapper && deltaFileWrapper.count > 0) {
          cloudButtonTour.index = 0;
          cloudButtonTour.runTour();
          settings.setValue("/QField/showCloudButtonGuide", false);
        }
      }
    }

    onReturnHome: {
      if (currentRubberband && currentRubberband.model.vertexCount > 1) {
        digitizingToolbar.cancelDialog.open();
        shouldReturnHome = true;
      } else if (!shouldReturnHome) {
        openWelcomeScreen();
        if (stateMachine.state === '3d') {
          mainWindow.close3DView();
        }
      }
    }

    onShowMainMenu: p => {
      mainMenu.popup(p.x - mainMenu.width - 2, p.y - 2);
    }

    onShowCloudPopup: {
      mainWindow.openCloudPopup();
    }

    onToggleMeasurementTool: {
      if (featureListForm.state === "ProcessingAlgorithmForm") {
        cancelAlgorithmDialog.visible = true;
      } else {
        activateMeasurementMode();
      }
    }

    onToggle3DView: {
      activate3DMode();
    }

    onShowPrintLayouts: p => {
      if (layoutListInstantiator.count > 1) {
        printMenu.popup(p.x, p.y);
      } else {
        mainMenu.close();
        displayToast(qsTr('Printing...'));
        printMenu.printName = layoutListInstantiator.count === 1 ? layoutListInstantiator.model.titleAt(0) : "";
        printMenu.printTimer.restart();
      }
    }

    onShowProjectFolder: {
      dashBoard.close();
      qfieldLocalDataPickerScreen.projectFolderView = true;
      qfieldLocalDataPickerScreen.model.resetToPath(projectInfo.filePath);
      qfieldLocalDataPickerScreen.visible = true;
    }

    // If the user clicks the "Return home" button in the middle of digitizing, we will ask if they want to discard their changes.
    // If they press cancel, nothing will happen, but if they press discard, we will discard their digitizing.
    // We will also use `shouldReturnHome` to know that we need to return home as well or not.
    property bool shouldReturnHome: false

    function ensureEditableLayerSelected() {
      var firstEditableLayer = null;
      var activeLayerLocked = false;
      for (var i = 0; i < layerTree.rowCount(); i++) {
        var index = layerTree.index(i, 0);
        if (firstEditableLayer === null) {
          if (layerTree.data(index, QfFlatLayerTreeModel.Type) === QfFlatLayerTreeModel.Layer && layerTree.data(index, QfFlatLayerTreeModel.ReadOnly) === false && layerTree.data(index, QfFlatLayerTreeModel.FeatureAdditionLocked) === false) {
            firstEditableLayer = layerTree.data(index, QfFlatLayerTreeModel.VectorLayerPointer);
          }
        }
        if (activeLayer != null && activeLayer === layerTree.data(index, QfFlatLayerTreeModel.VectorLayerPointer)) {
          if (layerTree.data(index, QfFlatLayerTreeModel.ReadOnly) === true || layerTree.data(index, QfFlatLayerTreeModel.FeatureAdditionLocked) === true) {
            activeLayerLocked = true;
          } else {
            break;
          }
        }
        if (firstEditableLayer !== null && (activeLayer == null || activeLayerLocked === true)) {
          activeLayer = firstEditableLayer;
          break;
        }
      }
    }
  }

  QfBookmarkProperties {
    id: bookmarkProperties

    Component.onCompleted: focusstack.addFocusTaker(this)
  }

  function openWelcomeScreen() {
    mainMenu.close();
    dashBoard.close();
    welcomeScreen.visible = true;
    welcomeScreen.focus = true;
    dashBoard.shouldReturnHome = false;
  }

  function activateMeasurementMode() {
    mainMenu.close();
    dashBoard.close();
    changeMode('measure');
  }

  function activate3DMode() {
    mainMenu.close();
    dashBoard.close();
    if (stateMachine.state !== '3d') {
      changeMode('3d');
    }
  }

  QfMenu {
    id: mainMenu
    objectName: "mainMenu"

    title: qsTr("Main Menu")

    topMargin: sceneTopMargin
    bottomMargin: sceneBottomMargin
    skipFirstRow: true
    minimumRowWidth: Math.max(50, undoRedoMetrics.width + (undoButton.leftPadding + undoButton.rightPadding + 42) * 2)

    TextMetrics {
      id: undoRedoMetrics
      font: undoButton.font
      text: undoButton.text + redoButton.text
    }

    Item {
      width: mainMenu.width
      height: 48
      clip: true

      MenuItem {
        id: undoButton
        enabled: featureHistory && featureHistory.isUndoAvailable
        height: 48
        width: parent.width / 2
        anchors.left: parent.left
        text: qsTr("Undo")
        font: QfTheme.defaultFont
        icon.source: QfTheme.getThemeVectorIcon("ic_undo_black_24dp")
        leftPadding: QfTheme.menuItemLeftPadding

        onClicked: {
          if (enabled) {
            const msg = featureHistory.undoMessage();
            if (featureHistory.undo()) {
              displayToast(msg);
            }
          }
        }
      }

      MenuSeparator {
        width: 1
        height: parent.height
        anchors.right: redoButton.left
      }

      MenuItem {
        id: redoButton
        enabled: featureHistory && featureHistory.isRedoAvailable
        height: 48
        width: parent.width / 2
        anchors.right: parent.right
        text: qsTr("Redo")
        font: QfTheme.defaultFont
        icon.source: QfTheme.getThemeVectorIcon("ic_redo_black_24dp")

        contentItem: IconLabel {
          leftPadding: undoButton.leftPadding
          spacing: redoButton.spacing
          mirrored: true
          display: redoButton.display
          icon: redoButton.icon
          text: redoButton.text
          font: redoButton.font
          color: redoButton.enabled ? redoButton.Material.foreground : redoButton.Material.hintTextColor
        }

        onClicked: {
          if (enabled) {
            const msg = featureHistory.redoMessage();
            if (featureHistory.redo()) {
              displayToast(msg);
            }
          }
        }
      }
    }

    MenuSeparator {
      width: parent.width
    }

    MenuItem {
      id: sensorItem
      text: qsTr("Sensors")

      font: QfTheme.defaultFont
      icon.source: QfTheme.getThemeVectorIcon("ic_sensor_on_black_24dp")
      height: visible ? 48 : 0
      visible: sensorListInstantiator.count > 0
      leftPadding: QfTheme.menuItemLeftPadding
      rightPadding: 40

      arrow: Shape {
        id: sensorItemArrowShape
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.rightMargin: width / 2
        width: 20
        height: 20
        visible: true
        opacity: sensorListInstantiator.count > 0 ? 1 : 0

        ShapePath {
          strokeWidth: 2
          strokeColor: QfTheme.mainColor
          fillColor: "transparent"

          startX: sensorItemArrowShape.width * 0.35
          startY: sensorItemArrowShape.height * 0.25

          PathLine {
            x: sensorItemArrowShape.width * 0.65
            y: sensorItemArrowShape.height * 0.5
          }
          PathLine {
            x: sensorItemArrowShape.width * 0.35
            y: sensorItemArrowShape.height * 0.75
          }
        }
      }

      onTriggered: {
        if (sensorListInstantiator.count > 0) {
          sensorMenu.popup(mainMenu.x, mainMenu.y + sensorItem.y);
        } else {
          mainMenu.close();
          toast.show(qsTr('No sensor available'), 'info', qsTr('Learn more'), function () {
            Qt.openUrlExternally('https://docs.qfield.org/how-to/advanced-how-tos/sensors/');
          });
        }
        highlighted = false;
      }
    }

    MenuSeparator {
      width: parent.width
      visible: sensorListInstantiator.count > 0
      height: visible ? undefined : 0
    }

    MenuItem {
      text: qsTr("Bookmarks")

      font: QfTheme.defaultFont
      icon.source: QfTheme.getThemeVectorIcon("ic_bookmark_black_24dp")
      height: 48
      leftPadding: QfTheme.menuItemLeftPadding

      onTriggered: {
        mainMenu.close();
        dashBoard.close();
        bookmarkList.show();
        highlighted = false;
      }
    }

    MenuItem {
      text: qsTr("Plugin Manager")

      font: QfTheme.defaultFont
      icon.source: QfTheme.getThemeVectorIcon("ic_plugin_black_24dp")
      height: 48
      leftPadding: QfTheme.menuItemLeftPadding

      onTriggered: {
        dashBoard.close();
        pluginManagerSettings.open();
      }
    }

    MenuItem {
      text: qsTr("Settings")

      font: QfTheme.defaultFont
      icon.source: QfTheme.getThemeVectorIcon("ic_tune_white_24dp")
      height: 48
      leftPadding: QfTheme.menuItemLeftPadding

      onTriggered: {
        dashBoard.close();
        qfieldSettings.reset();
        qfieldSettings.visible = true;
        highlighted = false;
      }
    }

    MenuItem {
      text: qsTr("Message Log")

      font: QfTheme.defaultFont
      height: 48
      icon.source: QfTheme.getThemeVectorIcon("ic_message_log_black_24dp")
      leftPadding: QfTheme.menuItemLeftPadding

      onTriggered: {
        dashBoard.close();
        messageLog.visible = true;
        highlighted = false;
      }

      QfBadge {
        width: 16
        height: width
        topMargin: 5
        rightMargin: 5
        alignment: QfBadge.Alignment.TopRight
        visible: messageLog.unreadMessages
        color: QfTheme.mainColor
        badgeText.text: messageLog.unreadMessagesCount >= 10 ? "+" : messageLog.unreadMessagesCount
        badgeText.color: QfTheme.light
        border.color: "transparent"
      }
    }

    MenuItem {
      text: qsTr("Lock Screen")

      font: QfTheme.defaultFont
      icon.source: QfTheme.getThemeVectorIcon("ic_lock_black_24dp")
      height: 48
      leftPadding: QfTheme.menuItemLeftPadding

      onTriggered: {
        mainMenu.close();
        dashBoard.close();
        screenLocker.enabled = true;
      }
    }

    MenuSeparator {
      width: parent.width
    }

    MenuItem {
      text: qsTr("About %1").arg(Qfield.name)

      font: QfTheme.defaultFont
      icon.source: Qfield.name === "QField" ? QfTheme.getThemeVectorIcon("ic_qfield_black_24dp") : ""
      height: 48
      leftPadding: Qfield.name === "QField" ? QfTheme.menuItemLeftPadding : QfTheme.menuItemIconlessLeftPadding

      onTriggered: {
        dashBoard.close();
        aboutDialog.visible = true;
        highlighted = false;
      }
    }
  }

  QfMenu {
    id: sensorMenu

    property alias printTimer: timer
    property alias printName: timer.printName

    title: qsTr("Sensors")

    topMargin: sceneTopMargin
    bottomMargin: sceneBottomMargin

    MenuItem {
      text: qsTr('Select sensor below')

      font: QfTheme.defaultFont
      height: 48
      leftPadding: QfTheme.menuItemLeftPadding

      enabled: false
    }

    Instantiator {
      id: sensorListInstantiator

      model: QfSensorListModel {
        project: qgisProject

        onSensorErrorOccurred: errorString => {
          displayToast(qsTr('Sensor error: %1').arg(errorString), 'error');
        }
      }

      MenuItem {
        text: SensorName
        icon.source: SensorStatus == Qgis.DeviceConnectionStatus.Connected ? QfTheme.getThemeVectorIcon("ic_sensor_on_black_24dp") : QfTheme.getThemeVectorIcon("ic_sensor_off_black_24dp")

        font: QfTheme.defaultFont
        leftPadding: QfTheme.menuItemLeftPadding
        height: 48

        onTriggered: {
          if (SensorStatus == Qgis.DeviceConnectionStatus.Connected) {
            displayToast(qsTr('Disconnecting sensor \'%1\'...').arg(SensorName));
            sensorListInstantiator.model.disconnectSensorId(SensorId);
            highlighted = false;
          } else {
            displayToast(qsTr('Connecting sensor \'%1\'...').arg(SensorName));
            sensorListInstantiator.model.connectSensorId(SensorId);
            highlighted = false;
          }
        }
      }

      onObjectAdded: (index, object) => {
        sensorMenu.insertItem(index + 1, object);
      }
      onObjectRemoved: (index, object) => {
        sensorMenu.removeItem(object);
      }
    }
  }

  QfMenu {
    id: printMenu

    property alias printTimer: timer
    property alias printName: timer.printName

    title: qsTr("Print")

    topMargin: sceneTopMargin
    bottomMargin: sceneBottomMargin

    MenuItem {
      text: qsTr('Select layout below')

      font: QfTheme.defaultFont
      height: 48
      leftPadding: QfTheme.menuItemLeftPadding

      enabled: false
    }

    Instantiator {
      id: layoutListInstantiator

      model: QfPrintLayoutListModel {
        project: qgisProject
      }

      MenuItem {
        text: Title

        font: QfTheme.defaultFont
        leftPadding: QfTheme.menuItemLeftPadding
        height: 48

        onTriggered: {
          highlighted = false;
          displayToast(qsTr('Printing...'));
          printMenu.printName = Title;
          printMenu.printTimer.restart();
        }
      }
      onObjectAdded: (index, object) => {
        printMenu.insertItem(index + 1, object);
      }
      onObjectRemoved: (index, object) => {
        printMenu.removeItem(object);
      }
    }

    Timer {
      id: timer

      property string printName: ''

      interval: 500
      repeat: false
      onTriggered: iface.print(printName)
    }
  }

  QfMenu {
    id: canvasMenu
    objectName: "canvasMenu"

    title: qsTr("Map Canvas Options")
    font: QfTheme.defaultFont

    property var point
    onPointChanged: {
      var displayPoint = QfGeometryUtils.reprojectPoint(canvasMenu.point, mapCanvas.mapSettings.destinationCrs, projectInfo.coordinateDisplayCrs);
      var isXY = QfCoordinateReferenceSystemUtils.defaultCoordinateOrderForCrsIsXY(projectInfo.coordinateDisplayCrs);
      var isGeographic = projectInfo.coordinateDisplayCrs.isGeographic;
      var xLabel = isGeographic ? qsTr('Lon') : 'X';
      var xValue = Number(displayPoint.x).toLocaleString(Qt.locale(), 'f', isGeographic ? 7 : 3);
      var yLabel = isGeographic ? qsTr('Lat') : 'Y';
      var yValue = Number(displayPoint.y).toLocaleString(Qt.locale(), 'f', isGeographic ? 7 : 3);
      const xItemText = isXY ? xLabel + ': ' + xValue : yLabel + ': ' + yValue;
      const yItemText = isXY ? yLabel + ': ' + yValue : xLabel + ': ' + xValue;
      cordinateItem.text = xItemText + "   " + yItemText;
    }

    topMargin: sceneTopMargin
    bottomMargin: sceneBottomMargin
    skipFirstRow: true
    minimumRowWidth: canvasMenuActionsToolbar.childrenRect.width + 4

    // Tweak the default delegate to align left padding and height of submenu items
    delegate: MenuItem {
      leftPadding: QfTheme.menuItemLeftPadding
      height: 48
    }

    readonly property bool isPasteMenuVisible: clipboardManager ? clipboardManager.holdsFeature : false

    onIsPasteMenuVisibleChanged: {
      updatePasteMenuVisibility();
    }

    function updatePasteMenuVisibility() {
      for (let i = 0; i < canvasMenu.count; i++) {
        if (canvasMenu.itemAt(i).text === pasteIntoLayers.title) {
          canvasMenu.itemAt(i).height = isPasteMenuVisible ? 48 : 0;
          canvasMenu.itemAt(i).visible = isPasteMenuVisible;
        }
      }
    }

    Component.onCompleted: {
      updatePasteMenuVisibility();
    }

    Row {
      id: canvasMenuActionsToolbar
      objectName: "canvasMenuActionsToolbar"
      leftPadding: 2
      rightPadding: 2
      spacing: 2
      height: children.length > 0 ? addBookmarkItem.height : 0
      clip: true

      property color hoveredColor: Qt.hsla(QfTheme.mainTextColor.hslHue, QfTheme.mainTextColor.hslSaturation, QfTheme.mainTextColor.hslLightness, 0.2)
    }

    MenuSeparator {
      width: parent.width
      height: canvasMenuActionsToolbar.children.length > 0 ? undefined : 0
    }

    MenuItem {
      id: cordinateItem
      text: ""
      height: 48
      leftPadding: QfTheme.menuItemLeftPadding
      font: QfTheme.defaultFont
      icon.source: QfTheme.getThemeVectorIcon("ic_copy_black_24dp")

      onTriggered: {
        const displayPoint = QfGeometryUtils.reprojectPoint(canvasMenu.point, mapCanvas.mapSettings.destinationCrs, projectInfo.coordinateDisplayCrs);
        platformUtilities.copyTextToClipboard(QfStringUtils.pointInformation(displayPoint, projectInfo.coordinateDisplayCrs));
        displayToast(qsTr('Coordinates copied to clipboard'));
      }
    }

    MenuSeparator {
      width: parent.width
    }

    MenuItem {
      id: addBookmarkItem
      text: qsTr("Add Bookmark")
      icon.source: QfTheme.getThemeVectorIcon("ic_add_bookmark_black_24dp")
      height: 48
      leftPadding: QfTheme.menuItemLeftPadding
      font: QfTheme.defaultFont

      onTriggered: {
        var name = qsTr('Untitled bookmark');
        var group = '';
        var id = bookmarkModel.addBookmarkAtPoint(canvasMenu.point, name, group);
        if (id !== '') {
          bookmarkProperties.bookmarkId = id;
          bookmarkProperties.bookmarkName = name;
          bookmarkProperties.bookmarkGroup = group;
          bookmarkProperties.open();
        }
      }
    }

    MenuItem {
      id: setDestinationItem
      text: qsTr("Set as Destination")
      icon.source: QfTheme.getThemeVectorIcon("ic_navigation_flag_purple_24dp")
      height: 48
      leftPadding: QfTheme.menuItemLeftPadding
      font: QfTheme.defaultFont

      onTriggered: {
        navigation.destination = canvasMenu.point;
      }
    }

    MenuSeparator {
      width: parent.width
    }

    MenuItem {
      id: lockMapRotation
      text: qsTr("Enable Map Rotation")
      height: 48
      leftPadding: QfTheme.menuItemCheckLeftPadding
      font: QfTheme.defaultFont
      checkable: true
      checked: qfieldSettings.enableMapRotation
      indicator.height: 20
      indicator.width: 20
      indicator.implicitHeight: 24
      indicator.implicitWidth: 24

      onTriggered: qfieldSettings.enableMapRotation = checked
    }

    MenuItem {
      text: qsTr('Lock Screen')

      font: QfTheme.defaultFont
      icon.source: QfTheme.getThemeVectorIcon("ic_lock_black_24dp")
      height: 48
      leftPadding: QfTheme.menuItemLeftPadding

      onTriggered: {
        screenLocker.enabled = true;
      }
    }

    MenuSeparator {
      enabled: canvasMenuFeatureListInstantiator.count > 0 || canvasMenu.isPasteMenuVisible
      width: parent.width
      visible: enabled
      height: enabled ? undefined : 0
    }

    Instantiator {
      id: canvasMenuFeatureListInstantiator
      active: false

      model: menuFeatureListModel

      QfMenu {
        id: featureMenu

        property int fid: featureId
        property var featureLayer: currentLayer
        height: visible ? implicitHeight : 0

        topMargin: sceneTopMargin
        bottomMargin: sceneBottomMargin

        title: layerName + ': ' + featureName
        font: QfTheme.defaultFont

        icon.source: iconForGeometry(feature.geometry.type)

        MenuItem {
          text: qsTr('Layer:') + ' ' + layerName
          height: 48
          enabled: false
        }
        MenuItem {
          text: qsTr('Feature:') + ' ' + featureName
          height: 48
          enabled: false
        }
        MenuSeparator {
          width: parent.width
        }

        MenuItem {
          text: qsTr('Open Feature Form')
          font: QfTheme.defaultFont
          icon.source: QfTheme.getThemeVectorIcon("ic_baseline-list_white_24dp")
          leftPadding: QfTheme.menuItemLeftPadding
          height: 48

          onTriggered: {
            featureListForm.model.setFeatures(menu.featureLayer, '@id = ' + menu.fid);
            featureListForm.selection.focusedItem = 0;
            featureListForm.state = "FeatureForm";
          }
        }

        MenuItem {
          text: qsTr('Cut Feature')
          font: QfTheme.defaultFont
          icon.source: QfTheme.getThemeVectorIcon("ic_cut_black_24dp")
          leftPadding: QfTheme.menuItemLeftPadding
          height: 48

          onTriggered: {
            clipboardManager.copyFeatureToClipboard(menu.featureLayer, menu.fid, true, true);
          }
        }

        MenuItem {
          text: qsTr('Copy Feature')
          font: QfTheme.defaultFont
          icon.source: QfTheme.getThemeVectorIcon("ic_copy_black_24dp")
          leftPadding: QfTheme.menuItemLeftPadding
          height: 48

          onTriggered: {
            clipboardManager.copyFeatureToClipboard(menu.featureLayer, menu.fid, true);
          }
        }

        MenuItem {
          text: qsTr('Duplicate Feature')
          font: QfTheme.defaultFont
          enabled: projectInfo.insertRights
          icon.source: QfTheme.getThemeVectorIcon("ic_duplicate_black_24dp")
          leftPadding: QfTheme.menuItemLeftPadding
          height: 48

          onTriggered: {
            featureListForm.model.setFeatures(menu.featureLayer, '@id = ' + menu.fid);
            featureListForm.selection.focusedItem = 0;
            featureListForm.multiSelection = true;
            featureListForm.selection.toggleSelectedItem(0);
            featureListForm.state = "FeatureList";
            if (featureListForm.model.canDuplicateSelection) {
              if (featureListForm.selection.model.duplicateFeature(featureListForm.selection.focusedLayer, featureListForm.selection.focusedFeature)) {
                displayToast(qsTr('Successfully duplicated feature'));
                featureListForm.selection.focusedItem = -1;
                moveFeaturesToolbar.initializeMoveFeatures();
                return;
              }
            }
            displayToast(qsTr('Feature duplication not available'));
          }
        }
      }

      onObjectAdded: (index, object) => {
        canvasMenu.insertMenu(canvasMenu.contentData.length - 3, object);
      }
      onObjectRemoved: (index, object) => {
        canvasMenu.removeMenu(object);
      }
    }

    MenuSeparator {
      enabled: canvasMenuFeatureListInstantiator.count > 0 && canvasMenu.isPasteMenuVisible
      width: parent.width
      visible: enabled
      height: enabled ? undefined : 0
    }

    QfMenu {
      id: pasteIntoLayers

      topMargin: sceneTopMargin
      bottomMargin: sceneBottomMargin

      title: qsTr("Paste Into Layer")
      font: QfTheme.defaultFont

      icon.source: QfTheme.getThemeVectorIcon("ic_paste_black_24dp")
      icon.color: enabled ? QfTheme.mainTextColor : QfTheme.mainTextDisabledColor

      onAboutToShow: {
        layersModel.clear();
        const feature = clipboardManager.pasteFeatureFromClipboard();
        const featureGeometryType = feature.geometry.type;
        const mapLayers = QfProjectUtils.mapLayers(qgisProject);
        for (let layerId in mapLayers) {
          const layer = mapLayers[layerId];
          if (layer.type === Qgis.LayerType.Vector) {
            const layerGeometryType = layer.geometryType();
            if (layerGeometryType !== Qgis.GeometryType.Null && layerGeometryType !== Qgis.GeometryType.Unknown && (featureGeometryType !== Qgis.GeometryType.Point || layerGeometryType === Qgis.GeometryType.Point)) {
              if (layer.supportsEditing && !layer.readOnly) {
                layersModel.append({
                  "LayerType": layerGeometryType,
                  "Layer": layer
                });
              }
            }
          }
        }
      }

      Repeater {
        model: ListModel {
          id: layersModel
        }

        delegate: MenuItem {
          text: Layer.name
          font: QfTheme.defaultFont

          height: 48
          leftPadding: QfTheme.menuItemLeftPadding

          icon.source: iconForGeometry(LayerType)

          onTriggered: {
            if (Layer) {
              const result = clipboardManager.pasteFeatureFromClipboardIntoLayer(Layer);
              if (result) {
                displayToast(qsTr("Feature pasted successfully"), 'info');
              } else {
                displayToast(qsTr("Failed to paste feature into layer"), 'error');
              }
              canvasMenu.close();
            }
          }
        }
      }
    }
  }

  function iconForGeometry(type) {
    switch (type) {
    case Qgis.GeometryType.Point:
      return QfTheme.getThemeVectorIcon('ic_geometry_point_24dp');
    case Qgis.GeometryType.Line:
      return QfTheme.getThemeVectorIcon('ic_geometry_line_24dp');
    case Qgis.GeometryType.Polygon:
      return QfTheme.getThemeVectorIcon('ic_geometry_polygon_24dp');
    default:
      return QfTheme.getThemeVectorIcon('ic_info_white_24dp');
    }
  }

  QfMenu {
    id: navigationMenu
    objectName: "navigationMenu"

    title: qsTr("Navigation Options")
    font: QfTheme.defaultFont

    topMargin: sceneTopMargin
    bottomMargin: sceneBottomMargin

    MenuItem {
      text: qsTr("Always Show Precise View")
      height: 48
      leftPadding: QfTheme.menuItemCheckLeftPadding
      font: QfTheme.defaultFont

      checkable: true
      checked: positioningSettings.alwaysShowPreciseView
      indicator.height: 20
      indicator.width: 20
      indicator.implicitHeight: 24
      indicator.implicitWidth: 24
      onCheckedChanged: positioningSettings.alwaysShowPreciseView = checked
    }

    MenuSeparator {
      width: parent.width
    }

    MenuItem {
      id: cancelNavigationItem
      text: qsTr("Clear Destination")
      height: 48
      leftPadding: QfTheme.menuItemIconlessLeftPadding
      font: QfTheme.defaultFont

      onTriggered: {
        navigation.clear();
      }
    }
  }

  QfMenu {
    id: gnssMenu
    objectName: "gnssMenu"

    title: qsTr("Positioning Options")
    font: QfTheme.defaultFont

    topMargin: sceneTopMargin
    bottomMargin: sceneBottomMargin

    MenuItem {
      id: positioningDeviceName
      text: positioningSettings.positioningDeviceName
      height: 48
      font: QfTheme.defaultFont
      enabled: false
    }

    MenuSeparator {
      width: parent.width
    }

    MenuItem {
      id: positioningItem
      text: qsTr("Enable Positioning")
      height: 48
      leftPadding: QfTheme.menuItemCheckLeftPadding
      font: QfTheme.defaultFont

      checkable: true
      checked: positioningSettings.positioningActivated
      indicator.height: 20
      indicator.width: 20
      indicator.implicitHeight: 24
      indicator.implicitWidth: 24

      onTriggered: positioningSettings.positioningActivated = checked
    }

    MenuItem {
      text: qsTr("Enable NTRIP Corrections")
      visible: positionSource.ntripSettings.isValid && positionSource.deviceCapabilities & QfAbstractGnssReceiver.NtripCorrection
      height: positionSource.ntripSettings.isValid && positionSource.deviceCapabilities & QfAbstractGnssReceiver.NtripCorrection ? 48 : 0
      leftPadding: QfTheme.menuItemCheckLeftPadding
      font: QfTheme.defaultFont

      checkable: true
      checked: positioningSettings.enableNtrip && positionSource.ntripState !== QfPositioning.NtripState.Disconnected
      indicator.height: 20
      indicator.width: 20
      indicator.implicitHeight: 24
      indicator.implicitWidth: 24

      onClicked: {
        if (positioningSettings.enableNtrip) {
          if (positionSource.ntripSettings.isValid && positionSource.ntripState === QfPositioning.NtripState.Disconnected) {
            // The server has disconnected, tapping on the toggle must indicate an intent to reconnect
            positioningSettings.enableNtrip = false;
            positioningSettings.enableNtrip = true;
          } else {
            positioningSettings.enableNtrip = false;
          }
        } else {
          positioningSettings.enableNtrip = true;
        }
      }
    }

    MenuItem {
      text: qsTr("Positioning Settings")
      height: 48
      leftPadding: QfTheme.menuItemIconlessLeftPadding
      font: QfTheme.defaultFont

      onTriggered: {
        qfieldSettings.currentPanel = 1;
        qfieldSettings.visible = true;
      }
    }

    MenuSeparator {
      width: parent.width
    }

    MenuItem {
      text: qsTr("Show Position Information")
      height: 48
      leftPadding: QfTheme.menuItemCheckLeftPadding
      font: QfTheme.defaultFont

      checkable: true
      checked: positioningSettings.showPositionInformation
      indicator.height: 20
      indicator.width: 20
      indicator.implicitHeight: 24
      indicator.implicitWidth: 24

      onTriggered: positioningSettings.showPositionInformation = checked
    }

    MenuItem {
      text: qsTr("Lock Coordinate Cursor to Location")
      height: 48
      leftPadding: QfTheme.menuItemCheckLeftPadding
      font: QfTheme.defaultFont
      enabled: positionSource.active && positionSource.positionInformation && positionSource.positionInformation.latitudeValid
      checkable: true
      checked: positioningSettings.positioningCoordinateLock
      indicator.height: 20
      indicator.width: 20
      indicator.implicitHeight: 24
      indicator.implicitWidth: 24

      onTriggered: {
        gnssCursorLockButton.click();
      }
    }

    MenuItem {
      text: qsTr("Lock Map Canvas to Location")
      height: 48
      leftPadding: QfTheme.menuItemCheckLeftPadding
      font: QfTheme.defaultFont
      enabled: positionSource.active && positionSource.positionInformation && positionSource.positionInformation.latitudeValid
      checkable: true
      checked: gnssButton.autoRefollow
      indicator.height: 20
      indicator.width: 20
      indicator.implicitHeight: 24
      indicator.implicitWidth: 24

      onTriggered: {
        gnssCanvasLockButton.click();
      }
    }

    MenuItem {
      text: qsTr("Add Bookmark at Location")
      icon.source: QfTheme.getThemeVectorIcon("ic_add_bookmark_black_24dp")
      height: 48
      leftPadding: QfTheme.menuItemLeftPadding
      font: QfTheme.defaultFont
      enabled: positionSource.active && positionSource.positionInformation && positionSource.positionInformation.latitudeValid

      onTriggered: {
        addBookmarkAtCurrentLocationButton.click();
      }
    }

    MenuItem {
      text: qsTr("Copy Location Coordinates")
      height: 48
      leftPadding: QfTheme.menuItemLeftPadding
      font: QfTheme.defaultFont
      icon.source: QfTheme.getThemeVectorIcon("ic_copy_black_24dp")
      enabled: positionSource.active && positionSource.positionInformation && positionSource.positionInformation.latitudeValid

      onTriggered: {
        copyCurrentLocationButton.click();
      }
    }
  }

  /* The feature form */
  QfFeatureListForm {
    id: featureListForm
    objectName: "featureForm"

    mapSettings: mapCanvas.mapSettings
    digitizingToolbar: digitizingToolbar
    moveFeaturesToolbar: moveFeaturesToolbar
    rotateFeaturesToolbar: rotateFeaturesToolbar
    codeReader: codeReader

    focus: visible

    anchors {
      right: parent.right
      bottom: parent.bottom
    }

    allowEdit: stateMachine.state === "digitize"
    allowDelete: stateMachine.state === "digitize"

    model: QfMultiFeatureListModel {}

    selection: QfFeatureListModelSelection {
      id: featureListModelSelection
      model: featureListForm.model
    }

    extentController.keepScale: qfieldSettings.locatorKeepScale

    selectionColor: "#ff7777"

    onShowMessage: displayToast(message)

    onRequestJumpToPoint: function (center, scale, handleMargins) {
      mapCanvasMap.jumpTo(center, scale, -1, handleMargins);
    }

    onEditGeometry: {
      // Set overall selected (i.e. current) layer to that of the feature geometry being edited,
      // important for snapping settings to make sense when set to current layer
      if (dashBoard.activeLayer != featureListForm.selection.focusedLayer) {
        dashBoard.activeLayer = featureListForm.selection.focusedLayer;
        displayToast(qsTr("Current layer switched to the one holding the selected geometry."));
      }
      geometryEditingFeature.vertexModel.geometry = featureListForm.selection.focusedGeometry;
      geometryEditingFeature.vertexModel.crs = featureListForm.selection.focusedLayer.crs;
      geometryEditingFeature.currentLayer = featureListForm.selection.focusedLayer;
      geometryEditingFeature.feature = featureListForm.selection.focusedFeature;
      if (geometryEditingVertexModel.editingAllowed) {
        featureListForm.state = "Hidden";
        geometryEditorsToolbar.init();
      } else {
        displayToast(qsTr("Editing of multipart geometry is not supported yet."), 'warning');
        geometryEditingVertexModel.clear();
      }
    }

    onStateChanged: {
      platformUtilities.setHandleVolumeKeys(qfieldSettings.digitizingVolumeKeys && state === "Hidden");
      if (state !== "Hidden" && bookmarkList.visible) {
        bookmarkList.hide();
      }
    }

    Component.onCompleted: focusstack.addFocusTaker(this)
  }

  QfDropShadow {
    anchors.fill: featureListForm
    horizontalOffset: mainWindow.width >= mainWindow.height ? -2 : 0
    verticalOffset: mainWindow.width < mainWindow.height ? -2 : 0
    radius: 6.0
    color: "#80000000"
    source: featureListForm
  }

  QfBookmarkList {
    id: bookmarkList
    objectName: "bookmarkList"

    model: bookmarkModel
    focus: visible
    anchors {
      right: parent.right
      bottom: parent.bottom
    }

    onRequestBookmarkProperties: (bookmarkId, bookmarkName, bookmarkGroup) => {
      bookmarkProperties.bookmarkId = bookmarkId;
      bookmarkProperties.bookmarkName = bookmarkName;
      bookmarkProperties.bookmarkGroup = bookmarkGroup;
      bookmarkProperties.open();
    }

    onStateChanged: {
      if (state !== "Hidden" && featureListForm.visible) {
        featureListForm.hide();
      }
    }

    onRequestBookmarkNavigation: bookmarkIndex => {
      navigation.destination = bookmarkList.model.getBookmarkPoint(bookmarkIndex);
      bookmarkList.hide();
    }

    Component.onCompleted: focusstack.addFocusTaker(this)
  }

  QfDropShadow {
    anchors.fill: bookmarkList
    horizontalOffset: mainWindow.width >= mainWindow.height ? -2 : 0
    verticalOffset: mainWindow.width < mainWindow.height ? -2 : 0
    radius: 6.0
    color: "#80000000"
    source: bookmarkList
  }

  QfOverlayFeatureFormDrawer {
    id: overlayFeatureFormDrawer
    objectName: "overlayFeatureFormDrawer"
    digitizingToolbar: digitizingToolbar
    codeReader: codeReader
    featureModel.currentLayer: dashBoard.activeLayer

    onRequestJumpToPoint: function (center, scale, handleMargins) {
      mapCanvasMap.jumpTo(center, scale, -1, handleMargins);
    }

    Component.onCompleted: focusstack.addFocusTaker(this)
  }

  function showAutoLockToast() {
    displayToast(qsTr('Map canvas lock paused'), 'info', qsTr('Unlock'), () => {
      gnssButton.autoRefollow = false;
    }, true, () => {
      if (positionSource.active && gnssButton.autoRefollow) {
        mapCanvasMap.freeze('follow');
        gnssButton.followActive = true;
        gnssButton.followLocation(true);
      }
    });
  }

  function displayToast(message, type, action_text, action_function, stop_function, is_animation_enabled) {
    toast.show(message, type, action_text, action_function, stop_function, is_animation_enabled);
  }

  function closeToast() {
    toast.close();
  }

  Timer {
    id: readProjectTimer

    interval: 250
    repeat: false
    onTriggered: iface.readProject()
  }

  Connections {
    target: iface

    function onExecuteAction(action) {
      const details = QfUrlUtils.getActionDetails(action);
      if (details.type === "local" && details.import !== undefined && details.import !== "") {
        importPermissionDialog.url = details.import;
        importPermissionDialog.open();
      } else if (details.type === "cloud" && details.project !== undefined && details.project !== "") {
        qfieldCloudScreen.prepareProjectRequest(details);
      }
    }

    function onVolumeKeyUp(volumeKeyCode) {
      if (stateMachine.state === 'browse' || !mapCanvasMap.isEnabled) {
        return;
      }
      switch (volumeKeyCode) {
      case Qt.Key_VolumeDown:
        if (mapCanvasMap.interactive) {
          digitizingToolbar.removeVertex();
        }
        break;
      case Qt.Key_VolumeUp:
        if (!geometryEditorsToolbar.canvasClicked(coordinateLocator.currentCoordinate)) {
          digitizingToolbar.triggerAddVertex();
        }
        break;
      default:
        break;
      }
    }

    function onImportTriggered(name) {
      busyOverlay.text = qsTr("Importing %1").arg(name);
      busyOverlay.state = "visible";
    }

    function onImportProgress(progress) {
      busyOverlay.progress = progress;
    }

    function onImportEnded(path, originalUrl) {
      busyOverlay.state = "hidden";
      if (path !== '') {
        if (QfFileUtils.fileExists(path)) {
          // A project or dataset path is provided, load it
          iface.loadFile(path);
          welcomeScreen.model.removeRecentProject(originalUrl);
          welcomeScreen.model.reloadModel();
        } else {
          // A directory path is provided, display it
          qfieldLocalDataPickerScreen.model.currentPath = path;
          qfieldLocalDataPickerScreen.visible = true;
        }
        welcomeScreen.visible = false;
      } else {
        displayToast(qsTr('Import URL failed'));
      }
    }

    function onLoadProjectTriggered(path, name) {
      qfieldAuthRequestHandler.isProjectLoading = true;
      messageLogModel.suppress({
        "WFS": [""],
        "WMS": [""],
        "PostGIS": ["fe_sendauth: no password supplied"]
      });
      qfieldLocalDataPickerScreen.visible = false;
      qfieldLocalDataPickerScreen.focus = false;
      welcomeScreen.visible = false;
      welcomeScreen.focus = false;
      projectCreationScreen.visible = false;
      projectCreationScreen.focus = false;
      if (changelogPopup.visible)
        changelogPopup.close();
      dashBoard.layerTree.freeze();
      mapCanvasMap.stopRendering();
      mapCanvasMap.freeze('projectload');
      busyOverlay.text = qsTr("Loading %1").arg(name !== '' ? name : path);
      busyOverlay.state = "visible";
      navigation.clearDestinationFeature();
      projectInfo.filePath = '';
      readProjectTimer.start();
    }

    function onLoadProjectEnded(path, name) {
      mapCanvasMap.unfreeze('projectload');
      busyOverlay.state = "hidden";
      dashBoard.layerTree.unfreeze(true);
      gnssButton.jumpedOnce = false;
      if (qfieldAuthRequestHandler.hasPendingAuthRequest) {
        qfieldAuthRequestHandler.handleLayerLogins();
      } else {
        // project in need of handling layer credentials
        qfieldAuthRequestHandler.isProjectLoading = true;
        messageLogModel.unsuppress({
          "WFS": [],
          "WMS": [],
          "PostGIS": []
        });
      }
      projectInfo.filePath = path;
      stateMachine.state = projectInfo.stateMode;
      platformUtilities.setHandleVolumeKeys(qfieldSettings.digitizingVolumeKeys && stateMachine.state != 'browse');
      let activeLayer = projectInfo.activeLayer;
      if (flatLayerTree.mapTheme != '') {
        const defaultActiveLayer = projectInfo.getDefaultActiveLayerForMapTheme(flatLayerTree.mapTheme);
        if (defaultActiveLayer !== null) {
          activeLayer = defaultActiveLayer;
        }
      }
      if (!qfieldAuthRequestHandler.hasPendingAuthRequest) {
        // only set active layer when not handling layer credentials
        dashBoard.activeLayer = activeLayer;
      }
      drawingTemplateModel.projectFilePath = path;
      mapCanvasBackground.color = mapCanvas.mapSettings.backgroundColor;
      const titleDecorationConfiguration = projectInfo.getTitleDecorationConfiguration();
      titleDecoration.color = titleDecorationConfiguration["color"];
      titleDecoration.style = titleDecorationConfiguration["hasOutline"] === true ? Text.Outline : Text.Normal;
      titleDecoration.styleColor = titleDecorationConfiguration["outlineColor"];
      titleDecorationBackground.color = titleDecorationConfiguration["backgroundColor"];
      titleDecoration.decorationText = titleDecorationConfiguration["text"];
      if (!titleDecoration.isExpressionTemplate) {
        titleDecoration.text = titleDecorationConfiguration["text"];
      }
      const copyrightDecorationConfiguration = projectInfo.getCopyrightDecorationConfiguration();
      copyrightDecoration.color = copyrightDecorationConfiguration["color"];
      copyrightDecoration.style = copyrightDecorationConfiguration["hasOutline"] === true ? Text.Outline : Text.Normal;
      copyrightDecoration.styleColor = copyrightDecorationConfiguration["outlineColor"];
      copyrightDecorationBackground.color = copyrightDecorationConfiguration["backgroundColor"];
      copyrightDecoration.decorationText = copyrightDecorationConfiguration["text"];
      if (!titleDecoration.isExpressionTemplate) {
        copyrightDecoration.text = copyrightDecorationConfiguration["text"];
      }
      const imageDecorationConfiguration = projectInfo.getImageDecorationConfiguration();
      imageDecoration.source = imageDecorationConfiguration["source"];
      imageDecoration.fillColor = imageDecorationConfiguration["fillColor"];
      imageDecoration.strokeColor = imageDecorationConfiguration["strokeColor"];
      const gridDecorationConfiguration = projectInfo.getGridDecorationConfiguration();
      gridDecoration.enabled = false;
      gridDecoration.xInterval = gridDecorationConfiguration["xInterval"];
      gridDecoration.yInterval = gridDecorationConfiguration["yInterval"];
      gridDecoration.xOffset = gridDecorationConfiguration["xOffset"];
      gridDecoration.yOffset = gridDecorationConfiguration["yOffset"];
      gridDecoration.prepareLines = gridDecorationConfiguration["hasLines"];
      gridDecoration.majorLineColor = gridDecorationConfiguration["lineColor"];
      gridDecoration.prepareMarkers = gridDecorationConfiguration["hasMarkers"];
      gridDecoration.markerColor = gridDecorationConfiguration["markerColor"];
      gridDecoration.prepareAnnotations = gridDecorationConfiguration["hasAnnotations"];
      gridDecoration.annotationPrecision = gridDecorationConfiguration["annotationPrecision"];
      gridDecoration.annotationColor = gridDecorationConfiguration["annotationColor"];
      gridDecoration.annotationHasOutline = gridDecorationConfiguration["annotationHasOutline"];
      gridDecoration.annotationOutlineColor = gridDecorationConfiguration["annotationOutlineColor"];
      gridDecoration.enabled = gridDecorationConfiguration["hasLines"] || gridDecorationConfiguration["hasMarkers"];
      recentProjectListModel.reloadModel();
      const cloudProjectId = QfCloudUtils.getProjectId(qgisProject.fileName);
      cloudProjectsModel.currentProjectId = cloudProjectId;
      if (cloudProjectsModel.currentProject) {
        const forceAutoPush = iface.readProjectBoolEntry("qfieldsync", "forceAutoPush", false);
        if (forceAutoPush) {
          cloudProjectsModel.currentProject.forceAutoPush = true;
          cloudProjectsModel.currentProject.autoPushEnabled = true;
          cloudProjectsModel.currentProject.autoPushIntervalMins = iface.readProjectNumEntry("qfieldsync", "forceAutoPushIntervalMins", 30);
        } else {
          cloudProjectsModel.currentProject.forceAutoPush = false;
        }
        switch (cloudProjectsModel.currentProject.userRole) {
        case 'reader':
          stateMachine.state = "browse";
          projectInfo.hasInsertRights = false;
          projectInfo.hasEditRights = false;
          break;
        case 'reporter':
          projectInfo.hasInsertRights = true;
          projectInfo.hasEditRights = false;
          break;
        case 'editor':
        case 'manager':
        case 'admin':
          projectInfo.hasInsertRights = true;
          projectInfo.hasEditRights = true;
          break;
        default:
          projectInfo.hasInsertRights = true;
          projectInfo.hasEditRights = true;
          break;
        }
        if (cloudProjectsModel.layerObserver.deltaFileWrapper.hasError) {
          qfieldCloudPopup.show();
        }
        if (cloudConnection.status === QfCloudConnection.LoggedIn) {
          projectInfo.cloudUserInformation = cloudConnection.userInformation;
          // Refresh cloud project details
          cloudProjectsModel.appendProject(cloudProjectId);
        } else {
          projectInfo.restoreCloudUserInformation();
        }
        if (cloudConnection.isReachable) {
          if (!settings.valueBool("/QField/showMapCanvasGuide", true) && settings.valueBool("/QField/showCloudButtonGuide", true) && cloudProjectsModel.layerObserver.deltaFileWrapper.count > 0) {
            cloudMenuTour.runTour();
            settings.setValue("/QField/showCloudButtonGuide", false);
          }
        }
      } else {
        projectInfo.hasInsertRights = true;
        projectInfo.hasEditRights = true;
      }
      if (stateMachine.state === "digitize" && !qfieldAuthRequestHandler.hasPendingAuthRequest) {
        dashBoard.ensureEditableLayerSelected();
      }
      const distanceString = iface.readProjectEntry("Measurement", "/DistanceUnits", "");
      const decodedDistanceUnits = distanceString !== "" ? UnitTypes.decodeDistanceUnit(distanceString) : Qgis.DistanceUnit.Meters;
      projectInfo.distanceUnits = decodedDistanceUnits !== Qgis.DistanceUnit.Unknown ? decodedDistanceUnits : mapCanvas.mapSettings.destinationCrs.mapUnits;
      const areaString = iface.readProjectEntry("Measurement", "/AreaUnits", "");
      const decodedAreaUnits = areaString !== "" ? UnitTypes.decodeAreaUnit(areaString) : Qgis.AreaUnit.SquareMeters;
      projectInfo.areaUnits = decodedAreaUnits !== Qgis.AreaUnit.Unknown ? decodedAreaUnits : UnitTypes.distanceToAreaUnit(mapCanvas.mapSettings.destinationCrs.mapUnits);
      if (qgisProject.displaySettings) {
        projectInfo.coordinateDisplayCrs = qgisProject.displaySettings.coordinateCrs;
      } else {
        projectInfo.coordinateDisplayCrs = !mapCanvas.mapSettings.destinationCrs.isGeographic && iface.readProjectEntry("PositionPrecision", "/DegreeFormat", "MU") !== "MU" ? QfCoordinateReferenceSystemUtils.wgs84Crs() : mapCanvas.mapSettings.destinationCrs;
      }
      layoutListInstantiator.model.reloadModel();
      geofencer.applyProjectSettings(qgisProject);
      positioningSettings.geofencingPreventDigitizingDuringAlert = iface.readProjectBoolEntry("qfieldsync", "/geofencingShouldPreventDigitizing", false);

      // Location arrow customization
      const locationArrowFillColor = iface.readProjectEntry("qfieldsync", "/locationArrowFillColor", "");
      if (locationArrowFillColor !== "") {
        locationMarker.color = locationArrowFillColor;
      } else {
        locationMarker.color = Qt.darker(QfTheme.positionColor, 1.25);
      }
      const locationArrowOutlineColor = iface.readProjectEntry("qfieldsync", "/locationArrowOutlineColor", "");
      if (locationArrowOutlineColor !== "") {
        locationMarker.strokeColor = locationArrowOutlineColor;
      } else {
        locationMarker.strokeColor = "white";
      }
      const locationArrowSize = iface.readProjectEntry("qfieldsync", "/locationArrowSize", "normal");
      switch (locationArrowSize) {
      case "tiny":
        locationMarker.sizeScale = 0.7;
        break;
      case "big":
        locationMarker.sizeScale = 1.5;
        break;
      case "biggest":
        locationMarker.sizeScale = 2.0;
        break;
      default:
        locationMarker.sizeScale = 1.0;
        break;
      }

      // Coordinate cursor customization
      const coordinateCursorFillColor = iface.readProjectEntry("qfieldsync", "/coordinateCursorFillColor", "");
      if (coordinateCursorFillColor !== "") {
        coordinateLocator.cursorFillColor = coordinateCursorFillColor;
      } else {
        coordinateLocator.cursorFillColor = "#000000";
      }
      const coordinateCursorOutlineColor = iface.readProjectEntry("qfieldsync", "/coordinateCursorOutlineColor", "");
      if (coordinateCursorOutlineColor !== "") {
        coordinateLocator.cursorOutlineColor = coordinateCursorOutlineColor;
      } else {
        coordinateLocator.cursorOutlineColor = "#FFFFFF";
      }
      const coordinateCursorSize = iface.readProjectEntry("qfieldsync", "/coordinateCursorSize", "normal");
      switch (coordinateCursorSize) {
      case "tiny":
        coordinateLocator.cursorSizeScale = 0.7;
        break;
      case "big":
        coordinateLocator.cursorSizeScale = 1.5;
        break;
      case "biggest":
        coordinateLocator.cursorSizeScale = 2.0;
        break;
      default:
        coordinateLocator.cursorSizeScale = 1.0;
        break;
      }

      mapCanvasTour.startOnFreshRun();
    }

    function onSetMapExtent(extent) {
      mapCanvas.mapSettings.extent = extent;
    }
  }

  Connections {
    target: flatLayerTree

    function onMapThemeChanged() {
      if (!flatLayerTree.isFrozen && flatLayerTree.mapTheme != '') {
        const defaultActiveLayer = projectInfo.getDefaultActiveLayerForMapTheme(flatLayerTree.mapTheme);
        if (defaultActiveLayer !== null) {
          dashBoard.activeLayer = defaultActiveLayer;
        }
      }
    }
  }

  Connections {
    target: bookmarkModel

    function onRequestJumpToPoint(center, scale, handleMargins) {
      mapCanvasMap.jumpTo(center, scale, -1, handleMargins);
    }
  }

  QfProjectInfo {
    id: projectInfo
    objectName: "projectInfo"

    mapSettings: mapCanvas.mapSettings
    layerTree: dashBoard.layerTree
    trackingModel: trackings.model

    property var distanceUnits: Qgis.DistanceUnit.Meters
    property var areaUnits: Qgis.AreaUnit.SquareMeters
    property var coordinateDisplayCrs: QfCoordinateReferenceSystemUtils.wgs84Crs()

    property bool hasInsertRights: true
    property bool hasEditRights: true

    property bool insertRights: hasInsertRights
    property bool editRights: hasEditRights
  }

  QfMessageLog {
    id: messageLog
    objectName: 'messageLog'

    anchors.fill: parent

    model: messageLogModel

    onFinished: {
      visible = false;
    }

    Component.onCompleted: {
      focusstack.addFocusTaker(this);
      unreadMessages = messageLogModel.rowCount() !== 0;
    }
  }

  QfBadLayerItem {
    id: badLayersView
    visible: false
  }

  Item {
    id: layerLogin

    Connections {
      target: qfieldAuthRequestHandler

      function onShowLoginDialog(realm, title) {
        loginDialog.realm = realm || "";
        loginDialog.credentialTitle = title;
        badLayersView.visible = false;
        loginDialogPopup.open();
      }

      function onReloadEverything() {
        iface.reloadProject();
      }

      function onShowLoginBrowser(url) {
        if (qfieldAuthRequestHandler.isProjectLoading) {
          browserPopup.url = url;
          browserPopup.fullscreen = false;
          browserPopup.clearCookiesOnOpen = true;
          browserPopup.open();
        } else {
          Qt.openUrlExternally(url);
        }
      }

      function onHideLoginBrowser() {
        browserPopup.close();
      }
    }

    Connections {
      target: browserPopup

      function onCancel() {
        qfieldAuthRequestHandler.abortAuthBrowser();
        browserPopup.close();
      }
    }

    QfBrowserPanel {
      id: browserPopup
      objectName: "browserPopup"
      parent: Overlay.overlay
    }

    QfPopup {
      id: loginDialogPopup
      parent: Overlay.overlay
      width: parent.width - QfTheme.popupScreenEdgeHorizontalMargin * 2
      height: parent.height - Math.max(QfTheme.popupScreenEdgeVerticalMargin * 2, mainWindow.sceneTopMargin * 2 + 4, mainWindow.sceneBottomMargin * 2 + 4)
      x: QfTheme.popupScreenEdgeHorizontalMargin
      y: (mainWindow.height - height) / 2
      closePolicy: Popup.CloseOnEscape

      QfLayerLoginDialog {
        id: loginDialog
        anchors.fill: parent
        visible: true
        inCancelation: false

        property string realm: ""

        onEnter: (username, password) => {
          qfieldAuthRequestHandler.enterCredentials(loginDialog.realm, username, password);
          inCancelation = false;
          loginDialogPopup.close();
        }

        onCancel: {
          inCancelation = true;
          loginDialogPopup.close();
        }
      }

      onClosed: {
        // handled here with parameter inCancelation because the loginDialog needs to be closed before the signal is fired
        qfieldAuthRequestHandler.loginDialogClosed(loginDialog.realm, loginDialog.inCancelation);
      }
    }
  }

  QfTrackerSettings {
    id: trackerSettings
    objectName: "trackerSettings"

    Component.onCompleted: focusstack.addFocusTaker(this)
  }

  QfTrackerFeatureForm {
    id: trackerFeatureForm

    onRequestJumpToPoint: function (center, scale, handleMargins) {
      mapCanvasMap.jumpTo(center, scale, -1, handleMargins);
    }
  }

  QfCloudConnection {
    id: cloudConnection
    objectName: "cloudConnection"

    property int previousStatus: QfCloudConnection.Disconnected

    onStatusChanged: {
      if (cloudConnection.status === QfCloudConnection.Disconnected && previousStatus === QfCloudConnection.LoggedIn) {
        displayToast(qsTr('Signed out'));
        qfieldCloudStatus.refresh();
      } else if (cloudConnection.status === QfCloudConnection.Connecting) {
        displayToast(qsTr('Connecting...'));
      } else if (cloudConnection.status === QfCloudConnection.LoggedIn) {
        displayToast(qsTr('Signed in'));
        if (QfCloudUtils.hasPendingAttachments(cloudConnection.username)) {
          // Go ahead and upload pending attachments in the background
          platformUtilities.uploadPendingAttachments(cloudConnection);
        }
        const cloudProjectId = QfCloudUtils.getProjectId(qgisProject.fileName);
        if (cloudProjectId) {
          projectInfo.cloudUserInformation = userInformation;
          // Refresh cloud project details
          cloudProjectsModel.appendProject(cloudProjectId);
        }
        // Reload recent projects to insure only current user projects are visible
        recentProjectListModel.reloadModel();
      }
      previousStatus = cloudConnection.status;
    }

    onLoginFailed: function (reason) {
      qfieldCloudStatus.refresh();
      displayToast(reason);
    }
  }

  QfCloudProjectsModel {
    id: cloudProjectsModel
    objectName: "cloudProjectsModel"

    cloudConnection: cloudConnection
    layerObserver: layerObserverAlias
    gpkgFlusher: gpkgFlusherAlias

    onProjectDownloaded: (projectId, projectName, projectOwner, hasError, errorString) => {
      if (hasError) {
        const ownerIsUser = cloudConnection.username === projectOwner;
        if (errorString.indexOf(`"code":"${QFieldCloudUtils.errorCodeOverQuota}"`) >= 0) {
          if (ownerIsUser && cloudConnection.url == cloudConnection.defaultUrl && Qt.platform !== "ios") {
            displayToast(qsTr("Project %1 cannot be packaged as your available storage is full.").arg(projectName), 'info', qsTr('Upgrade storage'), function () {
              Qt.openUrlExternally('https://app.qfield.cloud/plans');
            });
          } else {
            displayToast(qsTr("Project %1 cannot be packaged as the project owner's available storage is full.").arg(projectName), 'warning');
          }
        } else if (errorString.indexOf(`"code":"${QFieldCloudUtils.errorCodePlanInsufficient}"`) >= 0) {
          if (ownerIsUser && cloudConnection.url == cloudConnection.defaultUrl && Qt.platform !== "ios") {
            displayToast(qsTr("Project %1 cannot be downloaded as your subscription plan is insufficient.").arg(projectName), 'info', qsTr('Upgrade plan'), function () {
              Qt.openUrlExternally('https://app.qfield.cloud/plans');
            });
          } else {
            displayToast(qsTr("Project %1 cannot be downloaded as the project owner's subscription plan is insufficient.").arg(projectName), 'warning');
          }
        } else {
          displayToast(qsTr("Project %1 failed to download").arg(projectName), 'error');
          qfieldCloudStatus.refresh();
        }
      } else if (qfieldCloudScreen.visible || qfieldCloudPopup.visible || welcomeScreen.visible) {
        displayToast(qsTr("Project %1 successfully downloaded, it's now available to open").arg(projectName));
      }
    }

    onPushFinished: (projectId, isDownloadingProject, hasError, errorString) => {
      if (hasError) {
        qfieldCloudStatus.refresh();
        displayToast(qsTr("Changes failed to reach QFieldCloud: %1").arg(errorString), 'error');
        return;
      }
      if (!isDownloadingProject && (qfieldCloudScreen.visible || qfieldCloudPopup.visible || welcomeScreen.visible)) {
        displayToast(qsTr("Changes successfully pushed to QFieldCloud"));
      }
      if (QfCloudUtils.hasPendingAttachments(cloudConnection.username)) {
        // Go ahead and upload pending attachments in the background
        platformUtilities.uploadPendingAttachments(cloudConnection);
      }
    }

    onWarning: message => {
      displayToast(message);
    }
  }

  QfCloudDeltaHistory {
    id: qfieldCloudDeltaHistory
    objectName: "qfieldCloudDeltaHistory"

    model.cloudConnection: cloudConnection

    Component.onCompleted: focusstack.addFocusTaker(this)
  }

  QfCloudStatus {
    id: qfieldCloudStatus
    objectName: "qfieldCloudStatus"
    url: cloudConnection.url
  }

  QfWelcomeScreen {
    id: welcomeScreen
    objectName: "welcomeScreen"
    visible: !iface.hasProjectOnLaunch()

    model: QfRecentProjectListModel {
      id: recentProjectListModel
    }

    anchors.fill: parent

    onShowAbout: {
      aboutDialog.visible = true;
    }

    onShowLocalDataPicker: {
      qfieldLocalDataPickerScreen.projectFolderView = false;
      qfieldLocalDataPickerScreen.model.resetToRoot();
      qfieldLocalDataPickerScreen.visible = true;
    }

    onShowQFieldCloudScreen: {
      qfieldCloudStatus.refresh();
      qfieldCloudScreen.visible = true;
    }

    onShowSettings: {
      qfieldSettings.reset();
      qfieldSettings.visible = true;
    }

    onShowProjectCreationScreen: {
      projectCreationScreen.show();
    }

    Component.onCompleted: focusstack.addFocusTaker(this)
  }

  QfProjectCreationScreen {
    id: projectCreationScreen
    objectName: "projectCreationScreen"

    visible: false
    focus: visible
    width: parent.width
    height: parent.height

    onTriggerConnection: {
      qfieldCloudPopup.pendingAction = "connect";
      qfieldCloudPopup.show();
    }

    onTriggerCloudify: (title, path) => {
      iface.clearProject();
      cloudProjectsModel.currentProjectId = "";
      qfieldCloudPopup.cloudify(title, QfFileUtils.absolutePath(path));
    }

    onTriggerProjectLoad: (title, path) => {
      iface.loadFile(path, title);
    }

    Component.onCompleted: focusstack.addFocusTaker(this)
  }

  QfCloudScreen {
    id: qfieldCloudScreen
    objectName: "qfieldCloudScreen"

    anchors.fill: parent
    visible: false
    focus: visible

    cloudServiceStatus: qfieldCloudStatus
    cloudProjectsModel: cloudProjectsModel

    onFinished: {
      visible = false;
    }

    Component.onCompleted: focusstack.addFocusTaker(this)

    onViewProjectFolder: projectPath => {
      qfieldLocalDataPickerScreen.projectFolderView = true;
      qfieldLocalDataPickerScreen.model.resetToPath(projectPath);
      qfieldLocalDataPickerScreen.visible = true;
    }
  }

  function openCloudPopup() {
    qfieldCloudStatus.refresh();
    dashBoard.close();
    welcomeScreen.visible = false;
    qfieldCloudPopup.show();
  }

  QfCloudPopup {
    id: qfieldCloudPopup
    objectName: "qfieldCloudPopup"

    visible: false
    focus: visible
    parent: Overlay.overlay

    width: parent.width
    height: parent.height

    cloudServiceStatus: qfieldCloudStatus

    Component.onCompleted: focusstack.addFocusTaker(this)

    onAboutToShow: {
      cloudButtonTour.blockGuide();
      cloudMenuTour.blockGuide();
      settings.setValue("/QField/showCloudButtonGuide", false);
    }
  }

  QfCloudPackageLayersFeedback {
    id: cloudPackageLayersFeedback
    visible: false
  }

  QfLocalDataPickerScreen {
    id: qfieldLocalDataPickerScreen
    objectName: "qfieldLocalDataPickerScreen"

    anchors.fill: parent
    visible: false
    focus: visible

    onFinished: loading => {
      visible = false;
    }

    Component.onCompleted: focusstack.addFocusTaker(this)
  }

  QfAppSettings {
    id: qfieldSettings
    objectName: "qfieldSettings"

    anchors.fill: parent

    onFinished: {
      visible = false;
    }

    Component.onCompleted: focusstack.addFocusTaker(this)
  }

  QfChangelog {
    id: changelogPopup
    objectName: 'changelogPopup'
    parent: Overlay.overlay

    Component.onCompleted: {
      if (Qfield.name === "QField") {
        const changelogVersion = settings.value("/QField/ChangelogVersion", "");
        if (changelogVersion === "") {
          settings.setValue("/QField/ChangelogVersion", Qfield.version);
        } else if (changelogVersion !== Qfield.version) {
          open();
        }
      }
    }
  }

  QfAbout {
    id: aboutDialog
    anchors.fill: parent

    Component.onCompleted: focusstack.addFocusTaker(this)
  }

  QfToast {
    id: toast
    bottomSpacing: Math.max(60, mainWindow.sceneBottomMargin, informationDrawer.height, overlayFeatureFormDrawer.opened && !overlayFeatureFormDrawer.fullScreenView && overlayFeatureFormDrawer.y > 0 ? overlayFeatureFormDrawer.height : 0, !featureListForm.isFullscreen && !featureListForm.canvasOperationRequested && featureListForm.y > 0 ? featureListForm.height : 0)
  }

  MouseArea {
    id: codeReaderCatcher
    anchors.fill: parent
    enabled: codeReader.visible

    onClicked: mouse => {
      // Needed to avoid people interacting with the UI while the barcode reader is visible
      // (e.g. close the feature form while scanning a code to fill an attribute)
      return;
    }
  }

  QfCodeReader {
    id: codeReader
    objectName: 'codeReader'
    visible: false
  }

  QfSketcher {
    id: sketcher
    objectName: 'sketcher'
    visible: false

    Component.onCompleted: focusstack.addFocusTaker(this)
  }

  QfAppExpressionContextScopesGenerator {
    id: appScopesGenerator
    objectName: "appScopesGenerator"

    positionInformation: positionSource.positionInformation
    positionLocked: coordinateLocator.positionLocked
    cloudUserInformation: projectInfo.cloudUserInformation
  }

  Connections {
    target: locatorItem

    function onSearchTermChanged(searchTerm) {
      var lowered = searchTerm.toLowerCase();
      if (lowered === 'hello nyuki') {
        Qt.inputMethod.hide();
        locatorItem.searchTermHandled = true;
        nyuki.state = "shown";
      } else if (lowered === 'bye nyuki') {
        Qt.inputMethod.hide();
        locatorItem.searchTermHandled = true;
        nyuki.state = "hidden";
      }
    }
  }

  QfNyuki {
    id: nyuki
    anchors.bottom: parent.bottom
    anchors.bottomMargin: -200
    anchors.left: parent.left
    width: 200
    height: 200
  }

  DropArea {
    id: dropArea
    anchors.fill: parent
    onEntered: drag => {
      if (drag.urls.length !== 1 || !iface.isFileExtensionSupported(drag.urls[0])) {
        drag.accepted = false;
      } else {
        drag.accept(Qt.CopyAction);
        drag.accepted = true;
      }
    }
    onDropped: drop => {
      iface.loadFile(drop.urls[0]);
    }
  }

  QfBusyOverlay {
    id: busyOverlay
    objectName: 'busyOverlay'
    state: iface.hasProjectOnLaunch() ? "visible" : "hidden"
  }

  property bool closeAlreadyRequested: false

  onClosing: close => {
    if (screenLocker.enabled) {
      close.accepted = false;
      displayToast(qsTr("Unlock the screen to close project and app"));
      return;
    }
    if (!closeAlreadyRequested) {
      close.accepted = false;
      closeAlreadyRequested = true;
      const deltaFileWrapper = cloudProjectsModel.layerObserver.deltaFileWrapper;
      if (cloudConnection.isReachable && deltaFileWrapper && deltaFileWrapper.count > 0) {
        displayToast(qsTr("Pending changes are present. Upload these now or press back again to keep them pending and close the cloud project and app."), 'attention', qsTr("Upload local changes"), () => {
          closeAlreadyRequested = false;
          closingTimer.stop();
          openCloudPopup();
        });
      } else {
        displayToast(qsTr("Press back again to close project and app"));
      }
      closingTimer.start();
    } else {
      close.accepted = true;
    }
  }

  Timer {
    id: closingTimer
    interval: 2000
    onTriggered: {
      closeAlreadyRequested = false;
    }
  }

  // ! MODELS !
  QfFeatureModel {
    id: geometryEditingFeature
    project: qgisProject
    currentLayer: null
    appExpressionContextScopesGenerator: appScopesGenerator
    vertexModel: geometryEditingVertexModel
  }

  QfVertexModel {
    id: geometryEditingVertexModel
    currentPoint: coordinateLocator.currentCoordinate
    mapSettings: mapCanvas.mapSettings
    isHovering: mapCanvasMap.hovered
  }

  QfScreenLocker {
    id: screenLocker
    objectName: "screenLocker"
    enabled: false
  }

  QfDialog {
    id: importPermissionDialog
    parent: mainWindow.contentItem
    z: 10000 // 1000s are embedded feature forms, user a higher value to insure the dialog will always show above embedded feature forms

    width: Math.min(mainWindow.width - QfTheme.popupScreenEdgeVerticalMargin * 2, 400)

    property string url: ""
    property string serverName: ""
    property string fileName: ""

    onAboutToShow: {
      serverName = QfUrlUtils.urlDetail(url, "authority");
      fileName = QfUrlUtils.urlDetail(url, "filename");
      if (fileName === "") {
        fileName = QfUrlUtils.urlDetail(url, "path");
      }
    }

    title: qsTr("Import Confirmation")

    Column {
      width: parent.width

      Label {
        width: parent.width
        wrapMode: Text.WordWrap
        text: qsTr("Do you want to import <b>%1</b> from <b>%2</b> into %3?").arg(importPermissionDialog.fileName).arg(importPermissionDialog.serverName).arg(Qfield.name)
      }
    }

    onAccepted: {
      iface.importUrl(importPermissionDialog.url, "", true);
    }

    standardButtons: Dialog.Yes | Dialog.No
  }

  QfDialog {
    id: pluginPermissionDialog
    parent: mainWindow.contentItem
    z: 10000 // 1000s are embedded feature forms, user a higher value to insure the dialog will always show above embedded feature forms

    width: Math.min(mainWindow.width - QfTheme.popupScreenEdgeVerticalMargin * 2, 400)

    property string pluginName: ""
    property bool isProjectPlugin: false
    property alias permanent: permanentCheckBox.checked

    title: pluginPermissionDialog.isProjectPlugin ? pluginName + qsTr(" Project Plugin") : qsTr("Plugin Permission")

    Column {
      width: parent.width
      Label {
        width: parent.width
        wrapMode: Text.WordWrap
        text: pluginPermissionDialog.isProjectPlugin ? qsTr("Do you grant permission to activate the plugin?") : qsTr("Do you grant permission to activate `%1`?").arg(pluginPermissionDialog.pluginName)
      }

      CheckBox {
        id: permanentCheckBox
        text: qsTr('Remember my choice')
        font: QfTheme.defaultFont
      }
    }

    onAccepted: {
      pluginManager.grantRequestedPluginPermission(permanent);
      permanent = false;
    }

    onRejected: {
      pluginManager.denyRequestedPluginPermission(permanent);
      permanent = false;
    }
    standardButtons: Dialog.Yes | Dialog.No
  }

  QfDialog {
    id: cancelAlgorithmDialog
    parent: mainWindow.contentItem

    visible: false
    modal: true
    font: QfTheme.defaultFont

    z: 10000 // 1000s are embedded feature forms, user a higher value to insure the dialog will always show above embedded feature forms

    title: qsTr("Cancel algorithm operation")
    Label {
      width: parent.width
      wrapMode: Text.WordWrap
      text: qsTr("You are about to dismiss the ongoing algorithm operation, proceed?")
    }

    standardButtons: Dialog.Ok | Dialog.Cancel
    onAccepted: {
      featureListForm.state = "Hidden";
      mentMode();
    }
    onDiscarded: {
      cancelAlgorithmDialog.visible = false;
    }
  }

  Connections {
    target: pluginManager

    function onPluginPermissionRequested(pluginName, isProjectPlugin) {
      pluginPermissionDialog.pluginName = isProjectPlugin ? QfProjectUtils.title(qgisProject) : pluginName;
      pluginPermissionDialog.isProjectPlugin = isProjectPlugin;
      pluginPermissionDialog.open();
    }

    function onProjectPluginEnabled() {
      displayToast(qsTr("Project plugin loaded"));
    }
  }

  QfGuide {
    id: mapCanvasTour
    baseRoot: mainWindow
    objectName: 'mapCanvasTour'

    steps: [
      {
        "type": "information",
        "title": qsTr("Dashboard"),
        "description": qsTr("This button opens the dashboard. With the dashboard you can interact with the legend and map theme, or start digitizing by activating the editing mode. Long-pressing the button gives you immediate access to the main menu."),
        "target": () => [menuButton]
      },
      {
        "type": "information",
        "title": qsTr("Positioning"),
        "description": qsTr("This button toggles the positioning system. When enabled, a position marker will appear top of the map. Long-pressing the button will open the positioning menu where additional functionalities can be explored."),
        "target": () => [gnssButton]
      },
      {
        "type": "information",
        "title": qsTr("Search"),
        "description": qsTr("The search bar provides you with a quick way to find features within your project, jump to a typed latitude and longitude point, and much more."),
        "target": () => [locatorItem]
      }
    ]

    function startOnFreshRun() {
      const startupGuide = settings.valueBool("/QField/showMapCanvasGuide", true);
      if (startupGuide) {
        runTour();
      }
      settings.setValue("/QField/showMapCanvasGuide", false);
    }
  }

  QfGuide {
    id: dashboardTour
    baseRoot: mainWindow
    objectName: 'dashboardTour'
    z: dashBoard.z + 1
    index: -1

    steps: [
      {
        "type": "information",
        "title": qsTr("Digitizing toggle"),
        "description": qsTr("Switch between browse and digitize modes. Browse mode focuses on delivering the best experience viewing the map and its features, while digitize mode enables you to create features and edit geometries."),
        "target": () => [iface.findItemByObjectName('modeSwitch')]
      },
      {
        "type": "information",
        "title": qsTr("Legend"),
        "description": qsTr("The legend shows map layers and allows you to toggle visibility and opacity properties by <b>long-pressing on a layer to open a properties popup</b>. The popup offers additional functionalities such as zooming to layer extent and displaying features contained within vector layers."),
        "target": () => [iface.findItemByObjectName('legend')]
      },
      {
        "type": "information",
        "title": qsTr("Measurement"),
        "description": qsTr("Toggle the measurement tool to calculate distances and areas on the map."),
        "target": () => [iface.findItemByObjectName('measurementButton')]
      },
      {
        "type": "information",
        "title": qsTr("Print"),
        "description": qsTr("Export the map canvas to PDF using configured project print and atlas layouts."),
        "target": () => [iface.findItemByObjectName('printItemButton')]
      },
      {
        "type": "information",
        "title": qsTr("QFieldCloud"),
        "description": qsTr("Upload local changes, synchronize or discard changes to and from QFieldCloud when a cloud project is opened."),
        "target": () => [iface.findItemByObjectName('cloudButton')]
      },
      {
        "type": "information",
        "title": qsTr("Project folder"),
        "description": qsTr("Open the project folder to access project files, data sources, and related documents. Useful for managing project resources, manually uploading data to QFieldCloud, and sharing datasets, attachments, and layouts."),
        "target": () => [iface.findItemByObjectName('projectFolderButton')]
      }
    ]
  }

  QfGuide {
    id: cloudButtonTour
    baseRoot: mainWindow
    objectName: 'cloudButtonTour'
    z: dashBoard.z + 1
    index: -1

    steps: [
      {
        "type": "information",
        "title": qsTr("Local changes"),
        "description": qsTr("This project has pending changes which have not been uploaded yet. Tap the blue cloud button to open the cloud project panel and send them to QFieldCloud."),
        "target": () => [iface.findItemByObjectName('cloudButton')]
      }
    ]
  }

  QfGuide {
    id: cloudMenuTour
    baseRoot: mainWindow
    objectName: 'cloudMenuTour'

    steps: [
      {
        "type": "information",
        "title": qsTr("Local changes"),
        "description": qsTr("This cloud project has pending changes which have not been uploaded yet. Open the dashboard using this button, then tap the blue cloud icon to send them to QFieldCloud."),
        "target": () => [menuButton]
      }
    ]
  }

  Item {
    objectName: 'toursController'

    function blockGuides() {
      mapCanvasTour.blockGuide();
      cloudButtonTour.blockGuide();
      cloudMenuTour.blockGuide();
      settings.setValue("/QField/showMapCanvasGuide", false);
      settings.setValue("/QField/showDashBoardGuide", false);
      settings.setValue("/QField/showCloudButtonGuide", false);
    }
  }

  Rectangle {
    anchors.top: parent.top
    anchors.left: parent.left

    width: 14
    height: 14
    color: "transparent"

    MouseArea {
      enabled: mainWindow.sceneBorderless
      anchors.fill: parent
      cursorShape: enabled ? Qt.DragMoveCursor : Qt.ArrowCursor
      onPressed: mouse => {
        mainWindow.startSystemMove();
      }
    }
  }

  Rectangle {
    anchors.bottom: parent.bottom
    anchors.right: parent.right

    width: 14
    height: 14
    color: "transparent"

    MouseArea {
      enabled: mainWindow.sceneBorderless
      anchors.fill: parent
      cursorShape: enabled ? Qt.SizeFDiagCursor : Qt.ArrowCursor
      onPressed: mouse => {
        mainWindow.startSystemResize(Qt.RightEdge | Qt.BottomEdge);
      }
    }
  }
}
```


