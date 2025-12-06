

# File LocatorSettings.qml

[**File List**](files.md) **>** [**qml**](dir_1d81f7f4362e4398c0a695c4e466fcb0.md) **>** [**LocatorSettings.qml**](LocatorSettings_8qml.md)

[Go to the documentation of this file](LocatorSettings_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qgis
import org.qfield
import Theme

QfPopup {
  id: popup

  property alias locatorFiltersModel: locatorfiltersList.model

  width: Math.min(400, mainWindow.width - Theme.popupScreenEdgeHorizontalMargin)
  x: (parent.width - width) / 2
  y: (parent.height - height) / 2
  closePolicy: Popup.CloseOnEscape
  parent: Overlay.overlay
  focus: visible

  Page {
    id: page
    width: parent.width
    height: locatorfiltersList.height + 60
    padding: 5
    header: QfPageHeader {
      id: pageHeader
      title: qsTr("Search Bar Settings")

      showBackButton: false
      showApplyButton: false
      showCancelButton: true
      backgroundFill: false

      onCancel: {
        popup.close();
      }
    }

    Column {
      spacing: 4
      width: parent.width

      ListView {
        id: locatorfiltersList
        width: parent.width
        height: Math.min(contentHeight, mainWindow.height - 160)
        leftMargin: 5
        rightMargin: 5
        clip: true

        ScrollBar.vertical: QfScrollBar {
          verticalPadding: 15
        }

        delegate: Rectangle {
          id: rectangle
          width: parent ? parent.width : undefined
          height: inner.height
          color: "transparent"

          ColumnLayout {
            id: inner
            width: parent.width

            Text {
              Layout.fillWidth: true
              topPadding: 5
              text: Name
              font: Theme.defaultFont
              color: Theme.mainTextColor
              wrapMode: Text.WordWrap
            }
            Text {
              Layout.fillWidth: true
              bottomPadding: 5
              text: Description
              font: Theme.tipFont
              color: Theme.secondaryTextColor
              wrapMode: Text.WordWrap
            }
            Text {
              visible: Default ? false : true
              Layout.fillWidth: true
              bottomPadding: 5
              text: qsTr('When disabled, this locator filter can still be used by typing the prefix %1 in the search bar.').arg('<b>' + Prefix + '</b>')
              font: Theme.tipFont
              color: Theme.secondaryTextColor
              wrapMode: Text.WordWrap
            }
            CheckBox {
              Layout.fillWidth: true
              topPadding: 5
              bottomPadding: 5
              text: qsTr('Enable %1 locator by default').arg('<b>' + Name + '</b>')
              font: Theme.tipFont
              indicator.height: 16
              indicator.width: 16
              indicator.implicitHeight: 24
              indicator.implicitWidth: 24
              checked: Default ? true : false
              onClicked: Default = checked
            }
          }
        }
      }
    }
  }
}
```


