import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQuick.Shapes
import org.qfield

/**
 * \ingroup qml
 */
ColumnLayout {
  id: meterBar

  property alias value: progressBar.value
  property alias usedText: usedLabel.text
  property alias totalText: totalLabel.text
  property alias upgradeUrl: upgradeLink.url

  property color normalColor: Theme.qfieldcloudBlue
  property color warningColor: Theme.warningColor
  property color criticalColor: Theme.bookmarkRed
  property double warningThreshold: 0.75
  property double criticalThreshold: 0.9
  property int animationDuration: 1000
  property int barHeight: 8

  spacing: 8

  Item {
    id: upgradeLink
    Layout.fillWidth: true
    implicitHeight: upgradeLinkRow.implicitHeight

    property string url: ""
    property bool isCritical: meterBar.value >= meterBar.criticalThreshold

    Row {
      id: upgradeLinkRow
      spacing: 2

      Label {
        text: upgradeLink.isCritical ? qsTr("Upgrade") : qsTr("Manage")
        font: Theme.tipFont
        color: Theme.mainColor
      }
    }

    MouseArea {
      anchors.fill: parent
      cursorShape: Qt.PointingHandCursor
      onClicked: Qt.openUrlExternally(upgradeLink.url)
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
    }

    Behavior on value {
      NumberAnimation {
        duration: meterBar.animationDuration
      }
    }
  }

  RowLayout {
    Layout.fillWidth: true
    visible: usedLabel.text !== "" || totalLabel.text !== ""

    Label {
      id: usedLabel
      Layout.fillWidth: true
      font: Theme.tipFont
      color: Theme.mainTextColor
    }

    Label {
      id: totalLabel
      font: Theme.tipFont
      color: Theme.secondaryTextColor
    }
  }
}
