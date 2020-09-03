import QtQuick 2.12
import QtQuick.Controls 2.12
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
    property VectorLayer vectorLayer: VectorLayerPointer ? VectorLayerPointer : null
    property var itemRow: index
    property int itemPadding: 5 + ( 5 + 24 ) * TreeLevel
    property string itemType: Type
    property string layerType: LayerType

    id: rectangle
    width: parent ? parent.width : undefined
    height: line.height
    color: "#ffffff"

    Row {
      id: line
      leftPadding: itemPadding
      spacing: 5

      // Legend image / layer icon
      Rectangle {
          visible: Type != "group"
          height: 24
          width: 24
          anchors.verticalCenter: parent.verticalCenter

          Image {
              id: layerImage
              anchors.fill: parent
              anchors.margins: 4
              cache: false
              source: {
                if ( LegendImage != '' ) {
                  return "image://legend/" + LegendImage
                } else if ( LayerType == "vectorlayer" ) {
                  switch( VectorLayerPointer.geometryType() ) {
                  case QgsWkbTypes.PointGeometry:
                    return Theme.getThemeVectorIcon('ic_vectorlayer_point_18dp');
                  case QgsWkbTypes.LineGeometry:
                    return Theme.getThemeVectorIcon('ic_vectorlayer_line_18dp');
                  case QgsWkbTypes.PolygonGeometry:
                    return Theme.getThemeVectorIcon('ic_vectorlayer_polygon_18dp');
                  case QgsWkbTypes.NullGeometry:
                  case QgsWkbTypes.UnknownGeometry:
                    return Theme.getThemeVectorIcon('ic_vectorlayer_table_18dp');
                  }
                } else if ( LayerType == "rasterlayer" ) {
                  return Theme.getThemeVectorIcon('ic_rasterlayer_18dp');
                } else if ( LayerType == "meshlayer" ) {
                  return Theme.getThemeVectorIcon('ic_meshlayer_18dp');
                } else if ( LayerType == "vectortilelayer" ) {
                  return Theme.getThemeVectorIcon('ic_vectortilelayer_18dp');
                } else if ( LayerType == "annotationlayer" ) {
                  return Theme.getThemeVectorIcon('ic_annotationlayer_18dp');
                } else {
                  return '';
                }
              }
              width: 16
              height: 16
              sourceSize.width: 16 * screen.devicePixelRatio
              sourceSize.height: 16 * screen.devicePixelRatio
              fillMode: Image.PreserveAspectFit
              opacity: Visible ? 1 : 0.25
          }
      }

      Text {
        id: layerName
        width: rectangle.width
               - ( layerImage.source != '' ? 34 : 10 )
               - ( InTracking ? 34 : 0 )
               - ( ( ReadOnly || GeometryLocked ) ? 34 : 0 )
               - itemPadding
               - ( !IsValid ? 34 : 0 )
        padding: 3
        leftPadding: 0
        text: Name
        horizontalAlignment: Text.AlignLeft
        font.pointSize: itemType === "legend" ? Theme.strongTipFont.pointSize - 2 : Theme.tipFont.pointSize
        font.bold: itemType === "group" || (itemType === "layer" && vectorLayer != null && vectorLayer == currentLayer) ? true : false
        color: {
            if ( itemType === "layer" && vectorLayer != null && vectorLayer == currentLayer )
                return Theme.mainColor;
            else if ( IsValid )
                return Theme.darkGray;
            else
                return Theme.lightGray;
        }
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

      Rectangle {
          visible: !IsValid
          height: 24
          width: 24
          anchors.verticalCenter: parent.verticalCenter
          color: 'transparent'
          Image {
              anchors.fill: parent
              anchors.margins: 4
              fillMode: Image.PreserveAspectFit
              horizontalAlignment: Image.AlignHCenter
              verticalAlignment: Image.AlignVCenter
              source: Theme.getThemeVectorIcon('ic_error_outline_24dp')
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
              if (item.vectorLayer && item.vectorLayer.isValid) {
                currentLayer = item.vectorLayer
              }
          }
      }
      onPressed: {
          var item = table.itemAt(table.contentX + mouse.x, table.contentY + mouse.y)
          if (item && item.itemType) {
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
