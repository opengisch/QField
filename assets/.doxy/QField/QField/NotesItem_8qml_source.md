

# File NotesItem.qml

[**File List**](files.md) **>** [**qml**](dir_1d81f7f4362e4398c0a695c4e466fcb0.md) **>** [**NotesItem.qml**](NotesItem_8qml.md)

[Go to the documentation of this file](NotesItem_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qgis
import org.qfield
import Theme

QfPopup {
  id: popup

  property alias title: pageHader.title
  property alias notes: notesText.text

  parent: mainWindow.contentItem
  width: Math.min(450, mainWindow.width - Theme.popupScreenEdgeHorizontalMargin)
  height: Math.min(popupLayout.childrenRect.height + pageHader.childrenRect.height + 20, mainWindow.height - Math.max(Theme.popupScreenEdgeVerticalMargin * 2, mainWindow.sceneTopMargin * 2 + 4, mainWindow.sceneBottomMargin * 2 + 4))
  x: (mainWindow.width - width) / 2
  y: (mainWindow.height - height) / 2
  closePolicy: Popup.CloseOnEscape
  focus: visible

  Page {
    id: popupContent
    width: parent.width
    height: parent.height
    padding: 5

    header: QfPageHeader {
      id: pageHader
      showBackButton: true
      showCancelButton: false
      showMenuButton: false
      showApplyButton: false

      title: ""

      onBack: {
        popup.close();
      }
    }

    ScrollView {
      padding: 5
      ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
      ScrollBar.vertical: QfScrollBar {}
      contentWidth: popupLayout.width
      contentHeight: popupLayout.childrenRect.height
      height: parent.height
      clip: true

      ColumnLayout {
        id: popupLayout
        width: popupContent.width - 20
        spacing: 0

        Text {
          id: notesText
          Layout.fillWidth: true
          Layout.leftMargin: 5
          Layout.rightMargin: 5

          wrapMode: Text.WordWrap
          textFormat: Text.RichText
          text: ""
          font: Theme.defaultFont
          color: Theme.mainTextColor
          clip: true
        }
      }
    }
  }
}
```


