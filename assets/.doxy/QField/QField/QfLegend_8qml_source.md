

# File QfLegend.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfLegend.qml**](QfLegend_8qml.md)

[Go to the documentation of this file](QfLegend_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Controls.Material.impl
import QtQuick.Layouts
import org.qgis
import org.qfield.core
import org.qfield.gui

ListView {
  id: legend

  property QfInformationPopup informationPopup
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
    property bool isSelectedLayer: Type === QfFlatLayerTreeModel.Layer && VectorLayerPointer && VectorLayerPointer == activeLayer
    width: parent ? parent.width : undefined
    height: line.height + 7
    color: isSelectedLayer ? QfTheme.mainColor : "transparent"
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
            iconSource: QfTheme.getThemeVectorIcon('ic_legend_collapsed_state_24dp')
            iconColor: isSelectedLayer ? QfTheme.mainOverlayColor : QfTheme.mainTextColor
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
          visible: Type == QfFlatLayerTreeModel.Image
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
                if (!legend.isVisible || Type != QfFlatLayerTreeModel.Image)
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
            iconSource: !Visible ? QfTheme.getThemeVectorIcon('ic_hide_green_48dp') : QfTheme.getThemeVectorIcon('ic_show_green_48dp')
            iconColor: isSelectedLayer ? QfTheme.mainOverlayColor : QfTheme.mainTextColor
            bgcolor: "transparent"
            enabled: (allowActiveLayerChange || (projectInfo.activeLayer != VectorLayerPointer))
            onClicked: {
              layerTree.setData(legend.model.index(index, 0), !Visible, QfFlatLayerTreeModel.Visible);
              projectInfo.saveLayerTreeState();
              flatLayerTree.mapTheme = '';
            }
          }
        }

        // QfLegend icon
        Item {
          height: 24
          width: 24
          anchors.verticalCenter: parent.verticalCenter
          visible: Type != QfFlatLayerTreeModel.Image

          Image {
            anchors.fill: parent
            anchors.margins: 4
            fillMode: Image.PreserveAspectFit
            cache: false
            smooth: true
            mipmap: true
            source: {
              if (!legend.isVisible || Type == QfFlatLayerTreeModel.Image)
                return '';
              if (LegendImage != '') {
                return LegendImage;
              } else if (Type == QfFlatLayerTreeModel.Layer) {
                if (LayerType == "vectorlayer") {
                  switch (VectorLayerPointer.geometryType()) {
                  case Qgis.GeometryType.Point:
                    return QfTheme.getThemeVectorIcon('ic_vectorlayer_point_18dp');
                  case Qgis.GeometryType.Line:
                    return QfTheme.getThemeVectorIcon('ic_vectorlayer_line_18dp');
                  case Qgis.GeometryType.Polygon:
                    return QfTheme.getThemeVectorIcon('ic_vectorlayer_polygon_18dp');
                  case Qgis.GeometryType.Null:
                  case Qgis.GeometryType.Unknown:
                    return QfTheme.getThemeVectorIcon('ic_vectorlayer_table_18dp');
                  }
                } else if (LayerType == "rasterlayer") {
                  return QfTheme.getThemeVectorIcon('ic_rasterlayer_18dp');
                } else if (LayerType == "meshlayer") {
                  return QfTheme.getThemeVectorIcon('ic_meshlayer_18dp');
                } else if (LayerType == "vectortilelayer") {
                  return QfTheme.getThemeVectorIcon('ic_vectortilelayer_18dp');
                } else if (LayerType == "annotationlayer") {
                  return QfTheme.getThemeVectorIcon('ic_annotationlayer_18dp');
                }
              } else if (Type == QfFlatLayerTreeModel.Group) {
                return QfTheme.getThemeVectorIcon('ic_group_18dp');
              } else {
                return '';
              }
            }
            opacity: Visible ? 1 : 0.25
          }
        }

        Text {
          id: layerName
          visible: Type != QfFlatLayerTreeModel.Image
          width: rectangle.width - itemPadding - 46 // legend icon + right padding
          - collapsedState.width - (layerVisibility.isVisible ? layerVisibility.width : -5) - badges.width
          padding: 3
          leftPadding: 0
          text: Name
          horizontalAlignment: Text.AlignLeft
          font.pointSize: QfTheme.tipFont.pointSize
          font.bold: Type == QfFlatLayerTreeModel.Group || (Type == QfFlatLayerTreeModel.Layer && VectorLayerPointer && VectorLayerPointer == activeLayer) ? true : false
          elide: Text.ElideRight
          opacity: Visible ? 1 : 0.25
          color: {
            if (isSelectedLayer)
              return QfTheme.mainOverlayColor;
            else if (IsValid)
              return QfTheme.mainTextColor;
            else
              return QfTheme.secondaryTextColor;
          }
        }

        RowLayout {
          id: badges
          visible: Type != QfFlatLayerTreeModel.Image
          anchors.verticalCenter: parent.verticalCenter
          spacing: 4

          QfToolButton {
            id: trackingBadge
            property bool isVisible: InTracking ? true : false
            visible: isVisible
            height: 32
            width: 32
            padding: 0
            enabled: isVisible
            round: true
            bgcolor: QfTheme.mainColor
            icon.source: QfTheme.getThemeVectorIcon('directions_walk_24dp')
            icon.color: QfTheme.mainTextColor

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
                from: QfTheme.mainColor
                to: "#5a8725"
                duration: 2000
                easing.type: Easing.InOutQuad
              }
              ColorAnimation {
                from: "#5a8725"
                to: QfTheme.mainColor
                duration: 1000
                easing.type: Easing.InOutQuad
              }
            }
          }

          QfToolButton {
            id: invalidBadge
            property bool isVisible: Type == QfFlatLayerTreeModel.Layer && !IsValid
            visible: isVisible
            height: 32
            width: 32
            padding: 0
            enabled: isVisible
            bgcolor: 'transparent'
            opacity: 0.5
            icon.source: QfTheme.getThemeVectorIcon('ic_error_outline_24dp')
            icon.color: QfTheme.errorColor

            onClicked: {
              displayToast(qsTr('This layer is invalid. This might be due to a network issue, a missing file or a misconfiguration of the project.'));
            }
          }

          QfToolButton {
            id: lockedBadge
            property bool isVisible: ReadOnly || FeatureAdditionLocked
            visible: isVisible
            height: 32
            width: 32
            padding: 0
            enabled: isVisible
            bgcolor: 'transparent'
            opacity: 0.5

            icon.source: QfTheme.getThemeVectorIcon('ic_lock_black_24dp')
            icon.color: QfTheme.mainTextColor

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
            height: 32
            width: 32
            padding: 0
            enabled: isVisible
            bgcolor: 'transparent'
            opacity: 0.5

            icon.source: QfTheme.getThemeVectorIcon('ic_note_white_24dp')
            icon.color: QfTheme.mainTextColor

            onClicked: {
              informationPopup.header = MapLayerPointer.name;
              informationPopup.title = "";
              informationPopup.descriptionFormat = Text.RichText;
              informationPopup.description = Notes;
              informationPopup.author = "";
              informationPopup.open();
            }
          }

          QfToolButton {
            id: snappingBadge
            property bool isVisible: stateMachine.state === "digitize" && qgisProject.snappingConfig.mode === Qgis.SnappingMode.AdvancedConfiguration && Type === QfFlatLayerTreeModel.Layer && LayerType === "vectorlayer" && VectorLayerPointer.geometryType() !== Qgis.GeometryType.Null && VectorLayerPointer.geometryType() !== Qgis.GeometryType.Unknown
            visible: isVisible
            height: 32
            width: 32
            padding: 0
            enabled: isVisible
            round: true
            bgcolor: SnappingEnabled ? QfTheme.mainColor : QfTheme.controlBackgroundColor
            opacity: SnappingEnabled ? 1.0 : 0.5
            icon.source: QfTheme.getThemeVectorIcon('ic_snapping_white_24dp')
            icon.color: SnappingEnabled ? 'white' : QfTheme.mainTextColor

            onClicked: {
              SnappingEnabled = !SnappingEnabled;
              projectInfo.saveLayerSnappingConfiguration(VectorLayerPointer);
            }
          }
        }
      }
    }
  }

  QfLayerTreeItemProperties {
    id: itemProperties
    layerTree: legend.model
  }
}
```


