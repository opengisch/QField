

# File DashBoard.qml

[**File List**](files.md) **>** [**qml**](dir_1d81f7f4362e4398c0a695c4e466fcb0.md) **>** [**DashBoard.qml**](DashBoard_8qml.md)

[Go to the documentation of this file](DashBoard_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import org.qgis
import org.qfield
import Theme

Drawer {
  id: dashBoard
  objectName: "dashBoard"

  signal showMainMenu(point p)
  signal showPrintLayouts(point p)
  signal showCloudPopup
  signal showProjectFolder
  signal toggleMeasurementTool
  signal toggle3DView
  signal returnHome

  property bool preventFromOpening: overlayFeatureFormDrawer.visible
  property bool allowInteractive: true
  property alias allowActiveLayerChange: legend.allowActiveLayerChange
  property alias activeLayer: legend.activeLayer
  property alias layerTree: legend.model
  property MapSettings mapSettings

  Component.onCompleted: {
    if (Material.roundedScale) {
      Material.roundedScale = Material.NotRounded;
    }
  }

  width: Math.min(Math.max(330, closeButton.width + buttonsRow.width + menuButton.width + mainWindow.sceneLeftMargin + 1), mainWindow.width)
  height: parent.height
  edge: Qt.LeftEdge
  dragMargin: 10
  interactive: allowInteractive && buttonsRowContainer.width >= buttonsRow.width

  topPadding: 0
  leftPadding: 0
  rightPadding: 0
  bottomPadding: 0

  position: 0
  focus: visible
  clip: true

  onActiveLayerChanged: {
    if (activeLayer && activeLayer.readOnly && stateMachine.state === "digitize") {
      displayToast(qsTr("The layer %1 is read only.").arg(activeLayer.name));
    }
  }

  Connections {
    target: stateMachine

    function onStateChanged() {
      if (stateMachine.state === "measure") {
        return;
      }
      modeSwitch.checked = stateMachine.state === "digitize";
    }
  }

  background: Rectangle {
    anchors.fill: parent
    color: Theme.mainBackgroundColor
  }

  ColumnLayout {
    anchors.fill: parent

    Rectangle {
      height: mainWindow.sceneTopMargin + Math.max(buttonsRow.height + 8, buttonsRow.childrenRect.height)
      Layout.fillWidth: true
      Layout.preferredHeight: height

      color: "transparent"

      QfToolButton {
        id: closeButton
        anchors.left: parent.left
        anchors.leftMargin: mainWindow.sceneLeftMargin
        anchors.verticalCenter: buttonsRowContainer.verticalCenter
        iconSource: Theme.getThemeVectorIcon('ic_arrow_left_white_24dp')
        iconColor: Theme.mainTextColor
        bgcolor: "transparent"
        onClicked: close()
      }

      Flickable {
        id: buttonsRowContainer
        anchors.left: closeButton.right
        anchors.right: menuButton.left
        anchors.top: parent.top
        anchors.topMargin: mainWindow.sceneTopMargin + 4
        anchors.bottomMargin: 4
        height: buttonsRow.height
        contentWidth: buttonsRow.width
        contentHeight: buttonsRow.height
        flickableDirection: Flickable.HorizontalFlick
        clip: true

        ScrollBar.horizontal: QfScrollBar {
          visible: !dashBoard.interactive
          color: Theme.mainOverlayColor
          backgroundColor: Theme.mainColor
          _minSize: 2
          _maxSize: 2
        }

        Row {
          id: buttonsRow
          objectName: "dashboardActionsToolbar"
          height: 48
          spacing: 1

          QfToolButton {
            id: measurementButton
            objectName: "MeasurementButton"
            anchors.verticalCenter: parent.verticalCenter
            round: true
            iconSource: Theme.getThemeVectorIcon("ic_measurement_black_24dp")
            iconColor: Theme.mainTextColor
            bgcolor: "transparent"
            onClicked: {
              toggleMeasurementTool();
              highlighted = false;
            }
          }

          QfToolButton {
            id: view3DButton
            objectName: "View3DButton"
            anchors.verticalCenter: parent.verticalCenter
            round: true
            iconSource: Theme.getThemeVectorIcon("ic_3d_24dp")
            iconColor: Theme.mainTextColor
            bgcolor: "transparent"
            onClicked: {
              toggle3DView();
              highlighted = false;
            }
          }

          QfToolButton {
            id: printItemButton
            objectName: "PrintItemButton"
            anchors.verticalCenter: parent.verticalCenter
            round: true
            iconSource: Theme.getThemeVectorIcon("ic_print_black_24dp")
            iconColor: Theme.mainTextColor
            onClicked: {
              const p = mapToItem(mainWindow.contentItem, 0, 0);
              showPrintLayouts(p);
              highlighted = false;
            }
          }

          QfToolButton {
            id: cloudButton
            objectName: "CloudButton"
            anchors.verticalCenter: parent.verticalCenter
            iconSource: {
              if (cloudConnection.status === QFieldCloudConnection.LoggedIn && cloudProjectsModel.currentProject) {
                switch (cloudProjectsModel.currentProject.status) {
                case QFieldCloudProject.Downloading:
                  switch (cloudProjectsModel.currentProject.packagingStatus) {
                  case QFieldCloudProject.PackagingFinishedStatus:
                    return Theme.getThemeVectorIcon('ic_cloud_download_24dp');
                  default:
                    return Theme.getThemeVectorIcon('ic_cloud_active_24dp');
                  }
                case QFieldCloudProject.Pushing:
                  switch (cloudProjectsModel.currentProject.deltaFilePushStatus) {
                  case QFieldCloudProject.DeltaFileLocalStatus:
                    return Theme.getThemeVectorIcon('ic_cloud_upload_24dp');
                  default:
                    return Theme.getThemeVectorIcon('ic_cloud_active_24dp');
                  }
                case QFieldCloudProject.Idle:
                  return cloudProjectsModel.currentProject.isProjectOutdated ? Theme.getThemeVectorIcon('ic_cloud_attention_24dp') : Theme.getThemeVectorIcon('ic_cloud_active_24dp');
                default:
                  return Theme.getThemeVectorIcon('ic_cloud_white_24dp');
                }
              } else {
                return Theme.getThemeVectorIcon('ic_cloud_white_24dp');
              }
            }
            iconColor: {
              if (cloudConnection.status !== QFieldCloudConnection.LoggedIn || !cloudProjectsModel.currentProject) {
                return Theme.mainTextColor;
              } else {
                return "transparent";
              }
            }
            bgcolor: "transparent"

            onClicked: {
              if (featureListForm.state === "FeatureFormEdit") {
                featureListForm.requestCancel();
                return;
              }
              if (featureListForm.visible) {
                featureListForm.hide();
              }
              showCloudPopup();
            }

            SequentialAnimation {
              OpacityAnimator {
                from: 1
                to: 0.2
                duration: 2000
                target: cloudButton
              }
              OpacityAnimator {
                from: 0.2
                to: 1
                duration: 2000
                target: cloudButton
              }
              running: cloudProjectsModel.currentProject && (cloudProjectsModel.currentProject.status === QFieldCloudProject.Downloading || cloudProjectsModel.currentProject.status === QFieldCloudProject.Pushing)
              loops: Animation.Infinite

              onStopped: {
                cloudButton.opacity = 1;
              }
            }

            statusBadge.visible: showSync || showPush
            statusBadge.color: showSync ? Theme.mainColor : Theme.cloudColor
            statusBadge.enableGradient: showSync && showPush
            statusBadge.width: cloudButton.width / 2.8
            statusBadge.badgeText.color: Theme.light
            statusBadge.badgeText.text: {
              if (cloudProjectsModel.layerObserver.deltaFileWrapper && cloudProjectsModel.layerObserver.deltaFileWrapper.count > 0) {
                return cloudProjectsModel.layerObserver.deltaFileWrapper.count >= 10 ? '+' : cloudProjectsModel.layerObserver.deltaFileWrapper.count;
              }
              return '';
            }

            readonly property bool showSync: cloudProjectsModel.currentProject ? cloudProjectsModel.currentProject.isOutdated : false
            readonly property bool showPush: cloudProjectsModel.layerObserver.deltaFileWrapper && cloudProjectsModel.layerObserver.deltaFileWrapper.count > 0
          }

          QfToolButton {
            id: projectFolderButton
            objectName: "ProjectFolderButton"
            anchors.verticalCenter: parent.verticalCenter
            font: Theme.defaultFont
            iconSource: Theme.getThemeVectorIcon("ic_project_folder_black_24dp")
            iconColor: Theme.mainTextColor
            round: true
            onClicked: {
              showProjectFolder();
            }
          }
        }
      }

      QfToolButton {
        id: menuButton
        anchors.right: parent.right
        anchors.verticalCenter: buttonsRowContainer.verticalCenter
        iconSource: Theme.getThemeVectorIcon('ic_dot_menu_black_24dp')
        iconColor: Theme.mainTextColor
        bgcolor: "transparent"
        onClicked: {
          let p = mapToItem(mainWindow.contentItem, width, 0);
          showMainMenu(p);
        }
      }
    }

    GroupBox {
      id: mapThemeContainer
      Layout.fillWidth: true
      title: qsTr("Map Theme")
      leftPadding: 10
      rightPadding: 10
      topPadding: label.height + 5
      bottomPadding: 5

      property bool isLoading: false

      label: Label {
        x: parent.leftPadding
        height: 25
        width: parent.availableWidth
        leftPadding: mainWindow.sceneLeftMargin
        text: parent.title
        color: Theme.mainTextColor
        font: Theme.strongTipFont
        elide: Text.ElideRight
        verticalAlignment: Text.AlignVCenter
      }

      background: Rectangle {
        color: "transparent"
      }

      RowLayout {
        width: parent.width

        QfComboBox {
          id: mapThemeComboBox
          Layout.fillWidth: true
          Layout.leftMargin: mainWindow.sceneLeftMargin
          font: Theme.defaultFont

          popup.font: Theme.defaultFont
          popup.topMargin: mainWindow.sceneTopMargin
          popup.bottomMargin: mainWindow.sceneTopMargin

          Connections {
            target: iface

            function onLoadProjectTriggered() {
              mapThemeContainer.isLoading = true;
            }

            function onLoadProjectEnded() {
              var themes = qgisProject.mapThemeCollection.mapThemes;
              mapThemeComboBox.model = themes;
              mapThemeComboBox.enabled = themes.length > 1;
              mapThemeComboBox.opacity = themes.length > 1 ? 1 : 0.25;
              mapThemeContainer.visible = themes.length > 1 || flatLayerTree.isTemporal;
              flatLayerTree.updateCurrentMapTheme();
              mapThemeComboBox.currentIndex = flatLayerTree.mapTheme != '' ? mapThemeComboBox.find(flatLayerTree.mapTheme) : -1;
              mapThemeContainer.isLoading = false;
            }
          }

          Connections {
            target: flatLayerTree

            function onMapThemeChanged() {
              if (!mapThemeContainer.isLoading && mapThemeComboBox.currentText !== flatLayerTree.mapTheme) {
                mapThemeContainer.isLoading = true;
                mapThemeComboBox.currentIndex = flatLayerTree.mapTheme != '' ? mapThemeComboBox.find(flatLayerTree.mapTheme) : -1;
                mapThemeContainer.isLoading = false;
              }
            }
          }

          onCurrentTextChanged: {
            if (!mapThemeContainer.isLoading && qgisProject.mapThemeCollection.mapThemes.length > 1) {
              flatLayerTree.mapTheme = mapThemeComboBox.currentText;
            }
          }

          delegate: ItemDelegate {
            width: mapThemeComboBox.width
            height: 36
            text: modelData
            font.weight: mapThemeComboBox.currentIndex === index ? Font.DemiBold : Font.Normal
            font.pointSize: Theme.tipFont.pointSize
            highlighted: mapThemeComboBox.highlightedIndex == index
          }
        }

        QfToolButton {
          id: temporalButton
          Layout.alignment: Qt.AlignVCenter
          visible: flatLayerTree.isTemporal
          iconSource: Theme.getThemeVectorIcon('ic_temporal_black_24dp')
          iconColor: mapSettings.isTemporal ? Theme.mainColor : Theme.mainTextColor
          bgcolor: "transparent"
          onClicked: temporalProperties.open()
        }
      }
    }

    GroupBox {
      id: legendContainer
      Layout.fillWidth: true
      Layout.fillHeight: true
      title: qsTr("Legend")
      leftPadding: 5
      rightPadding: 5
      topPadding: label.height + 5
      bottomPadding: 5

      background: Rectangle {
        color: "transparent"
      }

      label: Label {
        x: mapThemeContainer.leftPadding
        height: 25
        width: parent.availableWidth
        leftPadding: mainWindow.sceneLeftMargin
        text: parent.title
        color: Theme.mainTextColor
        font: Theme.strongTipFont
        elide: Text.ElideRight
        verticalAlignment: Text.AlignVCenter
        clip: true

        QfButton {
          id: toggleAllButton

          anchors {
            verticalCenter: parent.verticalCenter
            right: parent.right
            rightMargin: 10
          }
          visible: legend.model.hasCollapsibleItems

          text: legend.model.isCollapsed ? qsTr('Expand All') : qsTr('Collapse All')
          bgcolor: Theme.darkTheme ? Theme.mainBackgroundColorSemiOpaque : Theme.lightestGraySemiOpaque
          color: Theme.mainTextColor
          icon.source: legend.model.isCollapsed ? Theme.getThemeVectorIcon('ic_expand_all_24dp') : Theme.getThemeVectorIcon('ic_collapse_all_24dp')
          icon.width: 14
          icon.height: 14
          font.pointSize: 8

          onClicked: {
            legend.model.setAllCollapsed(!legend.model.isCollapsed);
            projectInfo.saveLayerTreeState();
          }
        }
      }

      Legend {
        id: legend
        objectName: "Legend"
        isVisible: dashBoard.position > 0
        anchors.fill: parent
        anchors.leftMargin: mainWindow.sceneLeftMargin + 5
        anchors.rightMargin: 5
        bottomMargin: bottomRow.height + 4
      }
    }
  }

  Rectangle {
    id: bottomRow
    height: 48 + mainWindow.sceneBottomMargin
    width: parent.width
    anchors.bottom: parent.bottom
    color: Theme.darkTheme ? Theme.mainBackgroundColorSemiOpaque : Theme.lightestGraySemiOpaque

    Item {
      height: 48
      anchors.bottom: parent.bottom
      anchors.bottomMargin: mainWindow.sceneBottomMargin
      anchors.left: parent.left
      anchors.leftMargin: mainWindow.sceneLeftMargin
      anchors.right: parent.right

      MenuItem {
        id: homeButton
        width: parent.width - modeSwitch.width
        height: 48
        icon.source: Theme.getThemeVectorIcon("ic_home_black_24dp")
        font: Theme.defaultFont
        text: qsTr("Return home")

        onClicked: returnHome()
      }

      QfSwitch {
        id: modeSwitch
        objectName: "ModeSwitch"
        width: 56 + 36
        height: 48
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        indicator: Rectangle {
          implicitHeight: 36
          implicitWidth: 36 * 2
          x: modeSwitch.leftPadding
          radius: 4
          color: "#24212121"
          border.color: "#14FFFFFF"
          anchors.verticalCenter: parent.verticalCenter
          Image {
            width: 28
            height: 28
            anchors.left: parent.left
            anchors.leftMargin: 4
            anchors.verticalCenter: parent.verticalCenter
            source: Theme.getThemeVectorIcon('ic_map_white_24dp')
            sourceSize.width: parent.height * screen.devicePixelRatio
            sourceSize.height: parent.width * screen.devicePixelRatio
            opacity: 0.6
          }
          Image {
            width: 28
            height: 28
            anchors.right: parent.right
            anchors.rightMargin: 4
            anchors.verticalCenter: parent.verticalCenter
            source: Theme.getThemeVectorIcon('ic_create_white_24dp')
            sourceSize.width: parent.height * screen.devicePixelRatio
            sourceSize.height: parent.width * screen.devicePixelRatio
            opacity: 0.6
          }
          Rectangle {
            x: modeSwitch.checked ? parent.width - width : 0
            width: 36
            height: 36
            radius: 4
            color: projectInfo.insertRights ? Theme.mainColor : Theme.darkTheme ? Theme.mainBackgroundColorSemiOpaque : Theme.lightestGray
            border.color: Theme.mainOverlayColor
            Image {
              width: 28
              height: 28
              anchors.centerIn: parent
              source: modeSwitch.checked ? Theme.getThemeVectorIcon('ic_create_white_24dp') : Theme.getThemeVectorIcon('ic_map_white_24dp')
              sourceSize.width: parent.height * screen.devicePixelRatio
              sourceSize.height: parent.width * screen.devicePixelRatio
            }
            Behavior on x {
              PropertyAnimation {
                duration: 100
                easing.type: Easing.OutQuart
              }
            }
          }
        }

        onClicked: {
          if (projectInfo.insertRights) {
            mainWindow.toggleDigitizeMode();
          } else {
            checked = false;
            displayToast(qsTr("The project is read-only."));
          }
        }
      }
    }
  }

  TemporalProperties {
    id: temporalProperties
    mapSettings: dashBoard.mapSettings
  }
}
```


