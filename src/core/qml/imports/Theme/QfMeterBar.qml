import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQuick.Shapes
import org.qfield

/**
 * \ingroup qml
 */
Item {
  id: meterBar

  property alias value: progressBar.value
  property bool loading: false

  property alias showTitleLabel: titleLabel.visible
  property alias showUsageLabel: usageLabel.visible
  property alias usageText: usageLabel.text
  property string relatedUrl: ""

  property color normalColor: Theme.qfieldcloudBlue
  property color warningColor: Theme.warningColor
  property color criticalColor: Theme.bookmarkRed
  property double warningThreshold: 0.8
  property double criticalThreshold: 0.95
  property int animationDuration: 1000
  property int barHeight: 8

  implicitHeight: content.implicitHeight

  ColumnLayout {
    id: content
    width: parent.width
    spacing: 8

    RowLayout {
      id: titleLabel
      Layout.fillWidth: true

      Label {
        Layout.alignment: Qt.AlignVCenter
        Layout.maximumWidth: content.width - 14
        text: {
          if (meterBar.relatedUrl === "") {
            return qsTr("Storage");
          }
          return value >= meterBar.criticalThreshold ? qsTr("Tap to upgrade storage") : qsTr("Tap to manage storage");
        }
        font: Theme.tipFont
        color: Theme.mainTextColor
        wrapMode: Text.WordWrap
        opacity: meterBar.loading ? 0.4 : 1.0
      }

      Item {
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignVCenter

        Shape {
          width: 10
          height: 10
          anchors.verticalCenter: parent.verticalCenter
          visible: meterBar.relatedUrl !== ""

          ShapePath {
            strokeWidth: 1.5
            strokeColor: Theme.mainTextColor
            fillColor: "transparent"
            capStyle: ShapePath.RoundCap
            joinStyle: ShapePath.RoundJoin
            startX: 0
            startY: 5
            PathLine {
              x: 5.5
              y: 5
            }
          }

          ShapePath {
            strokeWidth: 1.5
            strokeColor: Theme.mainTextColor
            fillColor: "transparent"
            capStyle: ShapePath.RoundCap
            joinStyle: ShapePath.RoundJoin
            startX: 4
            startY: 1.5
            PathLine {
              x: 8
              y: 5
            }
            PathLine {
              x: 4
              y: 8.5
            }
          }
        }
      }
    }

    ProgressBar {
      id: progressBar
      Layout.fillWidth: true
      from: 0
      to: 1

      property color barColor: {
        if (meterBar.value < meterBar.warningThreshold) {
          return meterBar.normalColor;
        } else if (meterBar.value < meterBar.criticalThreshold) {
          return meterBar.warningColor;
        } else {
          return meterBar.criticalColor;
        }
      }

      background: Rectangle {
        implicitHeight: meterBar.barHeight
        radius: height / 2
        color: Theme.controlBackgroundAlternateColor
      }

      contentItem: Item {
        implicitHeight: meterBar.barHeight

        Rectangle {
          visible: !meterBar.loading
          width: progressBar.visualPosition * parent.width
          height: parent.height
          radius: height / 2
          gradient: Gradient {
            orientation: Gradient.Horizontal
            GradientStop {
              position: 0.0
              color: progressBar.barColor
            }
            GradientStop {
              position: 1.0
              color: Qt.lighter(progressBar.barColor, 1.4)
            }
          }
        }

        Rectangle {
          id: skeletonFill
          visible: meterBar.loading
          width: parent.width
          height: parent.height
          radius: height / 2
          color: Theme.controlBackgroundAlternateColor
          clip: true

          Rectangle {
            id: shimmer
            width: parent.width * 0.4
            height: parent.height
            radius: height / 2
            opacity: 0.6
            gradient: Gradient {
              orientation: Gradient.Horizontal
              GradientStop {
                position: 0.0
                color: "transparent"
              }
              GradientStop {
                position: 0.5
                color: Theme.mainBackgroundColor
              }
              GradientStop {
                position: 1.0
                color: "transparent"
              }
            }

            SequentialAnimation on x {
              loops: Animation.Infinite
              running: meterBar.loading && meterBar.visible
              NumberAnimation {
                from: -shimmer.width
                to: skeletonFill.width
                duration: 1200
                easing.type: Easing.InOutQuad
              }
              PauseAnimation {
                duration: 400
              }
            }
          }
        }
      }

      Behavior on value {
        NumberAnimation {
          duration: meterBar.animationDuration
        }
      }
    }

    Label {
      id: usageLabel
      Layout.fillWidth: true
      font: Theme.tipFont
      color: Theme.secondaryTextColor
      wrapMode: Text.WordWrap
      visible: !meterBar.loading
    }

    Rectangle {
      visible: meterBar.loading
      width: 150
      height: usageLabel.font.pointSize
      radius: 3
      color: Theme.controlBackgroundAlternateColor
      clip: true

      Rectangle {
        width: parent.width * 0.4
        height: parent.height
        radius: 3
        opacity: 0.6
        gradient: Gradient {
          orientation: Gradient.Horizontal
          GradientStop {
            position: 0.0
            color: "transparent"
          }
          GradientStop {
            position: 0.5
            color: Theme.mainBackgroundColor
          }
          GradientStop {
            position: 1.0
            color: "transparent"
          }
        }
        SequentialAnimation on x {
          loops: Animation.Infinite
          running: meterBar.loading && meterBar.visible
          NumberAnimation {
            from: -parent.width * 0.4
            to: parent.width
            duration: 1200
            easing.type: Easing.InOutQuad
          }
          PauseAnimation {
            duration: 400
          }
        }
      }
    }
  }

  MouseArea {
    anchors.fill: parent
    cursorShape: meterBar.relatedUrl !== "" ? Qt.PointingHandCursor : Qt.ArrowCursor
    onClicked: {
      if (meterBar.relatedUrl !== "") {
        Qt.openUrlExternally(meterBar.relatedUrl);
      }
    }
  }
}
