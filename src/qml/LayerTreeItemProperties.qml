import QtQuick 2.4
import "ui"

import org.qgis 1.0
import org.qfield 1.0

LayerTreeItemProperties {
  property var layerTree
  property var index

    property var trackingButtonVisible
    property var trackingButtonBgColor
    property var trackingButtonText

  x: (parent.width - width) / 2
  y: (parent.height - height) / 2

  onIndexChanged: {
    itemVisible = layerTree.data(index, FlatLayerTreeModel.Visible)
    title = qsTr("%1 : Properties and Functions").arg(layerTree.data(index, 0))
    trackingButtonVisible = layerTree.data(index, FlatLayerTreeModel.Type) === 'layer' && layerTree.data(index, FlatLayerTreeModel.Trackable) && positionSource.active ? true : false
    trackingButtonBgColor = trackingModel.layerInTracking( layerTree.data(index, FlatLayerTreeModel.VectorLayerPointer) ) ? '#F6A564' : '#64B5F6'
    trackingButtonText = trackingModel.layerInTracking( layerTree.data(index, FlatLayerTreeModel.VectorLayerPointer) ) ? qsTr('Stop tracking') : qsTr('Start tracking')
  }

  onItemVisibleChanged: {
    layerTree.setData(index, itemVisible, FlatLayerTreeModel.Visible);
  }

  onTrackingButtonClicked: {
      //start track
      if( trackingModel.layerInTracking( layerTree.data(index, FlatLayerTreeModel.VectorLayerPointer) ) ) {
          trackingModel.stopTracker(layerTree.data(index, FlatLayerTreeModel.VectorLayerPointer));
          displayToast( qsTr( 'Track on layer %1 stopped' ).arg( layerTree.data(index, FlatLayerTreeModel.VectorLayerPointer).name  ) )
      }else{
          trackingModel.createTracker(layerTree.data(index, FlatLayerTreeModel.VectorLayerPointer), itemVisible);
      }
      close()
  }
}
