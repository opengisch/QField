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
                    camera.start()
                }
            }
        },
        State {
            name: "PhotoPreview"
        }
    ]

    Camera {
        id: camera

        captureMode: Camera.CaptureStillImage

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

        Button {
          id: video_button_click
          width: 36 * dp
          height: 36 * dp
          visible: true

          anchors.right: parent.right
          anchors.top: parent.top

          bgcolor: "transparent"

          onClicked: camera.imageCapture.captureToLocation(qgisProject.homePath+ '/DCIM')

          iconSource: Style.getThemeIcon("ic_camera_white_36dp")
        }
        Button {
          id: video_button_cancel
          width: 36 * dp
          height: 36 * dp
          visible: true

          anchors.right: parent.right
          anchors.bottom: parent.bottom

          bgcolor: "transparent"

          onClicked: canceled()

          iconSource: Style.getThemeIcon("ic_clear_white_24dp")
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

          bgcolor: "transparent"

          onClicked: finished( currentPath)

          iconSource: Style.getThemeIcon("ic_save_white_24dp")
        }


        Button {
          id: buttonnok
          width: 36 * dp
          height: 36 * dp
          visible: true

          anchors.right: parent.right
          anchors.bottom: parent.bottom

          bgcolor: "transparent"

          onClicked: cameraItem.state = "PhotoCapture"

          iconSource: Style.getThemeIcon("ic_clear_white_24dp")
        }

    }
}
