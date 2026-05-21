import QtQuick
import QtQuick.Layouts
import org.qfield

/**
 * \ingroup qml
 */
Rectangle {
  id: container

  default property alias contents: containerLayout.children
  property alias layout: containerLayout
  property alias title: title.text
  property alias header: headerLayout.children

  width: parent.width
  height: childrenRect.height
  color: Theme.mainBackgroundColorSemiOpaque
  radius: 8
  clip: true

  Column {
    id: containerLayout
    width: parent.width - 10
    anchors.horizontalCenter: parent.horizontalCenter
    topPadding: 5
    bottomPadding: 5
    spacing: 4

    RowLayout {
      width: parent.width

      Text {
        id: title
        Layout.fillWidth: true
        Layout.leftMargin: 6
        Layout.rightMargin: 6
        text: qsTr("Positioning")
        font: Theme.strongTipFont
        color: Theme.mainTextColor
      }

      RowLayout {
        id: headerLayout
        Layout.rightMargin: 6
      }
    }
  }
}
