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
  width: Math.min(childrenRect.width, mainWindow.width - Theme.popupScreenEdgeMargin)
  height: Math.min(popupLayout.childrenRect.height + headerLayout.childrenRect.height + 20, mainWindow.height - Math.max(Theme.popupScreenEdgeMargin * 2, mainWindow.sceneTopMargin * 2 + 4, mainWindow.sceneBottomMargin * 2 + 4))
  x: (mainWindow.width - width) / 2
  y: (mainWindow.height - height) / 2
  padding: 0
  modal: true
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
    itemVisibleCheckBox.checked = layerTree.data(index, FlatLayerTreeModel.Visible);
    itemLabelsVisibleCheckBox.checked = layerTree.data(index, FlatLayerTreeModel.LabelsVisible);
    expandCheckBox.text = layerTree.data(index, FlatLayerTreeModel.Type) === 'group' ? qsTr('Expand group') : qsTr('Expand legend item');
    expandCheckBox.checked = !layerTree.data(index, FlatLayerTreeModel.IsCollapsed);
    reloadDataButtonVisible = layerTree.data(index, FlatLayerTreeModel.CanReloadData);
    zoomToButtonVisible = layerTree.data(index, FlatLayerTreeModel.HasSpatialExtent);
    showFeaturesListButtonVisible = isShowFeaturesListButtonVisible();
    showVisibleFeaturesListDropdownVisible = isShowVisibleFeaturesListDropdownVisible();
    trackingButtonVisible = isTrackingButtonVisible();
    trackingButtonText = trackingModel.layerInTracking(layerTree.data(index, FlatLayerTreeModel.VectorLayerPointer)) ? qsTr('Stop tracking') : qsTr('Setup tracking');

    // the layer tree model returns -1 for items that do not support the opacity setting
    opacitySliderVisible = layerTree.data(index, FlatLayerTreeModel.Opacity) > -1;
  }

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
        text: ''
        font: Theme.strongFont
        horizontalAlignment: Text.AlignLeft
        wrapMode: Text.WordWrap
      }
      QfToolButton {
        id: zoomInButton
        Layout.alignment: Qt.AlignTop
        Layout.rightMargin: 0
        round: true
        visible: reloadDataButtonVisible

        bgcolor: "transparent"
        iconSource: Theme.getThemeVectorIcon('refresh_24dp')
        iconColor: Theme.mainTextColor

        onClicked: {
          layerTree.data(index, FlatLayerTreeModel.MapLayerPointer).reload();
          close();
          dashBoard.visible = false;
          displayToast(qsTr('Reload of layer %1 triggered').arg(layerTree.data(index, Qt.DisplayName)));
        }
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

        FontMetrics {
          id: fontMetrics
          font: lockText.font
        }

        Text {
          id: invalidText
          visible: index !== undefined && !layerTree.data(index, FlatLayerTreeModel.IsValid)
          Layout.fillWidth: true
          bottomPadding: 15

          wrapMode: Text.WordWrap
          textFormat: Text.RichText
          text: qsTr('This layer is invalid. This might be due to a network issue, a missing file or a misconfiguration of the project.')
          font: Theme.tipFont
          color: Theme.errorColor
        }

        CheckBox {
          id: expandCheckBox
          Layout.fillWidth: true
          topPadding: 5
          bottomPadding: 5
          text: qsTr('Expand legend item')
          font: Theme.defaultFont
          visible: index && layerTree.data(index, FlatLayerTreeModel.HasChildren) ? true : false

          onClicked: {
            layerTree.setData(index, checkState === Qt.Unchecked, FlatLayerTreeModel.IsCollapsed);
            close();
          }
        }

        CheckBox {
          id: itemVisibleCheckBox
          Layout.fillWidth: true
          topPadding: 5
          bottomPadding: 5
          text: qsTr('Show on map')
          font: Theme.defaultFont
          // visible for all layer tree items but nonspatial layers
          visible: index && layerTree.data(index, FlatLayerTreeModel.HasSpatialExtent) ? true : false
          indicator.height: 16
          indicator.width: 16
          indicator.implicitHeight: 24
          indicator.implicitWidth: 24

          onClicked: {
            layerTree.setData(index, checkState === Qt.Checked, FlatLayerTreeModel.Visible);
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
          font: Theme.defaultFont
          visible: index && layerTree.data(index, FlatLayerTreeModel.HasLabels) ? true : false
          indicator.height: 16
          indicator.width: 16
          indicator.implicitHeight: 24
          indicator.implicitWidth: 24

          onClicked: {
            layerTree.setData(index, checkState === Qt.Checked, FlatLayerTreeModel.LabelsVisible);
            projectInfo.saveLayerStyle(layerTree.data(index, FlatLayerTreeModel.MapLayerPointer));
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
            Layout.alignment: Qt.AlignVCenter | Qt.alignHCenter
            Layout.leftMargin: 3
            Layout.rightMargin: 1
            width: 24
            height: 24
            padding: 0
            enabled: false

            icon.source: Theme.getThemeVectorIcon("ic_opacity_black_24dp")
            icon.color: Theme.mainTextColor
          }

          ColumnLayout {
            Layout.alignment: Layout.Center
            Layout.rightMargin: 6
            spacing: 0

            Text {
              Layout.fillWidth: true
              text: qsTr("Opacity")
              font: Theme.defaultFont
              color: Theme.mainTextColor
            }

            QfSlider {
              id: slider
              Layout.fillWidth: true
              value: index !== undefined ? layerTree.data(index, FlatLayerTreeModel.Opacity) * 100 : 0
              from: 0
              to: 100
              stepSize: 1
              suffixText: " %"
              height: 40

              onMoved: function () {
                layerTree.setData(index, value / 100, FlatLayerTreeModel.Opacity);
                projectInfo.saveLayerStyle(layerTree.data(index, FlatLayerTreeModel.MapLayerPointer));
              }
            }
          }
        }

        QfButton {
          id: zoomToButton
          Layout.fillWidth: true
          Layout.topMargin: 5
          text: index ? layerTree.data(index, FlatLayerTreeModel.Type) === 'group' ? qsTr('Zoom to group') : layerTree.data(index, FlatLayerTreeModel.Type) === 'legend' ? qsTr('Zoom to parent layer') : qsTr('Zoom to layer') : ''
          visible: zoomToButtonVisible
          icon.source: Theme.getThemeVectorIcon('zoom_out_map_24dp')

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
          icon.source: Theme.getThemeVectorIcon('ic_list_black_24dp')

          onClicked: {
            if (parseInt(layerTree.data(index, FlatLayerTreeModel.FeatureCount)) === 0) {
              displayToast(qsTr("The layer has no features"));
            } else {
              var vl = layerTree.data(index, FlatLayerTreeModel.VectorLayerPointer);
              var filter = layerTree.data(index, FlatLayerTreeModel.FilterExpression);
              featureForm.model.setFeatures(vl, filter);
              if (layerTree.data(index, FlatLayerTreeModel.HasSpatialExtent)) {
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
          icon.source: Theme.getThemeVectorIcon('directions_walk_24dp')

          onClicked: {
            var layer = layerTree.data(index, FlatLayerTreeModel.VectorLayerPointer);
            close();
            if (trackingModel.layerInTracking(layer)) {
              trackingModel.stopTracker(layer);
              displayToast(qsTr('Track on layer %1 stopped').arg(layer.name));
            } else {
              var tracker;
              var idx = projectInfo.restoreTracker(layer);
              if (idx.valid) {
                tracker = trackings.itemAt(idx.row).tracker;
              } else {
                idx = trackingModel.createTracker(layer);
                tracker = trackings.itemAt(idx.row).tracker;
                tracker.visible = itemVisibleCheckBox.checked;
                tracker.minimumDistance = positioningSettings.trackerMinimumDistanceConstraint ? positioningSettings.trackerMinimumDistance : 0;
                tracker.timeInterval = positioningSettings.trackerTimeIntervalConstraint ? positioningSettings.trackerTimeInterval : 0;
                tracker.maximumDistance = positioningSettings.trackerErroneousDistanceSafeguard ? positioningSettings.trackerErroneousDistance : 0;
                tracker.sensorCapture = positioningSettings.trackerSensorCaptureConstraint;
                tracker.conjunction = positioningSettings.trackerMeetAllConstraints;
                tracker.measureType = positioningSettings.trackerMeasureType;
              }
              trackingModel.requestTrackingSetup(layer);
            }
          }
        }

        Text {
          id: lockText

          property var padlockIcon: Theme.getThemeVectorIcon('ic_lock_black_24dp')
          property var padlockSize: fontMetrics.height - 5

          property bool isReadOnly: index !== undefined && layerTree.data(index, FlatLayerTreeModel.ReadOnly)
          property bool isGeometryLocked: index !== undefined && layerTree.data(index, FlatLayerTreeModel.GeometryLocked)

          visible: isReadOnly || isGeometryLocked
          Layout.fillWidth: true
          topPadding: 5

          wrapMode: Text.WordWrap
          textFormat: Text.RichText
          text: isReadOnly ? qsTr('Read-only layer') : qsTr('Geometry-locked layer')
          font: Theme.tipFont
          color: Theme.secondaryTextColor

          MouseArea {
            anchors.fill: parent
            onClicked: {
              if (lockText.isReadOnly)
                displayToast(qsTr('This layer is configured as "Read-Only" which disables adding, deleting and editing features.'));
              else
                displayToast(qsTr('This layer is configured as "Lock Geometries" which disables adding and deleting features, as well as modifying the geometries of existing features.'));
            }
          }
        }

        Text {
          id: creditsText
          Layout.fillWidth: true
          Layout.topMargin: 5
          wrapMode: Text.WordWrap
          textFormat: Text.RichText
          text: ''
          font.pointSize: Theme.tipFont.pointSize
          font.italic: true
          color: Theme.secondaryTextColor

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

      font: Theme.defaultFont
      height: 48
      leftPadding: Theme.menuItemLeftPadding

      onTriggered: {
        if (parseInt(layerTree.data(index, FlatLayerTreeModel.FeatureCount)) === 0) {
          displayToast(qsTr("The layer has no features"));
        } else {
          var vl = layerTree.data(index, FlatLayerTreeModel.VectorLayerPointer);
          var filter = layerTree.data(index, FlatLayerTreeModel.FilterExpression);
          featureForm.model.setFeatures(vl, filter, mapCanvas.mapSettings.visibleExtent);
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
      if (roles.includes(FlatLayerTreeModel.FeatureCount)) {
        updateTitle();
      }
    }
  }

  function updateTitle() {
    if (index === undefined)
      return;
    var title = layerTree.data(index, Qt.Name);
    var type = layerTree.data(index, FlatLayerTreeModel.Type);
    var vl = layerTree.data(index, FlatLayerTreeModel.VectorLayerPointer);
    if (vl) {
      if (type === 'legend') {
        title += ' (' + vl.name + ')';
      } else if (type === 'layer' && layerTree.data(index, FlatLayerTreeModel.IsValid)) {
        var count = layerTree.data(index, FlatLayerTreeModel.FeatureCount);
        if (count !== undefined && count >= 0) {
          var countSuffix = ' [' + count + ']';
          if (!title.endsWith(countSuffix))
            title += countSuffix;
        }
      }
    }
    titleLabel.text = title;
  }

  function updateCredits() {
    var credits = '';
    if (index !== undefined) {
      credits = StringUtils.insertLinks(layerTree.data(index, FlatLayerTreeModel.Credits));
    } else {
      credits = '';
    }
    creditsText.text = credits;
    creditsText.visible = credits !== '';
  }

  function isTrackingButtonVisible() {
    if (!index)
      return false;
    return layerTree.data(index, FlatLayerTreeModel.Type) === 'layer' && !layerTree.data(index, FlatLayerTreeModel.ReadOnly) && layerTree.data(index, FlatLayerTreeModel.Trackable);
  }

  function isShowFeaturesListButtonVisible() {
    return layerTree.data(index, FlatLayerTreeModel.IsValid) && layerTree.data(index, FlatLayerTreeModel.LayerType) === 'vectorlayer';
  }

  function isShowVisibleFeaturesListDropdownVisible() {
    return isShowFeaturesListButtonVisible() && layerTree.data(index, FlatLayerTreeModel.HasSpatialExtent);
  }
}
