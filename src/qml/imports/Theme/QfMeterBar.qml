import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import org.qfield

/**
 * \ingroup qml
 */
ColumnLayout {
  id: meterBar

  property alias value: progressBar.value
  property alias text: meterText.text
  property alias richText: meterText.richText

  property color normalColor: Theme.qfieldcloudBlue
  property color warningColor: Theme.warningColor
  property color criticalColor: Theme.bookmarkRed
  property double warningThreshold: 0.9
  property double criticalThreshold: 0.975
  property int animationDuration: 1000

  spacing: 4

  Label {
    id: meterText
    Layout.fillWidth: true
    visible: text !== ""
    color: Theme.mainTextColor
    font.pointSize: Theme.tipFont.pointSize
    font.italic: true

    property bool richText: false
    textFormat: richText ? Text.RichText : Text.PlainText
    onLinkActivated: link => Qt.openUrlExternally(link)
  }

  ProgressBar {
    id: progressBar
    Layout.fillWidth: true
    from: 0
    to: 1

    Material.accent: {
      if (meterBar.value < meterBar.warningThreshold) {
        return meterBar.normalColor;
      } else if (meterBar.value < meterBar.criticalThreshold) {
        return meterBar.warningColor;
      } else {
        return meterBar.criticalColor;
      }
    }

    Behavior on value {
      NumberAnimation {
        duration: meterBar.animationDuration
      }
    }
  }
}
