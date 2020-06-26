import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.12
import QtQml.Models 2.2
import QtGraphicalEffects 1.12

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
      leftPadding: 5
      spacing: 5

      Image {
        visible: LegendImage != ''
        cache: false
        source: {
            if ( LegendImage )
              return "image://legend/" + LegendImage
            else
              return ''
        }
        width: 24
        height: 24
        fillMode: Image.PreserveAspectFit
        anchors.verticalCenter: parent.verticalCenter
        opacity: Visible ? 1 : 0.25
      }

      Text {
        id: layerName
        width: rectangle.width
               - ( LegendImage != '' ? 34 : 10 )
               - ( InTracking ? 34 : 0 )
               - ( ( ReadOnly || GeometryLocked ) ? 34 : 0 )
        padding: 5
        text: Name
        horizontalAlignment: itemType == "group" ? Text.AlignHCenter : Text.AlignLeft
        font.pointSize: itemType === "legend" ? Theme.strongTipFont.pointSize - 2 : Theme.tipFont.pointSize
        font.bold: itemType === "group" || (itemType === "layer" && vectorLayer != null && vectorLayer == currentLayer) ? true : false
        color: itemType === "layer" && vectorLayer != null && vectorLayer == currentLayer ? Theme.mainColor : Theme.darkGray
        elide: Text.ElideRight
        opacity: Visible ? 1 : 0.25
      }

      Rectangle {
          visible: InTracking ? true : false
          height: 24
          width: 24
          anchors.verticalCenter: parent.verticalCenter
          radius: height / 2
          color: Theme.mainColor

          SequentialAnimation on color  {
              loops: Animation.Infinite
              ColorAnimation  { from: Theme.mainColor; to: "#5a8725"; duration: 2000; easing.type: Easing.InOutQuad }
              ColorAnimation  { from: "#5a8725"; to: Theme.mainColor; duration: 1000; easing.type: Easing.InOutQuad }
          }

          Image {
              anchors.fill: parent
              anchors.margins: 4
              fillMode: Image.PreserveAspectFit
              horizontalAlignment: Image.AlignHCenter
              verticalAlignment: Image.AlignVCenter
              source: Theme.getThemeIcon( 'ic_directions_walk_black_24dp' )
          }
      }

      Rectangle {
          visible: ReadOnly || GeometryLocked
          height: 24
          width: 24
          anchors.verticalCenter: parent.verticalCenter
          color: 'transparent'
          opacity: 0.25

          Image {
              anchors.fill: parent
              anchors.margins: 4
              fillMode: Image.PreserveAspectFit
              horizontalAlignment: Image.AlignHCenter
              verticalAlignment: Image.AlignVCenter
              source: Theme.getThemeIcon( 'ic_lock_black_24dp' )
          }
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
      onDoubleClicked: {
          var item = table.itemAt(table.contentX + mouse.x, table.contentY + mouse.y)
          if (item) {
            itemProperties.index = table.model.index(item.itemRow, 0)
            itemProperties.open()
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
