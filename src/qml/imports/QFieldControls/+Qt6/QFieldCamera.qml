import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Window 2.14
import QtMultimedia

import Theme 1.0

Popup {
  id: cameraItem
  z: 10000 // 1000s are embedded feature forms, use a higher value to insure feature form popups always show above embedded feature forms

  property bool isCapturing: state == "PhotoCapture" || state == "VideoCapture"
  property bool isPortraitMode: mainWindow.height > mainWindow.width

  property string currentPath

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

        active: cameraItem.visible
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
      }
    }

    VideoOutput {
      id: videoOutput
      anchors.fill: parent
      visible: cameraItem.state == "PhotoCapture" || cameraItem.state == "VideoCapture"
    }

    PinchHandler {
      enabled: cameraItem.visible && cameraItem.isCapturing
      target: null
      acceptedDevices: PointerDevice.TouchScreen | PointerDevice.TouchPad

      property real oldScale: 1.0

      onActiveChanged: {
        if (active) {
          oldScale = 1.0
        }
      }

      onActiveScaleChanged: {
        if (activeScale > oldScale) {
          camera.zoomIn(0.05)
        } else {
          camera.zoomOut(0.05)
        }
        oldScale = activeScale
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

    Video {
      id: videoPreview

      visible: cameraItem.state == "VideoPreview"

      anchors.fill: parent

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

    Rectangle {
      x: cameraItem.isPortraitMode ? 0 : parent.width - 100
      y: cameraItem.isPortraitMode ? parent.height - 100 : 0
      width: cameraItem.isPortraitMode ? parent.width : 100
      height: cameraItem.isPortraitMode ? 100 : parent.height

      color: Qt.hsla(Theme.darkGray.hslHue, Theme.darkGray.hslSaturation, Theme.darkGray.hslLightness, 0.3)

      Rectangle {
        id: captureRing
        anchors.centerIn: parent
        width: 64
        height: 64
        radius: 32
        color: Qt.hsla(Theme.darkGray.hslHue, Theme.darkGray.hslSaturation, Theme.darkGray.hslLightness, 0.3)
        border.color: cameraItem.state == "VideoCapture" && captureSession.recorder.recorderState != CameraRecorder.StoppedState
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
            } else if (cameraItem.state == "VideoCapture") {
              if (captureSession.recorder.recorderState == CameraRecorder.StoppedState) {
                captureSession.recorder.record()
              } else {
                captureSession.recorder.stop()
                videoPreview.source = captureSession.recorder.actualLocation
                var path = captureSession.recorder.actualLocation.toString()
                var filePos = path.indexOf('file://')
                currentPath = filePos === 0 ? path.substring(7) : path
                cameraItem.state = "VideoPreview"
              }
            } else if (cameraItem.state == "PhotoPreview" || cameraItem.state == "VideoPreview") {
              cameraItem.finished(currentPath)
            }
          }
        }
      }

      QfToolButton {
        id: zoomButton
        visible: cameraItem.isCapturing

        x: cameraItem.isPortraitMode ? (parent.width / 4) - (width / 2) : (parent.width - width) / 2
        y: cameraItem.isPortraitMode ? (parent.height - height) / 2 : (parent.height / 4) * 3 - (height / 2)

        iconColor: "white"
        bgcolor: Qt.hsla(Theme.darkGray.hslHue, Theme.darkGray.hslSaturation, Theme.darkGray.hslLightness, 0.3)
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
        visible: cameraItem.state == "VideoCapture" && captureSession.recorder.recorderState != CameraRecorder.StoppedState

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
        // Oddly enough, we can't reset a video capture
        if (cameraItem.state == "PhotoPreview") {
          cameraItem.state = "PhotoCapture"
        } else {
          if (currentPath != '') {
            platformUtilities.rmFile(currentPath)
          }
          cameraItem.canceled()
        }
      }
    }
  }
}
