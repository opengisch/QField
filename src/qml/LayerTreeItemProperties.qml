import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import org.qgis 1.0
import org.qfield 1.0

import Theme 1.0

Popup {
  property var layerTree
  property var index

  property bool zoomToLayerButtonVisible: false

  property bool trackingButtonVisible: false
  property var trackingButtonText

  property alias itemVisible: itemVisibleCheckBox.checked

  padding: 0

  x: (parent.width - width) / 2
  y: (parent.height - height) / 2

  onIndexChanged: {
    itemVisible = layerTree.data(index, FlatLayerTreeModel.Visible)
    title = qsTr("%1 : Properties and Functions").arg(layerTree.data(index, 0))
    zoomToLayerButtonVisible = isZoomToLayerButtonVisible()
    trackingButtonVisible = isTrackingButtonVisible()
    trackingButtonText = trackingModel.layerInTracking( layerTree.data(index, FlatLayerTreeModel.VectorLayerPointer) ) ? qsTr('Stop tracking') : qsTr('Start tracking')
  }

  onItemVisibleChanged: {
    layerTree.setData(index, itemVisible, FlatLayerTreeModel.Visible);
  }

  Page {
    padding: 10
    header: Label {
      padding: 10
      topPadding: 20
      bottomPadding: 5
      anchors.left:parent.left
      anchors.right:parent.right
      text: title
      font: Theme.strongFont
    }

    ColumnLayout{
      spacing: 4
      width: Math.min(mainWindow.width - 20, parent.width)

      FontMetrics {
        id: fontMetrics
      }

      Text {
        property var padlockIcon: Theme.getThemeIcon('ic_lock_black_24dp')
        property var padlockSize: fontMetrics.height - 5

        visible: index && (
                   layerTree.data(index, FlatLayerTreeModel.ReadOnly)
                   || layerTree.data(index, FlatLayerTreeModel.GeometryLocked))
        Layout.fillWidth: true
        Layout.maximumWidth: parent.width - 20

        wrapMode: Text.WordWrap
        textFormat: Text.RichText
        text: '<img src="' + padlockIcon + '" width="' + padlockSize + '" height="' + padlockSize + '"> '
              + (index && layerTree.data(index, FlatLayerTreeModel.ReadOnly)
                  ? qsTr('This layer is configured as "Read-Only" which disables adding, deleting and editing features.')
                  : qsTr('This layer is configured as "Lock Geometries" which disables adding and deleting features, as well as modifying the geometries of existing features.'))
      }

      CheckBox {
          id: itemVisibleCheckBox
          Layout.fillWidth: true
          Layout.maximumWidth: parent.width - 20
          text: qsTr('Show on map canvas')
          font: Theme.defaultFont

          indicator.height: 16
          indicator.width: 16
          indicator.implicitHeight: 24
          indicator.implicitWidth: 24
      }

      QfButton {
        id: zoomToLayerButton
        Layout.fillWidth: true
        Layout.maximumWidth: parent.width - 20
        font: Theme.defaultFont
        text: qsTr('Zoom to layer')
        visible: trackingButtonVisible

        onClicked: {
          mapCanvas.mapSettings.setCenterToLayer( layerTree.data( index, FlatLayerTreeModel.MapLayerPointer ) )
        }
      }

      QfButton {
        id: trackingButton
        Layout.fillWidth: true
        Layout.maximumWidth: parent.width - 20
        font: Theme.defaultFont
        text: trackingButtonText
        visible: trackingButtonVisible

        onClicked: {
            //start track
            if ( trackingModel.layerInTracking( layerTree.data(index, FlatLayerTreeModel.VectorLayerPointer) ) ) {
                 trackingModel.stopTracker(layerTree.data(index, FlatLayerTreeModel.VectorLayerPointer));
                 displayToast( qsTr( 'Track on layer %1 stopped' ).arg( layerTree.data(index, FlatLayerTreeModel.VectorLayerPointer).name  ) )
            } else {
                trackingModel.createTracker(layerTree.data(index, FlatLayerTreeModel.VectorLayerPointer), itemVisible);
            }
            close()
        }
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

  function isZoomToLayerButtonVisible() {
      if ( !index )
        return false

      return ( layerTree.data( index, FlatLayerTreeModel.Type ) === 'layer' )
  }
}
