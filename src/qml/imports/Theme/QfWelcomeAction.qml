import QtQuick
import QtQuick.Layouts
import org.qfield
import Theme

/**
 * \ingroup qml
 */
ColumnLayout {
  id: root

  property url iconSource
  property color iconColor
  property string label
  signal clicked

  spacing: 4

  QfToolButton {
    Layout.alignment: Qt.AlignHCenter
    Layout.preferredWidth: Math.min(Screen.height / 4, root.width / 1.5)
    Layout.preferredHeight: Layout.preferredWidth
    icon.width: width / 2.2
    icon.height: height / 2.2
    bgcolor: Theme.controlBackgroundAlternateColor
    round: false
    roundborder: true
    iconSource: root.iconSource
    iconColor: root.iconColor
    smooth: true
    onClicked: root.clicked()
  }

  Text {
    Layout.fillWidth: true
    text: root.label
    horizontalAlignment: Text.AlignHCenter
    wrapMode: Text.WordWrap
    color: Theme.mainTextColor
    font: Theme.tipFont
  }
}
