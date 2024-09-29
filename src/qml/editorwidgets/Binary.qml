import QtQuick
import QtQuick.Controls
import org.qfield
import Theme

EditorWidgetBase {
  id: binaryItem

  height: childrenRect.height

  Label {
    id: binaryValue
    topPadding: 10
    bottomPadding: 10
    anchors.left: parent.left
    anchors.right: parent.right
    font: Theme.defaultFont
    color: Theme.mainTextColor
    opacity: 0.45
    wrapMode: Text.Wrap

    text: qsTr('(Blob)')
  }

  Rectangle {
    anchors.left: parent.left
    anchors.right: parent.right
    y: binaryValue.height - height - binaryValue.bottomPadding / 2
    implicitWidth: 120
    height: 1
    color: Theme.accentLightColor
  }
}
