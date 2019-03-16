import QtQuick 2.0

import QtQuick.Controls 1.4 as Controls
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1
import org.qgis 1.0
import org.qfield 1.0
import QtQml.Models 2.2

import "js/style.js" as Style

TreeView {
  id: listView
  model: layerTree

  headerVisible: false
  frameVisible: false

  QtObject {
    id: properties

    property var previousIndex
  }

  property VectorLayer currentLayer

  Controls.TableViewColumn {
    role: "display"
  }

  rowDelegate: Rectangle {
    height: layerTree.data(listView.__model.mapRowToModelIndex(styleData.row), LayerTreeModel.Type) === 'legend' ? 36 * dp : 48 * dp
    color: styleData.row !== undefined && layerTree.data(listView.__model.mapRowToModelIndex(styleData.row), LayerTreeModel.VectorLayer) === currentLayer ? "#999" : "#fff"
  }

  itemDelegate: Item {
    height: Math.max(16, label.implicitHeight)
    property int implicitWidth: label.implicitWidth + 20

    RowLayout {
      height: layerTree.data(listView.__model.mapRowToModelIndex(styleData.row), LayerTreeModel.Type) === 'legend' ? 36 * dp : 48 * dp
      Image {
        source: "image://legend/" + layerTree.data(styleData.index, LayerTreeModel.LegendImage)
        width: 24 * dp
        height: 24 * dp
        Layout.alignment: Qt.AlignVCenter
      }

      Text {
        id: label
        horizontalAlignment: styleData.textAlignment
        Layout.alignment: Qt.AlignVCenter
        elide: styleData.elideMode
        text: styleData.value !== undefined ? styleData.value : ""
        renderType: Settings.isMobile ? Text.QtRendering : Text.NativeRendering
        color: layerTree.data(styleData.index, LayerTreeModel.Visible) ? "black" : "gray"
      }
    }
  }

  /**
   * User clicked an item
   *
   * @param index : QModelIndex
   */
  onClicked: {
    var nodeType = layerTree.data(index, LayerTreeModel.Type)
    if (nodeType !== 'layer') {
      if (listView.isExpanded(index))
          listView.collapse(index)
      else
          listView.expand(index)
    }
    else
      currentLayer = layerTree.data(index, LayerTreeModel.VectorLayer)
  }

  onPressAndHold: {
    itemProperties.index = index
    itemProperties.open()
  }

  onDoubleClicked: {
    if (listView.isExpanded(index))
        listView.collapse(index)
    else
        listView.expand(index)
  }

  LayerTreeItemProperties {
      id: itemProperties
      layerTree: listView.model

      modal: true
      closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
      parent: ApplicationWindow.overlay
  }
}
