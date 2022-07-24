import QtQuick 2.14
import QtQuick.Controls 2.14
import QtMultimedia 5.14

import org.qfield 1.0

import Theme 1.0

Item {
  id : barcodeReader

  signal finished(string path)
  signal canceled()

  property string currentPath

  BarcodeDecoder {
    id: barcodeDecoder
  }

  Camera {
    id: camera
    position: Camera.BackFace
    cameraState: barcodeReader.visible ? Camera.ActiveState : Camera.UnloadedState
  }

  VideoOutput {
    anchors.fill: parent

    source: camera
    autoOrientation: true
    fillMode: VideoOutput.PreserveAspectCrop

    filters: [
      BarcodeVideoFilter {
        active: barcodeReader.visible
        decoder: barcodeDecoder
      }
    ]
  }

  Rectangle {
    anchors.bottom: parent.bottom

    width: parent.width
    height: decodedText.contentHeight
    color: "white"

    Text {
      id: decodedText
      anchors.centerIn: parent
      width: parent.width

      padding: 10

      text: barcodeDecoder.decodedString
      font: Theme.titleFont
      color: Theme.mainColor
      horizontalAlignment: Text.AlignHCenter
    }
  }
}
