import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import org.qgis 1.0
import Theme 1.0

Drawer {
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

  /* Workaround for menu position, will need to be adjusted when updating menu to Quick2 */
  onShowMenu: mainMenu.popup(settingsButton.x + 2, 2)
  onShowCloudMenu: cloudPopup.show()

  onCurrentLayerChanged: {
    if ( currentLayer && currentLayer.readOnly && stateMachine.state == "digitize" )
      displayToast( qsTr( "The layer %1 is read only." ).arg( currentLayer.name ) )
  }

  ColumnLayout {
    anchors.fill: parent

    Rectangle {
      Layout.fillWidth: true
      height: childrenRect.height

      color: mainColor

      Row {
        height: 56
        spacing: 1

        QfToolButton {
          id: closeButton
          anchors.verticalCenter: parent.verticalCenter
          iconSource: Theme.getThemeIcon( 'ic_chevron_left_white_24dp' )
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
          iconSource: cloudProjectsModel.currentProjectId ? Theme.getThemeVectorIcon( 'ic_cloud_active_24dp' ) : Theme.getThemeVectorIcon( 'ic_cloud_24dp' )
          bgcolor: "transparent"

          onClicked: showCloudMenu()
        }
      }


      Switch {
        id: modeswitch
        height: 56
        width: ( 56 + 36 ) 
        anchors.right: parent.right
        indicator: Rectangle {
          implicitHeight: 36
          implicitWidth: 36 * 2
          x: modeswitch.leftPadding
          radius: 4
          color:  "#66212121"
          border.color: "#44FFFFFF"
          anchors.verticalCenter: parent.verticalCenter
          Image {
            height: parent.height
            width: parent.width / 2
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            source: Theme.getThemeIcon( 'ic_map_white_48dp' )
            opacity: 0.4
          }
          Image {
            height: parent.height
            width: parent.width / 2
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            source: Theme.getThemeIcon( 'ic_create_white_24dp' )
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
              height: parent.height
              width: parent.height
              anchors.right:  modeswitch.checked ? parent.right : undefined
              anchors.left:  modeswitch.checked ? undefined : parent.left
              anchors.verticalCenter: parent.verticalCenter
              source:  modeswitch.checked ? Theme.getThemeIcon( 'ic_create_white_24dp' ) : Theme.getThemeIcon( 'ic_map_white_24dp' )
            }
            Behavior on x {
              PropertyAnimation {
                duration: 100
                easing.type: Easing.InQuart
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

      ComboBox {
        id: mapThemeComboBox
        anchors { left: parent.left; right: parent.right }

        Connections {
          target: iface

          function onLoadProjectEnded() {
            mapThemeContainer.isLoading = true
            var themes = qgisProject.mapThemeCollection.mapThemes
            mapThemeComboBox.model = themes
            mapThemeContainer.visible = themes.length > 1
            flatLayerTree.updateCurrentMapTheme()
            mapThemeComboBox.currentIndex = flatLayerTree.mapTheme != '' ? mapThemeComboBox.find( flatLayerTree.mapTheme ) : -1
            mapThemeContainer.isLoading = false
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
            //color: Theme.lightGray
            radius: 2
          }
        }
      }
    }

    Legend {
      id: legend
      Layout.fillWidth: true
      Layout.fillHeight: true
    }
  }
}
