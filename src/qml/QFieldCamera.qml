import QtQuick 2.0
import QtMultimedia 5.4
import "js/style.js" as Style

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

        Button {
          id: video_button_click
          width: 36 * dp
          height: 36 * dp
          visible: true

          anchors.right: parent.right
          anchors.top: parent.top

          bgcolor: "#80CC28"
          iconSource: Style.getThemeIcon("ic_camera_white_36dp")

          onClicked: camera.imageCapture.captureToLocation(qgisProject.homePath+ '/DCIM')
        }
        Button {
          id: video_button_cancel
          width: 36 * dp
          height: 36 * dp
          visible: true

          anchors.right: parent.right
          anchors.bottom: parent.bottom

          bgcolor: "#80CC28"
          iconSource: Style.getThemeIcon("ic_clear_white_24dp")

          onClicked: cameraItem.canceled()
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
          width: 36 * dp
          height: 36 * dp
          visible: true

          anchors.right: parent.right
          anchors.top: parent.top

          bgcolor: "#80CC28"
          iconSource: Style.getThemeIcon("ic_save_white_24dp")

          onClicked: cameraItem.finished( currentPath )
        }


        Button {
          id: buttonnok
          width: 36 * dp
          height: 36 * dp
          visible: true

          anchors.right: parent.right
          anchors.bottom: parent.bottom

          bgcolor: "#80CC28"
          iconSource: Style.getThemeIcon("ic_clear_white_24dp")

          onClicked: cameraItem.state = "PhotoCapture"
        }
    }
}
