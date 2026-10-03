

# File QfBadLayerItem.qml

[**File List**](files.md) **>** [**app**](dir_5194e5fea318fda12687127c23f8aba1.md) **>** [**qml**](dir_408f7dc2ccecf14179dc3d37678eaa5b.md) **>** [**QfBadLayerItem.qml**](QfBadLayerItem_8qml.md)

[Go to the documentation of this file](QfBadLayerItem_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield.core
import org.qfield.app
import org.qfield.gui

Page {
  id: badLayerPage

  property alias model: table.model

  padding: 5

  width: mainWindow.width
  height: mainWindow.height

  header: QfPageHeader {
    title: qsTr('Unable to load some layers')

    showBackButton: false
    showApplyButton: false
    showCancelButton: true

    topMargin: mainWindow.sceneTopMargin

    onFinished: badLayerPage.visible = false
  }

  ColumnLayout {
    anchors.margins: 8
    anchors.bottomMargin: 8 + mainWindow.sceneBottomMargin
    anchors.fill: parent
    Layout.margins: 0
    spacing: 10

    Label {
      Layout.fillWidth: true
      Layout.fillHeight: false

      text: qsTr("The following layers could not be loaded, please review those and reconfigure the QGIS project.")
      font: QfTheme.defaultFont
      color: QfTheme.mainTextColor
      wrapMode: Text.WordWrap
    }

    Rectangle {
      Layout.fillWidth: true
      Layout.fillHeight: true
      color: QfTheme.controlBackgroundColor
      border.color: QfTheme.controlBorderColor
      border.width: 1

      ListView {
        id: table
        flickableDirection: Flickable.VerticalFlick
        boundsBehavior: Flickable.StopAtBounds
        clip: true
        spacing: 2
        anchors.fill: parent

        model: QfBadLayerHandler {
          project: qgisProject

          onBadLayersFound: {
            badLayerPage.visible = true;
          }
        }

        delegate: Rectangle {
          id: rectangle
          width: parent ? parent.width : undefined
          height: line.height
          color: "transparent"

          Column {
            id: line
            width: parent.width
            spacing: 0

            Text {
              id: name
              width: rectangle.width - padding * 2
              padding: 5
              text: LayerName
              font: QfTheme.strongTipFont
              color: QfTheme.mainTextColor
              wrapMode: Text.WordWrap
            }
            Text {
              id: uri
              width: rectangle.width - padding * 2
              padding: 5
              text: DataSource
              font: QfTheme.tipFont
              color: QfTheme.secondaryTextColor
              wrapMode: Text.WordWrap
            }
          }
        }
      }
    }

    Label {
      Layout.fillWidth: true
      Layout.fillHeight: false
      Layout.topMargin: 5

      text: qsTr('You may check the %1Portable Project%2 documentation page for more help.').arg("<a href=\"https://docs.qfield.org/how-to/project-setup/data_source_and_project_paths/\">").arg("</a>")
      textFormat: Text.RichText
      font: QfTheme.tipFont
      color: QfTheme.secondaryTextColor
      wrapMode: Text.WordWrap

      onLinkActivated: link => {
        Qt.openUrlExternally(link);
      }
    }
  }
}
```


