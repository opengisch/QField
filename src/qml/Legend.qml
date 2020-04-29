import QtQuick 2.12

import QtGraphicalEffects 1.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.12
import QtQml.Models 2.2

import org.qgis 1.0
import org.qfield 1.0
import Theme 1.0
import "."


ListView {
  property VectorLayer currentLayer

  id: table
  model: flatLayerTree
  flickableDirection: Flickable.VerticalFlick
  boundsBehavior: Flickable.StopAtBounds
  clip: true
  spacing: 1

  delegate: Rectangle {
    property VectorLayer vectorLayer: VectorLayerPointer
    property var itemRow: index
    property string itemType: Type

    id: rectangle
    width: parent.width
    height: line.height
    color: "#ffffff"

    Row {
      id: line
      leftPadding: 5 *dp
      spacing: 5 * dp

      Image {
        visible: LegendImage != ''
        cache: false
        source: {
            if ( LegendImage )
              return "image://legend/" + LegendImage
            else
              return ''
        }
        width: 24 * dp
        height: 24 * dp
        anchors.verticalCenter: parent.verticalCenter
        opacity: Visible ? 1 : 0.25
      }

      Text {
        id: layerName
        width: rectangle.width - ( LegendImage != '' ? 34 * dp : 10 * dp )
        padding: 5 * dp
        text: Name
        horizontalAlignment: itemType == "group" ? Text.AlignHCenter : Text.AlignLeft
        font.pointSize: itemType === "legend" ? Theme.strongTipFont.pointSize - 2 : Theme.tipFont.pointSize
        font.bold: itemType === "group" || (itemType === "layer" && vectorLayer != null && vectorLayer == currentLayer) ? true : false
        color: itemType === "layer" && vectorLayer != null && vectorLayer == currentLayer ? Theme.mainColor : Theme.darkGray
        elide: Text.ElideRight
        opacity: Visible ? 1 : 0.25
      }
    }
  }
  MouseArea {
      property Item pressedItem
      anchors.fill: parent
      onClicked: {
          var item = table.itemAt(table.contentX + mouse.x, table.contentY + mouse.y)
          if (item) {
              if (item.vectorLayer) {
                currentLayer = item.vectorLayer
              }
          }
      }
      onPressed: {
          var item = table.itemAt(table.contentX + mouse.x, table.contentY + mouse.y)
          if (item && item.itemType !== "group") {
              pressedItem = item;
              pressedItem.color = Theme.lightGray
          }
      }
      onPressAndHold: {
          if (pressedItem) {
            itemProperties.index = table.model.index(pressedItem.itemRow, 0)
            itemProperties.open()
          }
      }
      onCanceled: {
          if (pressedItem) {
              pressedItem.color = "transparent"
              pressedItem = null
          }
      }
      onReleased: {
          if (pressedItem) {
              pressedItem.color = "transparent"
              pressedItem = null
          }
      }
  }

  LayerTreeItemProperties {
      id: itemProperties
      layerTree: table.model

      modal: true
      closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
      parent: ApplicationWindow.overlay
  }
}
