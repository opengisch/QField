

# File QfCodeReader.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfCodeReader.qml**](QfCodeReader_8qml.md)

[Go to the documentation of this file](QfCodeReader_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Shapes
import QtMultimedia
import QtCore
import org.qfield.core
import org.qfield.gui

QfPopup {
  id: codeReader

  signal decoded(var string)
  signal accepted(var string)

  property string decodedString: ''
  property var barcodeRequestedItem: undefined //<! when a feature form is requesting a bardcode, this will be set to attribute editor widget which triggered the request
  property int popupWidth: mainWindow.width <= mainWindow.height ? mainWindow.width - QfTheme.popupScreenEdgeHorizontalMargin : mainWindow.height - QfTheme.popupScreenEdgeVerticalMargin
  property bool openedOnce: false
  property var imageResourceSource: undefined

  width: popupWidth
  height: Math.min(mainWindow.height - Math.max(QfTheme.popupScreenEdgeVerticalMargin * 2, mainWindow.sceneTopMargin * 2 + 4, mainWindow.sceneBottomMargin * 2 + 4), popupWidth + toolBar.height + acceptButton.height)
  x: (parent.width - width) / 2
  y: (parent.height - height) / 2
  z: 10000 // 1000s are embedded feature forms, use a higher value to insure feature form popups always show above embedded feature formes

  closePolicy: Popup.CloseOnEscape
  dim: true

  onAboutToShow: {
    openedOnce = true;
    // when NFC is not accessible, make sure the only option, QR, is active
    if (!Qfield.hasNfc && !settings.cameraActive) {
      settings.cameraActive = true;
    }
    decodedString = '';
    barcodeDecoder.clearDecodedString();
    if (cameraPermission.status === Qt.PermissionStatus.Undetermined) {
      cameraPermission.request();
    }
  }

  onOpened: {
    contentItem.forceActiveFocus();
  }

  onAboutToHide: {
    if (cameraLoader.active) {
      cameraLoader.item.camera.torchMode = Camera.TorchOff;
    }
  }

  onDecodedStringChanged: {
    if (decodedString != "") {
      decoded(decodedString);
    }
  }

  function pickImage() {
    imageResourceSource = platformUtilities.getGalleryPicture(platformUtilities.systemLocalDataLocation() + '/', 'codereader.{extension}', codeReader.contentItem);
    imageResourceSource.resourceReceived.connect(codeReader.decodeImageResource);
  }

  function decodeImageResource(path) {
    const filePath = platformUtilities.systemLocalDataLocation() + '/' + path;
    if (!barcodeDecoder.decodeImageFile(filePath)) {
      displayToast(qsTr('No readable code found in the selected image'));
    }
    platformUtilities.rmFile(filePath);
    imageResourceSource = undefined;
  }

  QfCameraPermission {
    id: cameraPermission
  }

  Settings {
    id: settings
    property bool cameraActive: true
    property bool nearfieldActive: true
  }

  QfBarcodeDecoder {
    id: barcodeDecoder

    onDecodedStringChanged: {
      if (decodedString !== '') {
        codeReader.decodedString = decodedString;
        decodedFlashAnimation.start();
      }
    }
  }

  Loader {
    id: nearfieldLoader
    active: Qfield.hasNfc && codeReader.visible && settings.nearfieldActive

    sourceComponent: Component {
      QfNearFieldReader {
        id: nearFieldReader
        active: true

        onReadStringChanged: {
          if (readString !== '') {
            displayToast(qsTr('NFC text tag detected'));
            codeReader.decodedString = readString;
            decodedFlashAnimation.start();
          }
        }
      }
    }
  }

  Page {
    width: parent.width
    height: parent.height
    padding: 5
    header: ToolBar {
      id: toolBar

      topPadding: 0
      leftPadding: 0
      rightPadding: 0
      bottomPadding: 0

      background: Rectangle {
        color: "transparent"
        height: QfTheme.toolButtonSize
      }

      RowLayout {
        width: parent.width
        height: QfTheme.toolButtonSize

        Label {
          Layout.leftMargin: 58
          Layout.fillWidth: true
          Layout.alignment: Qt.AlignVCenter
          text: qsTr('Code Reader')
          font: QfTheme.strongFont
          color: QfTheme.mainColor
          horizontalAlignment: Text.AlignHCenter
          wrapMode: Text.WordWrap
        }

        QfToolButton {
          id: closeButton
          Layout.rightMargin: 10
          Layout.alignment: Qt.AlignVCenter
          iconSource: QfTheme.getThemeVectorIcon('ic_close_white_24dp')
          iconColor: QfTheme.mainTextColor
          bgcolor: "transparent"

          onClicked: {
            codeReader.close();
          }
        }
      }
    }

    ColumnLayout {
      width: parent.width
      height: parent.height

      Rectangle {
        id: visualFeedback
        Layout.fillWidth: true
        Layout.fillHeight: true

        color: QfTheme.mainBackgroundColor
        radius: 10
        clip: true

        Rectangle {
          id: nearfieldFeedback
          visible: settings.nearfieldActive && !settings.cameraActive
          anchors.centerIn: parent
          width: 120
          height: width
          radius: width / 2
          color: nearfieldLoader.active && nearfieldLoader.item.targetInRange ? QfTheme.mainColor : "#44808080"

          SequentialAnimation {
            NumberAnimation {
              target: nearfieldFeedback
              property: "width"
              to: 120 + (Math.min(visualFeedback.width, visualFeedback.height) - 120)
              duration: 2000
              easing.type: Easing.InOutQuad
            }
            NumberAnimation {
              target: nearfieldFeedback
              property: "width"
              to: 120
              duration: 2000
              easing.type: Easing.InOutQuad
            }
            running: nearfieldFeedback.visible
            loops: Animation.Infinite
          }
        }

        Loader {
          id: cameraLoader
          // A note on the conditional expression below:
          // - On Android, loading and unloading the source component leads to freeze (as of Qt 6.5.2)
          // - On Linux, not loading and unloading the source component leads to blank VideoOutput (as of Qt 6.5.2)
          active: codeReader.openedOnce && (Qt.platform.os === "android" || codeReader.visible) && cameraPermission.status === Qt.PermissionStatus.Granted
          anchors.fill: parent

          sourceComponent: Component {
            id: cameraComponent

            Item {
              property alias camera: captureSession.camera
              property alias sink: videoOutput.videoSink

              CaptureSession {
                id: captureSession
                camera: Camera {
                  active: codeReader.visible && settings.cameraActive
                  flashMode: Camera.FlashOff
                }
                videoOutput: videoOutput
              }

              VideoOutput {
                id: videoOutput
                visible: settings.cameraActive
                anchors.fill: parent
                anchors.margins: 6
                fillMode: VideoOutput.PreserveAspectCrop
              }
            }
          }

          onLoaded: {
            barcodeDecoder.videoSink = item.sink;
          }
        }

        Rectangle {
          id: decodedFlash
          anchors.fill: parent
          anchors.margins: 6

          color: "transparent"
          SequentialAnimation {
            id: decodedFlashAnimation
            PropertyAnimation {
              target: decodedFlash
              property: "color"
              to: "white"
              duration: 0
            }
            PropertyAnimation {
              target: decodedFlash
              property: "color"
              to: "transparent"
              duration: 500
            }
          }
        }

        Shape {
          id: frame
          visible: settings.cameraActive
          anchors.fill: parent

          ShapePath {
            strokeWidth: 2.5
            strokeColor: "#333333"
            strokeStyle: ShapePath.SolidLine
            joinStyle: ShapePath.MiterJoin
            fillColor: "transparent"

            startX: 5
            startY: 10
            PathArc {
              x: 10
              y: 5
              radiusX: 5
              radiusY: 5
            }
            PathLine {
              x: frame.width - 10
              y: 5
            }
            PathArc {
              x: frame.width - 5
              y: 10
              radiusX: 5
              radiusY: 5
            }
            PathLine {
              x: frame.width - 5
              y: frame.height - 10
            }
            PathArc {
              x: frame.width - 10
              y: frame.height - 5
              radiusX: 5
              radiusY: 5
            }
            PathLine {
              x: 10
              y: frame.height - 5
            }
            PathArc {
              x: 5
              y: frame.height - 10
              radiusX: 5
              radiusY: 5
            }
            PathLine {
              x: 5
              y: 10
            }
          }
        }

        Shape {
          id: aim
          visible: settings.cameraActive
          anchors.fill: parent

          ShapePath {
            strokeWidth: 2.5
            strokeColor: "white"
            strokeStyle: ShapePath.SolidLine
            joinStyle: ShapePath.MiterJoin
            fillColor: "transparent"

            startX: 20
            startY: 60
            PathLine {
              x: 20
              y: 25
            }
            PathArc {
              x: 25
              y: 20
              radiusX: 5
              radiusY: 5
            }
            PathLine {
              x: 60
              y: 20
            }
            PathMove {
              x: aim.width - 60
              y: 20
            }
            PathLine {
              x: aim.width - 25
              y: 20
            }
            PathArc {
              x: aim.width - 20
              y: 25
              radiusX: 5
              radiusY: 5
            }
            PathLine {
              x: aim.width - 20
              y: 60
            }
            PathMove {
              x: aim.width - 20
              y: aim.height - 60
            }
            PathLine {
              x: aim.width - 20
              y: aim.height - 25
            }
            PathArc {
              x: aim.width - 25
              y: aim.height - 20
              radiusX: 5
              radiusY: 5
            }
            PathLine {
              x: aim.width - 60
              y: aim.height - 20
            }
            PathMove {
              x: 60
              y: aim.height - 20
            }
            PathLine {
              x: 25
              y: aim.height - 20
            }
            PathArc {
              x: 20
              y: aim.height - 25
              radiusX: 5
              radiusY: 5
            }
            PathLine {
              x: 20
              y: aim.height - 60
            }
          }
        }

        QfToolButton {
          id: flashlightButton
          anchors.bottom: buttonsRow.top
          anchors.bottomMargin: 10
          anchors.horizontalCenter: parent.horizontalCenter
          width: buttonsRow.width
          round: true
          iconSource: QfTheme.getThemeVectorIcon('ic_flashlight_white_48dp')
          iconColor: QfTheme.toolButtonColor
          bgcolor: QfTheme.toolButtonBackgroundSemiOpaqueColor

          visible: settings.cameraActive && cameraLoader.active && cameraLoader.item.camera.isTorchModeSupported(Camera.TorchOn)
          state: cameraLoader.active && cameraLoader.item.camera.torchMode === Camera.TorchOn ? "On" : "Off"
          states: [
            State {
              name: "Off"
              PropertyChanges {
                target: flashlightButton
                iconColor: QfTheme.toolButtonColor
                bgcolor: QfTheme.toolButtonBackgroundSemiOpaqueColor
              }
            },
            State {
              name: "On"
              PropertyChanges {
                target: flashlightButton
                iconColor: QfTheme.mainColor
                bgcolor: QfTheme.toolButtonBackgroundColor
              }
            }
          ]

          onClicked: {
            if (cameraLoader.item.camera.torchMode === Camera.TorchOff) {
              cameraLoader.item.camera.torchMode = Camera.TorchOn;
            } else {
              cameraLoader.item.camera.torchMode = Camera.TorchOff;
            }
          }
        }

        Row {
          id: buttonsRow
          anchors.bottom: parent.bottom
          anchors.bottomMargin: 20
          anchors.horizontalCenter: parent.horizontalCenter
          spacing: 10

          QfToolButton {
            id: cameraButton
            round: true
            iconSource: QfTheme.getThemeVectorIcon('ic_qr_code_black_24dp')
            iconColor: QfTheme.toolButtonColor
            bgcolor: QfTheme.toolButtonBackgroundSemiOpaqueColor

            visible: Qfield.hasNfc
            state: settings.cameraActive ? "On" : "Off"
            states: [
              State {
                name: "Off"
                PropertyChanges {
                  target: cameraButton
                  bgcolor: QfTheme.toolButtonBackgroundSemiOpaqueColor
                }
              },
              State {
                name: "On"
                PropertyChanges {
                  target: cameraButton
                  iconColor: QfTheme.mainColor
                  bgcolor: QfTheme.toolButtonBackgroundColor
                }
              }
            ]

            onClicked: {
              settings.cameraActive = !settings.cameraActive;
            }
          }

          QfToolButton {
            id: imageButton
            round: true
            iconSource: QfTheme.getThemeVectorIcon('ic_gallery_black_24dp')
            iconColor: QfTheme.toolButtonColor
            bgcolor: QfTheme.toolButtonBackgroundSemiOpaqueColor

            onClicked: {
              codeReader.pickImage();
            }
          }

          QfToolButton {
            id: nearfieldButton
            round: true
            iconSource: QfTheme.getThemeVectorIcon('ic_nfc_code_black_24dp')
            iconColor: QfTheme.toolButtonColor
            bgcolor: QfTheme.toolButtonBackgroundSemiOpaqueColor

            visible: Qfield.hasNfc
            state: settings.nearfieldActive ? "On" : "Off"
            states: [
              State {
                name: "Off"
                PropertyChanges {
                  target: nearfieldButton
                  bgcolor: QfTheme.toolButtonBackgroundSemiOpaqueColor
                }
              },
              State {
                name: "On"
                PropertyChanges {
                  target: nearfieldButton
                  iconColor: QfTheme.mainColor
                  bgcolor: QfTheme.toolButtonBackgroundColor
                }
              }
            ]

            onClicked: {
              settings.nearfieldActive = !settings.nearfieldActive;
            }
          }
        }
      }

      RowLayout {
        Layout.fillWidth: true

        Text {
          id: decodedText
          Layout.fillWidth: true

          text: codeReader.decodedString !== '' ? codeReader.decodedString : qsTr('Center your device on a code')
          font: QfTheme.tipFont
          color: QfTheme.mainTextColor
          horizontalAlignment: Text.AlignLeft
          elide: Text.ElideMiddle
          opacity: codeReader.decodedString !== '' ? 1 : 0.45
        }

        QfToolButton {
          id: acceptButton
          enabled: codeReader.decodedString !== ''
          opacity: enabled ? 1 : 0.2
          Layout.alignment: Qt.AlignVCenter
          iconSource: QfTheme.getThemeVectorIcon('ic_check_white_24dp')
          iconColor: enabled ? QfTheme.toolButtonColor : QfTheme.toolButtonBackgroundSemiOpaqueColor
          bgcolor: enabled ? QfTheme.mainColor : "transparent"
          round: true

          onClicked: {
            if (codeReader.barcodeRequestedItem != undefined) {
              codeReader.barcodeRequestedItem.requestedBarcodeReceived(codeReader.decodedString);
              codeReader.barcodeRequestedItem = undefined;
            } else {
              codeReader.accepted(codeReader.decodedString);
            }
            codeReader.close();
          }
        }
      }
    }
  }
}
```


