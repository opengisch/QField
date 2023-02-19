import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import QtQuick.Shapes 1.14
import QtMultimedia 5.14

import org.qfield 1.0

import Theme 1.0

Popup {
  id : audioRecorder

  signal finished(string path)
  signal canceled()

  property bool preRecording: true
  property int popupWidth: Math.min(400, mainWindow.width <= mainWindow.height ? mainWindow.width - Theme.popupScreenEdgeMargin : mainWindow.height - Theme.popupScreenEdgeMargin)

  width: popupWidth
  height: Math.min(mainWindow.height - Theme.popupScreenEdgeMargin, popupWidth + toolBar.height + recordButton.height)
  x: (parent.width - width) / 2
  y: (parent.height - height) / 2
  z: 10000 // 1000s are embedded feature forms, use a higher value to insure feature form popups always show above embedded feature formes
  padding: 0

  closePolicy: Popup.CloseOnEscape
  dim: true

  onAboutToShow: {
    preRecording = true;
  }

  AudioRecorder {
    id: recorder
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
          text: qsTr('Audio Recorder')
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
            audioRecorder.canceled()
          }
        }
      }
    }

    ColumnLayout {
      width: parent.width
      height: parent.height

      Rectangle {
        id: audioFeedback
        Layout.fillWidth: true
        Layout.fillHeight: true

        color: "transparent"

        Rectangle {
          anchors.centerIn: parent
          width: 120 + (Math.min(audioFeedback.width, audioFeedback.height) - 120) * recorder.level
          height: width
          radius: width / 2
          color: Theme.navigationBackgroundColor


          QfToolButton {
            id: recordButton
            anchors.centerIn: parent
            width: 120
            height: 120
            iconSource: ''
            round: true
            bgcolor: !recorder.recording ? "red" : "gray"

            onClicked: {
              if (preRecording) {
                recorder.record();
                preRecording = false;
              } else {
                if (recorder.recording) {
                  // As of Qt5.15, Android doesn't support pausing a recording, revisit in Qt6
                  recorder.stop();
                } else {
                  recorder.record();
                }
              }
            }
          }
        }
      }

      RowLayout {
        Layout.fillWidth: true

        Text {
          id: audioDuration
          Layout.fillWidth: true

          text: {
            if (!preRecording && recorder.duration > 0) {
              var seconds = Math.ceil(recorder.duration / 1000);
              var hours = Math.floor(seconds / 60 / 60) + '';
              seconds -= hours * 60 * 60;
              var minutes = Math.floor(seconds / 60) + '';
              seconds = (seconds - minutes * 60) + '';
              return hours.padStart(2,'0') + ':' + minutes.padStart(2,'0') + ':' + seconds.padStart(2,'0');
            } else {
              return '';
            }
          }
          font: Theme.tipFont
          horizontalAlignment: Text.AlignLeft
          elide: Text.ElideMiddle
        }

        QfToolButton {
          id: acceptButton
          Layout.alignment: Qt.AlignVCenter
          iconSource: Theme.getThemeIcon( 'ic_check_black_48dp' )
          round: true
          bgcolor: "transparent"
          enabled: !preRecording && recorder.duration !== 0
          opacity: enabled ? 1 : 0.25

          onClicked: {
            recorder.stop()
            var path = recorder.actualLocation.toString()
            var filePos = path.indexOf('file://')
            audioRecorder.finished(filePos === 0 ? path.substring(7) : path)
            audioRecorder.close();
          }
        }
      }
    }
  }
}
