import QtQuick
import QtQuick.Controls
import QtQuick.Shapes
import QtQuick.Window
import QtMultimedia
import QtCore
import org.qfield
import Theme

/**
 * \ingroup qml
 */
Popup {
  id: cameraItem
  z: 10000 // 1000s are embedded feature forms, use a higher value to insure feature form popups always show above embedded feature forms

  property bool isCapturing: state == "PhotoCapture" || state == "VideoCapture"
  property bool isPortraitMode: mainWindow.height > mainWindow.width

  property string currentPath: ''
  property var currentPosition: PositioningUtils.createEmptyGnssPositionInformation()

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
    if (cameraPermission.status === Qt.PermissionStatus.Undetermined) {
      cameraPermission.request();
    } else if (microphonePermission.status === Qt.PermissionStatus.Undetermined) {
      microphonePermission.request();
    }
    recorder.mediaFormat.audioCodec = MediaFormat.AudioCodec.AAC;
    recorder.mediaFormat.videoCodec = MediaFormat.VideoCodec.H264;
    recorder.mediaFormat.fileFormat = MediaFormat.MPEG4;
  }

  Component.onCompleted: {
    let cameraPicked = false;
    if (cameraSettings.deviceId != '') {
      for (const device of mediaDevices.videoInputs) {
        if (device.id == cameraSettings.deviceId) {
          camera.cameraDevice = device;
          cameraPicked = true;
        }
      }
    }
    if (!cameraPicked) {
      camera.cameraDevice = mediaDevices.defaultVideoInput;
    }
    camera.applyCameraFormat(false);
  }

  QfCameraPermission {
    id: cameraPermission

    onStatusChanged: {
      if (microphonePermission.status === Qt.PermissionStatus.Undetermined) {
        microphonePermission.request();
      }
    }
  }
  QfMicrophonePermission {
    id: microphonePermission

    onStatusChanged: {
      if (cameraPermission.status === Qt.PermissionStatus.Undetermined) {
        cameraPermission.request();
      }
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

    mode: ExpressionEvaluator.ExpressionMode
    expressionText: "format_date(now(), 'yyyy-MM-dd @ HH:mm') || if(@gnss_coordinate is not null, format('\n" + qsTr("Latitude") + " %1 | " + qsTr("Longitude") + " %2 | " + qsTr("Altitude") + " %3\n" + qsTr("Speed") + " %4 | " + qsTr("Orientation") + " %5', coalesce(format_number(y(@gnss_coordinate), 7), 'N/A'), coalesce(format_number(x(@gnss_coordinate), 7), 'N/A'), coalesce(format_number(z(@gnss_coordinate), 3) || ' m', 'N/A'), if(@gnss_ground_speed != 'nan', format_number(@gnss_ground_speed, 3) || ' m/s', 'N/A'), if(@gnss_orientation != 'nan', format_number(@gnss_orientation, 1) || ' °', 'N/A')), '')"

    project: qgisProject
    positionInformation: currentPosition
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

    CaptureSession {
      id: captureSession

      camera: Camera {
        id: camera

        property bool restarting: false
        active: cameraItem.visible && cameraPermission.status === Qt.PermissionStatus.Granted && !restarting

        function applyCameraFormat(restart) {
          if (cameraSettings.pixelFormat != 0) {
            let fallbackIndex = -1;
            let i = 0;
            for (let format of camera.cameraDevice.videoFormats) {
              if (format.resolution == cameraSettings.resolution && format.pixelFormat == cameraSettings.pixelFormat) {
                camera.cameraFormat = format;
                fallbackIndex = -1;
                break;
              } else if (format.resolution == cameraSettings.resolution) {
                // If we can't match the pixel format and resolution, go for resolution match across devices
                fallbackIndex = i;
              }
              i++;
            }
            if (fallbackIndex >= 0) {
              camera.cameraFormat = camera.cameraDevice.videoFormats[fallbackIndex];
            }
            if (restart) {
              camera.restarting = true;
              camera.restarting = false;
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
          photoPreview.source = UrlUtils.fromString(path);
          cameraItem.state = "PhotoPreview";
        }
      }
      recorder: MediaRecorder {
        id: recorder

        onRecorderStateChanged: {
          if (cameraItem.state == "VideoPreview" && recorderState === MediaRecorder.StoppedState) {
            videoPreview.source = captureSession.recorder.actualLocation;
            videoPreview.play();
          }
        }
      }
    }

    VideoOutput {
      id: videoOutput
      anchors.fill: parent
      visible: cameraItem.state == "PhotoCapture" || cameraItem.state == "VideoCapture"
    }

    Shape {
      id: grid
      visible: cameraSettings.showGrid
      anchors.centerIn: parent

      property bool isLandscape: (mainWindow.width / mainWindow.height) > (videoOutput.contentRect.width / videoOutput.contentRect.height)

      width: isLandscape ? videoOutput.contentRect.width * mainWindow.height / videoOutput.contentRect.height : mainWindow.width
      height: isLandscape ? mainWindow.height : videoOutput.contentRect.height * mainWindow.width / videoOutput.contentRect.width

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

      fillMode: Image.PreserveAspectFit
      smooth: true
      focus: visible
    }

    PinchArea {
      id: pinchArea
      enabled: cameraItem.visible && cameraItem.isCapturing
      anchors.fill: parent

      onPinchUpdated: pinch => {
        if (pinch.scale > pinch.previousScale) {
          camera.zoomIn(0.05);
        } else {
          camera.zoomOut(0.05);
        }
      }
    }

    WheelHandler {
      enabled: cameraItem.visible && cameraItem.isCapturing
      target: null
      grabPermissions: PointerHandler.CanTakeOverFromHandlersOfDifferentType | PointerHandler.ApprovesTakeOverByItems

      onWheel: event => {
        if (event.angleDelta.y > 0) {
          camera.zoomIn(0.25);
        } else {
          camera.zoomOut(0.25);
        }
      }
    }

    Rectangle {
      x: cameraItem.isPortraitMode ? 0 : parent.width - 100
      y: cameraItem.isPortraitMode ? parent.height - 100 : 0
      width: cameraItem.isPortraitMode ? parent.width : 100
      height: cameraItem.isPortraitMode ? 100 : parent.height

      color: Theme.darkGraySemiOpaque

      Rectangle {
        x: cameraItem.isPortraitMode ? 0 : parent.width - 100
        y: cameraItem.isPortraitMode ? parent.height - 100 - mainWindow.sceneBottomMargin : 0
        width: cameraItem.isPortraitMode ? parent.width : 100
        height: cameraItem.isPortraitMode ? 100 + mainWindow.sceneBottomMargin : parent.height

        color: Theme.darkGraySemiOpaque

        Rectangle {
          anchors.top: parent.top
          width: parent.width
          height: cameraItem.isPortraitMode ? parent.height - mainWindow.sceneBottomMargin : parent.height
          color: "transparent"

          Rectangle {
            id: captureRing
            anchors.centerIn: parent
            width: 64
            height: 64
            radius: 32
            color: Theme.darkGraySemiOpaque
            border.color: cameraItem.state == "VideoCapture" && captureSession.recorder.recorderState !== MediaRecorder.StoppedState ? "red" : "white"
            border.width: 2

            QfToolButton {
              id: captureButton

              anchors.centerIn: parent
              visible: camera.cameraStatus == Camera.ActiveStatus || camera.cameraStatus == Camera.LoadedStatus || camera.cameraStatus == Camera.StandbyStatus

              round: true
              roundborder: true
              iconSource: cameraItem.state == "PhotoPreview" || cameraItem.state == "VideoPreview" ? Theme.getThemeVectorIcon("ic_check_white_24dp") : ''
              iconColor: Theme.toolButtonColor
              bgcolor: cameraItem.state == "PhotoPreview" || cameraItem.state == "VideoPreview" ? Theme.mainColor : cameraItem.state == "VideoCapture" ? "red" : "white"

              onClicked: {
                if (cameraItem.state == "PhotoCapture") {
                  captureSession.imageCapture.captureToFile(qgisProject.homePath + '/DCIM/');
                  if (positionSource.active) {
                    currentPosition = positionSource.positionInformation;
                  } else {
                    currentPosition = PositioningUtils.createEmptyGnssPositionInformation();
                  }
                  if (cameraSettings.geoTagging && !positionSource.active) {
                    displayToast(qsTr("Image geotagging requires positioning to be turned on"), "warning");
                  }
                } else if (cameraItem.state == "VideoCapture") {
                  if (captureSession.recorder.recorderState === MediaRecorder.StoppedState) {
                    captureSession.recorder.record();
                  } else {
                    cameraItem.state = "VideoPreview";
                    captureSession.recorder.stop();
                    var path = captureSession.recorder.actualLocation.toString();
                    var filePos = path.indexOf('file://');
                    currentPath = filePos === 0 ? path.substring(7) : path;
                  }
                } else if (cameraItem.state == "PhotoPreview" || cameraItem.state == "VideoPreview") {
                  if (cameraItem.state == "PhotoPreview") {
                    if (cameraSettings.geoTagging && positionSource.active) {
                      FileUtils.addImageMetadata(currentPath, currentPosition);
                    }
                    if (cameraSettings.stamping) {
                      FileUtils.addImageStamp(currentPath, stampExpressionEvaluator.evaluate());
                    }
                  }
                  cameraItem.finished(currentPath);
                }
              }
            }
          }

          QfToolButton {
            id: zoomButton
            visible: cameraItem.isCapturing && (camera.maximumZoomFactor !== 1.0 || camera.minimumZoomFactor !== 1.0)

            x: cameraItem.isPortraitMode ? (parent.width / 4) - (width / 2) : (parent.width - width) / 2
            y: cameraItem.isPortraitMode ? (parent.height - height) / 2 : (parent.height / 4) * 3 - (height / 2)

            iconColor: Theme.toolButtonColor
            bgcolor: Theme.toolButtonBackgroundSemiOpaqueColor
            round: true

            text: camera.zoomFactor.toFixed(1) + 'X'
            font: Theme.tinyFont

            onClicked: {
              camera.zoomFactor = 1;
            }
          }

          QfToolButton {
            id: flashButton
            visible: cameraItem.isCapturing && camera.isFlashModeSupported(Camera.FlashOn)

            x: cameraItem.isPortraitMode ? (parent.width / 4) * 3 - (width / 2) : (parent.width - width) / 2
            y: cameraItem.isPortraitMode ? (parent.height - height) / 2 : (parent.height / 4) - (height / 2)

            iconSource: {
              switch (camera.flashMode) {
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
              if (camera.flashMode === Camera.FlashOff) {
                camera.flashMode = Camera.FlashOn;
              } else {
                camera.flashMode = Camera.FlashOff;
              }
            }
          }

          Rectangle {
            visible: cameraItem.state == "VideoCapture" && captureSession.recorder.recorderState !== MediaRecorder.StoppedState

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
                if (captureSession.recorder.duration > 0) {
                  var seconds = Math.ceil(captureSession.recorder.duration / 1000);
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
      anchors.leftMargin: 4
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
      anchors.leftMargin: 4
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

      topMargin: sceneTopMargin
      bottomMargin: sceneBottomMargin
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
          enabled: !checked
          checkable: true
          checked: deviceId == cameraSettings.deviceId || (isDefault && cameraSettings.deviceId == '')
          indicator.height: 20
          indicator.width: 20
          indicator.implicitHeight: 24
          indicator.implicitWidth: 24

          onCheckedChanged: {
            if (checked && cameraSettings.deviceId !== modelData.id) {
              cameraSettings.deviceId = modelData.id;
              camera.cameraDevice = modelData;
              camera.applyCameraFormat(true);
            }
          }
        }
      }
    }

    QfMenu {
      id: resolutionSelectionMenu

      topMargin: sceneTopMargin
      bottomMargin: sceneBottomMargin
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
        model: camera.cameraDevice.videoFormats

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
          enabled: !checked
          checkable: true
          checked: cameraSettings.resolution == resolution && cameraSettings.pixelFormat == pixelFormat
          indicator.height: 20
          indicator.width: 20
          indicator.implicitHeight: 24
          indicator.implicitWidth: 24

          onCheckedChanged: {
            if (checked && (cameraSettings.resolution != resolution || cameraSettings.pixelFormat != pixelFormat)) {
              cameraSettings.resolution = resolution;
              cameraSettings.pixelFormat = pixelFormat;
              camera.applyCameraFormat(true);
            }
          }
        }
      }
    }
  }
}
