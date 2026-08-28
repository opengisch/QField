import QtQuick
import QtQuick.Controls
import QtQuick.Shapes
import QtQuick.Window
import QtMultimedia
import QtCore
import org.qfield.core
import org.qfield.gui

/**
 * \ingroup qml
 */
Popup {
  id: cameraItem
  z: 10000 // 1000s are embedded feature forms, use a higher value to insure feature form popups always show above embedded feature forms

  property bool isCapturing: false

  readonly property bool isReady: !isCapturing && (state == "PhotoCapture" || state == "VideoCapture")
  readonly property bool isPortraitMode: mainWindow.height > mainWindow.width

  property string currentPath: ''
  property var currentPosition: QfPositioningUtils.createEmptyGnssPositionInformation()
  property var currentProjectedPosition: undefined

  property bool captureLoaderActivated: false

  property bool allowCaptureModeToggle: false

  readonly property int panelExtraSpace: allowCaptureModeToggle ? 70 : 0
  readonly property int captureOffset: allowCaptureModeToggle ? -25 : 0
  property int userRotation: 0
  property bool userMirror: false

  property alias currentLayer: stampExpressionEvaluator.layer
  property alias currentFeature: stampExpressionEvaluator.feature

  function requiredPermissionsGranted() {
    if (cameraPermission.status !== Qt.PermissionStatus.Granted) {
      return false;
    }
    if (state === "VideoCapture" && microphonePermission.status !== Qt.PermissionStatus.Granted) {
      return false;
    }
    return true;
  }

  function tryActivateCaptureLoader() {
    if (!captureLoaderActivated && requiredPermissionsGranted()) {
      captureLoaderActivated = true;
    }
  }

  function discardCapture() {
    if (currentPath !== '') {
      platformUtilities.rmFile(currentPath);
      currentPath = '';
    }
  }

  function videoResolutionForQuality(cameraResolution, quality) {
    if (cameraResolution.width < 1 || cameraResolution.height < 1) {
      return Qt.size(0, 0);
    }
    const shorterSide = Math.min(cameraResolution.width, cameraResolution.height);
    if (quality <= 0 || quality >= shorterSide) {
      return cameraResolution;
    }
    const toEven = side => side + (side & 1);
    const longerSide = toEven(Math.round(Math.max(cameraResolution.width, cameraResolution.height) * quality / shorterSide));
    return cameraResolution.width < cameraResolution.height ? Qt.size(toEven(quality), longerSide) : Qt.size(longerSide, toEven(quality));
  }

  signal finished(string path)
  signal canceled

  x: 0
  y: 0
  width: mainWindow.width
  height: mainWindow.height
  padding: 0

  parent: mainWindow.contentItem
  modal: true
  focus: true
  closePolicy: Popup.NoAutoClose

  property string state: "PhotoCapture"
  onStateChanged: {
    if (state == "PhotoCapture") {
      photoPreview.source = '';
      videoPreview.source = '';
    } else if (state == "VideoCapture") {
      photoPreview.source = '';
      videoPreview.source = '';
    }
  }

  onAboutToShow: {
    currentPath = "";
    photoPreview.source = "";
    videoPreview.stop();
    videoPreview.source = "";
    userRotation = 0;
    userMirror = false;

    captureLoaderActivated = false;

    if (cameraPermission.status === Qt.PermissionStatus.Undetermined) {
      cameraPermission.request();
    } else if (state == "VideoCapture" && microphonePermission.status === Qt.PermissionStatus.Undetermined) {
      microphonePermission.request();
    }

    tryActivateCaptureLoader();
  }

  onAboutToHide: {
    captureLoaderActivated = false;
  }

  QfCameraPermission {
    id: cameraPermission

    onStatusChanged: {
      if (state == "VideoCapture" && microphonePermission.status === Qt.PermissionStatus.Undetermined) {
        microphonePermission.request();
      }
      cameraItem.tryActivateCaptureLoader();
    }
  }
  QfMicrophonePermission {
    id: microphonePermission

    onStatusChanged: {
      if (cameraPermission.status === Qt.PermissionStatus.Undetermined) {
        cameraPermission.request();
      }
      cameraItem.tryActivateCaptureLoader();
    }
  }

  Settings {
    id: cameraSettings
    property bool stamping: false
    property bool geoTagging: true
    property bool showGrid: false
    property string deviceId: ''
    property size resolution: Qt.size(0, 0)
    property int pixelFormat: 0
    property int videoQuality: 0
  }

  QfExpressionEvaluator {
    id: stampExpressionEvaluator

    property string defaultTextTemplate: "[% format_date(now(), 'yyyy-MM-dd @ HH:mm') || if(@gnss_coordinate is not null, format('\n" + qsTr("Latitude") + " %1 | " + qsTr("Longitude") + " %2 | " + qsTr("Altitude") + " %3\n" + qsTr("Speed") + " %4 | " + qsTr("Orientation") + " %5', coalesce(format_number(y(@gnss_coordinate), 7), 'N/A'), coalesce(format_number(x(@gnss_coordinate), 7), 'N/A'), coalesce(format_number(@corrected_elevation, 3) || ' ' || @corrected_elevation_unit, 'N/A'), if(@gnss_ground_speed != 'nan', format_number(@gnss_ground_speed, 3) || ' m/s', 'N/A'), if(@gnss_orientation != 'nan', format_number(@gnss_orientation, 1) || ' °', 'N/A')), '') %]"

    mode: QfExpressionEvaluator.ExpressionTemplateMode
    expressionText: ""

    project: qgisProject
    appExpressionContextScopesGenerator: QfAppExpressionContextScopesGenerator {
      positionInformation: currentPosition
      cloudUserInformation: appScopesGenerator.cloudUserInformation
    }

    variables: {
      "corrected_elevation": currentProjectedPosition ? currentProjectedPosition.z : currentPosition.altitude,
      "corrected_elevation_unit": UnitTypes.toAbbreviatedString(positionSource.coordinateTransformer.destinationCrs.mapUnit)
    }
  }

  Page {
    width: parent.width
    height: parent.height
    padding: 0
    focus: true

    background: Rectangle {
      anchors.fill: parent
      color: "#000000"
    }

    Timer {
      id: captureFocusTimer
      interval: 2000
      repeat: false

      onTriggered: {
        // Auto-focus fix failing, fail back video frame capture
        captureLoader.item.videoSinkCapture.saveToFile(qgisProject.homePath + '/DCIM/');
        // Reset camera to avoid random capture completition
        captureLoader.item.camera.restarting = true;
        captureLoader.item.camera.restarting = false;
      }
    }

    MediaDevices {
      id: mediaDevices
    }

    Loader {
      id: captureLoader
      anchors.fill: parent

      active: cameraItem.visible && captureLoaderActivated
      asynchronous: true

      sourceComponent: Component {
        Item {
          id: captureItem
          anchors.fill: parent

          property alias captureSession: captureSession
          property alias camera: camera
          property alias imageCapture: imageCapture
          property alias recorder: recorder
          property alias videoOutput: videoOutput
          property alias videoSinkCapture: videoSinkCapture
          property alias orientationNormalizer: orientationNormalizer

          // falls back to the largest supported format while the camera has no active one
          readonly property size cameraResolution: {
            const activeResolution = camera.cameraFormat.resolution;
            if (activeResolution.width > 0 && activeResolution.height > 0) {
              return activeResolution;
            }
            let largest = Qt.size(0, 0);
            for (const format of camera.cameraDevice.videoFormats) {
              if (format.resolution.width * format.resolution.height > largest.width * largest.height) {
                largest = format.resolution;
              }
            }
            return largest;
          }

          QfCameraOrientationNormalizer {
            id: orientationNormalizer
            cameraPosition: {
              const device = camera.cameraDevice;
              if (device.position === CameraDevice.FrontFace) {
                return QfCameraOrientationNormalizer.FrontFace;
              }
              if (device.position === CameraDevice.BackFace) {
                return QfCameraOrientationNormalizer.BackFace;
              }
              return device.description.toLowerCase().includes("front") ? QfCameraOrientationNormalizer.FrontFace : QfCameraOrientationNormalizer.BackFace;
            }
          }

          QfVideoSinkCapture {
            id: videoSinkCapture
            videoSink: videoOutput.videoSink

            onImageSaved: path => {
              captureButtonAnimation.stop();
              cameraItem.isCapturing = false;
              if (path !== "") {
                currentPath = path;
                photoPreview.source = "file://" + currentPath;
                cameraItem.state = "PhotoPreview";
              } else {
                cameraItem.state = "PhotoCapture";
              }
            }
          }

          VideoOutput {
            id: videoOutput
            anchors.fill: parent
            visible: cameraItem.state == "PhotoCapture" || cameraItem.state == "VideoCapture"
            orientation: orientationNormalizer.previewRotation
          }

          CaptureSession {
            id: captureSession

            camera: Camera {
              id: camera

              property bool restarting: false
              active: cameraItem.visible && cameraPermission.status === Qt.PermissionStatus.Granted && !restarting

              onErrorOccurred: (error, errorString) => {
                console.log('QField Camera error ' + error + ': ' + errorString);
              }

              function applyCameraFormat() {
                if (cameraSettings.pixelFormat != 0) {
                  let fallbackIndex = -1;
                  let i = 0;
                  for (let format of camera.cameraDevice.videoFormats) {
                    if (format.resolution == cameraSettings.resolution && format.pixelFormat == cameraSettings.pixelFormat) { // coercion required
                      camera.cameraFormat = format;
                      fallbackIndex = -1;
                      break;
                    } else if (format.resolution == cameraSettings.resolution) { // coercion required
                      // If we can't match the pixel format and resolution, go for resolution match across devices
                      fallbackIndex = i;
                    }
                    i++;
                  }
                  if (fallbackIndex >= 0) {
                    camera.cameraFormat = camera.cameraDevice.videoFormats[fallbackIndex];
                  }
                }
              }

              function zoomIn(increase) {
                var zoom = camera.zoomFactor + increase;
                if (zoom < camera.maximumZoomFactor) {
                  camera.zoomFactor = zoom;
                } else {
                  camera.zoomFactor = camera.maximumZoomFactor;
                }
              }

              function zoomOut(decrease) {
                var zoom = camera.zoomFactor - decrease;
                if (zoom > 1) {
                  camera.zoomFactor = zoom;
                } else {
                  camera.zoomFactor = 1;
                }
              }
            }

            videoOutput: videoOutput
            imageCapture: ImageCapture {
              id: imageCapture

              fileFormat: ImageCapture.JPEG
              quality: ImageCapture.VeryHighQuality

              onImageSaved: (requestId, path) => {
                if (cameraItem.isCapturing) {
                  cameraItem.isCapturing = false;
                  currentPath = path;
                  orientationNormalizer.normalizeImageOrientation(currentPath);
                  photoPreview.source = "file://" + currentPath;
                }
              }

              onPreviewChanged: {
                if (cameraItem.state == "PhotoCapture") {
                  captureFocusTimer.stop();
                  captureButtonAnimation.stop();
                  cameraItem.state = "PhotoPreview";
                }
              }

              onErrorOccurred: (id, error, errorString) => {
                console.log('QField ImageCapture error ' + error + ': ' + errorString);
              }
            }

            recorder: MediaRecorder {
              id: recorder

              videoResolution: cameraItem.videoResolutionForQuality(captureItem.cameraResolution, cameraSettings.videoQuality)

              onRecorderStateChanged: {
                if (recorderState !== MediaRecorder.StoppedState) {
                  return;
                }
                if (cameraItem.state == "VideoPreview") {
                  videoPreview.source = recorder.actualLocation;
                  videoPreview.play();
                } else {
                  platformUtilities.rmFile(QfUrlUtils.toLocalFile(recorder.actualLocation.toString()));
                }
              }
            }
          }
        }
      }

      onLoaded: {
        item.recorder.mediaFormat.audioCodec = MediaFormat.AudioCodec.AAC;
        item.recorder.mediaFormat.videoCodec = MediaFormat.VideoCodec.H264;
        item.recorder.mediaFormat.fileFormat = MediaFormat.MPEG4;
        let cameraPicked = false;
        if (cameraSettings.deviceId != '') {
          for (const device of mediaDevices.videoInputs) {
            if (device.id == cameraSettings.deviceId) { // coercion required
              item.camera.cameraDevice = device;
              cameraPicked = true;
            }
          }
        }
        if (!cameraPicked) {
          item.camera.cameraDevice = mediaDevices.defaultVideoInput;
        }
        item.camera.applyCameraFormat();
      }
    }

    Shape {
      id: grid
      visible: cameraSettings.showGrid
      anchors.centerIn: parent

      property bool readyForGrid: captureLoader.status === Loader.Ready && captureLoader.item && captureLoader.item.videoOutput.contentRect.height > 0 && captureLoader.item.videoOutput.contentRect.width > 0
      property bool isLandscape: readyForGrid ? (mainWindow.width / mainWindow.height) > (captureLoader.item.videoOutput.contentRect.width / captureLoader.item.videoOutput.contentRect.height) : true

      width: readyForGrid ? (isLandscape ? captureLoader.item.videoOutput.contentRect.width * mainWindow.height / captureLoader.item.videoOutput.contentRect.height : mainWindow.width) : mainWindow.width
      height: readyForGrid ? (isLandscape ? mainWindow.height : captureLoader.item.videoOutput.contentRect.height * mainWindow.width / captureLoader.item.videoOutput.contentRect.width) : mainWindow.height

      ShapePath {
        strokeColor: "#99000000"
        strokeWidth: 3
        fillColor: "transparent"

        startX: grid.width / 3
        startY: 0

        PathLine {
          x: grid.width / 3
          y: grid.height
        }
        PathMove {
          x: grid.width / 3 * 2
          y: 0
        }
        PathLine {
          x: grid.width / 3 * 2
          y: grid.height
        }
        PathMove {
          x: 0
          y: grid.height / 3
        }
        PathLine {
          x: grid.width
          y: grid.height / 3
        }
        PathMove {
          x: 0
          y: grid.height / 3 * 2
        }
        PathLine {
          x: grid.width
          y: grid.height / 3 * 2
        }
      }

      ShapePath {
        strokeColor: "#AAFFFFFF"
        strokeWidth: 1
        fillColor: "transparent"

        startX: grid.width / 3
        startY: 0

        PathLine {
          x: grid.width / 3
          y: grid.height
        }
        PathMove {
          x: grid.width / 3 * 2
          y: 0
        }
        PathLine {
          x: grid.width / 3 * 2
          y: grid.height
        }
        PathMove {
          x: 0
          y: grid.height / 3
        }
        PathLine {
          x: grid.width
          y: grid.height / 3
        }
        PathMove {
          x: 0
          y: grid.height / 3 * 2
        }
        PathLine {
          x: grid.width
          y: grid.height / 3 * 2
        }
      }
    }

    Video {
      id: videoPreview
      anchors.fill: parent
      visible: cameraItem.state == "VideoPreview"

      loops: MediaPlayer.Infinite
      muted: true
    }

    Image {
      id: photoPreview

      visible: cameraItem.state == "PhotoPreview"
      anchors.fill: parent
      cache: false
      fillMode: Image.PreserveAspectFit
      smooth: true

      rotation: cameraItem.userRotation
      transform: Scale {
        origin.x: photoPreview.width / 2
        origin.y: photoPreview.height / 2
        xScale: cameraItem.userMirror ? -1 : 1
      }
    }

    PinchArea {
      id: pinchArea
      enabled: cameraItem.visible && cameraItem.isReady && captureLoader.item
      anchors.fill: parent

      onPinchUpdated: pinch => {
        if (pinch.scale > pinch.previousScale) {
          captureLoader.item.camera.zoomIn(0.05);
        } else {
          captureLoader.item.camera.zoomOut(0.05);
        }
      }
    }

    WheelHandler {
      enabled: cameraItem.visible && cameraItem.isReady && captureLoader.item
      target: null
      grabPermissions: PointerHandler.CanTakeOverFromHandlersOfDifferentType | PointerHandler.ApprovesTakeOverByItems

      onWheel: event => {
        if (event.angleDelta.y > 0) {
          captureLoader.item.camera.zoomIn(0.25);
        } else {
          captureLoader.item.camera.zoomOut(0.25);
        }
      }
    }

    Rectangle {
      id: captureFlash
      anchors.fill: parent
      anchors.margins: 6
      color: "transparent"

      SequentialAnimation {
        id: captureFlashAnimation
        PropertyAnimation {
          target: captureFlash
          property: "color"
          to: "white"
          duration: 0
        }
        PropertyAnimation {
          target: captureFlash
          property: "color"
          to: "transparent"
          duration: 1000
        }
      }
    }

    Rectangle {
      id: captureBar
      width: cameraItem.isPortraitMode ? parent.width : 100 + mainWindow.sceneBottomMargin + cameraItem.panelExtraSpace
      height: cameraItem.isPortraitMode ? 100 + cameraItem.panelExtraSpace + mainWindow.sceneRightMargin : parent.height
      x: cameraItem.isPortraitMode ? 0 : parent.width - width
      y: cameraItem.isPortraitMode ? parent.height - height : 0

      color: QfTheme.darkGraySemiOpaque

      Rectangle {
        width: cameraItem.isPortraitMode ? parent.width : 100 + mainWindow.sceneBottomMargin + cameraItem.panelExtraSpace
        height: cameraItem.isPortraitMode ? 100 + cameraItem.panelExtraSpace + mainWindow.sceneRightMargin : parent.height
        x: cameraItem.isPortraitMode ? 0 : parent.width - width
        y: cameraItem.isPortraitMode ? parent.height - height : 0

        color: QfTheme.darkGraySemiOpaque

        Rectangle {
          anchors.top: parent.top
          width: cameraItem.isPortraitMode ? parent.width : parent.width - mainWindow.sceneRightMargin
          height: cameraItem.isPortraitMode ? parent.height - mainWindow.sceneBottomMargin : parent.height
          color: "transparent"

          Rectangle {
            id: captureRing
            anchors.centerIn: parent
            anchors.verticalCenterOffset: cameraItem.isPortraitMode ? cameraItem.captureOffset : 0
            anchors.horizontalCenterOffset: !cameraItem.isPortraitMode ? cameraItem.captureOffset : 0
            width: 64
            height: 64
            radius: 32
            color: QfTheme.darkGraySemiOpaque
            border.color: cameraItem.state == "VideoCapture" && captureLoader.item && captureLoader.item.recorder.recorderState !== MediaRecorder.StoppedState ? "red" : "white"
            border.width: 2

            QfToolButton {
              id: captureButton

              anchors.centerIn: parent
              visible: captureLoader.status == Loader.Ready && captureLoader.item
              enabled: !cameraItem.isCapturing
              opacity: enabled ? 1 : 0.5

              readonly property bool recording: cameraItem.state == "VideoCapture" && captureLoader.item && captureLoader.item.recorder.recorderState !== MediaRecorder.StoppedState
              round: true
              roundborder: true

              backgroundRadius: recording ? 8 : width / 2
              scale: recording ? 0.65 : 1

              Behavior on backgroundRadius {
                NumberAnimation {
                  duration: 200
                  easing.type: Easing.OutQuad
                }
              }

              Behavior on scale {
                NumberAnimation {
                  duration: 200
                  easing.type: Easing.OutQuad
                }
              }

              iconSource: cameraItem.state == "PhotoPreview" || cameraItem.state == "VideoPreview" ? QfTheme.getThemeVectorIcon("ic_check_white_24dp") : ''
              iconColor: QfTheme.toolButtonColor
              bgcolor: cameraItem.state == "PhotoPreview" || cameraItem.state == "VideoPreview" ? QfTheme.mainColor : cameraItem.state == "VideoCapture" ? "red" : "white"

              width: 48
              height: width

              SequentialAnimation {
                id: captureButtonAnimation
                loops: Animation.Infinite
                PropertyAnimation {
                  target: captureButton
                  property: "width"
                  to: 0
                  duration: 0
                }
                PropertyAnimation {
                  target: captureButton
                  property: "width"
                  to: 48
                  duration: 2500
                  easing.type: Easing.OutQuad
                }
                PropertyAnimation {
                  target: captureButton
                  property: "width"
                  to: 0
                  duration: 2500
                  easing.type: Easing.OutQuad
                }

                onStopped: {
                  if (captureButton.width !== 48) {
                    captureButton.width = 48;
                  }
                }
              }

              onClicked: {
                if (!captureLoader.item) {
                  return;
                }
                if (cameraItem.state == "PhotoCapture") {
                  platformUtilities.createDir(qgisProject.homePath, 'DCIM');
                  captureLoader.item.camera.focusMode = Camera.FocusModeAuto;
                  captureLoader.item.imageCapture.captureToFile(qgisProject.homePath + '/DCIM/');
                  cameraItem.isCapturing = true;
                  captureFocusTimer.restart();
                  captureButtonAnimation.start();
                  captureFlashAnimation.start();
                  captureLoader.item.orientationNormalizer.recordCaptureOrientation();
                  if (positionSource.active) {
                    currentPosition = positionSource.positionInformation;
                    currentProjectedPosition = positionSource.projectedPosition;
                  } else {
                    currentPosition = QfPositioningUtils.createEmptyGnssPositionInformation();
                    currentProjectedPosition = undefined;
                  }
                } else if (cameraItem.state == "VideoCapture") {
                  if (captureLoader.item.recorder.recorderState === MediaRecorder.StoppedState) {
                    platformUtilities.createDir(qgisProject.homePath, 'DCIM');
                    captureLoader.item.recorder.outputLocation = QfUrlUtils.fromString(qgisProject.homePath + '/DCIM/');
                    captureLoader.item.recorder.record();
                  } else {
                    cameraItem.state = "VideoPreview";
                    captureLoader.item.recorder.stop();
                    const path = captureLoader.item.recorder.actualLocation.toString();
                    currentPath = QfUrlUtils.toLocalFile(path);
                  }
                } else if (cameraItem.state == "PhotoPreview" || cameraItem.state == "VideoPreview") {
                  if (!currentPath || currentPath === "")
                    return;
                  if (cameraItem.state == "PhotoPreview") {
                    if (cameraSettings.geoTagging && positionSource.active) {
                      QfFileUtils.addImageMetadata(currentPath, currentPosition);
                    }
                    if (cameraSettings.stamping || iface.readProjectBoolEntry("qfieldsync", "forceStamping")) {
                      stampExpressionEvaluator.expressionText = iface.readProjectEntry("qfieldsync", "stampingDetailsTemplate", stampExpressionEvaluator.defaultTextTemplate);
                      if (stampExpressionEvaluator.expressionText === "") {
                        stampExpressionEvaluator.expressionText = stampExpressionEvaluator.defaultTextTemplate;
                      }
                      QfFileUtils.addImageStamp(currentPath, stampExpressionEvaluator.evaluate(), iface.readProjectEntry("qfieldsync", "stampingFontStyle"), iface.readProjectNumEntry("qfieldsync", "stampingHorizontalAlignment", 0), iface.readProjectEntry("qfieldsync", "stampingImageDecoration"));
                    }
                  }
                  if (cameraItem.userRotation !== 0 || cameraItem.userMirror) {
                    captureLoader.item.orientationNormalizer.applyEditsToImage(currentPath, cameraItem.userRotation, cameraItem.userMirror);
                  }
                  cameraItem.finished(currentPath);
                }
              }
            }
          }

          Rectangle {
            id: photoEditButtons
            visible: cameraItem.state == "PhotoPreview"

            width: photoEditButtonsRow.width + 8
            height: photoEditButtonsRow.height + 8
            radius: height / 2
            color: Qt.hsla(QfTheme.toolButtonBackgroundSemiOpaqueColor.hslHue, QfTheme.toolButtonBackgroundSemiOpaqueColor.hslSaturation, QfTheme.toolButtonBackgroundSemiOpaqueColor.hslLightness, 0.3)

            rotation: cameraItem.isPortraitMode ? 0 : -90

            x: cameraItem.isPortraitMode ? captureRing.x + captureRing.width / 2 - width / 2 : captureRing.x + captureRing.width / 2 - width / 2 - (20 + mainWindow.sceneBottomMargin + cameraItem.panelExtraSpace)
            y: cameraItem.isPortraitMode ? captureRing.y - height - 20 : captureRing.y + captureRing.height / 2 - height / 2

            Row {
              id: photoEditButtonsRow
              anchors.centerIn: parent
              spacing: 4

              QfToolButton {
                id: rotateLeftButton
                width: 40
                height: 40
                padding: 2
                round: true
                iconColor: QfTheme.toolButtonColor
                bgcolor: QfTheme.toolButtonBackgroundSemiOpaqueColor
                onClicked: cameraItem.userRotation = (cameraItem.userRotation + 270) % 360

                Image {
                  anchors.centerIn: parent
                  width: 24
                  height: 24
                  source: QfTheme.getThemeVectorIcon("ic_rotate_grey_24dp")
                  rotation: cameraItem.isPortraitMode ? 0 : 90
                }
              }

              QfToolButton {
                id: reflectButton
                width: 40
                height: 40
                padding: 2
                round: true
                iconColor: QfTheme.toolButtonColor
                bgcolor: QfTheme.toolButtonBackgroundSemiOpaqueColor
                onClicked: cameraItem.userMirror = !cameraItem.userMirror

                Image {
                  anchors.centerIn: parent
                  width: 24
                  height: 24
                  source: QfTheme.getThemeVectorIcon("ic_reflect_grey_24dp")
                  rotation: cameraItem.isPortraitMode ? 0 : 90
                }
              }

              QfToolButton {
                id: rotateRightButton
                width: 40
                height: 40
                padding: 2
                round: true
                iconColor: QfTheme.toolButtonColor
                bgcolor: QfTheme.toolButtonBackgroundSemiOpaqueColor
                onClicked: cameraItem.userRotation = (cameraItem.userRotation + 90) % 360

                Image {
                  anchors.centerIn: parent
                  width: 24
                  height: 24
                  source: QfTheme.getThemeVectorIcon("ic_rotate_grey_24dp")
                  mirror: true
                  rotation: cameraItem.isPortraitMode ? 0 : 90
                }
              }
            }
          }

          QfSwitch {
            id: modeSwitch

            readonly property int slotSize: 36
            readonly property int highlightInset: 1

            visible: cameraItem.allowCaptureModeToggle && cameraItem.isReady && captureLoader.item && captureLoader.item.recorder.recorderState === MediaRecorder.StoppedState

            width: slotSize * 2 + 4
            height: 40
            padding: 0

            rotation: cameraItem.isPortraitMode ? 0 : -90

            x: cameraItem.isPortraitMode ? captureRing.x + captureRing.width / 2 - width / 2 : captureRing.x + captureRing.width + 10 - (width - height) / 2
            y: cameraItem.isPortraitMode ? captureRing.y + captureRing.height + 10 : captureRing.y + captureRing.height / 2 - height / 2

            checked: cameraItem.state == "VideoCapture"

            indicator: Rectangle {
              implicitHeight: modeSwitch.slotSize
              implicitWidth: modeSwitch.slotSize * 2
              x: (modeSwitch.width - implicitWidth) / 2
              radius: 4
              color: QfTheme.darkGraySemiOpaque
              border.color: Qt.alpha(QfTheme.light, 0.08)
              border.width: 1
              anchors.verticalCenter: parent.verticalCenter

              QfToolButton {
                width: modeSwitch.slotSize
                height: modeSwitch.slotSize
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                round: false
                iconSource: QfTheme.getThemeVectorIcon('ic_camera_photo_black_24dp')
                iconColor: QfTheme.gray
                bgcolor: 'transparent'
                enabled: false
                rotation: cameraItem.isPortraitMode ? 0 : 90
              }

              QfToolButton {
                width: modeSwitch.slotSize
                height: modeSwitch.slotSize
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                round: false
                iconSource: QfTheme.getThemeVectorIcon('ic_camera_video_black_24dp')
                iconColor: QfTheme.gray
                bgcolor: 'transparent'
                enabled: false
                rotation: cameraItem.isPortraitMode ? 0 : 90
              }

              Rectangle {
                readonly property int inset: modeSwitch.highlightInset
                x: modeSwitch.checked ? parent.width - width - inset : inset
                y: inset
                width: modeSwitch.slotSize - inset * 2
                height: modeSwitch.slotSize - inset * 2
                radius: 3
                color: Qt.alpha(QfTheme.light, 0.25)
                clip: true

                QfToolButton {
                  width: modeSwitch.slotSize
                  height: modeSwitch.slotSize
                  anchors.centerIn: parent
                  round: false
                  hoverEnabled: false
                  iconSource: modeSwitch.checked ? QfTheme.getThemeVectorIcon('ic_camera_video_black_24dp') : QfTheme.getThemeVectorIcon('ic_camera_photo_black_24dp')
                  iconColor: QfTheme.toolButtonColor
                  bgcolor: 'transparent'
                  enabled: false
                  rotation: cameraItem.isPortraitMode ? 0 : 90
                }

                Behavior on x {
                  PropertyAnimation {
                    duration: 100
                    easing.type: Easing.OutQuart
                  }
                }
              }
            }

            onClicked: {
              cameraItem.state = checked ? "VideoCapture" : "PhotoCapture";
            }
          }

          QfToolButton {
            id: zoomButton
            visible: captureLoader.item && captureLoader.item.camera.maximumZoomFactor !== 1.0

            x: cameraItem.isPortraitMode ? (parent.width / 4) - (width / 2) : (parent.width - width) / 2 + cameraItem.captureOffset
            y: cameraItem.isPortraitMode ? (parent.height - height) / 2 + cameraItem.captureOffset : (parent.height / 4) * 3 - (height / 2)

            iconColor: QfTheme.toolButtonColor
            bgcolor: QfTheme.toolButtonBackgroundSemiOpaqueColor
            round: true

            text: captureLoader.item ? captureLoader.item.camera.zoomFactor.toFixed(1) + 'X' : '1.0X'
            font: QfTheme.tinyFont

            onClicked: {
              if (captureLoader.item) {
                captureLoader.item.camera.zoomFactor = 1;
              }
            }
          }

          QfToolButton {
            id: flashButton
            visible: captureLoader.item && captureLoader.item.camera.isFlashModeSupported(Camera.FlashOn)

            x: cameraItem.isPortraitMode ? (parent.width / 4) * 3 - (width / 2) : (parent.width - width) / 2 + cameraItem.captureOffset
            y: cameraItem.isPortraitMode ? (parent.height - height) / 2 + cameraItem.captureOffset : (parent.height / 4) - (height / 2)

            iconSource: {
              if (!captureLoader.item) {
                return '';
              }
              switch (captureLoader.item.camera.flashMode) {
              case Camera.FlashAuto:
                return QfTheme.getThemeVectorIcon('ic_flash_auto_black_24dp');
              case Camera.FlashOn:
                return QfTheme.getThemeVectorIcon('ic_flash_on_black_24dp');
              case Camera.FlashOff:
                return QfTheme.getThemeVectorIcon('ic_flash_off_black_24dp');
              default:
                return '';
              }
            }
            iconColor: QfTheme.toolButtonColor
            bgcolor: QfTheme.toolButtonBackgroundSemiOpaqueColor
            round: true

            onClicked: {
              if (!captureLoader.item) {
                return;
              }
              if (captureLoader.item.camera.flashMode === Camera.FlashOff) {
                captureLoader.item.camera.flashMode = Camera.FlashOn;
              } else {
                captureLoader.item.camera.flashMode = Camera.FlashOff;
              }
            }
          }

          Rectangle {
            id: recordingIndicator
            visible: cameraItem.state == "VideoCapture" && captureLoader.item && captureLoader.item.recorder.recorderState !== MediaRecorder.StoppedState

            x: cameraItem.isPortraitMode ? captureRing.x + captureRing.width / 2 - width / 2 : captureRing.x + captureRing.width / 2 - width / 2
            y: cameraItem.isPortraitMode ? captureRing.y - height - 20 : captureRing.y - height - 20

            width: durationLabelMetrics.boundingRect('00:00:00').width + 34
            height: durationLabelMetrics.boundingRect('00:00:00').height + 10
            radius: 6

            color: QfTheme.darkGraySemiOpaque

            Row {
              anchors.centerIn: parent
              spacing: 6

              Rectangle {
                anchors.verticalCenter: parent.verticalCenter
                width: 8
                height: width
                radius: width / 2
                color: "red"
              }

              Text {
                id: durationLabel
                text: {
                  if (captureLoader.item && captureLoader.item.recorder.duration > 0) {
                    var seconds = Math.ceil(captureLoader.item.recorder.duration / 1000);
                    var hours = Math.floor(seconds / 60 / 60) + '';
                    seconds -= hours * 60 * 60;
                    var minutes = Math.floor(seconds / 60) + '';
                    seconds = (seconds - minutes * 60) + '';
                    return hours.padStart(2, '0') + ':' + minutes.padStart(2, '0') + ':' + seconds.padStart(2, '0');
                  } else {
                    // tiny bit of a cheat here as the first second isn't triggered
                    return '00:00:01';
                  }
                }
                color: QfTheme.light
              }
            }

            FontMetrics {
              id: durationLabelMetrics
              font: durationLabel.font
            }
          }

          Rectangle {
            visible: cameraItem.state == "VideoPreview" && recordedSizeLabel.text !== ''

            x: captureRing.x + captureRing.width / 2 - width / 2
            y: captureRing.y - height - 20

            width: recordedSizeLabel.width + 20
            height: durationLabelMetrics.boundingRect('00:00:00').height + 10
            radius: 6

            color: QfTheme.darkGraySemiOpaque

            Text {
              id: recordedSizeLabel
              anchors.centerIn: parent
              text: {
                if (videoPreview.duration <= 0) {
                  return '';
                }
                const bytes = QfFileUtils.fileSize(cameraItem.currentPath);
                return bytes > 0 ? QfFileUtils.representFileSize(bytes) : '';
              }
              font: durationLabel.font
              color: QfTheme.light
            }
          }
        }
      }
    }

    Rectangle {
      id: videoQualityPanel
      visible: cameraItem.state == "VideoCapture" && captureLoader.item && captureLoader.item.recorder.recorderState === MediaRecorder.StoppedState && qualities.length > 1

      readonly property list<int> qualities: {
        const cameraResolution = captureLoader.item ? captureLoader.item.cameraResolution : Qt.size(0, 0);
        const shorterSide = Math.min(cameraResolution.width, cameraResolution.height);
        return [360, 480, 720, 1080].filter(quality => quality < shorterSide).concat([0]);
      }

      readonly property int selectedQuality: qualities.indexOf(cameraSettings.videoQuality) >= 0 ? cameraSettings.videoQuality : 0
      readonly property size targetResolution: captureLoader.item ? cameraItem.videoResolutionForQuality(captureLoader.item.cameraResolution, selectedQuality) : Qt.size(0, 0)

      readonly property real availableWidth: cameraItem.isPortraitMode ? parent.width : parent.width - captureBar.width

      width: Math.min(availableWidth - 80, 320)
      height: videoQualityColumn.height + 20
      radius: 6
      color: QfTheme.darkGraySemiOpaque

      x: availableWidth / 2 - width / 2
      y: parent.height - height - 20 - (cameraItem.isPortraitMode ? captureBar.height : mainWindow.sceneBottomMargin)

      Column {
        id: videoQualityColumn
        anchors.centerIn: parent
        width: parent.width - 20
        spacing: 6

        Text {
          width: parent.width
          horizontalAlignment: Text.AlignHCenter
          elide: Text.ElideRight
          text: {
            const quality = videoQualityPanel.selectedQuality;
            const label = quality > 0 ? quality + 'p' : qsTr("Highest");
            const resolution = videoQualityPanel.targetResolution;
            return resolution.width > 0 ? label + ' — ' + resolution.width + ' × ' + resolution.height : label;
          }
          font: QfTheme.tipFont
          color: QfTheme.light
        }

        QfSlider {
          id: qualitySlider

          width: parent.width
          implicitHeight: 34
          showValueLabel: false

          from: 0
          to: videoQualityPanel.qualities.length - 1
          stepSize: 1
          snapMode: Slider.SnapAlways
          value: Math.max(0, videoQualityPanel.qualities.indexOf(videoQualityPanel.selectedQuality))

          onMoved: {
            cameraSettings.videoQuality = videoQualityPanel.qualities[value];
          }
        }
      }
    }

    QfToolButton {
      id: backButton

      anchors.left: parent.left
      anchors.leftMargin: mainWindow.sceneLeftMargin + 4
      anchors.top: parent.top
      anchors.topMargin: mainWindow.sceneTopMargin + 4

      iconSource: QfTheme.getThemeVectorIcon("ic_chevron_left_white_24dp")
      iconColor: QfTheme.toolButtonColor
      bgcolor: QfTheme.toolButtonBackgroundSemiOpaqueColor
      round: true

      onClicked: {
        if (cameraItem.state == "PhotoPreview") {
          cameraItem.userRotation = 0;
          cameraItem.userMirror = false;
          cameraItem.state = "PhotoCapture";
          cameraItem.discardCapture();
        } else if (cameraItem.state == "VideoPreview") {
          videoPreview.stop();
          cameraItem.state = "VideoCapture";
          cameraItem.discardCapture();
        } else {
          if (captureLoader.item && captureLoader.item.recorder.recorderState !== MediaRecorder.StoppedState) {
            captureLoader.item.recorder.stop();
          }
          cameraItem.discardCapture();
          cameraItem.canceled();
        }
      }
    }

    QfToolButtonDrawer {
      name: "cameraSettingsDrawer"
      visible: cameraItem.isReady
      anchors.left: parent.left
      anchors.leftMargin: mainWindow.sceneLeftMargin + 4
      anchors.top: backButton.bottom
      anchors.topMargin: 4

      iconSource: QfTheme.getThemeVectorIcon("ic_camera_settings_black_24dp")
      iconColor: QfTheme.toolButtonColor
      bgcolor: QfTheme.toolButtonBackgroundSemiOpaqueColor
      spacing: 4
      collapsed: false

      QfToolButton {
        id: cameraSelectionButton

        width: 40
        height: cameraSelectionMenu.count > 1 ? width : 0
        visible: cameraSelectionMenu.count
        padding: 2
        enabled: captureLoader.item

        iconSource: QfTheme.getThemeVectorIcon("ic_camera_switch_black_24dp")
        iconColor: QfTheme.toolButtonColor
        bgcolor: QfTheme.toolButtonBackgroundSemiOpaqueColor
        round: true

        onClicked: {
          cameraSelectionMenu.popup(cameraSelectionButton.x, cameraSelectionButton.y);
        }
      }

      QfToolButton {
        id: resolutionSelectionButton

        width: 40
        height: resolutionSelectionMenu.count > 1 ? width : 0
        visible: resolutionSelectionMenu.count
        padding: 2
        enabled: captureLoader.item

        iconSource: QfTheme.getThemeVectorIcon("ic_camera_resolution_black_24dp")
        iconColor: QfTheme.toolButtonColor
        bgcolor: QfTheme.toolButtonBackgroundSemiOpaqueColor
        round: true

        onClicked: {
          resolutionSelectionMenu.popup(resolutionSelectionButton.x, resolutionSelectionButton.y);
        }
      }

      QfToolButton {
        id: stampingButton

        width: 40
        height: 40
        padding: 2

        iconSource: QfTheme.getThemeVectorIcon("ic_text_black_24dp")
        iconColor: cameraSettings.stamping ? QfTheme.mainColor : QfTheme.toolButtonColor
        bgcolor: QfTheme.toolButtonBackgroundSemiOpaqueColor
        round: true

        onClicked: {
          cameraSettings.stamping = !cameraSettings.stamping;
          displayToast(cameraSettings.stamping ? qsTr("Details stamping enabled") : qsTr("Details stamping disabled"));
        }
      }

      QfToolButton {
        id: geotagButton

        width: 40
        height: 40
        padding: 2

        iconSource: positionSource.active ? QfTheme.getThemeVectorIcon("ic_geotag_white_24dp") : QfTheme.getThemeVectorIcon("ic_geotag_missing_white_24dp")
        iconColor: cameraSettings.geoTagging ? QfTheme.mainColor : QfTheme.toolButtonColor
        bgcolor: QfTheme.toolButtonBackgroundSemiOpaqueColor
        round: true

        onClicked: {
          cameraSettings.geoTagging = !cameraSettings.geoTagging;
          displayToast(cameraSettings.geoTagging ? qsTr("Geotagging enabled") : qsTr("Geotagging disabled"));
        }
      }

      QfToolButton {
        id: gridButton

        width: 40
        height: 40
        padding: 2

        iconSource: QfTheme.getThemeVectorIcon("ic_3x3_grid_white_24dp")
        iconColor: cameraSettings.showGrid ? QfTheme.mainColor : QfTheme.toolButtonColor
        bgcolor: QfTheme.toolButtonBackgroundSemiOpaqueColor
        round: true

        onClicked: {
          cameraSettings.showGrid = !cameraSettings.showGrid;
          displayToast(cameraSettings.showGrid ? qsTr("Grid enabled") : qsTr("Grid disabled"));
        }
      }
    }

    QfMenu {
      id: cameraSelectionMenu

      topMargin: mainWindow.sceneTopMargin
      bottomMargin: mainWindow.sceneBottomMargin
      z: 10000 // 1000s are embedded feature forms, use higher value

      Repeater {
        model: mediaDevices.videoInputs

        delegate: MenuItem {
          property string deviceId: modelData.id
          property bool isDefault: modelData.isDefault

          text: modelData.description + (modelData.position !== CameraDevice.UnspecifiedPosition ? ' (' + (modelData.position === CameraDevice.FrontFace ? qsTr('front') : qsTr('back')) + ')' : '')
          height: 48
          leftPadding: QfTheme.menuItemCheckLeftPadding
          font: QfTheme.defaultFont
          enabled: !checked && captureLoader.item
          checkable: true
          checked: deviceId == cameraSettings.deviceId || (isDefault && cameraSettings.deviceId == '')
          indicator.height: 20
          indicator.width: 20
          indicator.implicitHeight: 24
          indicator.implicitWidth: 24

          onToggled: {
            if (checked && cameraSettings.deviceId !== modelData.id) {
              cameraSettings.deviceId = modelData.id;
              if (captureLoader.item) {
                captureLoader.item.camera.cameraDevice = modelData;
                captureLoader.item.camera.applyCameraFormat();
              }
            }
          }
        }
      }
    }

    QfMenu {
      id: resolutionSelectionMenu

      topMargin: mainWindow.sceneTopMargin
      bottomMargin: mainWindow.sceneBottomMargin
      z: 10000 // 1000s are embedded feature forms, use higher value

      function ratioFromResolution(resolution) {
        let smallerValue = Math.min(resolution.width, resolution.height);
        let gdc = 0;
        for (let i = 1; i < smallerValue; i++) {
          if (resolution.width % i === 0 && resolution.height % i === 0) {
            gdc = i;
          }
        }
        return resolution.width / gdc + ':' + resolution.height / gdc;
      }

      function pixelFormatDescription(pixelFormat) {
        switch (pixelFormat) {
        case 13:
          return 'YUV420P';
        case 14:
          return 'YUV422P';
        case 17:
          return 'YUYV';
        case 29:
          return 'JPEG';
        }
        return '' + pixelFormat;
      }

      Repeater {
        model: captureLoader.item ? captureLoader.item.camera.cameraDevice.videoFormats : []

        delegate: MenuItem {
          property int pixelFormat: modelData.pixelFormat
          property size resolution: modelData.resolution

          text: {
            let details = [];
            let ratio = resolutionSelectionMenu.ratioFromResolution(resolution);
            if (ratio !== '') {
              details.push(ratio);
            }
            let description = resolutionSelectionMenu.pixelFormatDescription(pixelFormat);
            if (description !== '') {
              details.push(description);
            }
            return resolution.width + ' × ' + resolution.height + (details.length > 0 ? ' — ' + details.join(' / ') : '');
          }
          height: 48
          leftPadding: QfTheme.menuItemCheckLeftPadding
          font: QfTheme.defaultFont
          enabled: !checked && captureLoader.item
          checkable: true
          checked: cameraSettings.resolution == resolution && cameraSettings.pixelFormat == pixelFormat
          indicator.height: 20
          indicator.width: 20
          indicator.implicitHeight: 24
          indicator.implicitWidth: 24

          onToggled: {
            if (checked && (cameraSettings.resolution != resolution || cameraSettings.pixelFormat != pixelFormat)) {
              cameraSettings.resolution = resolution;
              cameraSettings.pixelFormat = pixelFormat;
              if (captureLoader.item) {
                captureLoader.item.camera.applyCameraFormat();
              }
            }
          }
        }
      }
    }

    Keys.onReleased: event => {
      if (event.key === Qt.Key_Back || event.key === Qt.Key_Escape) {
        event.accepted = true;
        backButton.clicked();
      }
    }
  }
}
