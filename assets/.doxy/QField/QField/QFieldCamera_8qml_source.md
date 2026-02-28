

# File QFieldCamera.qml

[**File List**](files.md) **>** [**qml**](dir_1d81f7f4362e4398c0a695c4e466fcb0.md) **>** [**QFieldCamera.qml**](QFieldCamera_8qml.md)

[Go to the documentation of this file](QFieldCamera_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Shapes
import QtQuick.Window
import QtMultimedia
import QtCore
import org.qfield
import Theme

Popup {
  id: cameraItem
  z: 10000 // 1000s are embedded feature forms, use a higher value to insure feature form popups always show above embedded feature forms

  property bool isCapturing: state == "PhotoCapture" || state == "VideoCapture"
  property bool isPortraitMode: mainWindow.height > mainWindow.width

  property string currentPath: ''
  property var currentPosition: PositioningUtils.createEmptyGnssPositionInformation()
  property var currentProjectedPosition: undefined

  property bool captureLoaderActivated: false

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
  }

  ExpressionEvaluator {
    id: stampExpressionEvaluator

    property string defaultTextTemplate: "[% format_date(now(), 'yyyy-MM-dd @ HH:mm') || if(@gnss_coordinate is not null, format('\n" + qsTr("Latitude") + " %1 | " + qsTr("Longitude") + " %2 | " + qsTr("Altitude") + " %3\n" + qsTr("Speed") + " %4 | " + qsTr("Orientation") + " %5', coalesce(format_number(y(@gnss_coordinate), 7), 'N/A'), coalesce(format_number(x(@gnss_coordinate), 7), 'N/A'), coalesce(format_number(@corrected_elevation, 3) || ' ' || @corrected_elevation_unit, 'N/A'), if(@gnss_ground_speed != 'nan', format_number(@gnss_ground_speed, 3) || ' m/s', 'N/A'), if(@gnss_orientation != 'nan', format_number(@gnss_orientation, 1) || ' °', 'N/A')), '') %]"

    mode: ExpressionEvaluator.ExpressionTemplateMode
    expressionText: ""

    project: qgisProject
    appExpressionContextScopesGenerator: AppExpressionContextScopesGenerator {
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

    background: Rectangle {
      anchors.fill: parent
      color: "#000000"
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
          anchors.fill: parent

          property alias captureSession: captureSession
          property alias camera: camera
          property alias imageCapture: imageCapture
          property alias recorder: recorder
          property alias videoOutput: videoOutput   // expose it

          VideoOutput {
            id: videoOutput
            anchors.fill: parent
            visible: cameraItem.state == "PhotoCapture" || cameraItem.state == "VideoCapture"
          }

          CaptureSession {
            id: captureSession

            camera: Camera {
              id: camera
              property bool restarting: false
              active: cameraItem.visible && cameraPermission.status === Qt.PermissionStatus.Granted && !restarting

              function applyCameraFormat() {
                if (cameraSettings.pixelFormat != 0) {
                  let fallbackIndex = -1;
                  let i = 0;
                  for (let format of camera.cameraDevice.videoFormats) {
                    if (format.resolution === cameraSettings.resolution && format.pixelFormat === cameraSettings.pixelFormat) {
                      camera.cameraFormat = format;
                      fallbackIndex = -1;
                      break;
                    } else if (format.resolution === cameraSettings.resolution) {
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

              onImageSaved: (requestId, path) => {
                currentPath = path;
              }

              onPreviewChanged: {
                cameraItem.state = "PhotoPreview";
                photoPreview.source = "";
                Qt.callLater(() => photoPreview.source = imageCapture.preview);
              }
            }

            recorder: MediaRecorder {
              id: recorder

              onRecorderStateChanged: {
                if (cameraItem.state == "VideoPreview" && recorderState === MediaRecorder.StoppedState) {
                  videoPreview.source = recorder.actualLocation;
                  videoPreview.play();
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
            if (device.id === cameraSettings.deviceId) {
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
      focus: visible
    }

    PinchArea {
      id: pinchArea
      enabled: cameraItem.visible && cameraItem.isCapturing && captureLoader.item
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
      enabled: cameraItem.visible && cameraItem.isCapturing && captureLoader.item
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
      width: cameraItem.isPortraitMode ? parent.width : 100 + mainWindow.sceneBottomMargin
      height: cameraItem.isPortraitMode ? 100 + mainWindow.sceneRightMargin : parent.height
      x: cameraItem.isPortraitMode ? 0 : parent.width - width
      y: cameraItem.isPortraitMode ? parent.height - height : 0

      color: Theme.darkGraySemiOpaque

      Rectangle {
        width: cameraItem.isPortraitMode ? parent.width : 100 + mainWindow.sceneBottomMargin
        height: cameraItem.isPortraitMode ? 100 + mainWindow.sceneRightMargin : parent.height
        x: cameraItem.isPortraitMode ? 0 : parent.width - width
        y: cameraItem.isPortraitMode ? parent.height - height : 0

        color: Theme.darkGraySemiOpaque

        Rectangle {
          anchors.top: parent.top
          width: cameraItem.isPortraitMode ? parent.width : parent.width - mainWindow.sceneRightMargin
          height: cameraItem.isPortraitMode ? parent.height - mainWindow.sceneBottomMargin : parent.height
          color: "transparent"

          Rectangle {
            id: captureRing
            anchors.centerIn: parent
            width: 64
            height: 64
            radius: 32
            color: Theme.darkGraySemiOpaque
            border.color: cameraItem.state == "VideoCapture" && captureLoader.item && captureLoader.item.recorder.recorderState !== MediaRecorder.StoppedState ? "red" : "white"
            border.width: 2

            QfToolButton {
              id: captureButton

              anchors.centerIn: parent
              visible: captureLoader.status == Loader.Ready && captureLoader.item

              round: true
              roundborder: true
              iconSource: cameraItem.state == "PhotoPreview" || cameraItem.state == "VideoPreview" ? Theme.getThemeVectorIcon("ic_check_white_24dp") : ''
              iconColor: Theme.toolButtonColor
              bgcolor: cameraItem.state == "PhotoPreview" || cameraItem.state == "VideoPreview" ? Theme.mainColor : cameraItem.state == "VideoCapture" ? "red" : "white"

              onClicked: {
                if (!captureLoader.item)
                  return;
                if (cameraItem.state == "PhotoCapture") {
                  platformUtilities.createDir(qgisProject.homePath, 'DCIM');
                  captureLoader.item.imageCapture.captureToFile(qgisProject.homePath + '/DCIM/');
                  captureFlashAnimation.start();
                  if (positionSource.active) {
                    currentPosition = positionSource.positionInformation;
                    currentProjectedPosition = positionSource.projectedPosition;
                  } else {
                    currentPosition = PositioningUtils.createEmptyGnssPositionInformation();
                    currentProjectedPosition = undefined;
                  }
                } else if (cameraItem.state == "VideoCapture") {
                  if (captureLoader.item.recorder.recorderState === MediaRecorder.StoppedState) {
                    captureLoader.item.recorder.record();
                  } else {
                    cameraItem.state = "VideoPreview";
                    captureLoader.item.recorder.stop();
                    const path = captureLoader.item.recorder.actualLocation.toString();
                    currentPath = UrlUtils.toLocalFile(path);
                  }
                } else if (cameraItem.state == "PhotoPreview" || cameraItem.state == "VideoPreview") {
                  if (!currentPath || currentPath === "")
                    return;
                  if (cameraItem.state == "PhotoPreview") {
                    if (cameraSettings.geoTagging && positionSource.active) {
                      FileUtils.addImageMetadata(currentPath, currentPosition);
                    }
                    if (cameraSettings.stamping || iface.readProjectBoolEntry("qfieldsync", "forceStamping")) {
                      stampExpressionEvaluator.expressionText = iface.readProjectEntry("qfieldsync", "stampingDetailsTemplate", stampExpressionEvaluator.defaultTextTemplate);
                      if (stampExpressionEvaluator.expressionText === "") {
                        stampExpressionEvaluator.expressionText = stampExpressionEvaluator.defaultTextTemplate;
                      }
                      FileUtils.addImageStamp(currentPath, stampExpressionEvaluator.evaluate(), iface.readProjectEntry("qfieldsync", "stampingFontStyle"), iface.readProjectNumEntry("qfieldsync", "stampingHorizontalAlignment", 0), iface.readProjectEntry("qfieldsync", "stampingImageDecoration"));
                    }
                  }
                  cameraItem.finished(currentPath);
                }
              }
            }
          }

          QfToolButton {
            id: zoomButton
            visible: cameraItem.isCapturing && captureLoader.item && (captureLoader.item.camera.maximumZoomFactor !== 1.0 || captureLoader.item.camera.minimumZoomFactor !== 1.0)

            x: cameraItem.isPortraitMode ? (parent.width / 4) - (width / 2) : (parent.width - width) / 2
            y: cameraItem.isPortraitMode ? (parent.height - height) / 2 : (parent.height / 4) * 3 - (height / 2)

            iconColor: Theme.toolButtonColor
            bgcolor: Theme.toolButtonBackgroundSemiOpaqueColor
            round: true

            text: captureLoader.item ? captureLoader.item.camera.zoomFactor.toFixed(1) + 'X' : '1.0X'
            font: Theme.tinyFont

            onClicked: {
              if (captureLoader.item) {
                captureLoader.item.camera.zoomFactor = 1;
              }
            }
          }

          QfToolButton {
            id: flashButton
            visible: cameraItem.isCapturing && captureLoader.item && captureLoader.item.camera.isFlashModeSupported(Camera.FlashOn)

            x: cameraItem.isPortraitMode ? (parent.width / 4) * 3 - (width / 2) : (parent.width - width) / 2
            y: cameraItem.isPortraitMode ? (parent.height - height) / 2 : (parent.height / 4) - (height / 2)

            iconSource: {
              if (!captureLoader.item)
                return '';
              switch (captureLoader.item.camera.flashMode) {
              case Camera.FlashAuto:
                return Theme.getThemeVectorIcon('ic_flash_auto_black_24dp');
              case Camera.FlashOn:
                return Theme.getThemeVectorIcon('ic_flash_on_black_24dp');
              case Camera.FlashOff:
                return Theme.getThemeVectorIcon('ic_flash_off_black_24dp');
              default:
                return '';
              }
            }
            iconColor: Theme.toolButtonColor
            bgcolor: Theme.toolButtonBackgroundSemiOpaqueColor
            round: true

            onClicked: {
              if (!captureLoader.item)
                return;
              if (captureLoader.item.camera.flashMode === Camera.FlashOff) {
                captureLoader.item.camera.flashMode = Camera.FlashOn;
              } else {
                captureLoader.item.camera.flashMode = Camera.FlashOff;
              }
            }
          }

          Rectangle {
            visible: cameraItem.state == "VideoCapture" && captureLoader.item && captureLoader.item.recorder.recorderState !== MediaRecorder.StoppedState

            x: cameraItem.isPortraitMode ? captureRing.x + captureRing.width / 2 - width / 2 : captureRing.x + captureRing.width / 2 - width / 2
            y: cameraItem.isPortraitMode ? captureRing.y - height - 20 : captureRing.y - height - 20

            width: durationLabelMetrics.boundingRect('00:00:00').width + 20
            height: durationLabelMetrics.boundingRect('00:00:00').height + 10
            radius: 6

            color: 'red'

            Text {
              id: durationLabel
              anchors.centerIn: parent
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
              color: "white"
            }

            FontMetrics {
              id: durationLabelMetrics
              font: durationLabel.font
            }
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

      iconSource: Theme.getThemeVectorIcon("ic_chevron_left_white_24dp")
      iconColor: Theme.toolButtonColor
      bgcolor: Theme.toolButtonBackgroundSemiOpaqueColor
      round: true

      onClicked: {
        if (cameraItem.state == "PhotoPreview") {
          cameraItem.state = "PhotoCapture";
        } else if (cameraItem.state == "VideoPreview") {
          videoPreview.stop();
          cameraItem.state = "VideoCapture";
        } else {
          if (currentPath != '') {
            platformUtilities.rmFile(currentPath);
          }
          cameraItem.canceled();
        }
      }
    }

    QfToolButtonDrawer {
      name: "cameraSettingsDrawer"

      anchors.left: parent.left
      anchors.leftMargin: mainWindow.sceneLeftMargin + 4
      anchors.top: backButton.bottom
      anchors.topMargin: 4

      iconSource: Theme.getThemeVectorIcon("ic_camera_settings_black_24dp")
      iconColor: Theme.toolButtonColor
      bgcolor: Theme.toolButtonBackgroundSemiOpaqueColor
      spacing: 4
      collapsed: false

      QfToolButton {
        id: cameraSelectionButton

        width: 40
        height: cameraSelectionMenu.count > 1 ? width : 0
        visible: cameraSelectionMenu.count
        padding: 2
        enabled: captureLoader.item

        iconSource: Theme.getThemeVectorIcon("ic_camera_switch_black_24dp")
        iconColor: Theme.toolButtonColor
        bgcolor: Theme.toolButtonBackgroundSemiOpaqueColor
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

        iconSource: Theme.getThemeVectorIcon("ic_camera_resolution_black_24dp")
        iconColor: Theme.toolButtonColor
        bgcolor: Theme.toolButtonBackgroundSemiOpaqueColor
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

        iconSource: Theme.getThemeVectorIcon("ic_text_black_24dp")
        iconColor: cameraSettings.stamping ? Theme.mainColor : Theme.toolButtonColor
        bgcolor: Theme.toolButtonBackgroundSemiOpaqueColor
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

        iconSource: positionSource.active ? Theme.getThemeVectorIcon("ic_geotag_white_24dp") : Theme.getThemeVectorIcon("ic_geotag_missing_white_24dp")
        iconColor: cameraSettings.geoTagging ? Theme.mainColor : Theme.toolButtonColor
        bgcolor: Theme.toolButtonBackgroundSemiOpaqueColor
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

        iconSource: Theme.getThemeVectorIcon("ic_3x3_grid_white_24dp")
        iconColor: cameraSettings.showGrid ? Theme.mainColor : Theme.toolButtonColor
        bgcolor: Theme.toolButtonBackgroundSemiOpaqueColor
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
          leftPadding: Theme.menuItemCheckLeftPadding
          font: Theme.defaultFont
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
          leftPadding: Theme.menuItemCheckLeftPadding
          font: Theme.defaultFont
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
  }
}
```


