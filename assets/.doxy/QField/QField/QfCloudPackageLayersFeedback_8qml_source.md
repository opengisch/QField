

# File QfCloudPackageLayersFeedback.qml

[**File List**](files.md) **>** [**app**](dir_5194e5fea318fda12687127c23f8aba1.md) **>** [**qml**](dir_408f7dc2ccecf14179dc3d37678eaa5b.md) **>** [**QfCloudPackageLayersFeedback.qml**](QfCloudPackageLayersFeedback_8qml.md)

[Go to the documentation of this file](QfCloudPackageLayersFeedback_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield.core
import org.qfield.gui

QfDialog {
  property int selectedCount: 0
  property bool isDeleted: false
  property alias packagedLayersListViewModel: packagedLayersListView.model

  parent: mainWindow.contentItem
  width: mainWindow.width - QfTheme.popupScreenEdgeVerticalMargin * 2
  height: Math.min(300 + packagedLayersListView.contentHeight, mainWindow.height - Math.max(QfTheme.popupScreenEdgeVerticalMargin * 2, mainWindow.sceneTopMargin * 2 + 4, mainWindow.sceneBottomMargin * 2 + 4))

  ColumnLayout {
    id: layout
    anchors.fill: parent

    Label {
      Layout.fillWidth: true
      text: qsTr("Some layers have not been packaged correctly. These layers might be misconfigured or their data source is not accessible from the QFieldCloud server.")
      font: QfTheme.defaultFont
      wrapMode: Text.WordWrap
    }

    Label {
      Layout.fillWidth: true
      text: qsTr("Please check the detailed feedback below and the latest packaging job logs on the QFieldCloud website.")
      font: QfTheme.defaultFont
      wrapMode: Text.WordWrap
    }

    Rectangle {
      Layout.fillWidth: true
      Layout.fillHeight: true
      Layout.topMargin: 10
      Layout.preferredHeight: packagedLayersListView.contentHeight
      color: QfTheme.controlBackgroundColor
      border.color: QfTheme.controlBorderColor
      border.width: 1

      ListView {
        id: packagedLayersListView
        anchors.fill: parent
        clip: true

        model: []

        delegate: Item {
          width: ListView.view.width
          height: descriptionText.contentHeight + 10

          Text {
            id: descriptionText
            anchors.centerIn: parent
            width: parent.width - 20
            text: modelData
            font: QfTheme.resultFont
            color: QfTheme.secondaryTextColor
            wrapMode: Text.Wrap
          }
        }
      }
    }
  }

  standardButtons: Dialog.Ok
}
```


