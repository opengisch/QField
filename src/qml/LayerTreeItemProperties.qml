import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import org.qgis 1.0
import org.qfield 1.0

import Theme 1.0

Popup {
  id: popup

  property var layerTree
  property var index

  property bool zoomToLayerButtonVisible: false
  property bool showFeaturesListButtonVisible: false
  property bool showAllFeaturesButtonVisible: false
  property bool reloadDataButtonVisible: false

  property bool trackingButtonVisible: false
  property var trackingButtonText

  property bool expandCheckBoxVisible: false


  width: Math.min( childrenRect.width, mainWindow.width - 20 )
  x: (parent.width - width) / 2
  y: (parent.height - height) / 2
  padding: 0

  onIndexChanged: {
    updateTitle();

    itemVisibleCheckBox.checked = layerTree.data(index, FlatLayerTreeModel.Visible);

    expandCheckBoxVisible = layerTree.data(index, FlatLayerTreeModel.HasChildren)
    expandCheckBox.text = layerTree.data( index, FlatLayerTreeModel.Type ) === 'group' ? qsTr('Expand group') : qsTr('Expand legend item')
    expandCheckBox.checked = !layerTree.data(index, FlatLayerTreeModel.IsCollapsed)

    reloadDataButtonVisible = layerTree.data(index, FlatLayerTreeModel.CanReloadData)
    zoomToLayerButtonVisible = isSpatialLayer()
    showFeaturesListButtonVisible = isShowFeaturesListButtonVisible();

    trackingButtonVisible = isTrackingButtonVisible()
    trackingButtonText = trackingModel.layerInTracking( layerTree.data(index, FlatLayerTreeModel.VectorLayerPointer) ) ? qsTr('Stop tracking') : qsTr('Start tracking')
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
        // everything but nonspatial vector layer
        visible: !!(index && (!isSpatialLayer() && layerTree.data( index, FlatLayerTreeModel.LayerType ) !== 'vectorlayer' || isSpatialLayer()))
        indicator.height: 16
        indicator.width: 16
        indicator.implicitHeight: 24
        indicator.implicitWidth: 24

        onClicked: {
          layerTree.setData(index, checkState === Qt.Checked, FlatLayerTreeModel.Visible);
          close()
        }
      }

      CheckBox {
        id: expandCheckBox
        Layout.fillWidth: true
        text: qsTr('Expand legend item')
        font: Theme.defaultFont
        visible: expandCheckBoxVisible

        onClicked: {
          layerTree.setData(index, checkState === Qt.Unchecked, FlatLayerTreeModel.IsCollapsed);
          close()
        }
      }

      QfButton {
        id: zoomToLayerButton
        Layout.fillWidth: true
        Layout.topMargin: 5
        font: Theme.defaultFont
        text: qsTr('Zoom to layer')
        visible: zoomToLayerButtonVisible
        icon.source: Theme.getThemeVectorIcon( 'zoom_out_map_24dp' )

        onClicked: {
          mapCanvas.mapSettings.setCenterToLayer( layerTree.data( index, FlatLayerTreeModel.MapLayerPointer ) )
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
          var vl = layerTree.data( index, FlatLayerTreeModel.VectorLayerPointer )

          if ( layerTree.data( index, FlatLayerTreeModel.Type ) === 'layer' ) {
            featureForm.model.setFeatures( vl )
          } else {
            // one day, we should be able to show only the features that correspond to the given legend item
            featureForm.model.setFeatures( vl )
          }
          mapCanvas.mapSettings.setCenterToLayer( layerTree.data( index, FlatLayerTreeModel.VectorLayerPointer ) )
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

      onDataChanged: updateTitle();
  }

  function updateTitle() {
      title = layerTree.data(index, Qt.DisplayName)
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

  function isSpatialLayer() {
    if ( !index )
      return false

    if (! layerTree.data( index, FlatLayerTreeModel.IsValid )
      || (
          layerTree.data( index, FlatLayerTreeModel.Type ) !== 'layer'
          && layerTree.data( index, FlatLayerTreeModel.Type ) !== 'legend'
          ))
      return false

    var vl = layerTree.data( index, FlatLayerTreeModel.VectorLayerPointer )
    if (!vl)
      return true

    if ( vl.geometryType() === QgsWkbTypes.NullGeometry || vl.geometryType() === QgsWkbTypes.UnknownGeometry)
      return false

    return true
  }

  function isShowFeaturesListButtonVisible() {
    return layerTree.data( index, FlatLayerTreeModel.IsValid )
        && layerTree.data( index, FlatLayerTreeModel.LayerType ) === 'vectorlayer'
  }
}
