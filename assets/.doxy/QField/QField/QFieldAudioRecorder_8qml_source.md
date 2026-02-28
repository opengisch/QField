

# File QFieldAudioRecorder.qml

[**File List**](files.md) **>** [**qml**](dir_1d81f7f4362e4398c0a695c4e466fcb0.md) **>** [**QFieldAudioRecorder.qml**](QFieldAudioRecorder_8qml.md)

[Go to the documentation of this file](QFieldAudioRecorder_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Shapes
import QtMultimedia
import QtCore
import org.qfield
import Theme

QfPopup {
  id: audioRecorder

  signal finished(string path)
  signal canceled

  property bool preRecording: true
  property bool hasRecordedClip: player.duration > 0
  property int popupWidth: Math.min(400, mainWindow.width <= mainWindow.height ? mainWindow.width - Theme.popupScreenEdgeVerticalMargin : mainWindow.height - Theme.popupScreenEdgeVerticalMargin)

  width: popupWidth
  height: Math.min(mainWindow.height - Math.max(Theme.popupScreenEdgeVerticalMargin * 2, mainWindow.sceneTopMargin * 2 + 4, mainWindow.sceneBottomMargin * 2 + 4), popupWidth + toolBar.height + recordButton.height)
  x: (parent.width - width) / 2
  y: (parent.height - height) / 2
  z: 10000 // 1000s are embedded feature forms, use a higher value to insure feature form popups always show above embedded feature formes
  parent: mainWindow.contentItem

  closePolicy: Popup.CloseOnEscape
  dim: true

  onAboutToShow: {
    preRecording = true;
    player.source = '';
    if (microphonePermission.status === Qt.PermissionStatus.Undetermined) {
      microphonePermission.request();
    }
    recorder.mediaFormat.audioCodec = MediaFormat.AudioCodec.AAC;
    recorder.mediaFormat.fileFormat = MediaFormat.Mpeg4Audio;
  }

  QfMicrophonePermission {
    id: microphonePermission
  }

  AudioRecorder {
    id: recorder
  }

  Loader {
    id: captureSessionLoader
    active: microphonePermission.status !== Qt.PermissionStatus.Undetermined

    property AudioRecorder audioRecorder: recorder

    sourceComponent: Component {
      CaptureSession {
        audioInput: AudioInput {}
        recorder: audioRecorder
      }
    }
  }

  Timer {
    id: playerLoader
    interval: 500
    repeat: true
    running: false

    onTriggered: {
      player.source = UrlUtils.fromString(recorder.actualLocation.toString());
    }
  }

  Video {
    id: player

    property bool loaded: false

    anchors.left: parent.left
    anchors.top: parent.top
    visible: false
    width: parent.width
    height: parent.height - 54

    muted: false
    volume: 1.0

    onDurationChanged: {
      if (duration > 0 && !loaded) {
        playerLoader.stop();
        loaded = true;
      }
      positionSlider.to = duration / 1000;
      positionSlider.value = 0;
    }

    onPositionChanged: {
      positionSlider.value = position / 1000;
    }
  }

  Page {
    width: parent.width
    height: parent.height
    padding: 10
    header: ToolBar {
      id: toolBar

      topPadding: 0
      leftPadding: 0
      rightPadding: 0
      bottomPadding: 0

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
          iconSource: Theme.getThemeVectorIcon('ic_close_white_24dp')
          iconColor: Theme.mainTextColor
          bgcolor: "transparent"

          onClicked: {
            audioRecorder.canceled();
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
          id: levelFeedback
          anchors.centerIn: parent
          width: 120 + (Math.min(audioFeedback.width, audioFeedback.height) - 120) * 0
          height: width
          radius: width / 2
          color: "#44808080"

          SequentialAnimation {
            NumberAnimation {
              target: levelFeedback
              property: "width"
              to: 120 + (Math.min(audioFeedback.width, audioFeedback.height) - 120)
              duration: 2000
              easing.type: Easing.InOutQuad
            }
            NumberAnimation {
              target: levelFeedback
              property: "width"
              to: 120
              duration: 2000
              easing.type: Easing.InOutQuad
            }
            running: recorder.recorderState === MediaRecorder.RecordingState
            loops: Animation.Infinite
          }

          QfToolButton {
            id: recordButton
            anchors.centerIn: parent
            width: 120
            height: 120
            iconSource: ''
            round: true
            bgcolor: recorder.recorderState === MediaRecorder.RecordingState ? "#99FF0000" : "#FF0000"

            Rectangle {
              anchors.centerIn: parent
              width: parent.width / 2
              height: width

              radius: recorder.recorderState === MediaRecorder.RecordingState ? 10 : width / 2
              color: "#FFFFFF"

              Behavior on radius {
                PropertyAnimation {
                  duration: 250
                  easing.type: Easing.InOutQuad
                }
              }
            }

            onClicked: {
              if (preRecording) {
                recorder.record();
                preRecording = false;
              } else {
                if (recorder.recorderState === MediaRecorder.RecordingState) {
                  // As of Qt5.15, Android doesn't support pausing a recording, revisit in Qt6
                  recorder.stop();
                  playerLoader.start();
                } else {
                  playerLoader.stop();
                  player.source = '';
                  recorder.record();
                }
              }
            }
          }
        }
      }

      RowLayout {
        Layout.fillWidth: true

        QfToolButton {
          id: playButton
          enabled: audioRecorder.hasRecordedClip
          opacity: enabled ? 1 : 0.25

          iconSource: player.playbackState == MediaPlayer.PlayingState ? Theme.getThemeVectorIcon('ic_pause_black_24dp') : Theme.getThemeVectorIcon('ic_play_black_24dp')
          iconColor: Theme.mainTextColor
          bgcolor: "transparent"

          onClicked: {
            if (player.playbackState == MediaPlayer.PlayingState) {
              player.pause();
            } else {
              player.play();
            }
          }
        }

        Slider {
          id: positionSlider
          Layout.fillWidth: true

          from: 0
          to: 0

          enabled: audioRecorder.hasRecordedClip
          opacity: enabled ? 1 : 0.25

          onMoved: {
            player.seek(value * 1000);
          }
        }

        Label {
          id: durationLabel
          Layout.preferredWidth: durationLabelMetrics.boundingRect('00:00:00').width
          Layout.rightMargin: 14

          color: player.playbackState == MediaPlayer.PlayingState ? Theme.mainTextColor : Theme.mainTextDisabledColor
          font: Theme.tipFont
          horizontalAlignment: Text.AlignHCenter
          verticalAlignment: Text.AlignVCenter

          text: {
            if (!preRecording && recorder.duration > 0) {
              var seconds = Math.ceil(recorder.duration / 1000);
              var hours = Math.floor(seconds / 60 / 60) + '';
              seconds -= hours * 60 * 60;
              var minutes = Math.floor(seconds / 60) + '';
              seconds = (seconds - minutes * 60) + '';
              return hours.padStart(2, '0') + ':' + minutes.padStart(2, '0') + ':' + seconds.padStart(2, '0');
            } else {
              return '-';
            }
          }
        }

        FontMetrics {
          id: durationLabelMetrics
          font: durationLabel.font
        }

        QfToolButton {
          id: acceptButton
          enabled: audioRecorder.hasRecordedClip
          opacity: enabled ? 1 : 0.2
          Layout.alignment: Qt.AlignVCenter
          iconSource: Theme.getThemeVectorIcon('ic_check_white_24dp')
          iconColor: enabled ? Theme.toolButtonColor : Theme.toolButtonBackgroundSemiOpaqueColor
          bgcolor: enabled ? Theme.mainColor : "transparent"
          round: true

          onClicked: {
            const path = recorder.actualLocation.toString();
            audioRecorder.finished(UrlUtils.toLocalFile(path));
            audioRecorder.close();
          }
        }
      }
    }
  }
}
```


