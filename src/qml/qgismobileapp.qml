/***************************************************************************
                            qgismobileapp.qml
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
import org.qfield
import Theme

/**
 * \defgroup qml
 * \brief QField QML items
 */

/**
 * \ingroup qml
 */
ApplicationWindow {
  id: mainWindow
  objectName: 'mainWindow'
  visible: true
  flags: Qt.Window | Qt.WindowTitleHint | Qt.WindowSystemMenuHint | (sceneBorderless ? Qt.FramelessWindowHint : 0) | (Qt.platform.os === "ios" || Qt.platform.os === "android" ? Qt.MaximizeUsingFullscreenGeometryHint : 0) | (Qt.platform.os !== "ios" && Qt.platform.os !== "android" ? Qt.WindowMinMaxButtonsHint | Qt.WindowCloseButtonHint : 0)

  Material.theme: Theme.darkTheme ? "Dark" : "Light"
  Material.accent: Theme.mainColor

  property bool sceneLoaded: false
  property bool sceneBorderless: false
  property double sceneTopMargin: platformUtilities.sceneMargins(mainWindow)["top"]
  property double sceneBottomMargin: platformUtilities.sceneMargins(mainWindow)["bottom"]

  onSceneLoadedChanged: {
    // This requires the scene to be fully loaded not to crash due to possibility of
    // a thread blocking permission request being thrown
    if (positioningSettings.positioningActivated) {
      positionSource.active = true;
    }
  }

  Timer {
    id: refreshSceneMargins
    running: false
    repeat: false
    interval: 50

    readonly property bool screenIsPortrait: (Screen.primaryOrientation === Qt.PortraitOrientation || Screen.primaryOrientation === Qt.InvertedPortraitOrientation)
    onScreenIsPortraitChanged: {
      start();
    }

    onTriggered: {
      mainWindow.sceneTopMargin = platformUtilities.sceneMargins(mainWindow)["top"];
      mainWindow.sceneBottomMargin = platformUtilities.sceneMargins(mainWindow)["bottom"];
    }
  }

  Settings {
    property alias x: mainWindow.x
    property alias y: mainWindow.y
    property alias width: mainWindow.width
    property alias height: mainWindow.height

    property int minimumSize: Qt.platform.os !== "ios" && Qt.platform.os !== "android" ? 300 : 50
    property string screenConfiguration: ''

    Component.onCompleted: {
      if (Qt.platform.os !== "ios" && Qt.platform.os !== "android") {
        let currentScreensConfiguration = `${Qt.application.screens.length}`;
        for (let screen of Qt.application.screens) {
          currentScreensConfiguration += `:${screen.width}x${screen.height}-${screen.virtualX}-${screen.virtualY}`;
        }
        if (currentScreensConfiguration != screenConfiguration) {
          screenConfiguration = currentScreensConfiguration;
          width = Math.max(width, minimumSize);
          height = Math.max(height, minimumSize);
          x = Math.min(x, mainWindow.screen.width - width);
          y = Math.min(y, mainWindow.screen.height - height);
        }
      }
    }
  }

  LocatorModelSuperBridge {
    id: locatorBridge
    objectName: "locatorBridge"

    activeLayer: dashBoard.activeLayer
    bookmarks: bookmarkModel
    featureListController: featureForm.extentController
    mapSettings: mapCanvas.mapSettings
    navigation: navigation
    geometryHighlighter: geometryHighlighter.geometryWrapper
    keepScale: qfieldSettings.locatorKeepScale

    onMessageEmitted: {
      displayToast(text);
    }
  }

  FocusStack {
    id: focusstack
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
          if (featureForm.visible) {
            featureForm.hide();
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
      welcomeScreen.openLocalDataPicker();
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
  property Rubberband currentRubberband
  property LayerObserver layerObserverAlias: layerObserver
  property QgsGpkgFlusher gpkgFlusherAlias: gpkgFlusher

  signal closeMeasureTool
  signal changeMode(string mode)
  signal toggleDigitizeMode

  Item {
    id: stateMachine

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
          target: featureForm
          state: "Hidden"
        }
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
    if (stateMachine.state === mode)
      return;
    stateMachine.lastState = stateMachine.state;
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
    }
  }

  onCloseMeasureTool: {
    overlayFeatureFormDrawer.close();
    changeMode(stateMachine.lastState);
  }

  /**
   * The position source to access GNSS devices
   */
  Positioning {
    id: positionSource
    objectName: "positionSource"

    deviceId: positioningSettings.positioningDevice

    property bool currentness: false
    property alias destinationCrs: positionSource.coordinateTransformer.destinationCrs
    property real bearingTrueNorth: 0.0

    coordinateTransformer: CoordinateTransformer {
      destinationCrs: mapCanvas.mapSettings.destinationCrs
      transformContext: qgisProject ? qgisProject.transformContext : CoordinateReferenceSystemUtils.emptyTransformContext()
      deltaZ: 0
      skipAltitudeTransformation: positioningSettings.skipAltitudeCorrection
      verticalGrid: positioningSettings.verticalGrid
    }

    elevationCorrectionMode: positioningSettings.elevationCorrectionMode
    antennaHeight: positioningSettings.antennaHeightActivated ? positioningSettings.antennaHeight : 0
    logging: positioningSettings.logging

    onPositionInformationChanged: {
      if (active) {
        bearingTrueNorth = PositioningUtils.bearingTrueNorth(positionSource.projectedPosition, mapCanvas.mapSettings.destinationCrs);
        if (gnssButton.followActive) {
          gnssButton.followLocation(false);
        }
      }
    }

    onOrientationChanged: {
      if (active && gnssButton.followOrientationActive) {
        gnssButton.followOrientation();
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

  PositioningSettings {
    id: positioningSettings
    objectName: "positioningSettings"

    onPositioningActivatedChanged: {
      if (mainWindow.sceneLoaded) {
        if (positioningActivated) {
          displayToast(qsTr("Activating positioning service"));
          positionSource.active = true;
        } else {
          positionSource.active = false;
        }
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
    }
  }

  Item {
    id: mapCanvas
    objectName: "mapCanvas"
    clip: true

    DragHandler {
      id: freehandHandler
      property bool isDigitizing: false
      enabled: freehandButton.visible && freehandButton.freehandDigitizing && !digitizingToolbar.rubberbandModel.frozen && (!featureForm.visible || digitizingToolbar.geometryRequested)
      acceptedDevices: !qfieldSettings.mouseAsTouchScreen ? PointerDevice.Stylus | PointerDevice.Mouse : PointerDevice.Stylus
      grabPermissions: PointerHandler.CanTakeOverFromHandlersOfSameType | PointerHandler.CanTakeOverFromHandlersOfDifferentType | PointerHandler.ApprovesTakeOverByAnything

      onActiveChanged: {
        if (active) {
          geometryEditorsToolbar.canvasFreehandBegin();
        } else {
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
            if (!geometryEditorsToolbar.canvasClicked(centroid.position)) {
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
          screenCenterX = mapCanvas.mapSettings.coordinateToScreen(featureForm.extentController.getCentroidFromSelected()).x;
          screenCenterY = mapCanvas.mapSettings.coordinateToScreen(featureForm.extentController.getCentroidFromSelected()).y;
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
      enabled: !(positionSource.active && positioningSettings.positioningCoordinateLock) && (!digitizingToolbar.rubberbandModel || !digitizingToolbar.rubberbandModel.frozen)
      acceptedDevices: !qfieldSettings.mouseAsTouchScreen ? PointerDevice.Stylus | PointerDevice.Mouse : PointerDevice.Stylus
      grabPermissions: PointerHandler.TakeOverForbidden

      property bool hasBeenHovered: false
      property bool skipHover: false

      function pointInItem(point, item) {
        var itemCoordinates = item.mapToItem(mainWindow.contentItem, 0, 0);
        return point.position.x >= itemCoordinates.x && point.position.x <= itemCoordinates.x + item.width && point.position.y >= itemCoordinates.y && point.position.y <= itemCoordinates.y + item.height;
      }

      onPointChanged: {
        if (skipHover || !mapCanvasMap.hovered) {
          return;
        }

        // when hovering various toolbars, reset coordinate locator position for nicer UX
        if (!freehandHandler.active && (pointInItem(point, digitizingToolbar) || pointInItem(point, elevationProfileButton))) {
          coordinateLocator.sourceLocation = mapCanvas.mapSettings.coordinateToScreen(digitizingToolbar.rubberbandModel.lastCoordinate);
        } else if (!freehandHandler.active && pointInItem(point, geometryEditorsToolbar)) {
          coordinateLocator.sourceLocation = mapCanvas.mapSettings.coordinateToScreen(geometryEditorsToolbar.editorRubberbandModel.lastCoordinate);
        } else if (!freehandHandler.active) {
          // after a click, it seems that the position is sent once at 0,0 => weird)
          if (point.position !== Qt.point(0, 0)) {
            coordinateLocator.sourceLocation = point.position;
          }
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
     * the stylus binding to the CoordinateLocator.
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

    /* The map canvas */
    MapCanvas {
      id: mapCanvasMap

      property bool isEnabled: !dashBoard.opened && !aboutDialog.visible && !welcomeScreen.visible && !qfieldSettings.visible && !qfieldLocalDataPickerScreen.visible && !qfieldCloudScreen.visible && !qfieldCloudPopup.visible && !codeReader.visible && !sketcher.visible && !overlayFeatureFormDrawer.visible && !rotateFeaturesToolbar.rotateFeaturesRequested
      interactive: isEnabled && !screenLocker.enabled && !snapToCommonAngleMenu.visible
      isMapRotationEnabled: qfieldSettings.enableMapRotation
      incrementalRendering: true
      quality: qfieldSettings.quality
      previewJobsEnabled: qfieldSettings.previewJobsEnabled
      forceDeferredLayersRepaint: trackings.count > 0
      freehandDigitizing: freehandButton.freehandDigitizing && freehandHandler.active

      rightMargin: featureForm.x > 0 ? featureForm.width : 0
      bottomMargin: informationDrawer.height > mainWindow.sceneBottomMargin ? informationDrawer.height : 0

      anchors.fill: parent

      function pointInItem(point, item) {
        var itemCoordinates = item.mapToItem(mainWindow.contentItem, 0, 0);
        return point.x >= itemCoordinates.x && point.x <= itemCoordinates.x + item.width && point.y >= itemCoordinates.y && point.y <= itemCoordinates.y + item.height;
      }

      onClicked: (point, type) => {
        if (type === "stylus" && (overlayFeatureFormDrawer.opened || (featureForm.visible && pointInItem(point, featureForm)))) {
          return;
        }
        if (!digitizingToolbar.geometryRequested && featureForm.state == "FeatureFormEdit") {
          return;
        }
        if (locatorItem.state == "on") {
          locatorItem.state = "off";
          return;
        }
        if (type === "stylus") {
          if (pointInItem(point, digitizingToolbar) || pointInItem(point, zoomToolbar) || pointInItem(point, mainToolbar) || pointInItem(point, mainMenuBar) || pointInItem(point, geometryEditorsToolbar) || pointInItem(point, locationToolbar) || pointInItem(point, digitizingToolbarContainer) || pointInItem(point, locatorItem)) {
            return;
          }

          // Check if geometry editor is taking over
          const positionLocked = positionSource.active && positioningSettings.positioningCoordinateLock;
          if (geometryEditorsToolbar.stateVisible) {
            if (!positionLocked) {
              geometryEditorsToolbar.canvasClicked(point, type);
            }
            return;
          }
          if ((stateMachine.state === "digitize" && digitizingFeature.currentLayer) || stateMachine.state === "measure") {
            if (!positionLocked && (!featureForm.visible || digitizingToolbar.geometryRequested)) {
              if (Number(currentRubberband.model.geometryType) === Qgis.GeometryType.Point || Number(currentRubberband.model.geometryType) === Qgis.GeometryType.Null) {
                digitizingToolbar.confirm();
              } else {
                digitizingToolbar.addVertex();
              }
            }
          } else {
            if (!featureForm.canvasOperationRequested && !overlayFeatureFormDrawer.visible && featureForm.state !== "FeatureFormEdit") {
              identifyTool.isMenuRequest = false;
              identifyTool.identify(point);
            }
          }
        }
      }

      onConfirmedClicked: point => {
        // Check if geometry editor is taking over
        const positionLocked = positionSource.active && positioningSettings.positioningCoordinateLock;
        if (geometryEditorsToolbar.stateVisible) {
          if (!positionLocked) {
            geometryEditorsToolbar.canvasClicked(point, '');
          }
          return;
        }
        if (qfieldSettings.fingerTapDigitizing && ((stateMachine.state === "digitize" && digitizingFeature.currentLayer) || stateMachine.state === "measure")) {
          if (!positionLocked && (!featureForm.visible || digitizingToolbar.geometryRequested)) {
            coordinateLocator.sourceLocation = point;
          }
        } else if (!featureForm.canvasOperationRequested && !overlayFeatureFormDrawer.visible && featureForm.state !== "FeatureFormEdit") {
          identifyTool.isMenuRequest = false;
          identifyTool.identify(point);
        }
      }

      onLongPressed: (point, type) => {
        if (type === "stylus") {
          if (overlayFeatureFormDrawer.opened || (featureForm.visible && pointInItem(point, featureForm))) {
            return;
          }

          // Check if geometry editor is taking over
          if (geometryEditorsToolbar.canvasLongPressed(point, type)) {
            return;
          }
          if (stateMachine.state === "digitize" && dashBoard.activeLayer) {
            // the sourceLocation test checks if a (stylus) hover is active
            if ((Number(currentRubberband.model.geometryType) === Qgis.GeometryType.Line && currentRubberband.model.vertexCount >= 2) || (Number(currentRubberband.model.geometryType) === Qgis.GeometryType.Polygon && currentRubberband.model.vertexCount >= 2)) {
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

      onAboutToWheelZoom: {
        if (gnssButton.followActive)
          gnssButton.followActiveSkipExtentChanged = true;
      }

      GridRenderer {
        id: gridDecoration
        mapSettings: mapCanvas.mapSettings
      }
    }

    /**************************************************
   * Overlays, including:
   * - Coordinate Locator
   * - Location Marker
   * - Identify Highlight
   * - Digitizing Rubberband
   **************************************************/

    /** The identify tool **/
    IdentifyTool {
      id: identifyTool

      property bool isMenuRequest: false

      mapSettings: mapCanvas.mapSettings
      model: isMenuRequest ? canvasMenuFeatureListModel : featureForm.model
      searchRadiusMm: 3
    }

    /** A rubberband for measuring **/
    MeasuringTool {
      id: measuringTool
      visible: stateMachine.state === 'measure'
      anchors.fill: parent

      measuringRubberband.model.currentCoordinate: coordinateLocator.currentCoordinate
      measuringRubberband.mapSettings: mapCanvas.mapSettings
    }

    /** Tracking sessions **/
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

      TrackingSession {
      }
    }

    /** A rubberband for ditizing **/
    Rubberband {
      id: digitizingRubberband

      mapSettings: mapCanvas.mapSettings

      model: RubberbandModel {
        frozen: false
        currentCoordinate: coordinateLocator.currentCoordinate
        measureValue: {
          if (coordinateLocator.positionLocked) {
            switch (positioningSettings.digitizingMeasureType) {
            case Tracker.Timestamp:
              return coordinateLocator.positionInformation.utcDateTime.getTime();
            case Tracker.GroundSpeed:
              return coordinateLocator.positionInformation.speed;
            case Tracker.Bearing:
              return coordinateLocator.positionInformation.direction;
            case Tracker.HorizontalAccuracy:
              return coordinateLocator.positionInformation.hacc;
            case Tracker.VerticalAccuracy:
              return coordinateLocator.positionInformation.vacc;
            case Tracker.PDOP:
              return coordinateLocator.positionInformation.pdop;
            case Tracker.HDOP:
              return coordinateLocator.positionInformation.hdop;
            case Tracker.VDOP:
              return coordinateLocator.positionInformation.vdop;
            }
          } else {
            return Number.NaN;
          }
        }
        vectorLayer: digitizingToolbar.geometryRequested ? digitizingToolbar.geometryRequestedLayer : dashBoard.activeLayer
        crs: mapCanvas.mapSettings.destinationCrs
      }

      visible: stateMachine.state === "digitize"
    }

    GeometryRenderer {
      id: geometryEditorRenderer
    }

    /** A rubberband for the different geometry editors **/
    Rubberband {
      id: geometryEditorsRubberband
      color: '#96000000'

      mapSettings: mapCanvas.mapSettings

      model: RubberbandModel {
        frozen: false
        currentCoordinate: coordinateLocator.currentCoordinate
        crs: mapCanvas.mapSettings.destinationCrs
        geometryType: Qgis.GeometryType.Line
      }
    }

    BookmarkHighlight {
      id: bookmarkHighlight
      mapSettings: mapCanvas.mapSettings
    }

    Navigation {
      id: navigation
      objectName: "navigation"

      mapSettings: mapCanvas.mapSettings
      location: positionSource.active ? positionSource.projectedPosition : GeometryUtils.emptyPoint()

      proximityAlarm: positioningSettings.preciseViewProximityAlarm && informationDrawer.positioningPreciseView.visible && informationDrawer.positioningPreciseView.hasAcceptableAccuracy && !informationDrawer.positioningPreciseView.hasAlarmSnoozed
      proximityAlarmThreshold: positioningSettings.preciseViewPrecision
    }

    NavigationHighlight {
      id: navigationHighlight
      navigation: navigation
    }

    LinePolygon {
      id: elevationProfileHighlight

      visible: informationDrawer.elevationProfile.visible
      mapSettings: mapCanvas.mapSettings
      geometry: QgsGeometryWrapper {
        qgsGeometry: informationDrawer.elevationProfile.profileCurve
        crs: informationDrawer.elevationProfile.crs
      }
      color: "#FFFFFF"
      lineWidth: 4
    }

    /** A coordinate locator for digitizing **/
    CoordinateLocator {
      id: coordinateLocator
      anchors.fill: parent
      anchors.bottomMargin: informationDrawer.height > mainWindow.sceneBottomMargin ? informationDrawer.height : 0
      visible: stateMachine.state === "digitize" || stateMachine.state === 'measure'
      highlightColor: digitizingToolbar.isDigitizing ? currentRubberband.color : "#CFD8DC"
      mapSettings: mapCanvas.mapSettings
      currentLayer: dashBoard.activeLayer
      positionInformation: positionSource.positionInformation
      positionLocked: positionSource.active && positioningSettings.positioningCoordinateLock
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
    LocationMarker {
      id: locationMarker
      visible: positionSource.active && positionSource.positionInformation && positionSource.positionInformation.latitudeValid

      mapSettings: mapCanvas.mapSettings

      location: positionSource.projectedPosition
      accuracy: positionSource.projectedHorizontalAccuracy
      direction: positionSource.positionInformation && positionSource.positionInformation.directionValid ? positionSource.positionInformation.direction : -1
      speed: positionSource.positionInformation && positionSource.positionInformation.speedValid ? positionSource.positionInformation.speed : -1
      orientation: !isNaN(positionSource.orientation) ? positionSource.orientation + positionSource.bearingTrueNorth < 0 ? 360 + positionSource.orientation + positionSource.bearingTrueNorth : positionSource.orientation + positionSource.bearingTrueNorth : -1
    }

    /* Rubberband for vertices  */
    Item {
      // highlighting geometry (point, line, surface)
      Rubberband {
        id: editingRubberband
        vertexModel: geometryEditingVertexModel
        mapSettings: mapCanvas.mapSettings
      }

      // highlighting vertices
      VertexRubberband {
        id: vertexRubberband
        model: geometryEditingVertexModel
        mapSettings: mapCanvas.mapSettings
      }
    }

    /* Highlight the currently selected item on the feature list */
    FeatureListSelectionHighlight {
      id: featureListHighlight
      visible: !moveFeaturesToolbar.moveFeaturesRequested && !rotateFeaturesToolbar.rotateFeaturesRequested

      selectionModel: featureForm.selection
      mapSettings: mapCanvas.mapSettings

      color: "yellow"
      focusedColor: "#ff7777"
      selectedColor: Theme.mainColor
      width: 5
    }

    /* Highlight the currently selected item being moved or rotate */
    FeatureListSelectionHighlight {
      id: moveAndRotateFeaturesHighlight
      visible: moveFeaturesToolbar.moveFeaturesRequested || rotateFeaturesToolbar.rotateFeaturesRequested
      showSelectedOnly: true

      selectionModel: featureForm.selection
      mapSettings: mapCanvas.mapSettings

      // take rotation into account
      property double rotationRadians: -mapSettings.rotation * Math.PI / 180
      translateX: mapToScreenTranslateX.screenDistance * Math.cos(rotationRadians) - mapToScreenTranslateY.screenDistance * Math.sin(rotationRadians)
      translateY: mapToScreenTranslateY.screenDistance * Math.cos(rotationRadians) + mapToScreenTranslateX.screenDistance * Math.sin(rotationRadians)
      rotationDegrees: 0

      color: "yellow"
      focusedColor: "#ff7777"
      selectedColor: Theme.mainColor
      width: 5
    }

    /* Highlight features identified by locator or relation editor widgets */
    GeometryHighlighter {
      id: geometryHighlighter
      objectName: "geometryHighlighter"
    }

    MapToScreen {
      id: mapToScreenTranslateX
      mapSettings: mapCanvas.mapSettings
      mapDistance: moveFeaturesToolbar.moveFeaturesRequested ? mapCanvas.mapSettings.center.x - moveFeaturesToolbar.startPoint.x : 0
    }
    MapToScreen {
      id: mapToScreenTranslateY
      mapSettings: mapCanvas.mapSettings
      mapDistance: moveFeaturesToolbar.moveFeaturesRequested ? mapCanvas.mapSettings.center.y - moveFeaturesToolbar.startPoint.y : 0
    }

    ProcessingAlgorithmPreview {
      id: processingAlgorithmPreview
      algorithm: featureForm.algorithm
      mapSettings: mapCanvas.mapSettings
    }
  }

  Geofencer {
    id: geofencer

    position: positionSource.projectedPosition
    positionCrs: mapCanvas.mapSettings.destinationCrs

    readonly property int longVibration: 1000
    readonly property int shortVibration: 500

    onIsWithinChanged: {
      if (behavior == Geofencer.AlertWhenInsideGeofencedArea && geofencer.isWithin) {
        platformUtilities.vibrate(longVibration);
        displayToast(qsTr("Position has trespassed into ‘%1’").arg(isWithinAreaName), 'error');
      } else if (behavior == Geofencer.AlertWhenOutsideGeofencedArea && !geofencer.isWithin) {
        platformUtilities.vibrate(longVibration);
        displayToast(qsTr("Position outside areas after leaving ‘%1’").arg(lastWithinAreaName), 'error');
      } else if (behavior == Geofencer.InformWhenEnteringLeavingGeofencedArea) {
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

    color: Theme.errorColor
  }

  InformationDrawer {
    id: informationDrawer

    navigation: navigation
    positionSource: positionSource
    positioningSettings: positioningSettings
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

    ExpressionEvaluator {
      id: decorationExpressionEvaluator
      mode: ExpressionEvaluator.ExpressionTemplateMode
      mapSettings: mapCanvas.mapSettings
      project: qgisProject
      positionInformation: positionSource.positionInformation
      cloudUserInformation: projectInfo.cloudUserInformation
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
      anchors.left: parent.left
      anchors.leftMargin: 56
      anchors.top: parent.top
      anchors.topMargin: mainWindow.sceneTopMargin + 4

      width: parent.width - anchors.leftMargin * 2
      height: 48
      radius: 4

      color: '#55000000'

      Text {
        id: titleDecoration

        property string decorationText: ''
        property bool isExpressionTemplate: decorationText.match('\[%.*%\]')
        property bool isExpressionPositioning: isExpressionTemplate && decorationText.match('\[%.*(@gnss_|@position_).*%\]')

        width: parent.width - 4
        height: parent.height
        leftPadding: 2
        rightPadding: 2

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        wrapMode: Text.WordWrap
        elide: Text.ElideRight

        font: Theme.strongFont
        fontSizeMode: Text.Fit

        text: ''
      }
    }

    Rectangle {
      id: copyrightDecorationBackground

      visible: copyrightDecoration.text != ''

      anchors.left: parent.left
      anchors.leftMargin: 56
      anchors.bottom: parent.bottom
      anchors.bottomMargin: 56

      width: parent.width - anchors.leftMargin * 2
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

        font: Theme.tinyFont

        text: ''
      }
    }

    ParametizedImage {
      id: imageDecoration

      visible: source != ''

      anchors.left: parent.left
      anchors.leftMargin: 56
      anchors.bottom: copyrightDecorationBackground.top
      anchors.bottomMargin: 4

      width: parent.width - anchors.leftMargin * 2
      height: 48

      source: ""
    }

    Text {
      id: coordinateLocatorInformationOverlay

      property bool coordinatesIsXY: CoordinateReferenceSystemUtils.defaultCoordinateOrderForCrsIsXY(projectInfo.coordinateDisplayCrs)
      property bool coordinatesIsGeographic: projectInfo.coordinateDisplayCrs.isGeographic

      DistanceArea {
        id: digitizingGeometryMeasure

        property VectorLayer currentLayer: dashBoard.activeLayer

        rubberbandModel: currentRubberband ? currentRubberband.model : null
        project: qgisProject
        crs: qgisProject ? qgisProject.crs : CoordinateReferenceSystemUtils.invalidCrs()
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
          var point = GeometryUtils.reprojectPoint(coordinateLocator.currentCoordinate, coordinateLocator.mapSettings.destinationCrs, projectInfo.coordinateDisplayCrs);
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

      font: Theme.strongTipFont
      style: Text.Outline
      styleColor: Theme.light
    }

    QfToolButton {
      id: compassArrow
      rotation: mapCanvas.mapSettings.rotation
      visible: rotation != 0
      anchors.left: parent.left
      anchors.bottom: parent.bottom
      anchors.leftMargin: 4
      anchors.bottomMargin: 54
      round: true
      bgcolor: Theme.toolButtonBackgroundSemiOpaqueColor

      Shape {
        width: compassArrow.width
        height: compassArrow.height

        ShapePath {
          strokeWidth: 3
          strokeColor: "transparent"
          strokeStyle: ShapePath.SolidLine
          fillColor: Theme.mainColor
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
          fillColor: Theme.toolButtonColor
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

      onClicked: mapCanvas.mapSettings.rotation = 0
    }

    ScaleBar {
      visible: qfieldSettings.showScaleBar
      mapSettings: mapCanvas.mapSettings
      anchors.left: parent.left
      anchors.bottom: parent.bottom
      anchors.leftMargin: 8
      anchors.bottomMargin: 10
    }

    Column {
      id: pluginsToolbar
      objectName: "pluginsToolbar"

      anchors.right: locatorItem.right
      anchors.top: locatorItem.top
      anchors.topMargin: 48 + 4
      spacing: 4
    }

    QfToolButton {
      id: alertIcon
      iconSource: Theme.getThemeVectorIcon("ic_alert_black_24dp")
      round: true
      bgcolor: "transparent"
      visible: !screenLocker.enabled && messageLog.unreadMessages
      anchors.right: pluginsToolbar.right
      anchors.top: pluginsToolbar.bottom
      anchors.topMargin: 4

      onClicked: messageLog.visible = true
    }

    Column {
      id: zoomToolbar
      anchors.right: parent.right
      anchors.rightMargin: 10
      anchors.bottom: parent.bottom
      anchors.bottomMargin: (parent.height - zoomToolbar.height / 2) / 2
      spacing: 8
      visible: !screenLocker.enabled && (locationToolbar.height + digitizingToolbarContainer.height) / (digitizingToolbarContainer.y) < 0.41

      QfToolButton {
        id: zoomInButton
        round: true
        anchors.right: parent.right

        bgcolor: Theme.toolButtonBackgroundColor
        iconSource: Theme.getThemeVectorIcon("ic_add_white_24dp")
        iconColor: Theme.toolButtonColor

        width: 36
        height: 36

        onClicked: {
          if (gnssButton.followActive)
            gnssButton.followActiveSkipExtentChanged = true;
          mapCanvasMap.zoomIn(Qt.point(mapCanvas.x + mapCanvas.width / 2, mapCanvas.y + mapCanvas.height / 2));
        }
      }
      QfToolButton {
        id: zoomOutButton
        round: true
        anchors.right: parent.right

        bgcolor: Theme.toolButtonBackgroundColor
        iconSource: Theme.getThemeVectorIcon("ic_remove_white_24dp")
        iconColor: Theme.toolButtonColor

        width: 36
        height: 36

        onClicked: {
          if (gnssButton.followActive)
            gnssButton.followActiveSkipExtentChanged = true;
          mapCanvasMap.zoomOut(Qt.point(mapCanvas.x + mapCanvas.width / 2, mapCanvas.y + mapCanvas.height / 2));
        }
      }
    }

    LocatorItem {
      id: locatorItem
      objectName: "locatorItem"

      locatorBridge: locatorBridge

      anchors.right: parent.right
      anchors.top: parent.top
      anchors.topMargin: mainWindow.sceneTopMargin + 4
      anchors.rightMargin: 4

      visible: !screenLocker.enabled && stateMachine.state !== 'measure'

      onStateChanged: {
        if (state == "off") {
          focus = false;
          if (featureForm.visible) {
            featureForm.focus = true;
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
      leftPadding: 4
      spacing: 4

      QfToolButton {
        id: menuButton
        round: true
        iconSource: Theme.getThemeVectorIcon("ic_menu_white_24dp")
        bgcolor: dashBoard.opened ? Theme.mainColor : Theme.darkGray

        onClicked: dashBoard.opened ? dashBoard.close() : dashBoard.open()

        onPressAndHold: {
          mainMenu.popup(menuButton.x, menuButton.y);
        }
      }

      QfActionButton {
        id: closeMeasureTool
        visible: stateMachine.state === 'measure'
        toolImage: Theme.getThemeVectorIcon("ic_measurement_black_24dp")
        toolText: qsTr('Close measure tool')

        onClicked: mainWindow.closeMeasureTool()
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
        toolImage: Theme.getThemeVectorIcon("ic_edit_geometry_white_24dp")
        toolText: qsTr('Cancel addition')

        onClicked: digitizingToolbar.cancel()
      }
    }

    Column {
      id: mainToolbar
      visible: !screenLocker.enabled
      anchors.left: mainMenuBar.left
      anchors.top: mainMenuBar.bottom
      anchors.leftMargin: 4
      anchors.topMargin: 4
      spacing: 4

      QfToolButtonDrawer {
        name: "digitizingDrawer"
        size: 48
        round: true
        bgcolor: Theme.toolButtonBackgroundColor
        iconSource: Theme.getThemeVectorIcon('ic_digitizing_settings_black_24dp')
        iconColor: Theme.toolButtonColor
        spacing: 4
        visible: stateMachine.state === "digitize" && dashBoard.activeLayer && dashBoard.activeLayer.isValid && (dashBoard.activeLayer.geometryType() === Qgis.GeometryType.Polygon || dashBoard.activeLayer.geometryType() === Qgis.GeometryType.Line || dashBoard.activeLayer.geometryType() === Qgis.GeometryType.Point)

        QfToolButton {
          id: snappingButton
          width: 40
          height: 40
          padding: 2
          round: true
          state: qgisProject && qgisProject.snappingConfig.enabled ? "On" : "Off"
          iconSource: Theme.getThemeVectorIcon("ic_snapping_white_24dp")
          iconColor: Theme.toolButtonColor
          bgcolor: Theme.toolButtonBackgroundSemiOpaqueColor

          states: [
            State {

              name: "Off"
              PropertyChanges {
                target: snappingButton
                iconColor: Theme.toolButtonColor
                bgcolor: Theme.toolButtonBackgroundSemiOpaqueColor
              }
            },
            State {
              name: "On"
              PropertyChanges {
                target: snappingButton
                iconColor: Theme.mainColor
                bgcolor: Theme.toolButtonBackgroundColor
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
          width: 40
          height: 40
          padding: 2
          round: true
          state: qgisProject && qgisProject.topologicalEditing ? "On" : "Off"
          iconSource: Theme.getThemeVectorIcon("ic_topology_white_24dp")
          iconColor: Theme.toolButtonColor
          bgcolor: Theme.toolButtonBackgroundSemiOpaqueColor

          states: [
            State {

              name: "Off"
              PropertyChanges {
                target: topologyButton
                iconColor: Theme.toolButtonColor
                bgcolor: Theme.toolButtonBackgroundSemiOpaqueColor
              }
            },
            State {
              name: "On"
              PropertyChanges {
                target: topologyButton
                iconColor: Theme.mainColor
                bgcolor: Theme.toolButtonBackgroundColor
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
          width: visible ? 40 : 0
          height: visible ? 40 : 0
          padding: 2
          round: true
          visible: hoverHandler.hasBeenHovered && !(positionSource.active && positioningSettings.positioningCoordinateLock) && stateMachine.state === "digitize" && ((digitizingToolbar.geometryRequested && digitizingToolbar.geometryRequestedLayer && digitizingToolbar.geometryRequestedLayer.isValid && (digitizingToolbar.geometryRequestedLayer.geometryType() === Qgis.GeometryType.Polygon || digitizingToolbar.geometryRequestedLayer.geometryType() === Qgis.GeometryType.Line)) || (!digitizingToolbar.geometryRequested && dashBoard.activeLayer && dashBoard.activeLayer.isValid && (dashBoard.activeLayer.geometryType() === Qgis.GeometryType.Polygon || dashBoard.activeLayer.geometryType() === Qgis.GeometryType.Line)))
          iconSource: Theme.getThemeVectorIcon("ic_freehand_white_24dp")
          iconColor: Theme.toolButtonColor
          bgcolor: Theme.toolButtonBackgroundSemiOpaqueColor

          property bool freehandDigitizing: false
          state: freehandDigitizing ? "On" : "Off"

          states: [
            State {
              name: "Off"
              PropertyChanges {
                target: freehandButton
                iconColor: Theme.toolButtonColor
                bgcolor: Theme.toolButtonBackgroundSemiOpaqueColor
              }
            },
            State {
              name: "On"
              PropertyChanges {
                target: freehandButton
                iconColor: Theme.mainColor
                bgcolor: Theme.toolButtonBackgroundColor
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

          width: visible ? 40 : 0
          height: visible ? 40 : 0
          round: true
          visible: dashBoard.activeLayer && (dashBoard.activeLayer.geometryType() === Qgis.GeometryType.Polygon || dashBoard.activeLayer.geometryType() === Qgis.GeometryType.Line)
          iconSource: Theme.getThemeVectorIcon("ic_common_angle_white_24dp")
          iconColor: Theme.toolButtonColor
          bgcolor: Theme.toolButtonBackgroundSemiOpaqueColor

          state: qfieldSettings.snapToCommonAngleIsEnabled ? "On" : "Off"

          states: [
            State {
              name: "Off"
              PropertyChanges {
                target: snapToCommonAngleButton
                iconColor: Theme.toolButtonColor
                bgcolor: Theme.toolButtonBackgroundSemiOpaqueColor
              }
            },
            State {
              name: "On"
              PropertyChanges {
                target: snapToCommonAngleButton
                iconColor: Theme.mainColor
                bgcolor: Theme.toolButtonBackgroundColor
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
            width: Theme.menuItemIconlessLeftPadding + Math.max(angles.count * 35, tolorences.count * 55) + 24

            MenuItem {
              text: qsTr("Relative angle")
              font: Theme.defaultFont
              height: 48
              leftPadding: Theme.menuItemCheckLeftPadding

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
              color: Theme.mainTextColor
              font: Theme.defaultFont
              leftPadding: Theme.menuItemIconlessLeftPadding
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
                leftMargin: Theme.menuItemIconlessLeftPadding
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
                color: Theme.mainColor
                radius: width / 2
              }

              delegate: Item {
                width: 35
                height: width
                enabled: !selected

                property bool selected: modelData === qfieldSettings.snapToCommonAngleDegrees

                Text {
                  text: qsTr("%1°").arg(modelData)
                  font: parent.selected ? Theme.strongTipFont : Theme.tipFont
                  anchors.centerIn: parent
                  color: Theme.mainTextColor
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
              color: Theme.mainTextColor
              font: Theme.defaultFont
              leftPadding: Theme.menuItemIconlessLeftPadding
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
                leftMargin: Theme.menuItemIconlessLeftPadding
                rightMargin: 4
              }
              spacing: 3
              orientation: ListView.Horizontal
              model: [qsTr("Narrow"), qsTr("Normal"), qsTr("Large")]
              highlight: Rectangle {
                width: 35
                height: parent.height
                color: Theme.mainColor
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
                  font: parent.selected ? Theme.strongTipFont : Theme.tipFont
                  anchors.centerIn: parent
                  color: Theme.mainTextColor
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
        iconSource: Theme.getThemeVectorIcon("ic_elevation_white_24dp")
        iconColor: Theme.toolButtonColor
        bgcolor: Theme.toolButtonBackgroundSemiOpaqueColor

        property bool elevationProfileActive: false
        state: elevationProfileActive ? "On" : "Off"

        states: [
          State {
            name: "Off"
            PropertyChanges {
              target: elevationProfileButton
              iconColor: Theme.toolButtonColor
              bgcolor: Theme.toolButtonBackgroundSemiOpaqueColor
            }
          },
          State {
            name: "On"
            PropertyChanges {
              target: elevationProfileButton
              iconColor: Theme.mainColor
              bgcolor: Theme.toolButtonBackgroundColor
            }
          }
        ]

        onClicked: {
          elevationProfileActive = !elevationProfileActive;

          // Draw an elevation profile if we have enough points to do so
          if (digitizingToolbar.rubberbandModel.vertexCount > 2) {
            // Clear the pre-existing profile to trigger a zoom to full updated profile curve
            informationDrawer.elevationProfile.clear();
            informationDrawer.elevationProfile.profileCurve = GeometryUtils.lineFromRubberband(digitizingToolbar.rubberbandModel, informationDrawer.elevationProfile.crs);
            informationDrawer.elevationProfile.refresh();
          }
          settings.setValue("/QField/Measuring/ElevationProfile", elevationProfileActive);
        }

        Component.onCompleted: {
          elevationProfileActive = settings.valueBool("/QField/Measuring/ElevationProfile", false);
        }
      }
    }

    BusyIndicator {
      id: busyIndicator
      anchors.left: mainMenuBar.left
      anchors.top: mainToolbar.bottom
      width: menuButton.width + 10
      height: width
      running: mapCanvasMap.isRendering
    }

    Column {
      id: locationToolbar
      anchors.right: parent.right
      anchors.rightMargin: 4
      anchors.bottom: digitizingToolbarContainer.top
      anchors.bottomMargin: 4

      spacing: 4

      QfToolButton {
        id: navigationButton
        visible: navigation.isActive
        round: true
        anchors.right: parent.right

        property bool isFollowLocationActive: positionSource.active && gnssButton.followActive && followIncludeDestination
        iconSource: Theme.getThemeVectorIcon("ic_navigation_flag_purple_24dp")
        iconColor: isFollowLocationActive ? Theme.toolButtonColor : Theme.navigationColor
        bgcolor: isFollowLocationActive ? Theme.navigationColor : Theme.toolButtonBackgroundColor

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
            mapCanvas.mapSettings.setCenter(navigation.destination);
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
        id: gnssLockButton
        anchors.right: parent.right
        state: positionSource.active && positioningSettings.positioningCoordinateLock ? "On" : "Off"
        visible: gnssButton.state === "On" && (stateMachine.state === "digitize" || stateMachine.state === 'measure')
        round: true
        checkable: true
        checked: positioningSettings.positioningCoordinateLock

        states: [
          State {
            name: "Off"
            PropertyChanges {
              target: gnssLockButton
              iconSource: Theme.getThemeVectorIcon("ic_location_locked_white_24dp")
              iconColor: Theme.toolButtonColor
              bgcolor: Theme.toolButtonBackgroundSemiOpaqueColor
            }
          },
          State {
            name: "On"
            PropertyChanges {
              target: gnssLockButton
              iconSource: Theme.getThemeVectorIcon("ic_location_locked_active_white_24dp")
              iconColor: Theme.positionColor
              bgcolor: Theme.toolButtonBackgroundColor
            }
          }
        ]

        onCheckedChanged: {
          if (gnssButton.state === "On") {
            if (checked) {
              if (freehandButton.freehandDigitizing) {
                // deactivate freehand digitizing when cursor locked is on
                freehandButton.clicked();
              }
              displayToast(qsTr("Coordinate cursor now locked to position"));
              if (positionSource.positionInformation.latitudeValid) {
                var screenLocation = mapCanvas.mapSettings.coordinateToScreen(locationMarker.location);
                if (screenLocation.x < 0 || screenLocation.x > mainWindow.width || screenLocation.y < 0 || screenLocation.y > mainWindow.height) {
                  mapCanvas.mapSettings.setCenter(positionSource.projectedPosition);
                }
              }
              positioningSettings.positioningCoordinateLock = true;
            } else {
              displayToast(qsTr("Coordinate cursor unlocked"));
              positioningSettings.positioningCoordinateLock = false;
              // deactivate any active averaged position collection
              positionSource.averagedPosition = false;
            }
          }
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
        / When set to true, the map will rotate to match the device's current magnetometer/compass orientatin.
        */
        property bool followOrientationActive: false
        /*
        / When set to true, map canvas rotation changes will not result in the
        / deactivation of the above followOrientationActive mode.
        */
        property bool followActiveSkipRotationChanged: false

        states: [
          State {
            name: "Off"
            PropertyChanges {
              target: gnssButton
              iconSource: Theme.getThemeVectorIcon("ic_location_disabled_white_24dp")
              iconColor: Theme.toolButtonColor
              bgcolor: Theme.toolButtonBackgroundSemiOpaqueColor
            }
          },
          State {
            name: "On"
            PropertyChanges {
              target: gnssButton
              iconSource: trackings.count > 0 ? Theme.getThemeVectorIcon("ic_location_tracking_white_24dp") : positionSource.positionInformation && positionSource.positionInformation.latitudeValid ? Theme.getThemeVectorIcon("ic_location_valid_white_24dp") : Theme.getThemeVectorIcon("ic_location_white_24dp")
              iconColor: followActive ? Theme.toolButtonColor : Theme.positionColor
              bgcolor: followActive ? Theme.positionColor : Theme.toolButtonBackgroundColor
            }
          }
        ]

        onClicked: {
          if (followActive) {
            followOrientationActive = true;
            followOrientation();
            displayToast(qsTr("Canvas follows location and compass orientation"));
          } else {
            followActive = true;
            if (positionSource.projectedPosition.x) {
              if (!positionSource.active) {
                positioningSettings.positioningActivated = true;
              } else {
                followLocation(true);
                displayToast(qsTr("Canvas follows location"));
              }
            } else {
              if (positionSource.valid) {
                if (positionSource.active) {
                  displayToast(qsTr("Waiting for location"));
                } else {
                  positioningSettings.positioningActivated = true;
                }
              }
            }
          }
        }

        onPressAndHold: {
          gnssMenu.popup(locationToolbar.x + locationToolbar.width - gnssMenu.width, locationToolbar.y + locationToolbar.height - gnssMenu.height);
        }

        property int followLocationMinScale: 125
        property int followLocationMinMargin: 40
        property int followLocationScreenFraction: settings ? settings.value("/QField/Positioning/FollowScreenFraction", 5) : 5

        function followLocation(forceRecenter) {
          var screenLocation = mapCanvas.mapSettings.coordinateToScreen(positionSource.projectedPosition);
          if (navigation.isActive && navigationButton.followIncludeDestination) {
            if (mapCanvas.mapSettings.scale > followLocationMinScale) {
              var screenDestination = mapCanvas.mapSettings.coordinateToScreen(navigation.destination);
              if (forceRecenter || screenDestination.x < followLocationMinMargin || screenDestination.x > (mainWindow.width - followLocationMinMargin) || screenDestination.y < followLocationMinMargin || screenDestination.y > (mainWindow.height - followLocationMinMargin) || screenLocation.x < followLocationMinMargin || screenLocation.x > (mainWindow.width - followLocationMinMargin) || screenLocation.y < followLocationMinMargin || screenLocation.y > (mainWindow.height - followLocationMinMargin) || (Math.abs(screenDestination.x - screenLocation.x) < mainWindow.width / 3 && Math.abs(screenDestination.y - screenLocation.y) < mainWindow.height / 3)) {
                gnssButton.followActiveSkipExtentChanged = true;
                var points = [positionSource.projectedPosition, navigation.destination];
                mapCanvas.mapSettings.setExtentFromPoints(points, followLocationMinScale, true);
              }
            }
          } else {
            var threshold = Math.min(mainWindow.width, mainWindow.height) / followLocationScreenFraction;
            if (forceRecenter || screenLocation.x < mapCanvas.x + threshold || screenLocation.x > mapCanvas.width - threshold || screenLocation.y < mapCanvas.y + threshold || screenLocation.y > mapCanvas.height - threshold) {
              gnssButton.followActiveSkipExtentChanged = true;
              mapCanvas.mapSettings.setCenter(positionSource.projectedPosition, true);
            }
          }
        }
        function followOrientation() {
          if (!isNaN(positionSource.orientation) && Math.abs(-positionSource.orientation - mapCanvas.mapSettings.rotation) >= 10) {
            gnssButton.followActiveSkipRotationChanged = true;
            mapCanvas.mapSettings.rotation = -positionSource.orientation;
          }
        }

        Rectangle {
          anchors {
            top: parent.top
            right: parent.right
            rightMargin: 2
            topMargin: 2
          }

          width: 12
          height: 12
          radius: width / 2

          border.width: 1.5
          border.color: "white"

          visible: positioningSettings.accuracyIndicator && gnssButton.state === "On"
          color: !positionSource.positionInformation || !positionSource.positionInformation.haccValid || positionSource.positionInformation.hacc > positioningSettings.accuracyBad ? Theme.accuracyBad : positionSource.positionInformation.hacc > positioningSettings.accuracyExcellent ? Theme.accuracyTolerated : Theme.accuracyExcellent
        }
      }

      Connections {
        target: mapCanvas.mapSettings

        function onExtentChanged() {
          if (gnssButton.followActive) {
            if (gnssButton.followActiveSkipExtentChanged) {
              gnssButton.followActiveSkipExtentChanged = false;
            } else {
              gnssButton.followActive = false;
              gnssButton.followOrientationActive = false;
              displayToast(qsTr("Canvas stopped following location"));
            }
          }
        }

        function onRotationChanged() {
          if (gnssButton.followOrientationActive) {
            if (gnssButton.followActiveSkipRotationChanged) {
              gnssButton.followActiveSkipRotationChanged = false;
            } else {
              gnssButton.followOrientationActive = false;
            }
          }
        }
      }
    }

    Column {
      id: digitizingToolbarContainer
      anchors.right: parent.right
      anchors.rightMargin: 4
      anchors.bottom: parent.bottom
      anchors.bottomMargin: 4

      spacing: 4

      DigitizingToolbar {
        id: digitizingToolbar

        stateVisible: !screenLocker.enabled && (!positioningSettings.geofencingPreventDigitizingDuringAlert || !geofencer.isAlerting) && ((stateMachine.state === "digitize" && dashBoard.activeLayer && !dashBoard.activeLayer.readOnly &&
            // unfortunately there is no way to call QVariant::toBool in QML so the value is a string
            dashBoard.activeLayer.customProperty('QFieldSync/is_geometry_locked') !== 'true' && !geometryEditorsToolbar.stateVisible && !moveFeaturesToolbar.stateVisible && !rotateFeaturesToolbar.stateVisible && (projectInfo.editRights || projectInfo.insertRights)) || stateMachine.state === 'measure' || (stateMachine.state === "digitize" && digitizingToolbar.geometryRequested))
        rubberbandModel: currentRubberband ? currentRubberband.model : null
        mapSettings: mapCanvas.mapSettings
        showConfirmButton: stateMachine.state === "digitize"
        screenHovering: mapCanvasMap.hovered

        digitizingLogger.type: stateMachine.state === 'measure' ? '' : 'add'

        FeatureModel {
          id: digitizingFeature
          project: qgisProject
          currentLayer: digitizingToolbar.geometryRequested ? digitizingToolbar.geometryRequestedLayer : dashBoard.activeLayer
          positionInformation: positionSource.positionInformation
          topSnappingResult: coordinateLocator.topSnappingResult
          positionLocked: positionSource.active && positioningSettings.positioningCoordinateLock
          cloudUserInformation: projectInfo.cloudUserInformation
          geometry: Geometry {
            id: digitizingGeometry
            rubberbandModel: digitizingRubberband.model
            vectorLayer: digitizingToolbar.geometryRequested ? digitizingToolbar.geometryRequestedLayer : dashBoard.activeLayer
          }
        }

        property string previousStateMachineState: ''
        onGeometryRequestedChanged: {
          if (geometryRequested) {
            digitizingRubberband.model.reset();
            previousStateMachineState = stateMachine.state;
            stateMachine.state = "digitize";
          } else {
            stateMachine.state = previousStateMachineState;
          }
        }

        onVertexCountChanged: {
          if (stateMachine.state === 'measure' && elevationProfileButton.elevationProfileActive) {
            if (rubberbandModel.vertexCount > 2) {
              // Clear the pre-existing profile to trigger a zoom to full updated profile curve
              informationDrawer.elevationProfile.clear();
              informationDrawer.elevationProfile.profileCurve = GeometryUtils.lineFromRubberband(rubberbandModel, informationDrawer.elevationProfile.crs);
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
                if (overlayFeatureFormDrawer.featureForm.model.constraintsHardValid) {
                  // when the constrainst are fulfilled
                  // indirect action, no need to check for success and display a toast, the log is enough
                  overlayFeatureFormDrawer.featureForm.featureCreated = overlayFeatureFormDrawer.featureForm.create();
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
              if (!overlayFeatureFormDrawer.featureModel.create()) {
                displayToast(qsTr("Failed to create feature!"), 'error');
              }
            } else {
              if (!overlayFeatureFormDrawer.featureModel.save()) {
                displayToast(qsTr("Failed to save feature!"), 'error');
              }
            }
            digitizingRubberband.model.reset();
            digitizingFeature.resetFeature();
          }
          coordinateLocator.sourceLocation = undefined;
        }
      }

      GeometryEditorsToolbar {
        id: geometryEditorsToolbar

        featureModel: geometryEditingFeature
        mapSettings: mapCanvas.mapSettings
        editorRubberbandModel: geometryEditorsRubberband.model
        editorRenderer: geometryEditorRenderer
        screenHovering: mapCanvasMap.hovered

        stateVisible: !screenLocker.enabled && (stateMachine.state === "digitize" && geometryEditingVertexModel.vertexCount > 0)
      }

      ConfirmationToolbar {
        id: moveFeaturesToolbar

        property bool moveFeaturesRequested: false
        property var startPoint: undefined // QgsPoint or undefined
        property var endPoint: undefined // QgsPoint or undefined
        signal moveConfirmed
        signal moveCanceled

        stateVisible: moveFeaturesRequested

        onConfirm: {
          endPoint = GeometryUtils.point(mapCanvas.mapSettings.center.x, mapCanvas.mapSettings.center.y);
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
          if (featureForm && featureForm.selection.model.selectedCount === 1) {
            featureForm.extentController.zoomToSelected();
          }
          startPoint = GeometryUtils.point(mapCanvas.mapSettings.center.x, mapCanvas.mapSettings.center.y);
          moveAndRotateFeaturesHighlight.rotationDegrees = 0;
          moveFeaturesRequested = true;
        }
      }

      ConfirmationToolbar {
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
          if (featureForm && featureForm.selection.model.selectedCount === 1) {
            featureForm.extentController.zoomToSelected();
          }
          moveAndRotateFeaturesHighlight.rotationDegrees = 0;
          rotateFeaturesRequested = true;
        }
      }
    }
  }

  LocatorSettings {
    id: locatorSettings
    locatorFiltersModel: locatorItem.locatorFiltersModel

    Component.onCompleted: focusstack.addFocusTaker(this)
  }

  PluginManagerSettings {
    id: pluginManagerSettings

    Component.onCompleted: focusstack.addFocusTaker(this)
  }

  DashBoard {
    id: dashBoard
    objectName: "dashBoard"

    allowActiveLayerChange: !digitizingToolbar.isDigitizing
    allowInteractive: !welcomeScreen.visible && !qfieldSettings.visible && !qfieldCloudScreen.visible && !qfieldLocalDataPickerScreen.visible && !codeReader.visible && !screenLocker.enabled
    mapSettings: mapCanvas.mapSettings

    Component.onCompleted: focusstack.addFocusTaker(this)

    onReturnHome: {
      if (currentRubberband && currentRubberband.model.vertexCount > 1) {
        digitizingToolbar.cancelDialog.open();
        shouldReturnHome = true;
      } else if (!shouldReturnHome) {
        openWelcomeScreen();
      }
    }

    onShowMainMenu: p => {
      mainMenu.popup(p.x - mainMenu.width - 2, p.y - 2);
    }

    onShowCloudPopup: {
      dashBoard.close();
      qfieldCloudPopup.show();
    }

    onToggleMeasurementTool: {
      if (featureForm.state === "ProcessingAlgorithmForm") {
        cancelAlgorithmDialog.visible = true;
      } else {
        activateMeasurementMode();
      }
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
          if (layerTree.data(index, FlatLayerTreeModel.Type) === 'layer' && layerTree.data(index, FlatLayerTreeModel.ReadOnly) === false && layerTree.data(index, FlatLayerTreeModel.GeometryLocked) === false) {
            firstEditableLayer = layerTree.data(index, FlatLayerTreeModel.VectorLayerPointer);
          }
        }
        if (activeLayer != null && activeLayer === layerTree.data(index, FlatLayerTreeModel.VectorLayerPointer)) {
          if (layerTree.data(index, FlatLayerTreeModel.ReadOnly) === true || layerTree.data(index, FlatLayerTreeModel.GeometryLocked) === true) {
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

  BookmarkProperties {
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

  Menu {
    id: mainMenu
    title: qsTr("Main Menu")

    topMargin: sceneTopMargin
    bottomMargin: sceneBottomMargin

    width: {
      let result = Math.max(50, undoRedoMetrics.width + undoButton.leftPadding * 2 + undoButton.rightPadding * 2 + 42 * 2);
      let padding = 0;
      // Skip first Row item
      for (let i = 1; i < count; ++i) {
        const item = itemAt(i);
        result = Math.max(item.contentItem.implicitWidth, result);
        padding = Math.max(item.leftPadding + item.rightPadding, padding);
      }
      return mainWindow.width > 0 ? Math.min(result + padding, mainWindow.width - 20) : result + padding;
    }

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
        icon.source: Theme.getThemeVectorIcon("ic_undo_black_24dp")
        leftPadding: Theme.menuItemLeftPadding

        onClicked: {
          if (enabled) {
            const msg = featureHistory.undoMessage();
            if (featureHistory.undo()) {
              displayToast(msg);
            }
            dashBoard.close();
            mainMenu.close();
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
        icon.source: Theme.getThemeVectorIcon("ic_redo_black_24dp")

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
            dashBoard.close();
            mainMenu.close();
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

      font: Theme.defaultFont
      icon.source: Theme.getThemeVectorIcon("ic_sensor_on_black_24dp")
      height: 48
      leftPadding: Theme.menuItemLeftPadding
      rightPadding: 40

      arrow: Canvas {
        x: parent.width - width
        y: (parent.height - height) / 2
        implicitWidth: 40
        implicitHeight: 40
        opacity: sensorListInstantiator.count > 0 ? 1 : 0
        onPaint: {
          var ctx = getContext("2d");
          ctx.strokeStyle = Theme.mainColor;
          ctx.lineWidth = 1;
          ctx.moveTo(15, 15);
          ctx.lineTo(width - 15, height / 2);
          ctx.lineTo(15, height - 15);
          ctx.stroke();
        }
      }

      onTriggered: {
        if (sensorListInstantiator.count > 0) {
          sensorMenu.popup(mainMenu.x, mainMenu.y + sensorItem.y);
        } else {
          mainMenu.close();
          toast.show(qsTr('No sensor available'), 'info', qsTr('Learn more'), function () {
              Qt.openUrlExternally('https://docs.qfield.org/how-to/sensors/');
            });
        }
        highlighted = false;
      }
    }

    MenuSeparator {
      width: parent.width
    }

    MenuItem {
      text: qsTr("Settings")

      font: Theme.defaultFont
      icon.source: Theme.getThemeVectorIcon("ic_tune_white_24dp")
      height: 48
      leftPadding: Theme.menuItemLeftPadding

      onTriggered: {
        dashBoard.close();
        qfieldSettings.reset();
        qfieldSettings.visible = true;
        highlighted = false;
      }
    }

    MenuItem {
      text: qsTr("Message Log")

      font: Theme.defaultFont
      height: 48
      icon.source: Theme.getThemeVectorIcon("ic_message_log_black_24dp")
      leftPadding: Theme.menuItemLeftPadding

      onTriggered: {
        dashBoard.close();
        messageLog.visible = true;
        highlighted = false;
      }
    }

    MenuItem {
      text: qsTr("Lock Screen")

      font: Theme.defaultFont
      icon.source: Theme.getThemeVectorIcon("ic_lock_black_24dp")
      height: 48
      leftPadding: Theme.menuItemLeftPadding

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
      text: qsTr("About QField")

      font: Theme.defaultFont
      icon.source: Theme.getThemeVectorIcon("ic_qfield_black_24dp")
      height: 48
      leftPadding: Theme.menuItemLeftPadding

      onTriggered: {
        dashBoard.close();
        aboutDialog.visible = true;
        highlighted = false;
      }
    }
  }

  Menu {
    id: sensorMenu

    property alias printTimer: timer
    property alias printName: timer.printName

    title: qsTr("Sensors")

    topMargin: sceneTopMargin
    bottomMargin: sceneBottomMargin

    width: {
      let result = 50;
      let padding = 0;
      for (let i = 0; i < count; ++i) {
        let item = itemAt(i);
        result = Math.max(item.contentItem.implicitWidth, result);
        padding = Math.max(item.leftPadding + item.rightPadding, padding);
      }
      return mainWindow.width > 0 ? Math.min(result + padding, mainWindow.width - 20) : result + padding;
    }

    MenuItem {
      text: qsTr('Select sensor below')

      font: Theme.defaultFont
      height: 48
      leftPadding: Theme.menuItemLeftPadding

      enabled: false
    }

    Instantiator {
      id: sensorListInstantiator

      model: SensorListModel {
        project: qgisProject

        onSensorErrorOccurred: errorString => {
          displayToast(qsTr('Sensor error: %1').arg(errorString), 'error');
        }
      }

      MenuItem {
        text: SensorName
        icon.source: SensorStatus == Qgis.DeviceConnectionStatus.Connected ? Theme.getThemeVectorIcon("ic_sensor_on_black_24dp") : Theme.getThemeVectorIcon("ic_sensor_off_black_24dp")

        font: Theme.defaultFont
        leftPadding: Theme.menuItemLeftPadding
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

  Menu {
    id: printMenu

    property alias printTimer: timer
    property alias printName: timer.printName

    title: qsTr("Print")

    topMargin: sceneTopMargin
    bottomMargin: sceneBottomMargin

    width: {
      let result = 50;
      let padding = 0;
      for (let i = 0; i < count; ++i) {
        let item = itemAt(i);
        result = Math.max(item.contentItem.implicitWidth, result);
        padding = Math.max(item.leftPadding + item.rightPadding, padding);
      }
      return mainWindow.width > 0 ? Math.min(result + padding, mainWindow.width - 20) : result + padding;
    }

    MenuItem {
      text: qsTr('Select layout below')

      font: Theme.defaultFont
      height: 48
      leftPadding: Theme.menuItemLeftPadding

      enabled: false
    }

    Instantiator {
      id: layoutListInstantiator

      model: PrintLayoutListModel {
        project: qgisProject
      }

      MenuItem {
        text: Title

        font: Theme.defaultFont
        leftPadding: Theme.menuItemLeftPadding
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

  Menu {
    id: canvasMenu
    objectName: "canvasMenu"

    title: qsTr("Map Canvas Options")
    font: Theme.defaultFont

    property var point
    onPointChanged: {
      var displayPoint = GeometryUtils.reprojectPoint(canvasMenu.point, mapCanvas.mapSettings.destinationCrs, projectInfo.coordinateDisplayCrs);
      var isXY = CoordinateReferenceSystemUtils.defaultCoordinateOrderForCrsIsXY(projectInfo.coordinateDisplayCrs);
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

    width: {
      const toolbarWidth = canvasMenuActionsToolbar.childrenRect.width + 4;
      let result = 0;
      let padding = 0;
      // Skip first Row item
      for (let i = 1; i < count; ++i) {
        const item = itemAt(i);
        result = Math.max(item.contentItem.implicitWidth, result);
        padding = Math.max(item.padding, padding);
      }
      return Math.min(Math.max(toolbarWidth, result + padding * 2), mainWindow.width - 20);
    }

    Row {
      id: canvasMenuActionsToolbar
      objectName: "canvasMenuActionsToolbar"
      leftPadding: 2
      rightPadding: 2
      spacing: 2
      height: children.length > 0 ? addBookmarkItem.height : 0
      clip: true

      property color hoveredColor: Qt.hsla(Theme.mainTextColor.hslHue, Theme.mainTextColor.hslSaturation, Theme.mainTextColor.hslLightness, 0.2)
    }

    MenuSeparator {
      width: parent.width
      height: canvasMenuActionsToolbar.children.length > 0 ? undefined : 0
    }

    MenuItem {
      id: cordinateItem
      text: ""
      height: 48
      leftPadding: Theme.menuItemLeftPadding
      font: Theme.defaultFont
      icon.source: Theme.getThemeVectorIcon("ic_copy_black_24dp")

      onTriggered: {
        const displayPoint = GeometryUtils.reprojectPoint(canvasMenu.point, mapCanvas.mapSettings.destinationCrs, projectInfo.coordinateDisplayCrs);
        platformUtilities.copyTextToClipboard(StringUtils.pointInformation(displayPoint, projectInfo.coordinateDisplayCrs));
        displayToast(qsTr('Coordinates copied to clipboard'));
      }
    }

    MenuSeparator {
      width: parent.width
    }

    MenuItem {
      id: addBookmarkItem
      text: qsTr("Add Bookmark")
      icon.source: Theme.getThemeVectorIcon("ic_bookmark_black_24dp")
      height: 48
      leftPadding: Theme.menuItemLeftPadding
      font: Theme.defaultFont

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
      icon.source: Theme.getThemeVectorIcon("ic_navigation_flag_purple_24dp")
      height: 48
      leftPadding: Theme.menuItemLeftPadding
      font: Theme.defaultFont

      onTriggered: {
        navigation.destination = canvasMenu.point;
      }
    }

    MenuItem {
      id: lockMapRotation
      text: qsTr("Enable Map Rotation")
      height: 48
      leftPadding: Theme.menuItemCheckLeftPadding
      font: Theme.defaultFont
      checkable: true
      checked: qfieldSettings.enableMapRotation
      indicator.height: 20
      indicator.width: 20
      indicator.implicitHeight: 24
      indicator.implicitWidth: 24

      onTriggered: qfieldSettings.enableMapRotation = checked
    }

    MenuSeparator {
      width: parent.width
    }

    MenuItem {
      text: qsTr('Lock Screen')

      font: Theme.defaultFont
      icon.source: Theme.getThemeVectorIcon("ic_lock_black_24dp")
      height: 48
      leftPadding: Theme.menuItemLeftPadding

      onTriggered: {
        screenLocker.enabled = true;
      }
    }

    MenuSeparator {
      enabled: canvasMenuFeatureListInstantiator.count > 0
      width: parent.width
      visible: enabled
      height: enabled ? undefined : 0
    }

    Instantiator {
      id: canvasMenuFeatureListInstantiator

      model: MultiFeatureListModel {
        id: canvasMenuFeatureListModel
      }

      Menu {
        id: featureMenu

        property int fid: featureId
        property var featureLayer: currentLayer

        topMargin: sceneTopMargin
        bottomMargin: sceneBottomMargin

        title: layerName + ': ' + featureName
        font: Theme.defaultFont

        width: {
          let result = 50;
          let padding = 0;
          for (let i = 0; i < count; ++i) {
            let item = itemAt(i);
            result = Math.max(item.contentItem.implicitWidth, result);
            padding = Math.max(item.leftPadding + item.rightPadding, padding);
          }
          return mainWindow.width > 0 ? Math.min(result + padding, mainWindow.width - 20) : result + padding;
        }

        Component.onCompleted: {
          if (featureMenu.icon !== undefined) {
            featureMenu.icon.source = Theme.getThemeVectorIcon('ic_info_white_24dp');
          }
        }

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
          font: Theme.defaultFont
          icon.source: Theme.getThemeVectorIcon("ic_baseline-list_white_24dp")
          leftPadding: Theme.menuItemLeftPadding
          height: 48

          onTriggered: {
            featureForm.model.setFeatures(menu.featureLayer, '@id = ' + menu.fid);
            featureForm.selection.focusedItem = 0;
            featureForm.state = "FeatureForm";
          }
        }

        MenuItem {
          text: qsTr('Copy Feature Attributes')
          font: Theme.defaultFont
          icon.source: Theme.getThemeVectorIcon("ic_copy_black_24dp")
          leftPadding: Theme.menuItemLeftPadding
          height: 48

          onTriggered: {
            clipboardManager.copyFeatureToClipboard(menu.featureLayer, menu.fid, true);
          }
        }

        MenuItem {
          text: qsTr('Duplicate Feature')
          font: Theme.defaultFont
          enabled: projectInfo.insertRights
          icon.source: Theme.getThemeVectorIcon("ic_duplicate_black_24dp")
          leftPadding: Theme.menuItemLeftPadding
          height: 48

          onTriggered: {
            featureForm.model.setFeatures(menu.featureLayer, '@id = ' + menu.fid);
            featureForm.selection.focusedItem = 0;
            featureForm.multiSelection = true;
            featureForm.selection.toggleSelectedItem(0);
            featureForm.state = "FeatureList";
            if (featureForm.model.canDuplicateSelection) {
              if (featureForm.selection.model.duplicateFeature(featureForm.selection.focusedLayer, featureForm.selection.focusedFeature)) {
                displayToast(qsTr('Successfully duplicated feature'));
                featureForm.selection.focusedItem = -1;
                moveFeaturesToolbar.initializeMoveFeatures();
                return;
              }
            }
            displayToast(qsTr('Feature duplication not available'));
          }
        }
      }

      onObjectAdded: (index, object) => {
        canvasMenu.insertMenu(index + 11, object);
      }
      onObjectRemoved: (index, object) => {
        canvasMenu.removeMenu(object);
      }
    }
  }

  Menu {
    id: navigationMenu
    title: qsTr("Navigation Options")
    font: Theme.defaultFont

    topMargin: sceneTopMargin
    bottomMargin: sceneBottomMargin

    width: {
      let result = 50;
      let padding = 0;
      for (let i = 0; i < count; ++i) {
        let item = itemAt(i);
        result = Math.max(item.contentItem.implicitWidth, result);
        padding = Math.max(item.leftPadding + item.rightPadding, padding);
      }
      return mainWindow.width > 0 ? Math.min(result + padding, mainWindow.width - 20) : result + padding;
    }

    MenuItem {
      id: preciseViewItem
      text: qsTr("Precise View Settings")

      font: Theme.defaultFont
      height: 48
      leftPadding: Theme.menuItemLeftPadding
      rightPadding: 40

      arrow: Canvas {
        x: parent.width - width
        y: (parent.height - height) / 2
        implicitWidth: 40
        implicitHeight: 40
        visible: true
        onPaint: {
          var ctx = getContext("2d");
          ctx.strokeStyle = Theme.mainColor;
          ctx.lineWidth = 1;
          ctx.moveTo(15, 15);
          ctx.lineTo(width - 15, height / 2);
          ctx.lineTo(15, height - 15);
          ctx.stroke();
        }
      }

      onTriggered: {
        preciseViewMenu.popup(navigationMenu.x, navigationMenu.y - preciseViewItem.y);
        highlighted = false;
      }
    }

    MenuSeparator {
      width: parent.width
    }

    MenuItem {
      id: cancelNavigationItem
      text: qsTr("Clear Destination")
      height: 48
      leftPadding: Theme.menuItemLeftPadding
      font: Theme.defaultFont

      onTriggered: {
        navigation.clear();
      }
    }
  }

  Menu {
    id: preciseViewMenu
    title: qsTr("Precise View Settings")
    font: Theme.defaultFont

    topMargin: sceneTopMargin
    bottomMargin: sceneBottomMargin

    width: {
      let result = 50;
      let padding = 0;
      for (let i = 0; i < count; ++i) {
        let item = itemAt(i);
        result = Math.max(item.contentItem.implicitWidth, result);
        padding = Math.max(item.leftPadding + item.rightPadding, padding);
      }
      return mainWindow.width > 0 ? Math.min(result + padding * 2, mainWindow.width - 20) : result + padding;
    }

    MenuItem {
      text: qsTr("%1 Precision").arg(UnitTypes.formatDistance(0.10, 2, projectInfo.distanceUnits))
      height: 48
      leftPadding: Theme.menuItemCheckLeftPadding
      font: Theme.defaultFont

      enabled: !checked
      checkable: true
      checked: positioningSettings.preciseViewPrecision == 0.10
      indicator.height: 20
      indicator.width: 20
      indicator.implicitHeight: 24
      indicator.implicitWidth: 24
      onCheckedChanged: if (checked)
        positioningSettings.preciseViewPrecision = 0.10
    }

    MenuItem {
      text: qsTr("%1 Precision").arg(UnitTypes.formatDistance(0.25, 2, projectInfo.distanceUnits))
      height: 48
      leftPadding: Theme.menuItemCheckLeftPadding
      font: Theme.defaultFont

      enabled: !checked
      checkable: true
      checked: positioningSettings.preciseViewPrecision == 0.25
      indicator.height: 20
      indicator.width: 20
      indicator.implicitHeight: 24
      indicator.implicitWidth: 24
      onCheckedChanged: if (checked)
        positioningSettings.preciseViewPrecision = 0.25
    }

    MenuItem {
      text: qsTr("%1 Precision").arg(UnitTypes.formatDistance(0.5, 2, projectInfo.distanceUnits))
      height: 48
      leftPadding: Theme.menuItemCheckLeftPadding
      font: Theme.defaultFont

      enabled: !checked
      checkable: true
      checked: positioningSettings.preciseViewPrecision == 0.5
      indicator.height: 20
      indicator.width: 20
      indicator.implicitHeight: 24
      indicator.implicitWidth: 24
      onCheckedChanged: if (checked)
        positioningSettings.preciseViewPrecision = 0.5
    }

    MenuItem {
      text: qsTr("%1 Precision").arg(UnitTypes.formatDistance(1, 2, projectInfo.distanceUnits))
      height: 48
      leftPadding: Theme.menuItemCheckLeftPadding
      font: Theme.defaultFont

      enabled: !checked
      checkable: true
      checked: positioningSettings.preciseViewPrecision == 1
      indicator.height: 20
      indicator.width: 20
      indicator.implicitHeight: 24
      indicator.implicitWidth: 24
      onCheckedChanged: if (checked)
        positioningSettings.preciseViewPrecision = 1
    }

    MenuItem {
      text: qsTr("%1 Precision").arg(UnitTypes.formatDistance(2.5, 2, projectInfo.distanceUnits))
      height: 48
      leftPadding: Theme.menuItemCheckLeftPadding
      font: Theme.defaultFont

      enabled: !checked
      checkable: true
      checked: positioningSettings.preciseViewPrecision == 2.5
      indicator.height: 20
      indicator.width: 20
      indicator.implicitHeight: 24
      indicator.implicitWidth: 24
      onCheckedChanged: if (checked)
        positioningSettings.preciseViewPrecision = 2.5
    }

    MenuItem {
      text: qsTr("%1 Precision").arg(UnitTypes.formatDistance(5, 2, projectInfo.distanceUnits))
      height: 48
      leftPadding: Theme.menuItemCheckLeftPadding
      font: Theme.defaultFont

      enabled: !checked
      checkable: true
      checked: positioningSettings.preciseViewPrecision == 5
      indicator.height: 20
      indicator.width: 20
      indicator.implicitHeight: 24
      indicator.implicitWidth: 24
      onCheckedChanged: if (checked)
        positioningSettings.preciseViewPrecision = 5
    }

    MenuItem {
      text: qsTr("%1 Precision").arg(UnitTypes.formatDistance(10, 2, projectInfo.distanceUnits))
      height: 48
      leftPadding: Theme.menuItemCheckLeftPadding
      font: Theme.defaultFont

      enabled: !checked
      checkable: true
      checked: positioningSettings.preciseViewPrecision == 10
      indicator.height: 20
      indicator.width: 20
      indicator.implicitHeight: 24
      indicator.implicitWidth: 24
      onCheckedChanged: if (checked)
        positioningSettings.preciseViewPrecision = 10
    }

    MenuSeparator {
      width: parent.width
    }

    MenuItem {
      text: qsTr("Always Show Precise View")
      height: 48
      leftPadding: Theme.menuItemCheckLeftPadding
      font: Theme.defaultFont

      checkable: true
      checked: positioningSettings.alwaysShowPreciseView
      indicator.height: 20
      indicator.width: 20
      indicator.implicitHeight: 24
      indicator.implicitWidth: 24
      onCheckedChanged: positioningSettings.alwaysShowPreciseView = checked
    }

    MenuItem {
      text: qsTr("Enable Audio Proximity Feedback")
      height: 48
      leftPadding: Theme.menuItemCheckLeftPadding
      font: Theme.defaultFont

      checkable: true
      checked: positioningSettings.preciseViewProximityAlarm
      indicator.height: 20
      indicator.width: 20
      indicator.implicitHeight: 24
      indicator.implicitWidth: 24
      onCheckedChanged: positioningSettings.preciseViewProximityAlarm = checked
    }
  }

  Menu {
    id: gnssMenu
    title: qsTr("Positioning Options")
    font: Theme.defaultFont

    topMargin: sceneTopMargin
    bottomMargin: sceneBottomMargin

    width: {
      let result = 50;
      let padding = 0;
      for (let i = 0; i < count; ++i) {
        let item = itemAt(i);
        result = Math.max(item.contentItem.implicitWidth, result);
        padding = Math.max(item.leftPadding + item.rightPadding, padding);
      }
      return mainWindow.width > 0 ? Math.min(result + padding, mainWindow.width - 20) : result + padding;
    }

    MenuItem {
      id: positioningDeviceName
      text: positioningSettings.positioningDeviceName
      height: 48
      font: Theme.defaultFont
      enabled: false
    }

    MenuSeparator {
      width: parent.width
    }

    MenuItem {
      id: positioningItem
      text: qsTr("Enable Positioning")
      height: 48
      leftPadding: Theme.menuItemCheckLeftPadding
      font: Theme.defaultFont

      checkable: true
      checked: positioningSettings.positioningActivated
      indicator.height: 20
      indicator.width: 20
      indicator.implicitHeight: 24
      indicator.implicitWidth: 24
      onCheckedChanged: positioningSettings.positioningActivated = checked
    }

    MenuItem {
      text: qsTr("Show Position Information")
      height: 48
      leftPadding: Theme.menuItemCheckLeftPadding
      font: Theme.defaultFont

      checkable: true
      checked: positioningSettings.showPositionInformation
      indicator.height: 20
      indicator.width: 20
      indicator.implicitHeight: 24
      indicator.implicitWidth: 24
      onCheckedChanged: positioningSettings.showPositionInformation = checked
    }

    MenuItem {
      text: qsTr("Positioning Settings")
      height: 48
      leftPadding: Theme.menuItemIconlessLeftPadding
      font: Theme.defaultFont

      onTriggered: {
        qfieldSettings.currentPanel = 1;
        qfieldSettings.visible = true;
      }
    }

    MenuSeparator {
      width: parent.width
    }

    MenuItem {
      text: qsTr("Center to Location")
      height: 48
      leftPadding: Theme.menuItemIconlessLeftPadding
      font: Theme.defaultFont

      onTriggered: {
        mapCanvas.mapSettings.setCenter(positionSource.projectedPosition, true);
      }
    }

    MenuItem {
      text: qsTr("Add Bookmark at Location")
      icon.source: Theme.getThemeVectorIcon("ic_bookmark_black_24dp")
      height: 48
      leftPadding: Theme.menuItemLeftPadding
      font: Theme.defaultFont

      onTriggered: {
        if (!positioningSettings.positioningActivated || positionSource.positionInformation === undefined || !positionSource.positionInformation.latitudeValid) {
          displayToast(qsTr('Current location unknown'));
          return;
        }
        var name = qsTr('My location') + ' (' + new Date().toLocaleString() + ')';
        var group = 'blue';
        var id = bookmarkModel.addBookmarkAtPoint(positionSource.projectedPosition, name, group);
        if (id !== '') {
          bookmarkProperties.bookmarkId = id;
          bookmarkProperties.bookmarkName = name;
          bookmarkProperties.bookmarkGroup = group;
          bookmarkProperties.open();
        }
      }
    }

    MenuItem {
      text: qsTr("Copy Location Coordinates")
      height: 48
      leftPadding: Theme.menuItemLeftPadding
      font: Theme.defaultFont
      icon.source: Theme.getThemeVectorIcon("ic_copy_black_24dp")

      onTriggered: {
        if (!positioningSettings.positioningActivated || positionSource.positionInformation === undefined || !positionSource.positionInformation.latitudeValid) {
          displayToast(qsTr('Current location unknown'));
          return;
        }
        var point = GeometryUtils.reprojectPoint(positionSource.sourcePosition, CoordinateReferenceSystemUtils.wgs84Crs(), projectInfo.coordinateDisplayCrs);
        var coordinates = StringUtils.pointInformation(point, projectInfo.coordinateDisplayCrs);
        coordinates += ' (' + qsTr('Accuracy') + ' ' + (positionSource.positionInformation && positionSource.positionInformation.haccValid ? positionSource.positionInformation.hacc.toLocaleString(Qt.locale(), 'f', 3) + " m" : qsTr("N/A")) + ')';
        platformUtilities.copyTextToClipboard(coordinates);
        displayToast(qsTr('Current location copied to clipboard'));
      }
    }
  }

  /* The feature form */
  FeatureListForm {
    id: featureForm
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

    model: MultiFeatureListModel {
    }

    selection: FeatureListModelSelection {
      id: featureListModelSelection
      model: featureForm.model
    }

    selectionColor: "#ff7777"

    onShowMessage: displayToast(message)

    onEditGeometry: {
      // Set overall selected (i.e. current) layer to that of the feature geometry being edited,
      // important for snapping settings to make sense when set to current layer
      if (dashBoard.activeLayer != featureForm.selection.focusedLayer) {
        dashBoard.activeLayer = featureForm.selection.focusedLayer;
        displayToast(qsTr("Current layer switched to the one holding the selected geometry."));
      }
      geometryEditingFeature.vertexModel.geometry = featureForm.selection.focusedGeometry;
      geometryEditingFeature.vertexModel.crs = featureForm.selection.focusedLayer.crs;
      geometryEditingFeature.currentLayer = featureForm.selection.focusedLayer;
      geometryEditingFeature.feature = featureForm.selection.focusedFeature;
      if (!geometryEditingVertexModel.editingAllowed) {
        displayToast(qsTr("Editing of multi geometry layer is not supported yet."));
        geometryEditingVertexModel.clear();
      } else {
        featureForm.state = "Hidden";
      }
      geometryEditorsToolbar.init();
    }

    Component.onCompleted: focusstack.addFocusTaker(this)

    //that the focus is set by selecting the empty space
    MouseArea {
      anchors.fill: parent
      propagateComposedEvents: true
      enabled: !parent.activeFocus

      //onPressed because onClicked shall be handled in underlying MouseArea
      onPressed: mouse => {
        parent.focus = true;
        mouse.accepted = false;
      }
    }
  }

  QfDropShadow {
    anchors.fill: featureForm
    horizontalOffset: mainWindow.width >= mainWindow.height ? -2 : 0
    verticalOffset: mainWindow.width < mainWindow.height ? -2 : 0
    radius: 6.0
    color: "#80000000"
    source: featureForm
  }

  OverlayFeatureFormDrawer {
    id: overlayFeatureFormDrawer
    objectName: "overlayFeatureFormDrawer"
    digitizingToolbar: digitizingToolbar
    codeReader: codeReader
    featureModel.currentLayer: dashBoard.activeLayer

    Component.onCompleted: focusstack.addFocusTaker(this)
  }

  function displayToast(message, type, action_text, action_function) {
    //toastMessage.text = message
    if (!welcomeScreen.visible)
      toast.show(message, type, action_text, action_function);
  }

  Timer {
    id: readProjectTimer

    interval: 250
    repeat: false
    onTriggered: iface.readProject()
  }

  Connections {
    target: iface

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

    function onImportEnded(path) {
      busyOverlay.state = "hidden";
      if (path !== '') {
        qfieldLocalDataPickerScreen.model.currentPath = path;
        qfieldLocalDataPickerScreen.visible = true;
        welcomeScreen.visible = false;
      } else {
        displayToast(qsTr('Import URL failed'));
      }
    }

    function onLoadProjectTriggered(path, name) {
      messageLogModel.suppress({
          "WFS": [""],
          "WMS": [""],
          "PostGIS": ["fe_sendauth: no password supplied"]
        });
      qfieldLocalDataPickerScreen.visible = false;
      qfieldLocalDataPickerScreen.focus = false;
      welcomeScreen.visible = false;
      welcomeScreen.focus = false;
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
      if (qfieldAuthRequestHandler.hasPendingAuthRequest) {
        qfieldAuthRequestHandler.handleLayerLogins();
      } else {
        // project in need of handling layer credentials
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
      gridDecoration.lineColor = gridDecorationConfiguration["lineColor"];
      gridDecoration.prepareMarkers = gridDecorationConfiguration["hasMarkers"];
      gridDecoration.markerColor = gridDecorationConfiguration["markerColor"];
      gridDecoration.prepareAnnotations = gridDecorationConfiguration["hasAnnotations"];
      gridDecoration.annotationPrecision = gridDecorationConfiguration["annotationPrecision"];
      gridDecoration.annotationColor = gridDecorationConfiguration["annotationColor"];
      gridDecoration.annotationHasOutline = gridDecorationConfiguration["annotationHasOutline"];
      gridDecoration.annotationOutlineColor = gridDecorationConfiguration["annotationOutlineColor"];
      gridDecoration.enabled = gridDecorationConfiguration["hasLines"] || gridDecorationConfiguration["hasMarkers"];
      recentProjectListModel.reloadModel();
      const cloudProjectId = QFieldCloudUtils.getProjectId(qgisProject.fileName);
      cloudProjectsModel.currentProjectId = cloudProjectId;
      cloudProjectsModel.refreshProjectModification(cloudProjectId);
      if (cloudProjectId !== '') {
        var cloudProjectData = cloudProjectsModel.getProjectData(cloudProjectId);
        switch (cloudProjectData.UserRole) {
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
        if (cloudProjectsModel.layerObserver.deltaFileWrapper.hasError()) {
          qfieldCloudPopup.show();
        }
        if (cloudConnection.status === QFieldCloudConnection.LoggedIn) {
          projectInfo.cloudUserInformation = cloudConnection.userInformation;
          cloudProjectsModel.refreshProjectFileOutdatedStatus(cloudProjectId);
        } else {
          projectInfo.restoreCloudUserInformation();
        }
      } else {
        projectInfo.hasInsertRights = true;
        projectInfo.hasEditRights = true;
      }
      if (stateMachine.state === "digitize" && !qfieldAuthRequestHandler.hasPendingAuthRequest) {
        dashBoard.ensureEditableLayerSelected();
      }
      var distanceString = iface.readProjectEntry("Measurement", "/DistanceUnits", "");
      projectInfo.distanceUnits = distanceString !== "" ? UnitTypes.decodeDistanceUnit(distanceString) : Qgis.DistanceUnit.Meters;
      var areaString = iface.readProjectEntry("Measurement", "/AreaUnits", "");
      projectInfo.areaUnits = areaString !== "" ? UnitTypes.decodeAreaUnit(areaString) : Qgis.AreaUnit.SquareMeters;
      if (qgisProject.displaySettings) {
        projectInfo.coordinateDisplayCrs = qgisProject.displaySettings.coordinateCrs;
      } else {
        projectInfo.coordinateDisplayCrs = !mapCanvas.mapSettings.destinationCrs.isGeographic && iface.readProjectEntry("PositionPrecision", "/DegreeFormat", "MU") !== "MU" ? CoordinateReferenceSystemUtils.wgs84Crs() : mapCanvas.mapSettings.destinationCrs;
      }
      layoutListInstantiator.model.reloadModel();
      geofencer.applyProjectSettings(qgisProject);
      positioningSettings.geofencingPreventDigitizingDuringAlert = iface.readProjectBoolEntry("qfieldsync", "/geofencingShouldPreventDigitizing", false);
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

  ProjectInfo {
    id: projectInfo

    mapSettings: mapCanvas.mapSettings
    layerTree: dashBoard.layerTree
    trackingModel: trackings.model

    property var distanceUnits: Qgis.DistanceUnit.Meters
    property var areaUnits: Qgis.AreaUnit.SquareMeters
    property var coordinateDisplayCrs: CoordinateReferenceSystemUtils.wgs84Crs()

    property bool hasInsertRights: true
    property bool hasEditRights: true

    property bool insertRights: hasInsertRights
    property bool editRights: hasEditRights
  }

  MessageLog {
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

  BadLayerItem {
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
        browserPopup.url = url;
        browserPopup.fullscreen = false;
        browserPopup.clearCookiesOnOpen = true;
        browserPopup.open();
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

    BrowserPanel {
      id: browserPopup
      objectName: "browserPopup"
      parent: Overlay.overlay
    }

    Popup {
      id: loginDialogPopup
      parent: Overlay.overlay
      width: parent.width - Theme.popupScreenEdgeMargin * 2
      height: parent.height - Math.max(Theme.popupScreenEdgeMargin * 2, mainWindow.sceneTopMargin * 2 + 4, mainWindow.sceneBottomMargin * 2 + 4)
      x: Theme.popupScreenEdgeMargin
      y: (mainWindow.height - height) / 2
      padding: 0
      modal: true
      closePolicy: Popup.CloseOnEscape

      LayerLoginDialog {
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

  About {
    id: aboutDialog
    anchors.fill: parent

    Component.onCompleted: focusstack.addFocusTaker(this)
  }

  TrackerSettings {
    id: trackerSettings
  }

  QFieldCloudConnection {
    id: cloudConnection

    property int previousStatus: QFieldCloudConnection.Disconnected

    onStatusChanged: {
      if (cloudConnection.status === QFieldCloudConnection.Disconnected && previousStatus === QFieldCloudConnection.LoggedIn) {
        displayToast(qsTr('Signed out'));
      } else if (cloudConnection.status === QFieldCloudConnection.Connecting) {
        displayToast(qsTr('Connecting...'));
      } else if (cloudConnection.status === QFieldCloudConnection.LoggedIn) {
        displayToast(qsTr('Signed in'));
        if (QFieldCloudUtils.hasPendingAttachments()) {
          // Go ahead and upload pending attachments in the background
          platformUtilities.uploadPendingAttachments(cloudConnection);
        }
        var cloudProjectId = QFieldCloudUtils.getProjectId(qgisProject.fileName);
        if (cloudProjectId) {
          projectInfo.cloudUserInformation = userInformation;
          cloudProjectsModel.refreshProjectFileOutdatedStatus(cloudProjectId);
        }
      }
      previousStatus = cloudConnection.status;
    }
    onLoginFailed: function (reason) {
      displayToast(reason);
    }
  }

  QFieldCloudProjectsModel {
    id: cloudProjectsModel
    cloudConnection: cloudConnection
    layerObserver: layerObserverAlias
    gpkgFlusher: gpkgFlusherAlias

    onProjectDownloaded: function (projectId, projectName, hasError, errorString) {
      return hasError ? displayToast(qsTr("Project %1 failed to download").arg(projectName), 'error') : displayToast(qsTr("Project %1 successfully downloaded, it's now available to open").arg(projectName));
    }

    onPushFinished: function (projectId, isDownloadingProject, hasError, errorString) {
      if (hasError) {
        displayToast(qsTr("Changes failed to reach QFieldCloud: %1").arg(errorString), 'error');
        return;
      }
      if (!isDownloadingProject) {
        displayToast(qsTr("Changes successfully pushed to QFieldCloud"));
      }
      if (QFieldCloudUtils.hasPendingAttachments()) {
        // Go ahead and upload pending attachments in the background
        platformUtilities.uploadPendingAttachments(cloudConnection);
      }
    }

    onWarning: message => displayToast(message)

    onDeltaListModelChanged: function () {
      qfieldCloudDeltaHistory.model = cloudProjectsModel.currentProjectData.DeltaList;
    }
  }

  QFieldCloudDeltaHistory {
    id: qfieldCloudDeltaHistory

    modal: true
    closePolicy: Popup.CloseOnEscape
    parent: Overlay.overlay
  }

  WelcomeScreen {
    id: welcomeScreen
    objectName: "welcomeScreen"
    visible: !iface.hasProjectOnLaunch()

    model: RecentProjectListModel {
      id: recentProjectListModel
    }

    anchors.fill: parent

    onOpenLocalDataPicker: {
      qfieldLocalDataPickerScreen.projectFolderView = false;
      qfieldLocalDataPickerScreen.model.resetToRoot();
      qfieldLocalDataPickerScreen.visible = true;
    }

    onShowQFieldCloudScreen: {
      qfieldCloudScreen.visible = true;
    }

    onShowSettings: {
      qfieldSettings.reset();
      qfieldSettings.visible = true;
    }

    Component.onCompleted: focusstack.addFocusTaker(this)
  }

  QFieldCloudScreen {
    id: qfieldCloudScreen

    anchors.fill: parent
    visible: false
    focus: visible

    onFinished: {
      visible = false;
    }

    Component.onCompleted: focusstack.addFocusTaker(this)
  }

  QFieldCloudPopup {
    id: qfieldCloudPopup
    visible: false
    focus: visible
    parent: Overlay.overlay

    width: parent.width
    height: parent.height
  }

  QFieldCloudPackageLayersFeedback {
    id: cloudPackageLayersFeedback
    visible: false
  }

  QFieldLocalDataPickerScreen {
    id: qfieldLocalDataPickerScreen

    anchors.fill: parent
    visible: false
    focus: visible

    onFinished: loading => {
      visible = false;
    }

    Component.onCompleted: focusstack.addFocusTaker(this)
  }

  QFieldSettings {
    id: qfieldSettings
    anchors.fill: parent

    onFinished: {
      visible = false;
    }

    Component.onCompleted: focusstack.addFocusTaker(this)
  }

  Changelog {
    id: changelogPopup
    objectName: 'changelogPopup'
    parent: Overlay.overlay

    Component.onCompleted: {
      const changelogVersion = settings.value("/QField/ChangelogVersion", "");
      if (changelogVersion === "") {
        settings.setValue("/QField/ChangelogVersion", appVersion);
      } else if (changelogVersion !== appVersion) {
        open();
      }
    }
  }

  Toast {
    id: toast
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

  CodeReader {
    id: codeReader
    objectName: 'codeReader'
    visible: false
  }

  QFieldSketcher {
    id: sketcher
    visible: false
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

  Nyuki {
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

  BusyOverlay {
    id: busyOverlay
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
      displayToast(qsTr("Press back again to close project and app"));
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
  FeatureModel {
    id: geometryEditingFeature
    project: qgisProject
    currentLayer: null
    positionInformation: positionSource.positionInformation
    positionLocked: positionSource.active && positioningSettings.positioningCoordinateLock
    vertexModel: geometryEditingVertexModel
    cloudUserInformation: projectInfo.cloudUserInformation
  }

  VertexModel {
    id: geometryEditingVertexModel
    currentPoint: coordinateLocator.currentCoordinate
    mapSettings: mapCanvas.mapSettings
    isHovering: mapCanvasMap.hovered
  }

  ScreenLocker {
    id: screenLocker
    objectName: "screenLocker"
    enabled: false
  }

  QfDialog {
    id: pluginPermissionDialog
    parent: mainWindow.contentItem
    z: 10000 // 1000s are embedded feature forms, user a higher value to insure the dialog will always show above embedded feature forms

    property alias permanent: permanentCheckBox.checked

    title: ''

    Column {
      Label {
        width: parent.width
        wrapMode: Text.WordWrap
        text: qsTr("Do you grant permission to activate `%1`?").arg(pluginPermissionDialog.title)
      }

      CheckBox {
        id: permanentCheckBox
        text: qsTr('Remember my choice')
        font: Theme.defaultFont
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
    font: Theme.defaultFont

    z: 10000 // 1000s are embedded feature forms, user a higher value to insure the dialog will always show above embedded feature forms

    title: qsTr("Cancel algorithm operation")
    Label {
      width: parent.width
      wrapMode: Text.WordWrap
      text: qsTr("You are about to dismiss the ongoing algorithm operation, proceed?")
    }

    standardButtons: Dialog.Ok | Dialog.Cancel
    onAccepted: {
      featureForm.state = "Hidden";
      mentMode();
    }
    onDiscarded: {
      cancelAlgorithmDialog.visible = false;
    }
  }

  Connections {
    target: pluginManager

    function onPluginPermissionRequested(pluginName) {
      pluginPermissionDialog.title = pluginName;
      pluginPermissionDialog.open();
    }
  }

  QFieldGuide {
    id: mapCanvasTour
    baseRoot: mainWindow
    objectName: 'mapCanvasTour'

    steps: [{
        "title": qsTr("Dashboard"),
        "description": qsTr("This button opens the dashboard. With the dashboard you can interact with the legend and map theme, or start digitizing by activating the editing mode. Long-pressing the button gives you immediate access to the main menu."),
        "target": () => [menuButton]
      }, {
        "title": qsTr("Positioning"),
        "description": qsTr("This button toggles the positioning system. When enabled, a position marker will appear top of the map. Long-pressing the button will open the positioning menu where additional functionalities can be explored."),
        "target": () => [gnssButton]
      }, {
        "title": qsTr("Search"),
        "description": qsTr("The search bar provides you with a quick way to find features within your project, jump to a typed latitude and longitude point, and much more."),
        "target": () => [locatorItem]
      }, {
        "title": qsTr("Zoom"),
        "description": qsTr("In addition to the pinch gesture, these buttons help you quickly zoom in and out."),
        "target": () => [zoomToolbar]
      }]

    function startOnFreshRun() {
      const startupGuide = settings.valueBool("/QField/showMapCanvasGuide", true);
      if (startupGuide) {
        runTour();
      }
      settings.setValue("/QField/showMapCanvasGuide", false);
    }
  }

  Item {
    objectName: 'toursController'

    function blockGuides() {
      mapCanvasTour.blockGuide();
      settings.setValue("/QField/showMapCanvasGuide", false);
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
