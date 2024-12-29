import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Controls.Material.impl
import QtQuick.Layouts
import org.qgis
import org.qfield
import Theme

/**
 * \ingroup qml
 */
ListView {
  id: legend

  property bool isVisible: false
  property VectorLayer activeLayer
  property bool allowActiveLayerChange

  model: flatLayerTree
  flickableDirection: Flickable.VerticalFlick
  boundsBehavior: Flickable.StopAtBounds
  clip: true
  spacing: 0

  function openProperties(index) {
    itemProperties.index = legend.model.index(index, 0);
    itemProperties.open();
  }

  delegate: Rectangle {
    id: rectangle
    property int itemPadding: 30 * TreeLevel
    property bool isSelectedLayer: Type === "layer" && VectorLayerPointer && VectorLayerPointer == activeLayer
    width: parent ? parent.width : undefined
    height: line.height + 7
    color: isSelectedLayer ? Theme.mainColor : "transparent"

    MouseArea {
      id: mouseArea
      anchors.fill: parent
      enabled: (allowActiveLayerChange || (projectInfo.activeLayer != VectorLayerPointer))
      acceptedButtons: Qt.LeftButton | Qt.RightButton
      onClicked: mouse => {
        if (!allowActiveLayerChange)
          return;
        if (ReadOnly || GeometryLocked)
          return;
        if (VectorLayerPointer && VectorLayerPointer.isValid) {
          activeLayer = VectorLayerPointer;
          projectInfo.activeLayer = VectorLayerPointer;
        }
      }

      onDoubleClicked: {
        openProperties(index);
      }

      onPressAndHold: {
        openProperties(index);
      }

      onReleased: mouse => {
        if (mouse.button === Qt.RightButton) {
          pressAndHold(mouse);
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
      spacing: 0

      // Collapsed state visual feedback
      Row {
        id: collapsedState
        anchors.verticalCenter: parent.verticalCenter
        height: 24

        Item {
          height: 24
          width: 24
          clip: true
          anchors.verticalCenter: parent.verticalCenter

          QfToolButton {
            height: 35
            width: height
            anchors.centerIn: parent
            iconSource: Theme.getThemeVectorIcon('ic_legend_collapsed_state_24dp')
            iconColor: isSelectedLayer ? Theme.mainOverlayColor : Theme.mainTextColor
            bgcolor: "transparent"
            visible: HasChildren
            enabled: HasChildren
            rotation: !IsCollapsed ? 90 : 0

            Behavior on rotation  {
              NumberAnimation {
                duration: 100
              }
            }

            onClicked: {
              if (HasChildren) {
                IsCollapsed = !IsCollapsed;
                projectInfo.saveLayerTreeState();
              }
            }
          }
        }

        // spacing item only when collapse button available
        Item {
          width: 5
          height: parent.height
        }
      }

      Row {
        anchors.verticalCenter: parent.verticalCenter
        spacing: 5

        Item {
          id: layerVisibility
          property bool isVisible: HasSpatialExtent
          height: 24
          width: visible ? parent.height : 0
          anchors.verticalCenter: parent.verticalCenter
          visible: isVisible

          QfToolButton {
            height: 35
            width: height
            opacity: Visible ? 1 : 0.25
            anchors.centerIn: parent
            iconSource: !Visible ? Theme.getThemeVectorIcon('ic_hide_green_48dp') : Theme.getThemeVectorIcon('ic_show_green_48dp')
            iconColor: isSelectedLayer ? Theme.mainOverlayColor : Theme.mainTextColor
            bgcolor: "transparent"
            visible: HasSpatialExtent
            enabled: (allowActiveLayerChange || (projectInfo.activeLayer != VectorLayerPointer))
            onClicked: {
              layerTree.setData(legend.model.index(index, 0), !Visible, FlatLayerTreeModel.Visible);
              projectInfo.saveLayerTreeState();
              flatLayerTree.mapTheme = '';
            }
          }
        }

        // Legend icon
        Item {
          height: 24
          width: 24
          anchors.verticalCenter: parent.verticalCenter

          Image {
            anchors.fill: parent
            anchors.margins: 4
            cache: false
            smooth: true
            mipmap: true
            source: {
              if (!legend.isVisible)
                return '';
              if (LegendImage != '') {
                return "image://legend/" + LegendImage;
              } else if (LayerType == "vectorlayer") {
                switch (VectorLayerPointer.geometryType()) {
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
              } else if (LayerType == "rasterlayer") {
                return Theme.getThemeVectorIcon('ic_rasterlayer_18dp');
              } else if (LayerType == "meshlayer") {
                return Theme.getThemeVectorIcon('ic_meshlayer_18dp');
              } else if (LayerType == "vectortilelayer") {
                return Theme.getThemeVectorIcon('ic_vectortilelayer_18dp');
              } else if (LayerType == "annotationlayer") {
                return Theme.getThemeVectorIcon('ic_annotationlayer_18dp');
              } else if (Type == "group") {
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
          width: rectangle.width - itemPadding - 46 // legend icon + right padding
          - collapsedState.width - (layerVisibility.isVisible ? layerVisibility.width : 0) - (trackingBadge.isVisible ? trackingBadge.width + 5 : 0) - (lockedBadge.isVisible ? lockedBadge.width + 5 : 0) - (invalidBadge.isVisible ? invalidBadge.width + 5 : 0) - (snappingBadge.isVisible ? snappingBadge.width + 5 : 0)
          padding: 3
          leftPadding: 0
          text: Name
          horizontalAlignment: Text.AlignLeft
          font.pointSize: Theme.tipFont.pointSize
          font.bold: Type === "group" || (Type === "layer" && VectorLayerPointer && VectorLayerPointer == activeLayer) ? true : false
          elide: Text.ElideRight
          opacity: Visible ? 1 : 0.25
          color: {
            if (isSelectedLayer)
              return Theme.mainOverlayColor;
            else if (IsValid)
              return Theme.mainTextColor;
            else
              return Theme.secondaryTextColor;
          }
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
          icon.source: Theme.getThemeVectorIcon('directions_walk_24dp')
          icon.color: Theme.mainTextColor

          onClicked: {
            displayToast(qsTr('This layer is is currently tracking positions.'), 'info', qsTr('Stop'), function () {
                if (trackingModel.layerInTracking(VectorLayerPointer)) {
                  trackingModel.stopTracker(VectorLayerPointer);
                  displayToast(qsTr('Track on layer %1 stopped').arg(VectorLayerPointer.name));
                }
              });
          }

          SequentialAnimation on bgcolor  {
            running: isVisible && legend.isVisible
            loops: Animation.Infinite
            ColorAnimation {
              from: Theme.mainColor
              to: "#5a8725"
              duration: 2000
              easing.type: Easing.InOutQuad
            }
            ColorAnimation {
              from: "#5a8725"
              to: Theme.mainColor
              duration: 1000
              easing.type: Easing.InOutQuad
            }
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
          icon.source: Theme.getThemeVectorIcon('ic_error_outline_24dp')
          icon.color: Theme.errorColor

          onClicked: {
            displayToast(qsTr('This layer is invalid. This might be due to a network issue, a missing file or a misconfiguration of the project.'));
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

          icon.source: Theme.getThemeVectorIcon('ic_lock_black_24dp')
          icon.color: Theme.mainTextColor

          onClicked: {
            if (ReadOnly) {
              displayToast(qsTr('This layer is configured as "Read-Only" which disables adding, deleting and editing features.'));
            } else {
              displayToast(qsTr('This layer is configured as "Lock Geometries" which disables adding and deleting features, as well as modifying the geometries of existing features.'));
            }
          }
        }

        QfToolButton {
          id: snappingBadge
          property bool isVisible: stateMachine.state === "digitize" && qgisProject.snappingConfig.mode === Qgis.SnappingMode.AdvancedConfiguration && Type === "layer" && LayerType === "vectorlayer" && VectorLayerPointer.geometryType() !== Qgis.GeometryType.Null && VectorLayerPointer.geometryType() !== Qgis.GeometryType.Unknown
          visible: isVisible
          height: 24
          width: 24
          padding: 4
          anchors.verticalCenter: parent.verticalCenter
          enabled: isVisible
          round: true
          bgcolor: SnappingEnabled ? Theme.mainColor : Theme.controlBackgroundColor
          opacity: SnappingEnabled ? 1.0 : 0.5
          icon.source: Theme.getThemeVectorIcon('ic_snapping_white_24dp')
          icon.color: SnappingEnabled ? 'white' : Theme.mainTextColor

          onClicked: {
            SnappingEnabled = !SnappingEnabled;
            projectInfo.saveLayerSnappingConfiguration(VectorLayerPointer);
          }
        }
      }
    }
  }

  LayerTreeItemProperties {
    id: itemProperties
    layerTree: legend.model
  }
}
