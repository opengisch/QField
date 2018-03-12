import QtQuick 2.0
import QtMultimedia 5.8
import "js/style.js" as Style
import QtQuick.Controls 1.4 as Controls

Item{
  id : cameraItem
  signal finished(string path)
  signal canceled()

  property string currentPath

  anchors.fill: parent

  state: "PhotoCapture"

  states: [
    State {
      name: "PhotoCapture"
      StateChangeScript {
        script: {
          camera.captureMode = Camera.CaptureStillImage
        }
      }
    },
    State {
      name: "PhotoPreview"
    }
  ]

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
  }

  VideoOutput {
    anchors.fill: parent
    visible: true
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

    Button {
      id: video_button_click
      visible: true

      anchors.right: parent.right
      anchors.top: parent.verticalCenter
      bgcolor: "red"
      round: true

      onClicked: camera.imageCapture.captureToLocation(qgisProject.homePath+ '/DCIM')
    }
  }

  Image {
    id: photoPreview

    visible: cameraItem.state == "PhotoPreview"

    anchors.fill: parent

    fillMode: Image.PreserveAspectFit
    smooth: true
    focus: visible

    Button {
      id: buttonok
      visible: true

      anchors.right: parent.right
      anchors.top: parent.verticalCenter
      bgcolor: "#80CC28"
      round: true

      iconSource: Style.getThemeIcon("ic_save_white_24dp")

      onClicked: cameraItem.finished( currentPath )
    }

    Button {
      id: buttonnok
      visible: true

      anchors.right: parent.right
      anchors.top: parent.top
      bgcolor: "#80CC28"
      round: true

      iconSource: Style.getThemeIcon("ic_clear_white_24dp")
      onClicked: cameraItem.state = "PhotoCapture"
    }
  }
}
