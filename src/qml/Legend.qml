import QtQuick 2.0

import QtQuick.Controls 1.4 as Controls
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1
import org.qgis 1.0
import org.qfield 1.0

import "js/style.js" as Style

Controls.TreeView {
  id: listView
  model: layerTree

  headerVisible: false

  property VectorLayer currentLayer: model.data(currentIndex, LayerTreeModel.VectorLayer)

  Controls.TableViewColumn {
    role: "display"
  }

  rowDelegate: Rectangle {
    height: 48 * dp
    color: styleData.selected ? "#999" : "#fff"
  }

  itemDelegate: Item {
    height: Math.max(16, label.implicitHeight)
    property int implicitWidth: label.implicitWidth + 20

    RowLayout {
      height: 48 * dp
      Image {
        source: "image://legend/" + layerTree.data(styleData.index, LayerTreeModel.LegendImage)
        width: 24 * dp
        height: 24 * dp
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: 1
      }

      Text {
        id: label
        horizontalAlignment: styleData.textAlignment
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: 1
        elide: styleData.elideMode
        text: styleData.value !== undefined ? styleData.value : ""
        renderType: Settings.isMobile ? Text.QtRendering : Text.NativeRendering
      }
    }
  }
}
