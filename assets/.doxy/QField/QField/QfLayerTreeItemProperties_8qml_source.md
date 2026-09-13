

# File QfLayerTreeItemProperties.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfLayerTreeItemProperties.qml**](QfLayerTreeItemProperties_8qml.md)

[Go to the documentation of this file](QfLayerTreeItemProperties_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qgis
import org.qfield.core
import org.qfield.gui

QfPopup {
  id: popup

  property var layerTree
  property var index

  property bool zoomToButtonVisible: false
  property bool showFeaturesListButtonVisible: false
  property bool showVisibleFeaturesListDropdownVisible: false
  property bool reloadDataButtonVisible: false

  property bool trackingButtonVisible: false
  property var trackingButtonText

  property bool opacitySliderVisible: false

  parent: mainWindow.contentItem
  width: Math.min(childrenRect.width, mainWindow.width - QfTheme.popupScreenEdgeHorizontalMargin)
  height: Math.min(popupLayout.childrenRect.height + headerLayout.childrenRect.height + 20, mainWindow.height - Math.max(QfTheme.popupScreenEdgeVerticalMargin * 2, mainWindow.sceneTopMargin * 2 + 4, mainWindow.sceneBottomMargin * 2 + 4))
  x: (mainWindow.width - width) / 2
  y: (mainWindow.height - height) / 2
  closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
  focus: visible

  onClosed: {
    index = undefined;
  }

  onIndexChanged: {
    if (index === undefined)
      return;
    updateTitle();
    updateCredits();
    itemVisibleCheckBox.checked = layerTree.data(index, QfFlatLayerTreeModel.Visible);
    itemLabelsVisibleCheckBox.checked = layerTree.data(index, QfFlatLayerTreeModel.LabelsVisible);
    expandCheckBox.text = layerTree.data(index, QfFlatLayerTreeModel.Type) === QfFlatLayerTreeModel.Group ? qsTr('Expand group') : qsTr('Expand legend item');
    expandCheckBox.checked = !layerTree.data(index, QfFlatLayerTreeModel.IsCollapsed);
    reloadDataButtonVisible = layerTree.data(index, QfFlatLayerTreeModel.CanReloadData);
    zoomToButtonVisible = layerTree.data(index, QfFlatLayerTreeModel.HasSpatialExtent);
    showFeaturesListButtonVisible = isShowFeaturesListButtonVisible();
    showVisibleFeaturesListDropdownVisible = isShowVisibleFeaturesListDropdownVisible();
    trackingButtonVisible = isTrackingButtonVisible();
    trackingButtonText = trackingModel.layerInActiveTracking(layerTree.data(index, QfFlatLayerTreeModel.VectorLayerPointer)) ? qsTr('Stop tracking') : qsTr('Setup tracking');

    // the layer tree model returns -1 for items that do not support the opacity setting
    opacitySliderVisible = layerTree.data(index, QfFlatLayerTreeModel.Opacity) > -1;
  }

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
        Layout.leftMargin: reloadDataButtonVisible ? zoomInButton.width + headerLayout.spacing : 0
        topPadding: 10
        bottomPadding: 10
        text: ''
        font: QfTheme.strongFont
        horizontalAlignment: Text.AlignHCenter
        wrapMode: Text.WrapAnywhere
      }
      QfToolButton {
        id: zoomInButton
        Layout.alignment: Qt.AlignTop
        Layout.rightMargin: 0
        round: true
        visible: reloadDataButtonVisible

        bgcolor: "transparent"
        iconSource: QfTheme.getThemeVectorIcon('refresh_24dp')
        iconColor: QfTheme.mainTextColor

        onClicked: {
          layerTree.data(index, QfFlatLayerTreeModel.MapLayerPointer).reload();
          close();
          dashBoard.visible = false;
          displayToast(qsTr('Reload of layer %1 triggered').arg(layerTree.data(index, Qt.DisplayName)));
        }
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

        FontMetrics {
          id: fontMetrics
          font: lockText.font
        }

        Text {
          id: invalidText
          visible: index !== undefined && !layerTree.data(index, QfFlatLayerTreeModel.IsValid)
          Layout.fillWidth: true
          bottomPadding: 15

          wrapMode: Text.WordWrap
          textFormat: Text.RichText
          text: qsTr('This layer is invalid. This might be due to a network issue, a missing file or a misconfiguration of the project.')
          font: QfTheme.tipFont
          color: QfTheme.errorColor
        }

        CheckBox {
          id: expandCheckBox
          Layout.fillWidth: true
          topPadding: 5
          bottomPadding: 5
          text: qsTr('Expand legend item')
          font: QfTheme.defaultFont
          visible: index && layerTree.data(index, QfFlatLayerTreeModel.HasChildren) ? true : false

          onClicked: {
            layerTree.setData(index, checkState === Qt.Unchecked, QfFlatLayerTreeModel.IsCollapsed);
            close();
          }
        }

        CheckBox {
          id: itemVisibleCheckBox
          Layout.fillWidth: true
          topPadding: 5
          bottomPadding: 5
          text: qsTr('Show on map')
          font: QfTheme.defaultFont
          // visible for all layer tree items but nonspatial layers
          visible: index && layerTree.data(index, QfFlatLayerTreeModel.Checkable) && layerTree.data(index, QfFlatLayerTreeModel.HasSpatialExtent) ? true : false
          indicator.height: 16
          indicator.width: 16
          indicator.implicitHeight: 24
          indicator.implicitWidth: 24

          onClicked: {
            layerTree.setData(index, checkState === Qt.Checked, QfFlatLayerTreeModel.Visible);
            flatLayerTree.mapTheme = '';
            projectInfo.saveLayerTreeState();
            close();
          }
        }

        CheckBox {
          id: itemLabelsVisibleCheckBox
          Layout.fillWidth: true
          topPadding: 5
          bottomPadding: 5
          text: qsTr('Show labels')
          font: QfTheme.defaultFont
          visible: index && layerTree.data(index, QfFlatLayerTreeModel.HasLabels) ? true : false
          indicator.height: 16
          indicator.width: 16
          indicator.implicitHeight: 24
          indicator.implicitWidth: 24

          onClicked: {
            layerTree.setData(index, checkState === Qt.Checked, QfFlatLayerTreeModel.LabelsVisible);
            projectInfo.saveLayerStyle(layerTree.data(index, QfFlatLayerTreeModel.MapLayerPointer));
            close();
          }
        }

        RowLayout {
          id: opacitySlider

          Layout.fillWidth: true
          Layout.topMargin: 4
          Layout.bottomMargin: 4
          spacing: 4
          visible: opacitySliderVisible

          QfToolButton {
            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
            Layout.preferredWidth: 24
            Layout.leftMargin: 4
            width: 24
            height: 24
            padding: 0
            enabled: false
            bgcolor: "transparent"

            icon.source: QfTheme.getThemeVectorIcon("ic_opacity_black_24dp")
            icon.color: QfTheme.mainTextColor
          }

          Text {
            Layout.alignment: Qt.AlignVCenter
            text: qsTr("Opacity")
            font: QfTheme.defaultFont
            color: QfTheme.mainTextColor
          }

          QfSlider {
            id: slider
            Layout.fillWidth: true
            Layout.rightMargin: 5
            Layout.alignment: Qt.AlignVCenter
            value: index !== undefined ? layerTree.data(index, QfFlatLayerTreeModel.Opacity) * 100 : 0
            from: 0
            to: 100
            stepSize: 1
            suffixText: " %"
            height: 40

            onMoved: function () {
              layerTree.setData(index, value / 100, QfFlatLayerTreeModel.Opacity);
              projectInfo.saveLayerStyle(layerTree.data(index, QfFlatLayerTreeModel.MapLayerPointer));
            }
          }
        }

        QfButton {
          id: zoomToButton
          Layout.fillWidth: true
          Layout.topMargin: 5
          text: index ? layerTree.data(index, QfFlatLayerTreeModel.Type) === QfFlatLayerTreeModel.Group ? qsTr('Zoom to group') : layerTree.data(index, QfFlatLayerTreeModel.Type) === QfFlatLayerTreeModel.Legend && layerTree.data(index, QfFlatLayerTreeModel.LayerType) === "vectorlayer" ? qsTr('Zoom to parent layer') : qsTr('Zoom to layer') : ''
          visible: zoomToButtonVisible
          icon.source: QfTheme.getThemeVectorIcon('zoom_out_map_24dp')

          onClicked: {
            mapCanvas.mapSettings.extent = layerTree.nodeExtent(index, mapCanvas.mapSettings);
            close();
            dashBoard.visible = false;
          }
        }

        QfButton {
          id: showFeaturesList
          Layout.fillWidth: true
          Layout.topMargin: 5
          dropdown: showVisibleFeaturesListDropdownVisible
          text: qsTr('Show features list')
          visible: showFeaturesListButtonVisible
          icon.source: QfTheme.getThemeVectorIcon('ic_list_black_24dp')

          onClicked: {
            if (parseInt(layerTree.data(index, QfFlatLayerTreeModel.FeatureCount)) === 0) {
              displayToast(qsTr("The layer has no features"));
            } else {
              var vl = layerTree.data(index, QfFlatLayerTreeModel.VectorLayerPointer);
              var filter = layerTree.data(index, QfFlatLayerTreeModel.FilterExpression);
              featureListForm.model.setFeatures(vl, filter);
              if (layerTree.data(index, QfFlatLayerTreeModel.HasSpatialExtent)) {
                mapCanvas.mapSettings.extent = layerTree.nodeExtent(index, mapCanvas.mapSettings);
              }
            }
            close();
            dashBoard.visible = false;
          }

          onDropdownClicked: {
            showFeaturesMenu.popup(showFeaturesList.width - showFeaturesMenu.width + 10, showFeaturesList.y + 10);
          }
        }

        QfButton {
          id: trackingButton
          Layout.fillWidth: true
          Layout.topMargin: 5
          text: trackingButtonText
          visible: trackingButtonVisible
          icon.source: QfTheme.getThemeVectorIcon('directions_walk_24dp')

          onClicked: {
            const layer = layerTree.data(index, QfFlatLayerTreeModel.VectorLayerPointer);
            popup.close();
            if (trackingModel.layerInActiveTracking(layer)) {
              trackingModel.stopTracker(layer);
              displayToast(qsTr('Tracking on layer %1 stopped').arg(layer.name));
            } else {
              trackerSettings.prepareSettings(layer);
              trackerSettings.open();
            }
          }
        }

        Text {
          id: lockText

          property var padlockIcon: QfTheme.getThemeVectorIcon('ic_lock_black_24dp')
          property real padlockSize: fontMetrics.height - 5

          property bool isReadOnly: index !== undefined && layerTree.data(index, QfFlatLayerTreeModel.ReadOnly)
          property bool isFeatureAdditionLocked: index !== undefined && layerTree.data(index, QfFlatLayerTreeModel.FeatureAdditionLocked)
          property bool isAttributeEditingLocked: index !== undefined && layerTree.data(index, QfFlatLayerTreeModel.AttributeEditingLocked)
          property bool isGeometryEditingLocked: index !== undefined && layerTree.data(index, QfFlatLayerTreeModel.GeometryEditingLocked)
          property bool isFeatureDeletionLocked: index !== undefined && layerTree.data(index, QfFlatLayerTreeModel.FeatureDeletionLocked)

          visible: isReadOnly || isFeatureAdditionLocked || isAttributeEditingLocked || isGeometryEditingLocked || isFeatureDeletionLocked
          Layout.fillWidth: true
          topPadding: 5

          wrapMode: Text.WordWrap
          textFormat: Text.RichText
          text: {
            if (isReadOnly) {
              return qsTr('Read-only layer');
            } else if (isFeatureAdditionLocked || isAttributeEditingLocked || isGeometryEditingLocked || isFeatureDeletionLocked) {
              let locks = [];
              if (isFeatureAdditionLocked) {
                locks.push(qsTr('feature addition'));
              }
              if (isAttributeEditingLocked) {
                locks.push(qsTr('attribute editing'));
              }
              if (isGeometryEditingLocked) {
                locks.push(qsTr('geometry editing'));
              }
              if (isFeatureDeletionLocked) {
                locks.push(qsTr('feature deletion'));
              }
              return qsTr('Disabled layer permissions: %1').arg(locks.join(', '));
            }
            return '';
          }
          font: QfTheme.tipFont
          color: QfTheme.secondaryTextColor
        }

        Text {
          id: creditsText
          Layout.fillWidth: true
          Layout.topMargin: 5
          wrapMode: Text.WordWrap
          textFormat: Text.RichText
          text: ''
          font.pointSize: QfTheme.tipFont.pointSize
          font.italic: true
          color: QfTheme.secondaryTextColor

          onLinkActivated: link => {
            Qt.openUrlExternally(link);
          }
        }
      }
    }
  }

  QfMenu {
    id: showFeaturesMenu
    title: qsTr("Show Features Menu")

    MenuItem {
      text: qsTr('Show visible features list')

      font: QfTheme.defaultFont
      height: 48
      leftPadding: QfTheme.menuItemLeftPadding

      onTriggered: {
        if (parseInt(layerTree.data(index, QfFlatLayerTreeModel.FeatureCount)) === 0) {
          displayToast(qsTr("The layer has no features"));
        } else {
          var vl = layerTree.data(index, QfFlatLayerTreeModel.VectorLayerPointer);
          var filter = layerTree.data(index, QfFlatLayerTreeModel.FilterExpression);
          featureListForm.model.setFeatures(vl, filter, mapCanvas.mapSettings.visibleExtent);
        }
        close();
        dashBoard.visible = false;
      }
    }
  }

  Connections {
    target: layerTree

    function onDataChanged(topleft, bottomright, roles) {
      if (index === undefined)
        return;
      if (roles.includes(QfFlatLayerTreeModel.FeatureCount)) {
        updateTitle();
      }
    }
  }

  function updateTitle() {
    if (index === undefined)
      return;
    const type = layerTree.data(index, QfFlatLayerTreeModel.Type);
    const vl = layerTree.data(index, QfFlatLayerTreeModel.VectorLayerPointer);
    let title = layerTree.data(index, Qt.Name);
    if (vl) {
      if (type === QfFlatLayerTreeModel.Legend) {
        title += ' (' + vl.name + ')';
      } else if (type === QfFlatLayerTreeModel.Layer && layerTree.data(index, QfFlatLayerTreeModel.IsValid)) {
        var count = layerTree.data(index, QfFlatLayerTreeModel.FeatureCount);
        if (count !== undefined && count >= 0) {
          var countSuffix = ' [' + count + ']';
          if (!title.endsWith(countSuffix))
            title += countSuffix;
        }
      }
    }
    titleLabel.text = title !== undefined ? title : "";
  }

  function updateCredits() {
    var credits = '';
    if (index !== undefined) {
      credits = QfStringUtils.insertLinks(layerTree.data(index, QfFlatLayerTreeModel.Credits));
    } else {
      credits = '';
    }
    creditsText.text = credits;
    creditsText.visible = credits !== '';
  }

  function isTrackingButtonVisible() {
    if (!index)
      return false;
    return layerTree.data(index, QfFlatLayerTreeModel.Type) === QfFlatLayerTreeModel.Layer && !layerTree.data(index, QfFlatLayerTreeModel.ReadOnly) && layerTree.data(index, QfFlatLayerTreeModel.Trackable);
  }

  function isShowFeaturesListButtonVisible() {
    return layerTree.data(index, QfFlatLayerTreeModel.IsValid) && layerTree.data(index, QfFlatLayerTreeModel.LayerType) === 'vectorlayer';
  }

  function isShowVisibleFeaturesListDropdownVisible() {
    return isShowFeaturesListButtonVisible() && layerTree.data(index, QfFlatLayerTreeModel.HasSpatialExtent);
  }
}
```


