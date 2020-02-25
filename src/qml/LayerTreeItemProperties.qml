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
    itemVisible = layerTree.data(index, LayerTreeModel.Visible)
    title = qsTr("%1 : Properties").arg(layerTree.data(index, 0))
    trackingButtonVisible = layerTree.data(index, LayerTreeModel.Type) === 'layer' && layerTree.data(index, LayerTreeModel.Trackable) && positionSource.active && stateMachine.state === "digitize" ? true : false
    trackingButtonBgColor = trackingModel.layerOnTrack( layerTree.data(index, LayerTreeModel.VectorLayer) ) ? '#F6A564' : '#64B5F6'
    trackingButtonText = trackingModel.layerOnTrack( layerTree.data(index, LayerTreeModel.VectorLayer) ) ? qsTr('Stop tracking') : qsTr('Start tracking')
  }

  onItemVisibleChanged: {
    layerTree.setData(index, itemVisible, LayerTreeModel.Visible);
  }

  onTrackingButtonClicked: {
      //start track
      if( trackingModel.layerOnTrack( layerTree.data(index, LayerTreeModel.VectorLayer) ) ) {
          trackingModel.stopTracker(layerTree.data(index, LayerTreeModel.VectorLayer));
          displayToast( qsTr( 'Track on layer %1 stopped' ).arg( layerTree.data(index, LayerTreeModel.VectorLayer).name  ) )
      }else{
          trackingModel.createTracker(layerTree.data(index, LayerTreeModel.VectorLayer), itemVisible);
      }
      close()
  }
}
