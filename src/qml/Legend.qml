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
  spacing: 0

  delegate: Rectangle {
    property VectorLayer vectorLayer: VectorLayerPointer ? VectorLayerPointer : null
    property var itemRow: index
    property int itemPadding: 5 + ( 5 + 24 ) * TreeLevel
    property string itemType: Type
    property string layerType: LayerType
    property bool isSelectedLayer: ( itemType === "layer" && vectorLayer != null && vectorLayer == currentLayer )

    id: rectangle
    width: parent ? parent.width : undefined
    height: line.height + 1
    state: "default"

    states: [
      State {
        name: "default"
        PropertyChanges { target: rectangle; color: isSelectedLayer ? Theme.mainColor : "transparent" }
      },
      State {
        name: "pressed"
        PropertyChanges { target: rectangle; color: Theme.lightGray }
      }
    ]

    Row {
      id: line
      leftPadding: itemPadding
      spacing: 5

      // Collapsed state visual feedback
      Rectangle {
          height: 24
          width: 10
          color: "transparent"
          anchors.verticalCenter: parent.verticalCenter

          Image {
              anchors.fill: parent
              source: isSelectedLayer
                  ? Theme.getThemeVectorIcon('ic_legend_collapsed_state_white_14dp')
                  : Theme.getThemeVectorIcon('ic_legend_collapsed_state_14dp')
              width: 10
              height: 10
              rotation: !IsCollapsed ? 90 : 0
              sourceSize.width: 12 * screen.devicePixelRatio
              sourceSize.height: 12 * screen.devicePixelRatio
              fillMode: Image.PreserveAspectFit
              visible: HasChildren
          }
      }

      // Legend image / layer icon
      Rectangle {
          height: 24
          width: 24
          color: "transparent"
          anchors.verticalCenter: parent.verticalCenter

          Image {
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
                } else if ( Type == "group" ) {
                  return Theme.getThemeVectorIcon('ic_group_18dp');
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
               - itemPadding
               - 46
               - ( InTracking ? 34 : 0 )
               - ( ( ReadOnly || GeometryLocked ) ? 34 : 0 )
               - ( !IsValid ? 34 : 0 )
        padding: 3
        leftPadding: 0
        text: Name
        horizontalAlignment: Text.AlignLeft
        font.pointSize: itemType === "legend" ? Theme.strongTipFont.pointSize - 2 : Theme.tipFont.pointSize
        font.bold: itemType === "group" || (itemType === "layer" && vectorLayer != null && vectorLayer == currentLayer) ? true : false
        color: {
            if ( isSelectedLayer )
                return Theme.light;
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
              source: Theme.getThemeVectorIcon( 'directions_walk_24dp' )
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
          visible: Type == 'layer' && !IsValid
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
              pressedItem.state = "pressed"
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
              pressedItem.state = "default"
              pressedItem = null
          }
      }
      onReleased: {
          if (pressedItem) {
              pressedItem.state = "default"
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
