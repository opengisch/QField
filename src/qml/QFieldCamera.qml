import QtQuick
import QtQuick.Controls
import QtQuick.Shapes
import QtQuick.Window
import QtMultimedia
import QtCore

import org.qfield 1.0

import Theme 1.0

Popup {
  id: cameraItem
  z: 10000 // 1000s are embedded feature forms, use a higher value to insure feature form popups always show above embedded feature forms

  property bool isCapturing: state == "PhotoCapture" || state == "VideoCapture"
  property bool isPortraitMode: mainWindow.height > mainWindow.width

  property string currentPath
  property var currentPosition

  signal finished(string path)
  signal canceled()

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
      photoPreview.source = ''
      videoPreview.source = ''
    } else if (state == "VideoCapture") {
      photoPreview.source = ''
      videoPreview.source = ''
    }
  }

  onAboutToShow: {
    if (cameraPermission.status === Qt.PermissionStatus.Undetermined) {
      cameraPermission.request()
    }
    if (microphonePermission.status === Qt.PermissionStatus.Undetermined) {
      microphonePermission.request()
    }
  }

  QfCameraPermission {
    id: cameraPermission
  }
  QfMicrophonePermission {
    id: microphonePermission
  }

  Settings {
    id: settings
    property bool geoTagging: true
    property bool showGrid: false
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

        active: cameraItem.visible && cameraPermission.status === Qt.PermissionStatus.Granted
        cameraDevice: mediaDevices.defaultVideoInput

        function zoomIn(increase) {
          var zoom = camera.zoomFactor + increase
          if (zoom < camera.maximumZoomFactor) {
            camera.zoomFactor = zoom
          } else {
            camera.zoomFactor = camera.maximumZoomFactor
          }
        }

        function zoomOut(decrease) {
          var zoom = camera.zoomFactor - decrease;
          if (zoom > 1) {
            camera.zoomFactor = zoom
          } else {
            camera.zoomFactor = 1
          }
        }
      }
      videoOutput: videoOutput
      imageCapture: ImageCapture {
        id: imageCapture

        onImageSaved: (requestId, path) => {
          currentPath  = path
          photoPreview.source = 'file://'+path
          cameraItem.state = "PhotoPreview"
        }
      }
      recorder: MediaRecorder {
        id: recorder

        onRecorderStateChanged: {
          if (cameraItem.state == "VideoPreview" && recorderState === MediaRecorder.StoppedState) {
            videoPreview.source = captureSession.recorder.actualLocation
            videoPreview.play()
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
      visible: settings.showGrid
      anchors.centerIn: parent

      property bool isLandscape: (mainWindow.width / mainWindow.height) > (videoOutput.contentRect.width / videoOutput.contentRect.height)

      width: isLandscape
             ? videoOutput.contentRect.width * mainWindow.height / videoOutput.contentRect.height
             : mainWindow.width
      height: isLandscape
              ? mainWindow.height
              : videoOutput.contentRect.height * mainWindow.width / videoOutput.contentRect.width

      ShapePath {
        strokeColor: "#99000000"
        strokeWidth: 3
        fillColor: "transparent"

        startX: grid.width / 3
        startY: 0

        PathLine { x: grid.width / 3; y: grid.height }
        PathMove { x: grid.width / 3 * 2; y: 0 }
        PathLine { x: grid.width / 3 * 2; y: grid.height }
        PathMove { x: 0; y: grid.height / 3 }
        PathLine { x: grid.width; y: grid.height / 3 }
        PathMove { x: 0; y: grid.height / 3 * 2 }
        PathLine { x: grid.width; y: grid.height / 3 * 2 }
      }

      ShapePath {
        strokeColor: "#AAFFFFFF"
        strokeWidth: 1
        fillColor: "transparent"

        startX: grid.width / 3
        startY: 0

        PathLine { x: grid.width / 3; y: grid.height }
        PathMove { x: grid.width / 3 * 2; y: 0 }
        PathLine { x: grid.width / 3 * 2; y: grid.height }
        PathMove { x: 0; y: grid.height / 3 }
        PathLine { x: grid.width; y: grid.height / 3 }
        PathMove { x: 0; y: grid.height / 3 * 2 }
        PathLine { x: grid.width; y: grid.height / 3 * 2 }
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

      onPinchUpdated: (pinch) => {
                        if (pinch.scale > pinch.previousScale) {
                          camera.zoomIn(0.05)
                        } else {
                          camera.zoomOut(0.05)
                        }
                      }
    }

    WheelHandler {
      enabled: cameraItem.visible && cameraItem.isCapturing
      target: null
      grabPermissions: PointerHandler.CanTakeOverFromHandlersOfDifferentType | PointerHandler.ApprovesTakeOverByItems

      onWheel: (event) => {
        if (event.angleDelta.y > 0)
        {
          camera.zoomIn(0.25)
        }
        else
        {
          camera.zoomOut(0.25)
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
            border.color: cameraItem.state == "VideoCapture" && captureSession.recorder.recorderState !== MediaRecorder.StoppedState
                          ? "red"
                          : "white"
            border.width: 2

            QfToolButton {
              id: captureButton

              anchors.centerIn: parent
              visible: camera.cameraStatus == Camera.ActiveStatus ||
                       camera.cameraStatus == Camera.LoadedStatus ||
                       camera.cameraStatus == Camera.StandbyStatus

              round: true
              roundborder: true
              iconSource: cameraItem.state == "PhotoPreview" || cameraItem.state == "VideoPreview"
                          ? Theme.getThemeIcon("ic_check_white_48dp")
                          : ''
              bgcolor: cameraItem.state == "PhotoPreview" || cameraItem.state == "VideoPreview"
                       ? Theme.mainColor
                       : cameraItem.state == "VideoCapture" ? "red" : "white"

              onClicked: {
                if (cameraItem.state == "PhotoCapture") {
                  captureSession.imageCapture.captureToFile(qgisProject.homePath+ '/DCIM/')
                  currentPosition = positionSource.positionInformation
                } else if (cameraItem.state == "VideoCapture") {
                  if (captureSession.recorder.recorderState === MediaRecorder.StoppedState) {
                    captureSession.recorder.record()
                  } else {
                    cameraItem.state = "VideoPreview"
                    captureSession.recorder.stop()
                    var path = captureSession.recorder.actualLocation.toString()
                    var filePos = path.indexOf('file://')
                    currentPath = filePos === 0 ? path.substring(7) : path
                  }
                } else if (cameraItem.state == "PhotoPreview" || cameraItem.state == "VideoPreview") {
                  if (cameraItem.state == "PhotoPreview") {
                    if (settings.geoTagging && positionSource.active) {
                      FileUtils.addImageMetadata(currentPath, currentPosition)
                    }
                  }
                  cameraItem.finished(currentPath)
                }
              }
            }
          }

          QfToolButton {
            id: zoomButton
            visible: cameraItem.isCapturing && (camera.maximumZoomFactor !== 1.0 || camera.minimumZoomFactor !== 1.0)

            x: cameraItem.isPortraitMode ? (parent.width / 4) - (width / 2) : (parent.width - width) / 2
            y: cameraItem.isPortraitMode ? (parent.height - height) / 2 : (parent.height / 4) * 3 - (height / 2)

            iconColor: "white"
            bgcolor: Theme.darkGraySemiOpaque
            round: true

            text: camera.zoomFactor.toFixed(1) +'X'
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
              switch(camera.flashMode) {
              case Camera.FlashAuto:
                return Theme.getThemeVectorIcon('ic_flash_auto_black_24dp');
              case Camera.FlashOn:
                return Theme.getThemeVectorIcon('ic_flash_on_black_24dp');
              case Camera.FlashOff:
                return Theme.getThemeVectorIcon('ic_flash_off_black_24dp');
              default:
                return'';
              }
            }
            iconColor: "white"
            bgcolor: Qt.hsla(Theme.darkGray.hslHue, Theme.darkGray.hslSaturation, Theme.darkGray.hslLightness, 0.5)
            round: true

            onClicked: {
              if (camera.flashMode === Camera.FlashOff) {
                camera.flashMode = Camera.FlashOn;
              } else {
                camera.flashMode = Camera.FlashOff
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
                  return hours.padStart(2,'0') + ':' + minutes.padStart(2,'0') + ':' + seconds.padStart(2,'0');
                } else {
                  // tiny bit of a cheat here as the first second isn't triggered
                  return '00:00:01';
                }
              }
              color: 'white'
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

      iconSource: Theme.getThemeIcon("ic_chevron_left_white_24dp")
      iconColor: "white"
      bgcolor: Qt.hsla(Theme.darkGray.hslHue, Theme.darkGray.hslSaturation, Theme.darkGray.hslLightness, 0.5)
      round: true

      onClicked: {
        if (cameraItem.state == "PhotoPreview") {
          cameraItem.state = "PhotoCapture"
        } else if (cameraItem.state == "VideoPreview") {
          videoPreview.stop()
          cameraItem.state = "VideoCapture"
        } else {
          if (currentPath != '') {
            platformUtilities.rmFile(currentPath)
          }
          cameraItem.canceled()
        }
      }
    }

    QfToolButton {
      id: geotagButton

      anchors.left: parent.left
      anchors.leftMargin: 4
      anchors.top: backButton.bottom
      anchors.topMargin: 4

      iconSource: positionSource.active ? Theme.getThemeIcon("ic_geotag_24dp") : Theme.getThemeIcon("ic_geotag_missing_24dp")
      iconColor: positionSource.active && settings.geoTagging ? Theme.mainColor : "white"
      bgcolor: Theme.darkGraySemiOpaque
      round: true

      onClicked: {
        if (positionSource.active) {
          settings.geoTagging = !settings.geoTagging
          displayToast(settings.geoTagging ? qsTr("Geotagging enabled") : qsTr("Geotagging disabled"))
        }
      }
    }

    QfToolButton {
      id: gridButton

      anchors.left: parent.left
      anchors.leftMargin: 4
      anchors.top: geotagButton.bottom
      anchors.topMargin: 4

      iconSource: Theme.getThemeVectorIcon("ic_3x3_grid_white_24dp")
      iconColor: settings.showGrid ? Theme.mainColor : "white"
      bgcolor: Theme.darkGraySemiOpaque
      round: true

      onClicked: {
        settings.showGrid = !settings.showGrid
        displayToast(settings.showGrid ? qsTr("Grid enabled") : qsTr("Grid disabled"))
      }
    }
  }
}
