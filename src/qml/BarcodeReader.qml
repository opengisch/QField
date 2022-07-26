import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import QtQuick.Shapes 1.14
import QtMultimedia 5.14

import org.qfield 1.0

import Theme 1.0

Popup {
  id : barcodeReader

  signal decoded(var string)

  property int itemSize: mainWindow.width <= mainWindow.height ? mainWindow.width - 80 : mainWindow.height - 80

  width: itemSize
  height: itemSize
  x: (parent.width - width) / 2
  y: (parent.height - height) / 2

  padding: 0

  onAboutToShow: {
    barcodeDecoder.decodedString = '';
  }

  BarcodeDecoder {
    id: barcodeDecoder

    onDecodedStringChanged: {
      if (decodedString !== '') {
        decodedFlashAnimation.start();
      }
    }
  }

  Camera {
    id: camera
    position: Camera.BackFace
    cameraState: barcodeReader.visible ? Camera.ActiveState : Camera.UnloadedState

    focus {
        focusMode: Camera.FocusContinuous
        focusPointMode: Camera.FocusPointCenter
    }
  }

  Page {
    width: parent.width
    height: parent.height
    padding: 10
    header: ToolBar {
      id: toolBar

      background: Rectangle {
        color: "transparent"
        height: 48
      }

      RowLayout {
        width: parent.width
        height: 48

        Label {
          Layout.leftMargin: 58
          Layout.fillWidth: true
          Layout.alignment: Qt.AlignVCenter
          text: qsTr('Code Reader')
          font: Theme.strongFont
          color: Theme.mainColor
          horizontalAlignment: Text.AlignHCenter
          wrapMode: Text.WordWrap
        }

        QfToolButton {
          id: closeButton
          Layout.rightMargin: 10
          Layout.alignment: Qt.AlignVCenter
          iconSource: Theme.getThemeIcon( 'ic_close_black_24dp' )
          bgcolor: "transparent"

          onClicked: {
            barcodeReader.close();
          }
        }
      }
    }

    ColumnLayout {
      width: parent.width
      height: parent.width - toolBar.height

      Rectangle {
        Layout.fillWidth: true
        Layout.fillHeight: true

        color: "#333333"
        radius: 10
        clip: true

        VideoOutput {
          anchors.fill: parent
          anchors.margins: 6

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
          id: decodedFlash
          anchors.fill: parent
          anchors.margins: 6

          color: "transparent"
          SequentialAnimation {
            id: decodedFlashAnimation
            PropertyAnimation { target: decodedFlash; property: "color"; to: "white"; duration: 0 }
            PropertyAnimation { target: decodedFlash; property: "color"; to: "transparent"; duration: 500 }
          }
        }

        Shape {
          id: frame
          anchors.fill: parent

          ShapePath {
            strokeWidth: 2.5
            strokeColor: "#333333"
            strokeStyle: ShapePath.SolidLine
            joinStyle: ShapePath.MiterJoin
            fillColor: "transparent"

            startX: 5
            startY: 10
            PathArc { x: 10; y: 5; radiusX: 5; radiusY: 5 }
            PathLine { x: frame.width - 10; y: 5; }
            PathArc { x: frame.width - 5; y: 10; radiusX: 5; radiusY: 5 }
            PathLine { x: frame.width - 5; y: frame.height - 10; }
            PathArc { x: frame.width - 10; y: frame.height - 5; radiusX: 5; radiusY: 5 }
            PathLine { x: 10; y: frame.height - 5; }
            PathArc { x: 5; y: frame.height - 10; radiusX: 5; radiusY: 5 }
            PathLine { x: 5; y: 10 }
          }
        }

        Shape {
          id: aim
          anchors.fill: parent

          ShapePath {
            strokeWidth: 2.5
            strokeColor: "white"
            strokeStyle: ShapePath.SolidLine
            joinStyle: ShapePath.MiterJoin
            fillColor: "transparent"

            startX: 20
            startY: 60
            PathLine { x: 20; y: 25 }
            PathArc { x: 25; y: 20; radiusX: 5; radiusY: 5 }
            PathLine { x: 60; y: 20 }
            PathMove { x: aim.width - 60; y: 20 }
            PathLine { x: aim.width - 25; y: 20 }
            PathArc { x: aim.width - 20; y: 25; radiusX: 5; radiusY: 5; }
            PathLine { x: aim.width - 20; y: 60 }
            PathMove { x: aim.width - 20; y: aim.height - 60 }
            PathLine { x: aim.width - 20; y: aim.height - 25 }
            PathArc { x: aim.width - 25; y: aim.height - 20; radiusX: 5; radiusY: 5 }
            PathLine { x: aim.width - 60; y: aim.height - 20 }
            PathMove { x: 60; y: aim.height - 20 }
            PathLine { x: 25; y: aim.height - 20 }
            PathArc { x: 20; y: aim.height - 25; radiusX: 5; radiusY: 5 }
            PathLine { x: 20; y: aim.height - 60; }
          }
        }
      }

      RowLayout {
        Layout.fillWidth: true

        Text {
          id: decodedText
          Layout.fillWidth: true

          text: barcodeDecoder.decodedString !== ''
                ? barcodeDecoder.decodedString
                : qsTr( 'Center your camera on a code')
          font: Theme.tipFont
          horizontalAlignment: Text.AlignLeft
          elide: Text.ElideMiddle
          opacity: barcodeDecoder.decodedString !== '' ? 1 : 0.2
        }

        QfToolButton {
          id: acceptButton
          Layout.alignment: Qt.AlignVCenter
          iconSource: Theme.getThemeIcon( 'ic_check_black_48dp' )
          bgcolor: "transparent"
          enabled: barcodeDecoder.decodedString !== ''
          opacity: enabled ? 1 : 0.2

          onClicked: {
            barcodeReader.close();
            barcodeReader.decoded(barcodeDecoder.decodedString);
          }
        }
      }
    }
  }
}
