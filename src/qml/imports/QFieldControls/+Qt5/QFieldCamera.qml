import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Window 2.14
import QtMultimedia 5.14
import Qt.labs.settings 1.0

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

  parent: ApplicationWindow.overlay
  x: 0
  y: 0
  height: parent.height
  width: parent.width
  padding: 0

  modal: true
  focus: true

  property string state: "PhotoCapture"
  onStateChanged: {
    if (state == "PhotoCapture") {
      camera.captureMode = Camera.CaptureStillImage
      photoPreview.source = ''
    } else if (state == "VideoCapture") {
      camera.captureMode = Camera.CaptureVideo
      videoPreview.source = '';
    }
  }

  Settings {
    id: settings
    property bool geoTagging: true
  }

  Page {
    width: parent.width
    height: parent.height
    padding: 0

    background: Rectangle {
      anchors.fill: parent
      color: "#000000"
    }

    Camera {
      id: camera

      cameraState: cameraItem.visible ? Camera.ActiveState : Camera.UnloadedState

      position: Camera.BackFace

      imageCapture {
        onImageSaved: {
          currentPath  = path
        }
        onImageCaptured: {
          photoPreview.source = preview
          cameraItem.state = "PhotoPreview"
        }
      }

      videoRecorder {
        audioEncodingMode: CameraRecorder.ConstantBitRateEncoding
        audioBitRate: 128000
        mediaContainer: "mp4"
      }

      function zoomIn(increase) {
        var zoom;
        if (camera.opticalZoom < camera.maximumOpticalZoom) {
          zoom = camera.opticalZoom + increase
          if (zoom < camera.maximumOpticalZoom) {
            camera.opticalZoom = zoom
          } else {
            camera.opticalZoom = camera.maximumOpticalZoom
          }
        } else {
          zoom = camera.digitalZoom + increase
          if (zoom < camera.maximumDigitalZoom) {
            camera.digitalZoom = zoom
          } else {
            camera.digitalZoom = camera.maximumDigitalZoom
          }
        }
      }

      function zoomOut(decrease) {
        var zoom;
        if (camera.digitalZoom > 1) {
          zoom = camera.digitalZoom - decrease
          if (zoom > 1) {
            camera.digitalZoom = zoom
          } else {
            camera.digitalZoom = 1
          }
        } else {
          zoom = camera.opticalZoom - decrease
          if (zoom > 1) {
            camera.opticalZoom = zoom
          } else {
            camera.opticalZoom = 1
          }
        }
      }
    }

    VideoOutput {
      anchors.fill: parent

      visible: cameraItem.state == "PhotoCapture" || cameraItem.state == "VideoCapture"

      focus : visible
      source: camera

      autoOrientation: true
    }

    MouseArea {
      anchors.fill: parent

      onClicked: {
        if (camera.lockStatus == Camera.Unlocked)
          camera.searchAndLock();
        else
          camera.unlock();
      }
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

      autoLoad: true
      autoPlay: true
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

      color: Theme.darkGraySemiOpaque

      Rectangle {
        id: captureRing
        anchors.centerIn: parent
        width: 64
        height: 64
        radius: 32
        color: Theme.darkGraySemiOpaque
        border.color: cameraItem.state == "VideoCapture" && camera.videoRecorder.recorderState != CameraRecorder.StoppedState
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
              camera.imageCapture.captureToLocation(qgisProject.homePath+ '/DCIM/')
              currentPosition = positionSource.positionInformation
            } else if (cameraItem.state == "VideoCapture") {
              if (camera.videoRecorder.recorderState == CameraRecorder.StoppedState) {
                camera.videoRecorder.record()
              } else {
                camera.videoRecorder.stop()
                videoPreview.source = camera.videoRecorder.actualLocation
                var path = camera.videoRecorder.actualLocation.toString()
                var filePos = path.indexOf('file://')
                currentPath = filePos === 0 ? path.substring(7) : path
                cameraItem.state = "VideoPreview"
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
        visible: cameraItem.isCapturing

        x: cameraItem.isPortraitMode ? (parent.width / 4) - (width / 2) : (parent.width - width) / 2
        y: cameraItem.isPortraitMode ? (parent.height - height) / 2 : (parent.height / 4) * 3 - (height / 2)

        iconColor: "white"
        bgcolor: Theme.darkGraySemiOpaque
        round: true

        text: (camera.digitalZoom * camera.opticalZoom).toFixed(1) +'X'
        font: Theme.tinyFont

        onClicked: {
          camera.opticalZoom = 1;
          camera.digitalZoom = 1;
        }
      }

      QfToolButton {
        id: flashButton
        visible: cameraItem.isCapturing && camera.flash.supportedModes.length > 1

        x: cameraItem.isPortraitMode ? (parent.width / 4) * 3 - (width / 2) : (parent.width - width) / 2
        y: cameraItem.isPortraitMode ? (parent.height - height) / 2 : (parent.height / 4) - (height / 2)

        iconSource: {
          switch(camera.flash.mode) {
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
          if (camera.flash.mode == Camera.FlashOff) {
            camera.flash.mode = Camera.FlashOn;
          } else {
            camera.flash.mode = Camera.FlashOff
          }
        }
      }

      Rectangle {
        visible: cameraItem.state == "VideoCapture" && camera.videoRecorder.recorderState != CameraRecorder.StoppedState

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
            if (camera.videoRecorder.duration > 0) {
              var seconds = Math.ceil(camera.videoRecorder.duration / 1000);
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
      bgcolor: Theme.darkGraySemiOpaque
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
  }
}
