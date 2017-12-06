import QtQuick 2.4
import "ui"

import org.qgis 1.0
import org.qfield 1.0

LayerTreeItemProperties {
  property var layerTree
  property var index

  x: (parent.width - width) / 2
  y: (parent.height - height) / 2

  onIndexChanged: {
    itemVisible = layerTree.data(index, LayerTreeModel.Visible)
    title = layerTree.data(index, 0)
  }

  onItemVisibleChanged: {
    layerTree.setData(index, itemVisible, LayerTreeModel.Visible)
  }
}
