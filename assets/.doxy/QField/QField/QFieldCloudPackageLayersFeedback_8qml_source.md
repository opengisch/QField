

# File QFieldCloudPackageLayersFeedback.qml

[**File List**](files.md) **>** [**qml**](dir_1d81f7f4362e4398c0a695c4e466fcb0.md) **>** [**QFieldCloudPackageLayersFeedback.qml**](QFieldCloudPackageLayersFeedback_8qml.md)

[Go to the documentation of this file](QFieldCloudPackageLayersFeedback_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield
import Theme

QfDialog {
  property int selectedCount: 0
  property bool isDeleted: false
  property alias packagedLayersListViewModel: packagedLayersListView.model

  parent: mainWindow.contentItem
  width: mainWindow.width - Theme.popupScreenEdgeVerticalMargin * 2
  height: Math.min(300 + packagedLayersListView.contentHeight, mainWindow.height - Math.max(Theme.popupScreenEdgeVerticalMargin * 2, mainWindow.sceneTopMargin * 2 + 4, mainWindow.sceneBottomMargin * 2 + 4))

  title: qsTr("QFieldCloud had troubles packaging your project")

  ColumnLayout {
    id: layout
    anchors.fill: parent

    Label {
      Layout.fillWidth: true
      text: qsTr("Some layers have not been packaged correctly on QFieldCloud. These layers might be misconfigured or their data source is not accessible from the QFieldCloud server. Please check the logs of the latest packaging job on the qfield.cloud website.")
      wrapMode: Text.WordWrap
    }

    ListView {
      id: packagedLayersListView
      model: []

      Layout.fillWidth: true
      Layout.fillHeight: true
      Layout.topMargin: 10
      Layout.preferredHeight: contentHeight

      delegate: Text {
        width: parent.width
        text: modelData
        font: Theme.resultFont
        color: Theme.secondaryTextColor
        wrapMode: Text.WordWrap
      }
    }
  }

  standardButtons: Dialog.Ok
}
```


