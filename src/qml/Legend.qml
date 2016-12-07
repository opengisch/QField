import QtQuick 2.0

import QtQuick.Controls 1.4 as Controls
import QtQuick.Controls 2.0
import org.qgis 1.0
import org.qfield 1.0

Controls.TreeView {
  id: listView
  model: layerTree

  property VectorLayer currentLayer: console.warn("info " + model.data(currentIndex, layerTree.VectorLayer)) || model.data(currentIndex, LayerTreeModel.VectorLayer)

  Controls.TableViewColumn {
    role: "display"
  }

  rowDelegate: Rectangle {
    height: 48 * dp
  }
}
