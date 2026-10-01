import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield.core
import org.qfield.gui

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
    property alias title: titleText.text
    property alias message: messageText.text
    property bool closable: false
    default property alias actions: actionRow.children

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
        Layout.leftMargin: 16
        Layout.rightMargin: panel.closable ? 4 : 16
        Layout.topMargin: panel.closable ? 4 : 14
        Layout.bottomMargin: actionRow.children.length > 0 ? 0 : 14
        spacing: 4

        ColumnLayout {
          Layout.fillWidth: true
          Layout.alignment: Qt.AlignTop
          Layout.topMargin: panel.closable ? 10 : 0
          spacing: 2

          Text {
            id: titleText
            Layout.fillWidth: true
            visible: text !== ""
            font: QfTheme.strongFont
            color: QfTheme.mainTextColor
            wrapMode: Text.WordWrap
          }

          Text {
            id: messageText
            Layout.fillWidth: true
            font: titleText.visible ? QfTheme.tipFont : QfTheme.defaultFont
            color: titleText.visible ? QfTheme.secondaryTextColor : QfTheme.mainTextColor
            wrapMode: Text.WordWrap
          }
        }

        QfToolButton {
          Layout.alignment: Qt.AlignTop
          visible: panel.closable
          iconSource: QfTheme.getThemeVectorIcon("ic_close_white_24dp")
          iconColor: QfTheme.secondaryTextColor

          onClicked: panel.SwipeView.view.collapse()
        }
      }

      RowLayout {
        id: actionRow

        Layout.fillWidth: true
        Layout.leftMargin: 16
        Layout.rightMargin: 16
        Layout.topMargin: children.length > 0 ? 12 : 0
        Layout.bottomMargin: children.length > 0 ? 14 : 0
        spacing: 8
      }
    }
  }
}
