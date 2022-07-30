import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import org.qgis 1.0
import org.qfield 1.0

import Theme 1.0

Popup {
  id: popup

  property var layerTree
  property var index

  property bool zoomToButtonVisible: false
  property bool showFeaturesListButtonVisible: false
  property bool showAllFeaturesButtonVisible: false
  property bool reloadDataButtonVisible: false

  property bool trackingButtonVisible: false
  property var trackingButtonText

  property bool opacitySliderVisible: false

  width: Math.min( childrenRect.width, mainWindow.width - Theme.popupScreenEdgeMargin)
  x: (parent.width - width) / 2
  y: (parent.height - height) / 2
  padding: 0

  onIndexChanged: {
    updateTitle();

    itemVisibleCheckBox.checked = layerTree.data(index, FlatLayerTreeModel.Visible);
    itemLabelsVisibleCheckBox.checked = layerTree.data(index, FlatLayerTreeModel.LabelsVisible);

    expandCheckBox.text = layerTree.data( index, FlatLayerTreeModel.Type ) === 'group' ? qsTr('Expand group') : qsTr('Expand legend item')
    expandCheckBox.checked = !layerTree.data(index, FlatLayerTreeModel.IsCollapsed)

    reloadDataButtonVisible = layerTree.data(index, FlatLayerTreeModel.CanReloadData)
    zoomToButtonVisible = layerTree.data(index, FlatLayerTreeModel.HasSpatialExtent)
    showFeaturesListButtonVisible = isShowFeaturesListButtonVisible();

    trackingButtonVisible = isTrackingButtonVisible()
    trackingButtonText = trackingModel.layerInTracking( layerTree.data(index, FlatLayerTreeModel.VectorLayerPointer) )
        ? qsTr('Stop tracking')
        : qsTr('Setup tracking')

    opacitySliderVisible = layerTree.data(index, FlatLayerTreeModel.HasSpatialExtent)
  }

  Page {
    width: parent.width
    padding: 10
    header: Label {
      padding: 10
      topPadding: 15
      bottomPadding: 0
      width: parent.width - 20
      text: title
      font: Theme.strongFont
      horizontalAlignment: Text.AlignHCenter
      wrapMode: Text.WordWrap
    }

    ColumnLayout {
      spacing: 4
      width: parent.width

      FontMetrics {
        id: fontMetrics
        font: lockText.font
      }

      Text {
        id: invalidText
        property var invalidIcon: Theme.getThemeVectorIcon('ic_error_outline_24dp')
        property var invalidSize: fontMetrics.height - 5

        visible: index !== undefined && !layerTree.data(index, FlatLayerTreeModel.IsValid)
        Layout.fillWidth: true

        wrapMode: Text.WordWrap
        textFormat: Text.RichText
        text: '<img src="' + invalidIcon + '" width="' + invalidSize + '" height="' + invalidSize + '"> '
              + qsTr('This layer is invalid. This might be due to a network issue, a missing file or a misconfiguration of the project.')
        font: Theme.tipFont
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
          close();
        }
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
          close()
        }
      }

      RowLayout {
        id: opacitySlider

        Layout.fillWidth: true
        Layout.topMargin: 4
        Layout.bottomMargin:4
        spacing: 4
        visible: opacitySliderVisible

        Image {
          source: Theme.getThemeVectorIcon("ic_opacity_black_24dp")
          Layout.preferredWidth: 22
          Layout.preferredHeight: 22
          Layout.leftMargin: 6
          Layout.rightMargin: 2
          Layout.bottomMargin: 8
          Layout.alignment: Qt.AlignVCenter | Qt.alignHCenter

          fillMode: Image.PreserveAspectFit
          smooth: true
        }

        ColumnLayout {
          Layout.alignment: Layout.Center
          Layout.rightMargin: 6
          spacing: 0

          Text {
            Layout.fillWidth: true
            text: qsTr("Opacity")
            font: Theme.defaultFont
          }
          Slider {
            Layout.fillWidth: true
            id: slider
            value: index !== undefined ? layerTree.data(index, FlatLayerTreeModel.Opacity) : 0
            onMoved: layerTree.setData(index, value, FlatLayerTreeModel.Opacity)
          }
        }
      }

      QfButton {
        id: zoomToButton
        Layout.fillWidth: true
        Layout.topMargin: 5
        font: Theme.defaultFont
        text: index ? layerTree.data( index, FlatLayerTreeModel.Type ) === 'group'
                      ? qsTr('Zoom to group')
                      : layerTree.data( index, FlatLayerTreeModel.Type ) === 'legend'
                        ? qsTr('Zoom to parent layer')
                        : qsTr('Zoom to layer') : ''
        visible: zoomToButtonVisible
        icon.source: Theme.getThemeVectorIcon( 'zoom_out_map_24dp' )

        onClicked: {
          mapCanvas.mapSettings.extent = layerTree.nodeExtent(index, mapCanvas.mapSettings);
          close()
          dashBoard.visible = false
        }
      }

      QfButton {
        id: reloadDataButton
        Layout.fillWidth: true
        Layout.topMargin: 5
        font: Theme.defaultFont
        text: qsTr('Reload data')
        visible: reloadDataButtonVisible
        icon.source: Theme.getThemeVectorIcon( 'refresh_24dp' )

        onClicked: {
          layerTree.data(index, FlatLayerTreeModel.MapLayerPointer).reload()
          close()
          dashBoard.visible = false
          displayToast(qsTr('Reload of layer %1 triggered').arg(layerTree.data(index, Qt.DisplayName)))
        }
      }

      QfButton {
        id: showFeaturesList
        Layout.fillWidth: true
        Layout.topMargin: 5
        text: qsTr('Show features list')
        visible: showFeaturesListButtonVisible
        icon.source: Theme.getThemeVectorIcon( 'list_24dp' )

        onClicked: {
          if ( parseInt(layerTree.data(index, FlatLayerTreeModel.FeatureCount)) === 0 ) {
            displayToast( qsTr( "The layer has no features" ) )
          } else {
            var vl = layerTree.data( index, FlatLayerTreeModel.VectorLayerPointer )

            if ( layerTree.data( index, FlatLayerTreeModel.Type ) === 'layer' ) {
              featureForm.model.setFeatures( vl )
            } else {
              // one day, we should be able to show only the features that correspond to the given legend item
              featureForm.model.setFeatures( vl )
            }
            if (layerTree.data(index, FlatLayerTreeModel.HasSpatialExtent)) {
              mapCanvas.mapSettings.extent = layerTree.nodeExtent( index, mapCanvas.mapSettings )
            }
          }

          close()
          dashBoard.visible = false
        }
      }

      QfButton {
        id: trackingButton
        Layout.fillWidth: true
        Layout.topMargin: 5
        font: Theme.defaultFont
        text: trackingButtonText
        visible: trackingButtonVisible
        icon.source: Theme.getThemeVectorIcon( 'directions_walk_24dp' )

        onClicked: {
            //start track
            if ( trackingModel.layerInTracking( layerTree.data(index, FlatLayerTreeModel.VectorLayerPointer) ) ) {
                 trackingModel.stopTracker(layerTree.data(index, FlatLayerTreeModel.VectorLayerPointer));
                 displayToast( qsTr( 'Track on layer %1 stopped' ).arg( layerTree.data(index, FlatLayerTreeModel.VectorLayerPointer).name  ) )
            } else {
                trackingModel.createTracker(layerTree.data(index, FlatLayerTreeModel.VectorLayerPointer), itemVisibleCheckBox.checked );
            }
            close()
        }
      }

      Text {
        id: lockText
        property var padlockIcon: Theme.getThemeIcon('ic_lock_black_24dp')
        property var padlockSize: fontMetrics.height - 5

        property bool isReadOnly: index !== undefined && layerTree.data(index, FlatLayerTreeModel.ReadOnly)
        property bool isGeometryLocked: index !== undefined && layerTree.data(index, FlatLayerTreeModel.GeometryLocked)

        visible: isReadOnly || isGeometryLocked
        Layout.fillWidth: true

        wrapMode: Text.WordWrap
        textFormat: Text.RichText
        text: '<img src="' + padlockIcon + '" width="' + padlockSize + '" height="' + padlockSize + '"> '
              + (isReadOnly ? qsTr('Read-Only Layer') : qsTr('Geometry Locked Layer'))
        font: Theme.tipFont

        MouseArea {
            anchors.fill: parent
            onClicked: {
                if ( lockText.isReadOnly )
                    displayToast(qsTr('This layer is configured as "Read-Only" which disables adding, deleting and editing features.'))
                else
                    displayToast(qsTr('This layer is configured as "Lock Geometries" which disables adding and deleting features, as well as modifying the geometries of existing features.'))
            }
        }
      }
    }
  }

  Connections {
      target: layerTree

      function onDataChanged(topleft, bottomright, roles) {
          if (roles.includes(FlatLayerTreeModel.FeatureCount)) {
            updateTitle();
          }
      }
  }

  function updateTitle() {
      if (index === undefined)
          return

      title = layerTree.data(index, Qt.Name)
      var vl = layerTree.data(index, FlatLayerTreeModel.VectorLayerPointer)

      if (vl && layerTree.data(index, FlatLayerTreeModel.IsValid) && layerTree.data( index, FlatLayerTreeModel.Type ) === 'layer') {
          var count = layerTree.data(index, FlatLayerTreeModel.FeatureCount)
          if (count !== undefined && count >= 0) {
              var countSuffix = ' [' + count + ']'

              if ( !title.endsWith(countSuffix) )
                  title += countSuffix
          }
      }
  }

  function isTrackingButtonVisible() {
    if ( !index )
      return false

    return layerTree.data( index, FlatLayerTreeModel.Type ) === 'layer'
        && layerTree.data( index, FlatLayerTreeModel.Trackable )
        && positionSource.active
  }

  function isShowFeaturesListButtonVisible() {
    return layerTree.data( index, FlatLayerTreeModel.IsValid )
        && layerTree.data( index, FlatLayerTreeModel.LayerType ) === 'vectorlayer'
  }
}
