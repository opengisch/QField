import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qgis
import org.qfield
import Theme

/**
 * \ingroup qml
 */
QfPopup {
  id: popup

  property alias header: pageHeader.title
  property alias title: titleText.text
  property alias description: descriptionText.text
  property alias descriptionFormat: descriptionText.textFormat
  property alias author: authorText.text

  parent: mainWindow.contentItem
  width: Math.min(450, mainWindow.width - Theme.popupScreenEdgeHorizontalMargin)
  height: Math.min(popupLayout.childrenRect.height + pageHeader.childrenRect.height + 30, mainWindow.height - Math.max(Theme.popupScreenEdgeVerticalMargin * 2, mainWindow.sceneTopMargin * 2 + 4, mainWindow.sceneBottomMargin * 2 + 4))
  x: (mainWindow.width - width) / 2
  y: (mainWindow.height - height) / 2
  closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
  focus: visible

  Page {
    id: popupContent
    width: parent.width
    height: parent.height
    padding: 5

    header: QfPageHeader {
      id: pageHeader
      showBackButton: true
      showCancelButton: false
      showMenuButton: false
      showApplyButton: false

      title: ""

      onBack: {
        popup.close();
      }
    }

    Flickable {
      anchors.fill: parent
      anchors.margins: 5
      contentWidth: popupContent.width - 20
      contentHeight: popupLayout.childrenRect.height
      clip: true

      ScrollBar.vertical: QfScrollBar {}

      ColumnLayout {
        id: popupLayout
        width: popupContent.width - 30
        x: 5
        spacing: 10

        Text {
          id: titleText
          Layout.fillWidth: true
          visible: text !== ""

          wrapMode: Text.Wrap
          text: ""
          font: Theme.strongTitleFont
          color: Theme.mainTextColor
        }

        Text {
          id: descriptionText
          Layout.fillWidth: true
          visible: text !== ""

          wrapMode: Text.Wrap
          textFormat: Text.MarkdownText
          text: ""
          font: Theme.defaultFont
          color: Theme.mainTextColor
        }

        ColumnLayout {
          Layout.fillWidth: true
          spacing: 5
          visible: authorText.text !== ""

          Text {
            id: authorLabel
            Layout.fillWidth: true
            font: Theme.strongFont
            color: Theme.mainTextColor

            text: qsTr("Author")
          }

          Text {
            id: authorText
            Layout.fillWidth: true

            wrapMode: Text.Wrap
            textFormat: Text.MarkdownText
            text: ""
            font: Theme.defaultFont
            color: Theme.mainTextColor
            clip: true
          }
        }
      }
    }
  }
}
