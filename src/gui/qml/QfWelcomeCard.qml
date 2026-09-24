import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield.core

/**
 * \ingroup qml_gui
 */
SwipeView {
  id: root

  clip: true
  interactive: false
  property bool collapsed: false
  Layout.preferredHeight: collapsed ? 0 : (currentItem ? currentItem.implicitHeight : implicitHeight)

  // The index the card should rest on. SwipeView jumps to the last page as
  // pages are appended incrementally, so we re-assert this once they settle.
  property int startIndex: 0
  onCountChanged: Qt.callLater(root.applyStartIndex)
  Component.onCompleted: Qt.callLater(root.applyStartIndex)
  function applyStartIndex() {
    if (count > 0) {
      currentIndex = Math.min(startIndex, count - 1);
    }
  }

  Behavior on implicitHeight {
    NumberAnimation {
      duration: 100
      easing.type: Easing.InQuad
    }
  }

  function collapse() {
    root.collapsed = true;
  }

  background: Rectangle {
    color: Qt.rgba(QfTheme.mainColor.r, QfTheme.mainColor.g, QfTheme.mainColor.b, 0.1)
    border.color: QfTheme.mainColorSemiOpaque
    border.width: 1
    radius: 8
  }

  component Panel: Item {
    id: panel
    property alias message: messageText.text
    default property alias actions: actionRow.children
    property int messageAlignment: Text.AlignHCenter

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

        Layout.fillWidth: true
        Layout.leftMargin: 12
        Layout.rightMargin: 12
        Layout.bottomMargin: children.length > 0 ? 8 : 0
        spacing: 10
      }
    }
  }
}
