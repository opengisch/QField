import QtQuick 2.14
import QtQuick.Controls 2.14
import QtMultimedia 5.14

import Theme 1.0

Popup {
  id: cameraItem
  z: 10000 // 1000s are embedded feature forms, use a higher value to insure feature form popups always show above embedded feature forms

  property string currentPath

  signal finished(string path)
  signal canceled()

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
    }

    VideoOutput {
      anchors.fill: parent

      visible: cameraItem.state == "PhotoCapture" || cameraItem.state == "VideoCapture"

      focus : visible
      source: camera

      autoOrientation: true

      MouseArea {
        anchors.fill: parent

        onClicked: {
          if (camera.lockStatus == Camera.Unlocked)
            camera.searchAndLock();
          else
            camera.unlock();
        }
      }

      QfToolButton {
        id: captureStillImageButton
        visible: cameraItem.state == "PhotoCapture"

        anchors.right: parent.right
        anchors.rightMargin: 5
        anchors.verticalCenter: parent.verticalCenter

        round: true
        roundborder: true
        bgcolor: "grey"
        borderColor: Theme.mainColor

        onClicked: camera.imageCapture.captureToLocation(qgisProject.homePath+ '/DCIM/')
      }

      QfToolButton {
        id: recordVideoButton
        visible: cameraItem.state == "VideoCapture"

        anchors.right: parent.right
        anchors.rightMargin: 5
        anchors.verticalCenter: parent.verticalCenter

        round: true
        roundborder: true
        bgcolor: camera.videoRecorder.recorderState != CameraRecorder.StoppedState  ? "black" : "red"
        borderColor: "red"//Theme.mainColor

        onClicked: {
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

    QfToolButton {
      id: okButton

      visible: cameraItem.state == "PhotoPreview" || cameraItem.state == "VideoPreview"

      anchors.right: parent.right
      anchors.rightMargin: 5
      anchors.verticalCenter: parent.verticalCenter
      bgcolor: Theme.mainColor
      round: true

      iconSource: Theme.getThemeIcon("ic_save_white_24dp")

      onClicked: cameraItem.finished(currentPath)
    }

    QfToolButton {
      id: cancelButton

      visible: cameraItem.state == "PhotoPreview" || cameraItem.state == "VideoPreview"

      anchors.right: parent.right
      anchors.rightMargin: 5
      anchors.top: parent.top
      anchors.topMargin: mainWindow.sceneTopMargin + 5
      bgcolor: Theme.mainColor
      round: true

      iconSource: Theme.getThemeIcon("ic_clear_white_24dp")
      onClicked: {
        platformUtilities.rmFile(currentPath)
        cameraItem.canceled()
      }
    }
  }
}
