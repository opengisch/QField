import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield.core
import org.qfield.gui

/**
 * \ingroup qml_gui
 */
Item {
  id: root
  property alias message: messageText.text
  property string iconSource: ""
  default property alias actions: actionRow.children
  property int messageAlignment: Text.AlignHCenter
  property alias actionsFillWidth: actionRow.fillWidth

  implicitWidth: parent ? parent.width : 0
  implicitHeight: layout.implicitHeight

  Rectangle {
    anchors.fill: parent
    color: Qt.rgba(QfTheme.mainColor.r, QfTheme.mainColor.g, QfTheme.mainColor.b, 0.1)
    border.color: QfTheme.mainColor
    border.width: 1
    radius: 8
  }

  ColumnLayout {
    id: layout
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.verticalCenter: parent.verticalCenter
    spacing: 0

    RowLayout {
      Layout.fillWidth: true
      Layout.leftMargin: 12
      Layout.rightMargin: 12
      Layout.topMargin: 12
      Layout.bottomMargin: actionRow.children.length > 0 ? 8 : 12
      spacing: 10

      QfToolButton {
        visible: root.iconSource !== ""
        Layout.alignment: Qt.AlignTop
        enabled: false
        bgcolor: "transparent"
        iconColor: QfTheme.mainColor
        iconSource: root.iconSource
      }

      Text {
        id: messageText
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignVCenter
        font: QfTheme.defaultFont
        color: QfTheme.mainTextColor
        horizontalAlignment: root.messageAlignment
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
      Layout.bottomMargin: children.length > 0 ? 10 : 0
      spacing: 6
    }
  }
}
