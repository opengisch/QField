import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qgis
import org.qfield
import Theme

/**
 * \ingroup qml
 */
Popup {
  id: popup

  property alias title: titleLabel.text
  property alias notes: notesText.text

  parent: mainWindow.contentItem
  width: Math.min(childrenRect.width, mainWindow.width - Theme.popupScreenEdgeMargin)
  height: Math.min(popupLayout.childrenRect.height + headerLayout.childrenRect.height + 20, mainWindow.height - Math.max(Theme.popupScreenEdgeMargin * 2, mainWindow.sceneTopMargin * 2 + 4, mainWindow.sceneBottomMargin * 2 + 4))
  x: (mainWindow.width - width) / 2
  y: (mainWindow.height - height) / 2
  padding: 0
  modal: true
  closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
  focus: visible

  Page {
    id: popupContent
    width: parent.width
    height: parent.height
    padding: 10

    header: RowLayout {
      id: headerLayout
      spacing: 2
      Label {
        id: titleLabel
        Layout.fillWidth: true
        Layout.leftMargin: 10
        topPadding: 10
        bottomPadding: 10
        text: ""
        font: Theme.strongFont
        horizontalAlignment: Text.AlignLeft
        wrapMode: Text.WordWrap
      }
    }

    ScrollView {
      padding: 0
      ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
      ScrollBar.vertical: QfScrollBar {
      }
      contentWidth: popupLayout.childrenRect.width
      contentHeight: popupLayout.childrenRect.height
      height: parent.height
      clip: true

      ColumnLayout {
        id: popupLayout
        width: popupContent.width - 20
        spacing: 4

        Text {
          id: notesText
          Layout.fillWidth: true
          rightPadding: 10
          bottomPadding: 15

          wrapMode: Text.WordWrap
          textFormat: Text.RichText
          text: ""
          font: Theme.tipFont
          color: Theme.mainTextColor
        }
      }
    }
  }
}
