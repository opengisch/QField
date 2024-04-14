import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Controls.Material 2.14
import QtQuick.Controls.Material.impl 2.14
import QtQuick.Layouts 1.14

import org.qgis 1.0
import org.qfield 1.0
import Theme 1.0
import "."


ListView {
  id: legend

  property bool isVisible: false
  property VectorLayer activeLayer

  model: flatLayerTree
  flickableDirection: Flickable.VerticalFlick
  boundsBehavior: Flickable.StopAtBounds
  clip: true
  spacing: 0

  delegate: Rectangle {
    property int itemPadding: 5 + ( 5 + 24 ) * TreeLevel
    property bool isSelectedLayer: Type === "layer" && VectorLayerPointer && VectorLayerPointer == activeLayer

    id: rectangle
    width: parent ? parent.width : undefined
    height: line.height + 7
    color: isSelectedLayer ? Theme.mainColor : "transparent"

    MouseArea {
      id: mouseArea
      anchors.fill: parent
      acceptedButtons: Qt.LeftButton | Qt.RightButton
      onClicked: (mouse) => {
        if (ReadOnly || GeometryLocked) {
          return
        }

        if (VectorLayerPointer && VectorLayerPointer.isValid) {
          activeLayer = VectorLayerPointer
          projectInfo.activeLayer = VectorLayerPointer
        }
      }
      onDoubleClicked: (mouse) => {
        if (HasChildren) {
          IsCollapsed = !IsCollapsed
          projectInfo.saveLayerTreeState();
        }
      }
      onPressAndHold: {
        itemProperties.index = legend.model.index(index, 0)
        itemProperties.open()
        itemProperties.forceActiveFocus()
      }
      onReleased: (mouse) => {
        if (mouse.button === Qt.RightButton) {
          pressAndHold(mouse)
        }
      }
    }

    Ripple {
      clip: true
      width: parent.width
      height: parent.height
      pressed: mouseArea.pressed
      anchor: parent
      active: mouseArea.pressed
      color: Material.rippleColor
    }

    Row {
      id: line
      anchors.verticalCenter: parent.verticalCenter
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
              smooth: true
              mipmap: true
              source: {
                if ( !legend.isVisible )
                  return '';

                if ( LegendImage != '' ) {
                  return "image://legend/" + LegendImage
                } else if ( LayerType == "vectorlayer" ) {
                  switch( VectorLayerPointer.geometryType() ) {
                  case Qgis.GeometryType.Point:
                    return Theme.getThemeVectorIcon('ic_vectorlayer_point_18dp');
                  case Qgis.GeometryType.Line:
                    return Theme.getThemeVectorIcon('ic_vectorlayer_line_18dp');
                  case Qgis.GeometryType.Polygon:
                    return Theme.getThemeVectorIcon('ic_vectorlayer_polygon_18dp');
                  case Qgis.GeometryType.Null:
                  case Qgis.GeometryType.Unknown:
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
               - (trackingBadge.isVisible ? trackingBadge.width + 5 : 0)
               - (lockedBadge.isVisible ? lockedBadge.width + 5 : 0)
               - (invalidBadge.isVisible ? invalidBadge.width + 5 : 0)
               - (snappingBadge.isVisible ? snappingBadge.width + 5 : 0)
        padding: 3
        leftPadding: 0
        text: Name
        horizontalAlignment: Text.AlignLeft
        font.pointSize: Theme.tipFont.pointSize
        font.bold: Type === "group" || (Type === "layer" && VectorLayerPointer && VectorLayerPointer == activeLayer) ? true : false
        color: {
            if ( isSelectedLayer )
                return Theme.light;
            else if ( IsValid )
                return Theme.mainTextColor
            else
                return Theme.secondaryTextColor
        }
        elide: Text.ElideRight
        opacity: Visible ? 1 : 0.25
      }

      QfToolButton {
        id: trackingBadge
        property bool isVisible: InTracking ? true : false
        visible: isVisible
        height: 24
        width: 24
        padding: 4
        anchors.verticalCenter: parent.verticalCenter
        enabled: isVisible

        round: true
        bgcolor: Theme.mainColor
        SequentialAnimation on bgcolor  {
            running: isVisible && legend.isVisible
            loops: Animation.Infinite
            ColorAnimation  { from: Theme.mainColor; to: "#5a8725"; duration: 2000; easing.type: Easing.InOutQuad }
            ColorAnimation  { from: "#5a8725"; to: Theme.mainColor; duration: 1000; easing.type: Easing.InOutQuad }
        }

        icon.source: Theme.getThemeVectorIcon( 'directions_walk_24dp' )
        icon.color: Theme.mainTextColor

        onClicked: {
          displayToast(qsTr('This layer is is currently tracking the device position.'))
        }
      }

      QfToolButton {
        id: invalidBadge
        property bool isVisible: Type === 'layer' && !IsValid
        visible: isVisible
        height: 24
        width: 24
        padding: 4
        anchors.verticalCenter: parent.verticalCenter
        enabled: isVisible

        bgcolor: 'transparent'
        opacity: 0.5

        icon.source: Theme.getThemeVectorIcon('ic_error_outline_24dp' )
        icon.color: Theme.errorColor

        onClicked: {
          displayToast(qsTr('This layer is invalid. This might be due to a network issue, a missing file or a misconfiguration of the project.'))
        }
      }

      QfToolButton {
        id: lockedBadge
        property bool isVisible: ReadOnly || GeometryLocked
        visible: isVisible
        height: 24
        width: 24
        padding: 4
        anchors.verticalCenter: parent.verticalCenter
        enabled: isVisible

        bgcolor: 'transparent'
        opacity: 0.5

        icon.source: Theme.getThemeVectorIcon( 'ic_lock_black_24dp' )
        icon.color: Theme.mainTextColor

        onClicked: {
          if ( ReadOnly ) {
            displayToast(qsTr('This layer is configured as "Read-Only" which disables adding, deleting and editing features.'))
          } else {
            displayToast(qsTr('This layer is configured as "Lock Geometries" which disables adding and deleting features, as well as modifying the geometries of existing features.'))
          }
        }
      }

      QfToolButton {
        id: snappingBadge
        property bool isVisible: stateMachine.state === "digitize" && qgisProject.snappingConfig.mode === Qgis.SnappingMode.AdvancedConfiguration && Type === 'layer' && LayerType === "vectorlayer"
        visible: isVisible
        height: 24
        width: 24
        padding: 4
        anchors.verticalCenter: parent.verticalCenter
        enabled: isVisible

        round: true
        bgcolor: SnappingEnabled ? Theme.mainColor : Theme.controlBackgroundColor
        opacity: SnappingEnabled ? 1.0 : 0.5

        icon.source: Theme.getThemeVectorIcon( 'ic_snapping_white_24dp' )
        icon.color: SnappingEnabled ? 'white' : Theme.mainTextColor

        onClicked: {
          SnappingEnabled = !SnappingEnabled
          projectInfo.saveLayerSnappingConfiguration(VectorLayerPointer)
        }
      }
    }
  }

  LayerTreeItemProperties {
      id: itemProperties
      layerTree: legend.model

      modal: true
      closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
  }
}
