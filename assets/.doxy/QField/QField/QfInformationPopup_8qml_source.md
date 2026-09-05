

# File QfInformationPopup.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfInformationPopup.qml**](QfInformationPopup_8qml.md)

[Go to the documentation of this file](QfInformationPopup_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qgis
import org.qfield.core
import org.qfield.gui

QfPopup {
  id: popup

  property alias header: pageHeader.title
  property alias title: titleText.text
  property alias description: descriptionText.text
  property alias descriptionFormat: descriptionText.textFormat
  property alias author: authorText.text

  parent: mainWindow.contentItem
  width: Math.min(450, mainWindow.width - QfTheme.popupScreenEdgeHorizontalMargin)
  height: Math.min(popupLayout.childrenRect.height + pageHeader.childrenRect.height + 30, mainWindow.height - Math.max(QfTheme.popupScreenEdgeVerticalMargin * 2, mainWindow.sceneTopMargin * 2 + 4, mainWindow.sceneBottomMargin * 2 + 4))
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
          font: QfTheme.strongTitleFont
          color: QfTheme.mainTextColor
        }

        Text {
          id: descriptionText
          Layout.fillWidth: true
          visible: text !== ""

          wrapMode: Text.Wrap
          textFormat: Text.MarkdownText
          text: ""
          font: QfTheme.defaultFont
          color: QfTheme.mainTextColor
        }

        ColumnLayout {
          Layout.fillWidth: true
          spacing: 5
          visible: authorText.text !== ""

          Text {
            id: authorLabel
            Layout.fillWidth: true
            font: QfTheme.strongFont
            color: QfTheme.mainTextColor

            text: qsTr("Author")
          }

          Text {
            id: authorText
            Layout.fillWidth: true

            wrapMode: Text.Wrap
            textFormat: Text.MarkdownText
            text: ""
            font: QfTheme.defaultFont
            color: QfTheme.mainTextColor
            clip: true
          }
        }
      }
    }
  }
}
```


