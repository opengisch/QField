

# File Legend.qml

[**File List**](files.md) **>** [**qml**](dir_1d81f7f4362e4398c0a695c4e466fcb0.md) **>** [**Legend.qml**](Legend_8qml.md)

[Go to the documentation of this file](Legend_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Controls.Material.impl
import QtQuick.Layouts
import org.qgis
import org.qfield
import Theme

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
    property bool isSelectedLayer: Type === FlatLayerTreeModel.Layer && VectorLayerPointer && VectorLayerPointer == activeLayer
    width: parent ? parent.width : undefined
    height: line.height + 7
    color: isSelectedLayer ? Theme.mainColor : "transparent"
    radius: 2

    MouseArea {
      id: mouseArea
      anchors.fill: parent
      enabled: (allowActiveLayerChange || (projectInfo.activeLayer != VectorLayerPointer))
      acceptedButtons: Qt.LeftButton | Qt.RightButton
      onClicked: mouse => {
        if (!allowActiveLayerChange)
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

            Behavior on rotation {
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

        Rectangle {
          visible: Type == FlatLayerTreeModel.Image
          width: rectangle.width - itemPadding - 36
          height: legendImage.height + 8
          color: "#f2f2f2" // hard-coded color as most legends are intented to be displayed against white backgrounds
          radius: 4

          Flickable {
            anchors.fill: parent
            anchors.margins: 4
            contentWidth: legendImage.width
            contentHeight: legendImage.height
            clip: true
            ScrollBar.horizontal: QfScrollBar {}

            Image {
              id: legendImage
              fillMode: Image.PreserveAspectFit
              cache: true
              smooth: true
              mipmap: true
              source: {
                if (!legend.isVisible || Type != FlatLayerTreeModel.Image)
                  return '';
                return LegendImage;
              }
            }
          }
        }

        Item {
          id: layerVisibility
          property bool isVisible: Checkable
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
          visible: Type != FlatLayerTreeModel.Image

          Image {
            anchors.fill: parent
            anchors.margins: 4
            fillMode: Image.PreserveAspectFit
            cache: false
            smooth: true
            mipmap: true
            source: {
              if (!legend.isVisible || Type == FlatLayerTreeModel.Image)
                return '';
              if (LegendImage != '') {
                return LegendImage;
              } else if (Type == FlatLayerTreeModel.Layer) {
                if (LayerType == "vectorlayer") {
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
                }
              } else if (Type == FlatLayerTreeModel.Group) {
                return Theme.getThemeVectorIcon('ic_group_18dp');
              } else {
                return '';
              }
            }
            opacity: Visible ? 1 : 0.25
          }
        }

        Text {
          id: layerName
          visible: Type != FlatLayerTreeModel.Image
          width: rectangle.width - itemPadding - 46 // legend icon + right padding
          - collapsedState.width - (layerVisibility.isVisible ? layerVisibility.width : -5) - badges.width
          padding: 3
          leftPadding: 0
          text: Name
          horizontalAlignment: Text.AlignLeft
          font.pointSize: Theme.tipFont.pointSize
          font.bold: Type == FlatLayerTreeModel.Group || (Type == FlatLayerTreeModel.Layer && VectorLayerPointer && VectorLayerPointer == activeLayer) ? true : false
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

        RowLayout {
          id: badges
          visible: Type != FlatLayerTreeModel.Image
          anchors.verticalCenter: parent.verticalCenter
          spacing: 4

          QfToolButton {
            id: trackingBadge
            property bool isVisible: InTracking ? true : false
            visible: isVisible
            height: 24
            width: 24
            padding: 4
            enabled: isVisible
            round: true
            bgcolor: Theme.mainColor
            icon.source: Theme.getThemeVectorIcon('directions_walk_24dp')
            icon.color: Theme.mainTextColor

            onClicked: {
              displayToast(qsTr('Tracking active on this layer.'), 'info', qsTr('Stop'), function () {
                if (trackingModel.layerInActiveTracking(VectorLayerPointer)) {
                  trackingModel.stopTracker(VectorLayerPointer);
                  displayToast(qsTr('Tracking on layer %1 stopped').arg(VectorLayerPointer.name));
                }
              });
            }

            SequentialAnimation on bgcolor {
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
            property bool isVisible: Type == FlatLayerTreeModel.Layer && !IsValid
            visible: isVisible
            height: 24
            width: 24
            padding: 4
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
            property bool isVisible: ReadOnly || FeatureAdditionLocked
            visible: isVisible
            height: 24
            width: 24
            padding: 4
            enabled: isVisible
            bgcolor: 'transparent'
            opacity: 0.5

            icon.source: Theme.getThemeVectorIcon('ic_lock_black_24dp')
            icon.color: Theme.mainTextColor

            onClicked: {
              if (ReadOnly) {
                displayToast(qsTr('This layer is configured as "Read-Only" which disables adding, deleting and editing features.'));
              } else {
                displayToast(qsTr('The permissions configuration of this layer disables feature addition.'));
              }
            }
          }

          QfToolButton {
            id: notesBadge
            property bool isVisible: HasNotes
            visible: isVisible
            height: 24
            width: 24
            padding: 4
            enabled: isVisible
            bgcolor: 'transparent'
            opacity: 0.5

            icon.source: Theme.getThemeVectorIcon('ic_note_white_24dp')
            icon.color: Theme.mainTextColor

            onClicked: {
              notesItem.title = Name;
              notesItem.notes = Notes;
              notesItem.open();
            }
          }

          QfToolButton {
            id: snappingBadge
            property bool isVisible: stateMachine.state === "digitize" && qgisProject.snappingConfig.mode === Qgis.SnappingMode.AdvancedConfiguration && Type === FlatLayerTreeModel.Layer && LayerType === "vectorlayer" && VectorLayerPointer.geometryType() !== Qgis.GeometryType.Null && VectorLayerPointer.geometryType() !== Qgis.GeometryType.Unknown
            visible: isVisible
            height: 24
            width: 24
            padding: 4
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
  }

  LayerTreeItemProperties {
    id: itemProperties
    layerTree: legend.model
  }

  NotesItem {
    id: notesItem
  }
}
```


