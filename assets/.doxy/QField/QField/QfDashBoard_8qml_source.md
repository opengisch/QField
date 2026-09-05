

# File QfDashBoard.qml

[**File List**](files.md) **>** [**app**](dir_5194e5fea318fda12687127c23f8aba1.md) **>** [**qml**](dir_408f7dc2ccecf14179dc3d37678eaa5b.md) **>** [**QfDashBoard.qml**](QfDashBoard_8qml.md)

[Go to the documentation of this file](QfDashBoard_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import org.qgis
import org.qfield.core
import org.qfield.gui

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
  property QfCloudProjectsModel cloudProjectsModel: null

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
    color: QfTheme.mainBackgroundColor
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
        iconSource: QfTheme.getThemeVectorIcon('ic_arrow_left_white_24dp')
        iconColor: QfTheme.mainTextColor
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
          color: QfTheme.mainOverlayColor
          backgroundColor: QfTheme.mainColor
          _minSize: 2
          _maxSize: 2
        }

        Row {
          id: buttonsRow
          objectName: "dashboardActionsToolbar"
          height: QfTheme.toolButtonSize
          spacing: 1

          QfToolButton {
            id: measurementButton
            objectName: "measurementButton"
            anchors.verticalCenter: parent.verticalCenter
            round: true
            iconSource: QfTheme.getThemeVectorIcon("ic_measurement_black_24dp")
            iconColor: QfTheme.mainTextColor
            bgcolor: "transparent"
            onClicked: {
              toggleMeasurementTool();
              highlighted = false;
            }
          }

          QfToolButton {
            id: view3DButton
            objectName: "view3DButton"
            anchors.verticalCenter: parent.verticalCenter
            round: true
            iconSource: QfTheme.getThemeVectorIcon("ic_3d_white_24dp")
            iconColor: QfTheme.mainTextColor
            bgcolor: "transparent"
            onClicked: {
              toggle3DView();
              highlighted = false;
            }
          }

          QfToolButton {
            id: printItemButton
            objectName: "printItemButton"
            anchors.verticalCenter: parent.verticalCenter
            round: true
            iconSource: QfTheme.getThemeVectorIcon("ic_print_black_24dp")
            iconColor: QfTheme.mainTextColor
            onClicked: {
              const p = mapToItem(mainWindow.contentItem, 0, 0);
              showPrintLayouts(p);
              highlighted = false;
            }
          }

          QfToolButton {
            id: cloudButton
            objectName: "cloudButton"
            anchors.verticalCenter: parent.verticalCenter
            iconSource: {
              if (cloudConnection.status === QfCloudConnection.LoggedIn && cloudProjectsModel.currentProject) {
                switch (cloudProjectsModel.currentProject.status) {
                case QfCloudProject.Downloading:
                  switch (cloudProjectsModel.currentProject.packagingStatus) {
                  case QfCloudProject.PackagingFinishedStatus:
                    return QfTheme.getThemeVectorIcon('ic_cloud_download_24dp');
                  default:
                    return QfTheme.getThemeVectorIcon('ic_cloud_active_24dp');
                  }
                case QfCloudProject.Pushing:
                  switch (cloudProjectsModel.currentProject.deltaFilePushStatus) {
                  case QfCloudProject.DeltaFileLocalStatus:
                    return QfTheme.getThemeVectorIcon('ic_cloud_upload_24dp');
                  default:
                    return QfTheme.getThemeVectorIcon('ic_cloud_active_24dp');
                  }
                case QfCloudProject.Idle:
                  return cloudProjectsModel.currentProject.isProjectOutdated ? QfTheme.getThemeVectorIcon('ic_cloud_attention_24dp') : QfTheme.getThemeVectorIcon('ic_cloud_active_24dp');
                default:
                  return QfTheme.getThemeVectorIcon('ic_cloud_white_24dp');
                }
              } else {
                return QfTheme.getThemeVectorIcon('ic_cloud_white_24dp');
              }
            }
            iconColor: {
              if (cloudConnection.status !== QfCloudConnection.LoggedIn || !cloudProjectsModel.currentProject) {
                return QfTheme.mainTextColor;
              } else {
                return QfTheme.cloudColor;
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
              running: cloudProjectsModel.currentProject && (cloudProjectsModel.currentProject.status === QfCloudProject.Downloading || cloudProjectsModel.currentProject.status === QfCloudProject.Pushing)
              loops: Animation.Infinite

              onStopped: {
                cloudButton.opacity = 1;
              }
            }

            statusBadge.visible: showSync || showPush
            statusBadge.color: showSync ? QfTheme.mainColor : QfTheme.cloudColor
            statusBadge.enableGradient: showSync && showPush
            statusBadge.width: cloudButton.width / 2.8
            statusBadge.badgeText.color: QfTheme.light
            statusBadge.badgeText.text: {
              if (cloudProjectsModel.layerObserver && cloudProjectsModel.layerObserver.deltaFileWrapper && cloudProjectsModel.layerObserver.deltaFileWrapper.count > 0) {
                return cloudProjectsModel.layerObserver.deltaFileWrapper.count >= 10 ? '+' : cloudProjectsModel.layerObserver.deltaFileWrapper.count;
              }
              return '';
            }

            readonly property bool showSync: cloudProjectsModel.currentProject ? cloudProjectsModel.currentProject.isOutdated : false
            readonly property bool showPush: cloudProjectsModel.layerObserver && cloudProjectsModel.layerObserver.deltaFileWrapper && cloudProjectsModel.layerObserver.deltaFileWrapper.count > 0
          }

          QfToolButton {
            id: projectFolderButton
            objectName: "projectFolderButton"
            anchors.verticalCenter: parent.verticalCenter
            font: QfTheme.defaultFont
            iconSource: QfTheme.getThemeVectorIcon("ic_project_folder_black_24dp")
            iconColor: QfTheme.mainTextColor
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
        iconSource: QfTheme.getThemeVectorIcon('ic_dot_menu_black_24dp')
        iconColor: QfTheme.mainTextColor
        bgcolor: "transparent"
        onClicked: {
          let p = mapToItem(mainWindow.contentItem, width, 0);
          showMainMenu(p);
        }
      }
    }

    RowLayout {
      id: projectInformationLayout
      Layout.fillWidth: true
      Layout.leftMargin: mainWindow.sceneLeftMargin + 10
      Layout.rightMargin: 6
      Layout.bottomMargin: 5

      Text {
        id: projectTitleText
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignVCenter
        text: {
          if (qgisProject) {
            if (qgisProject.title !== "") {
              return qgisProject.title;
            } else if (cloudProjectsModel.currentProject) {
              return cloudProjectsModel.currentProject.name;
            } else {
              return QfFileUtils.fileName(qgisProject.fileName, false);
            }
          }
          return "";
        }
        font: QfTheme.strongFont
        color: QfTheme.mainTextColor
        elide: Text.ElideRight
      }

      QfToolButton {
        id: temporalButton
        Layout.alignment: Qt.AlignVCenter
        width: 36
        height: 36
        padding: 0
        visible: flatLayerTree.isTemporal
        iconSource: QfTheme.getThemeVectorIcon('ic_temporal_black_24dp')
        iconColor: mapSettings.isTemporal ? QfTheme.mainColor : QfTheme.mainTextColor
        bgcolor: "transparent"
        onClicked: temporalProperties.open()
      }

      QfToolButton {
        id: projectInformationButton

        property string projectDescription: {
          if (qgisProject) {
            if (qgisProject.metadata.abstract !== "") {
              return qgisProject.metadata.abstract;
            } else if (cloudProjectsModel.currentProject && cloudProjectsModel.currentProject.description !== "") {
              return cloudProjectsModel.currentProject.description;
            }
          }
          return "";
        }

        property string projectAuthor: {
          if (qgisProject) {
            if (qgisProject.metadata.author !== "" && qgisProject.metadata.author !== "Not available" && qgisProject.metadata.author !== "root") {
              return qgisProject.metadata.author;
            } else if (cloudProjectsModel.currentProject) {
              return cloudProjectsModel.currentProject.owner;
            }
          }
          return "";
        }

        Layout.alignment: Qt.AlignVCenter
        visible: projectDescription != "" || projectAuthor != ""
        width: 36
        height: 36
        padding: 0
        iconSource: QfTheme.getThemeVectorIcon('ic_info_white_24dp')
        iconColor: QfTheme.mainTextColor
        bgcolor: "transparent"
        onClicked: {
          informationPopup.header = qsTr("Project Information");
          informationPopup.title = projectTitleText.text;

          informationPopup.descriptionFormat = Text.MarkdownText;
          informationPopup.description = projectDescription;
          informationPopup.author = projectAuthor;

          informationPopup.open();
        }
      }
    }

    GroupBox {
      id: mapThemeContainer
      objectName: "mapThemeContainer"
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
        color: QfTheme.mainTextColor
        font: QfTheme.strongTipFont
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
          font: QfTheme.defaultFont

          popup.font: QfTheme.defaultFont
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
              mapThemeContainer.visible = themes.length > 1;
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
            font.pointSize: QfTheme.tipFont.pointSize
            highlighted: mapThemeComboBox.highlightedIndex == index
          }
        }
      }
    }

    GroupBox {
      id: legendContainer
      objectName: "legendContainer"
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
        color: QfTheme.mainTextColor
        font: QfTheme.strongTipFont
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
          bgcolor: QfTheme.darkTheme ? QfTheme.mainBackgroundColorSemiOpaque : QfTheme.lightestGraySemiOpaque
          color: QfTheme.mainTextColor
          icon.source: legend.model.isCollapsed ? QfTheme.getThemeVectorIcon('ic_expand_all_24dp') : QfTheme.getThemeVectorIcon('ic_collapse_all_24dp')
          icon.width: 14
          icon.height: 14
          font.pointSize: QfTheme.tinyFont.pointSize - 2

          onClicked: {
            legend.model.setAllCollapsed(!legend.model.isCollapsed);
            projectInfo.saveLayerTreeState();
          }
        }
      }

      QfLegend {
        id: legend
        objectName: "legend"
        isVisible: dashBoard.position > 0
        anchors.fill: parent
        anchors.leftMargin: mainWindow.sceneLeftMargin + 5
        anchors.rightMargin: 5
        bottomMargin: bottomRow.height + 4
        informationPopup: informationPopup
      }
    }
  }

  Rectangle {
    id: bottomRow
    height: QfTheme.toolButtonSize + mainWindow.sceneBottomMargin
    width: parent.width
    anchors.bottom: parent.bottom
    color: QfTheme.darkTheme ? QfTheme.mainBackgroundColorSemiOpaque : QfTheme.lightestGraySemiOpaque

    Item {
      height: QfTheme.toolButtonSize
      anchors.bottom: parent.bottom
      anchors.bottomMargin: mainWindow.sceneBottomMargin
      anchors.left: parent.left
      anchors.leftMargin: mainWindow.sceneLeftMargin
      anchors.right: parent.right

      MenuItem {
        id: homeButton
        width: parent.width - modeSwitch.width
        height: parent.height
        anchors.verticalCenter: parent.verticalCenter
        icon.source: QfTheme.getThemeVectorIcon("ic_home_black_24dp")
        icon.width: QfTheme.toolButtonSize / 2
        icon.height: QfTheme.toolButtonSize / 2
        font: QfTheme.defaultFont
        text: qsTr("Return home")

        onClicked: returnHome()
      }

      QfSwitch {
        id: modeSwitch
        objectName: "modeSwitch"
        height: QfTheme.toolButtonSize
        width: height * 1.9
        leftPadding: height / 3
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        indicator: Rectangle {
          implicitHeight: modeSwitch.height * 0.75
          implicitWidth: modeSwitch.height * 1.5
          x: modeSwitch.leftPadding
          radius: modeSwitch.height / 12
          color: "#24212121"
          border.color: "#14FFFFFF"
          anchors.verticalCenter: parent.verticalCenter
          Image {
            width: modeSwitch.height * 0.58
            height: width
            anchors.left: parent.left
            anchors.leftMargin: modeSwitch.height / 12
            anchors.verticalCenter: parent.verticalCenter
            source: QfTheme.getThemeVectorIcon('ic_map_white_24dp')
            sourceSize.width: parent.height * screen.devicePixelRatio
            sourceSize.height: parent.width * screen.devicePixelRatio
            opacity: 0.6
          }
          Image {
            width: modeSwitch.height * 0.58
            height: width
            anchors.right: parent.right
            anchors.rightMargin: modeSwitch.height / 12
            anchors.verticalCenter: parent.verticalCenter
            source: QfTheme.getThemeVectorIcon('ic_create_white_24dp')
            sourceSize.width: parent.height * screen.devicePixelRatio
            sourceSize.height: parent.width * screen.devicePixelRatio
            opacity: 0.6
          }
          Rectangle {
            x: modeSwitch.checked ? parent.width - width : 0
            width: modeSwitch.height * 0.75
            height: width
            radius: modeSwitch.height / 12
            color: projectInfo.insertRights ? QfTheme.mainColor : QfTheme.darkTheme ? QfTheme.mainBackgroundColorSemiOpaque : QfTheme.lightestGray
            border.color: QfTheme.mainOverlayColor
            Image {
              width: modeSwitch.height * 0.58
              height: width
              anchors.centerIn: parent
              source: modeSwitch.checked ? QfTheme.getThemeVectorIcon('ic_create_white_24dp') : QfTheme.getThemeVectorIcon('ic_map_white_24dp')
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

  QfTemporalProperties {
    id: temporalProperties
    mapSettings: dashBoard.mapSettings
  }

  QfInformationPopup {
    id: informationPopup
  }
}
```


