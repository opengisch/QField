import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import org.qgis 1.0
import org.qfield 1.0
import Theme 1.0

Drawer {
  id: dashBoard
  objectName: "dashBoard"

  signal showMenu
  signal showCloudMenu

  property alias allowLayerChange: legend.enabled
  property alias currentLayer: legend.currentLayer
  property alias layerTree: legend.model
  property MapSettings mapSettings

  property color mainColor: Theme.mainColor

  width: Math.min( 300, mainWindow.width)
  height: parent.height
  edge: Qt.LeftEdge
  dragMargin: 10
  padding: 0

  property bool preventFromOpening: overlayFeatureFormDrawer.visible

  position: 0
  focus: opened
  clip: true

  onShowMenu: mainMenu.popup(settingsButton.x + 2, mainWindow.sceneTopMargin + settingsButton.y + 2)
  onShowCloudMenu: cloudPopup.show()

  onCurrentLayerChanged: {
    if ( currentLayer && currentLayer.readOnly && stateMachine.state == "digitize" )
      displayToast( qsTr( "The layer %1 is read only." ).arg( currentLayer.name ) )
  }

  ColumnLayout {
    anchors.fill: parent

    Rectangle {
      height: mainWindow.sceneTopMargin + Math.max(buttonsRow.height, buttonsRow.childrenRect.height)
      Layout.fillWidth: true
      Layout.preferredHeight: height

      color: mainColor

      Row {
        id: buttonsRow
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: mainWindow.sceneTopMargin
        width: parent.width
        height: 56
        spacing: 1

        QfToolButton {
          id: closeButton
          anchors.verticalCenter: parent.verticalCenter
          iconSource: Theme.getThemeVectorIcon( 'ic_arrow_left_white_24dp' )
          bgcolor: "transparent"
          onClicked: close()
        }

        QfToolButton {
          id: settingsButton
          anchors.verticalCenter: parent.verticalCenter
          iconSource: Theme.getThemeIcon( 'ic_settings_white_24dp' )
          bgcolor: "transparent"
          onClicked: showMenu()
        }

        QfToolButton {
          id: cloudButton
          anchors.verticalCenter: parent.verticalCenter
          iconSource: switch(cloudProjectsModel.currentProjectData.Status ) {
                        case QFieldCloudProjectsModel.Downloading:
                          switch ( cloudProjectsModel.currentProjectData.PackagingStatus ) {
                            case QFieldCloudProjectsModel.PackagingFinishedStatus:
                              return Theme.getThemeVectorIcon('ic_cloud_download_24dp');
                            default:
                              return Theme.getThemeVectorIcon('ic_cloud_active_24dp');
                          }
                        case QFieldCloudProjectsModel.Uploading:
                          switch ( cloudProjectsModel.currentProjectData.UploadDeltaStatus ) {
                            case QFieldCloudProjectsModel.DeltaFileLocalStatus:
                              return Theme.getThemeVectorIcon('ic_cloud_upload_24dp');
                            default:
                              return Theme.getThemeVectorIcon('ic_cloud_active_24dp');
                          }
                        case QFieldCloudProjectsModel.Idle:
                          return Theme.getThemeVectorIcon('ic_cloud_active_24dp');
                        default: Theme.getThemeVectorIcon( 'ic_cloud_24dp' );
                      }
          bgcolor: "transparent"

          onClicked: {
            if (featureForm.state == "FeatureFormEdit") {
              featureForm.requestCancel();
              return;
            }
            if (featureForm.visible) {
              featureForm.hide();
            }
            showCloudMenu()
          }
          bottomRightIndicatorText: cloudProjectsModel.layerObserver.deltaFileWrapper.count > 0
                                      ? cloudProjectsModel.layerObserver.deltaFileWrapper.count
                                      : cloudProjectsModel.layerObserver.deltaFileWrapper.count >= 10
                                        ? '+'
                                        : ''

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
            running: cloudProjectsModel.currentProjectData.Status === QFieldCloudProjectsModel.Downloading ||
                     cloudProjectsModel.currentProjectData.Status === QFieldCloudProjectsModel.Uploading
            loops: Animation.Infinite
            onStopped: {
              cloudButton.opacity = 1
            }
          }
        }
      }


      Switch {
        id: modeswitch
        visible: projectInfo.insertRights
        height: 56
        width: ( 56 + 36 )
        anchors.right: parent.right
        anchors.verticalCenter: buttonsRow.verticalCenter
        indicator: Rectangle {
          implicitHeight: 36
          implicitWidth: 36 * 2
          x: modeswitch.leftPadding
          radius: 4
          color:  "#66212121"
          border.color: "#44FFFFFF"
          anchors.verticalCenter: parent.verticalCenter
          Image {
            width: 28
            height: 28
            anchors.left: parent.left
            anchors.leftMargin: 4
            anchors.verticalCenter: parent.verticalCenter
            source: Theme.getThemeVectorIcon( 'ic_map_white_24dp' )
            sourceSize.width: parent.height * screen.devicePixelRatio
            sourceSize.height: parent.width * screen.devicePixelRatio
            opacity: 0.4
          }
          Image {
            width: 28
            height: 28
            anchors.right: parent.right
            anchors.rightMargin: 4
            anchors.verticalCenter: parent.verticalCenter
            source: Theme.getThemeVectorIcon( 'ic_create_white_24dp' )
            sourceSize.width: parent.height * screen.devicePixelRatio
            sourceSize.height: parent.width * screen.devicePixelRatio
            opacity: 0.4
          }
          Rectangle {
            x: modeswitch.checked ? parent.width - width : 0
            width: 36
            height: 36
            radius: 4
            color:  Theme.mainColor
            border.color: "white"
            Image {
              width: 28
              height: 28
              anchors.centerIn: parent
              source:  modeswitch.checked ? Theme.getThemeVectorIcon( 'ic_create_white_24dp' ) : Theme.getThemeVectorIcon( 'ic_map_white_24dp' )
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
        onPositionChanged: {
          if ( checked ) {
            changeMode( "digitize" )
          } else {
            changeMode( "browse" )
          }
        }
      }
    }

    GroupBox {
      Layout.fillWidth: true

      id: mapThemeContainer
      title: qsTr("Map Theme")
      leftPadding: 10
      rightPadding: 10
      topPadding: label.height + 5
      bottomPadding: 5

      property bool isLoading: false

      label: Label {
          x: parent.leftPadding
          y: 2
          width: parent.availableWidth
          text: parent.title
          color: Theme.mainColor
          font: Theme.strongTipFont
          elide: Text.ElideRight
      }

      background: Rectangle {
          y: parent.height - 1
          width: parent.width
          height: 1
          color: Theme.mainColor
      }

      RowLayout {
          width: parent.width
          ComboBox {
              id: mapThemeComboBox
              Layout.fillWidth: true

              Connections {
                  target: iface

                  function onLoadProjectTriggered() {
                      mapThemeContainer.isLoading = true
                  }

                  function onLoadProjectEnded() {
                      var themes = qgisProject.mapThemeCollection.mapThemes
                      mapThemeComboBox.model = themes
                      mapThemeComboBox.enabled = themes.length > 1
                      mapThemeComboBox.opacity = themes.length > 1 ? 1 : 0.25
                      mapThemeContainer.visible = themes.length > 1 || flatLayerTree.isTemporal
                      flatLayerTree.updateCurrentMapTheme()
                      mapThemeComboBox.currentIndex = flatLayerTree.mapTheme != '' ? mapThemeComboBox.find( flatLayerTree.mapTheme ) : -1
                      mapThemeContainer.isLoading = false
                  }
              }

              Connections {
                  target: flatLayerTree

                  function onMapThemeChanged() {
                      if (!mapThemeContainer.isLoading && mapThemeComboBox.currentText != flatLayerTree.mapTheme) {
                          mapThemeContainer.isLoading = true
                          mapThemeComboBox.currentIndex = flatLayerTree.mapTheme != '' ? mapThemeComboBox.find( flatLayerTree.mapTheme ) : -1
                          mapThemeContainer.isLoading = false
                      }
                  }
              }

              onCurrentTextChanged: {
                  if ( !mapThemeContainer.isLoading && qgisProject.mapThemeCollection.mapThemes.length > 1 ) {
                      flatLayerTree.mapTheme = mapThemeComboBox.currentText
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

              contentItem: Text {
                  height: 36
                  leftPadding: 8
                  text: mapThemeComboBox.displayText
                  font: Theme.tipFont
                  horizontalAlignment: Text.AlignLeft
                  verticalAlignment: Text.AlignVCenter
                  elide: Text.ElideRight
              }

              background: Item {
                  implicitWidth: 120
                  implicitHeight: 36

                  Rectangle {
                      anchors.fill: parent
                      id: backgroundRect
                      border.color: mapThemeComboBox.pressed ? "#17a81a" : Theme.mainColor
                      border.width: mapThemeComboBox.visualFocus ? 2 : 1
                      color: "transparent"
                      radius: 2
                  }
              }
          }

          QfToolButton {
              id: temporalButton
              Layout.alignment: Qt.AlignVCenter
              visible: flatLayerTree.isTemporal
              iconSource: mapSettings.isTemporal ? Theme.getThemeVectorIcon( 'ic_temporal_green_24dp' ) : Theme.getThemeVectorIcon( 'ic_temporal_black_24dp' )
              bgcolor: "white"
              onClicked: temporalProperties.open();
          }
      }
    }

    Legend {
      id: legend
      isVisible: position > 0
      Layout.fillWidth: true
      Layout.fillHeight: true
    }
  }

  TemporalProperties {
      id: temporalProperties
      mapSettings: mapSettings;
      modal: true
      closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
      parent: ApplicationWindow.overlay
  }
}
