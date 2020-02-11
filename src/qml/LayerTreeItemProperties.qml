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
    trackingButtonVisible = layerTree.data(index, LayerTreeModel.Type) === 'layer' && layerTree.data(index, LayerTreeModel.Trackable) && layerTrackings.itemAt( index.row ).activated ? true : false
    trackingButtonBgColor = layerTrackings.itemAt( index.row ).running ? '#50ff0000' : '#500000ff'
    trackingButtonText = layerTrackings.itemAt( index.row ).running ? qsTr('Stop tracking'): qsTr('Start tracking')
  }

  onItemVisibleChanged: {
    layerTree.setData(index, itemVisible, LayerTreeModel.Visible)
  }

  onTrackingButtonClicked: {
      //start track
      if( layerTrackings.itemAt( index.row ).running ){
          layerTrackings.itemAt( index.row ).stop();
      }else{
          layerTrackings.itemAt( index.row ).start( layerTree.data(index, LayerTreeModel.VectorLayer) );
      }
      close()
  }
}
