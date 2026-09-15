import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield.core

/**
 * \ingroup qml_gui
 */
Item {
  id: root

  property alias currentIndex: view.currentIndex
  property alias count: view.count

  default property alias panels: view.contentData

  implicitWidth: parent ? parent.width : 0
  implicitHeight: view.currentItem ? view.currentItem.implicitHeight : 0

  Behavior on implicitHeight {
    NumberAnimation {
      duration: 100
      easing.type: Easing.InQuad
    }
  }

  function collapse() {
    root.implicitHeight = 0;
  }

  component Panel: Item {
    id: panel
    property alias message: messageText.text
    default property alias actions: actionRow.children
    property int messageAlignment: Text.AlignHCenter
    property alias actionsFillWidth: actionRow.fillWidth

    implicitWidth: parent ? parent.width : 0
    implicitHeight: panelLayout.implicitHeight

    ColumnLayout {
      id: panelLayout
      anchors.left: parent.left
      anchors.right: parent.right
      anchors.verticalCenter: parent.verticalCenter
      spacing: 0

      RowLayout {
        Layout.fillWidth: true
        Layout.leftMargin: 12
        Layout.rightMargin: 12
        Layout.topMargin: 10
        Layout.bottomMargin: actionRow.children.length > 0 ? 6 : 10
        spacing: 10

        Text {
          id: messageText
          Layout.fillWidth: true
          Layout.alignment: Qt.AlignVCenter
          font: QfTheme.defaultFont
          color: QfTheme.mainTextColor
          horizontalAlignment: panel.messageAlignment
          wrapMode: Text.WordWrap
        }
      }

      RowLayout {
        id: actionRow

        property bool fillWidth: true

        Layout.fillWidth: fillWidth
        Layout.alignment: fillWidth ? Qt.AlignLeft : Qt.AlignHCenter
        Layout.leftMargin: 12
        Layout.rightMargin: 12
        Layout.bottomMargin: children.length > 0 ? 8 : 0
        spacing: 10
      }
    }
  }

  Rectangle {
    anchors.fill: parent
    color: Qt.rgba(QfTheme.mainColor.r, QfTheme.mainColor.g, QfTheme.mainColor.b, 0.1)
    border.color: QfTheme.mainColorSemiOpaque
    border.width: 1
    radius: 8
  }

  SwipeView {
    id: view
    anchors.fill: parent
    clip: true
    interactive: false
  }
}
