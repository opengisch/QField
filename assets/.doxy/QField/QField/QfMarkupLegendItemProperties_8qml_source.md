

# File QfMarkupLegendItemProperties.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfMarkupLegendItemProperties.qml**](QfMarkupLegendItemProperties_8qml.md)

[Go to the documentation of this file](QfMarkupLegendItemProperties_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qgis
import org.qfield.core
import org.qfield.gui

QfPopup {
  id: popup

  property QfMarkupCollection markupCollection

  parent: mainWindow.contentItem
  width: Math.min(childrenRect.width, mainWindow.width - QfTheme.popupScreenEdgeHorizontalMargin)
  height: Math.min(popupLayout.childrenRect.height + headerLayout.childrenRect.height + 40, mainWindow.height - Math.max(QfTheme.popupScreenEdgeVerticalMargin * 2, mainWindow.sceneTopMargin * 2 + 4, mainWindow.sceneBottomMargin * 2 + 4))
  x: (mainWindow.width - width) / 2
  y: (mainWindow.height - height) / 2
  closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
  focus: visible

  Page {
    id: popupContent
    width: parent.width
    height: parent.height
    padding: 0
    header: RowLayout {
      id: headerLayout
      spacing: 2
      Label {
        id: titleLabel
        Layout.fillWidth: true
        Layout.margins: 10
        text: markupCollection ? markupCollection.name : ''
        font: QfTheme.strongFont
        horizontalAlignment: Text.AlignHCenter
        wrapMode: Text.WrapAnywhere
      }
    }

    ScrollView {
      anchors.fill: parent
      padding: 5
      ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
      ScrollBar.vertical: QfScrollBar {}
      contentWidth: popupLayout.childrenRect.width
      contentHeight: popupLayout.childrenRect.height
      clip: true

      ColumnLayout {
        id: popupLayout
        width: popupContent.width - 10
        spacing: 4

        CheckBox {
          id: itemVisibleCheckBox

          property bool isVisible: markupCollection ? markupManager.hiddenCollectionUuids.indexOf(markupCollection.uuid) === -1 : false

          Layout.fillWidth: true
          topPadding: 5
          bottomPadding: 5
          text: qsTr('Show on map')
          font: QfTheme.defaultFont
          indicator.height: 16
          indicator.width: 16
          indicator.implicitHeight: 24
          indicator.implicitWidth: 24
          checked: isVisible

          onClicked: {
            if (markupCollection) {
              let hiddenCollectionUuids = markupManager.hiddenCollectionUuids;
              const idx = hiddenCollectionUuids.indexOf(markupCollection.uuid);
              if (idx === -1) {
                hiddenCollectionUuids.push(markupCollection.uuid);
              } else {
                hiddenCollectionUuids.splice(idx, 1);
              }
              markupManager.hiddenCollectionUuids = hiddenCollectionUuids;
              projectInfo.saveVisibleMarkupCollections();
            }
          }
        }

        QfButton {
          id: zoomToButton
          Layout.fillWidth: true
          Layout.topMargin: 5
          text: qsTr('Zoom to collection')
          icon.source: QfTheme.getThemeVectorIcon('zoom_out_map_24dp')

          onClicked: {
            if (markupCollection) {
              if (markupCollection.items.length === 0) {
                displayToast(qsTr("The collection has no features"));
              } else {
                let extent = markupCollection.extent();
                extent = QfGeometryUtils.reprojectRectangle(extent, QfCoordinateReferenceSystemUtils.wgs84Crs(), mapCanvas.mapSettings.destinationCrs);
                mapCanvas.mapSettings.extent = extent;
              }
              close();
              dashBoard.visible = false;
            }
          }
        }

        QfButton {
          id: showFeaturesList

          Layout.fillWidth: true
          Layout.topMargin: 5
          text: qsTr('Show features list')
          icon.source: QfTheme.getThemeVectorIcon('ic_list_black_24dp')

          onClicked: {
            if (markupCollection) {
              if (markupCollection.items.length === 0) {
                displayToast(qsTr("The collection has no features"));
              } else {
                featureListForm.model.setFeatures(markupCollection);
                let extent = markupCollection.extent();
                extent = QfGeometryUtils.reprojectRectangle(extent, QfCoordinateReferenceSystemUtils.wgs84Crs(), mapCanvas.mapSettings.destinationCrs);
                mapCanvas.mapSettings.extent = extent;
              }
              close();
              dashBoard.visible = false;
            }
          }
        }

        QfButton {
          id: exportFeaturesList

          Layout.fillWidth: true
          Layout.topMargin: 5
          text: qsTr('Export collection')
          icon.source: QfTheme.getThemeVectorIcon('ic_export_black_24dp')

          onClicked: {
            if (markupCollection) {
              if (markupCollection.items.length === 0) {
                displayToast(qsTr("The collection has no features"));
                return;
              }
              markupManager.exportCollection(markupCollection);
            }
          }
        }
      }
    }
  }
}
```


