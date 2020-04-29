import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11

import org.qgis 1.0
import Theme 1.0

Drawer {
  objectName: "dashBoard"

  signal showMenu

  property alias allowLayerChange: legend.enabled
  property alias currentLayer: legend.currentLayer
  property MapSettings mapSettings

  property color mainColor: Theme.mainColor

  width: Math.min( 300 * dp, mainWindow.width)
  height: parent.height
  edge: Qt.LeftEdge
  dragMargin: 10 * dp
  padding: 0

  property bool preventFromOpening: overlayFeatureFormDrawer.visible

  position: 0
  focus: opened
  clip: true

  /* Workaround for menu position, will need to be adjusted when updating menu to Quick2 */
  onShowMenu: mainMenu.popup(settingsButton.x + 2 * dp, 2 * dp)

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
        height: childrenRect.height
        spacing: 1 * dp

        ToolButton {
          height: 56 * dp
          width: 56 * dp

          contentItem: Rectangle {
            anchors.fill: parent
            color: mainColor
            enabled: welcomeScreen.visible
            Image {
              anchors.fill: parent
              fillMode: Image.Pad
              horizontalAlignment: Image.AlignHCenter
              verticalAlignment: Image.AlignVCenter
              source: Theme.getThemeIcon( 'ic_chevron_left_white_24dp' )
            }
          }

          onClicked: close()
        }

        ToolButton {
          id: settingsButton
          height: 56 * dp
          width: 56 * dp

          contentItem: Rectangle {
            anchors.fill: parent
            color: mainColor
            Image {
              anchors.fill: parent
              fillMode: Image.Pad
              horizontalAlignment: Image.AlignHCenter
              verticalAlignment: Image.AlignVCenter
              source: Theme.getThemeIcon( 'ic_settings_white_24dp' )
            }
          }

          onClicked: showMenu()
        }
      }


      Switch {
        id: modeswitch
        height: 56 * dp
        width: ( 56 + 36 )  * dp
        anchors.right: parent.right
        indicator: Rectangle {
          implicitHeight: 36 * dp
          implicitWidth: 36 * 2 * dp
          x: modeswitch.leftPadding
          radius: 4 * dp
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
            width: 36 * dp
            height: 36 * dp
            radius: 4 * dp
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
      id: mapThemeContainer
      Layout.fillWidth: true
      property bool isLoading: false

      ComboBox {
        id: mapThemeComboBox
        anchors { left: parent.left; right: parent.right }

        Connections {
          target: iface

          onLoadProjectEnded: {
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

        // [hidpi fixes]
        delegate: ItemDelegate {
          width: mapThemeComboBox.width
          height: 36 * dp
          text: modelData
          font.weight: mapThemeComboBox.currentIndex === index ? Font.DemiBold : Font.Normal
          font.pointSize: Theme.tipFont.pointSize
          highlighted: mapThemeComboBox.highlightedIndex == index
        }

        contentItem: Text {
          height: 36 * dp
          leftPadding: 8 * dp
          text: mapThemeComboBox.displayText
          font: Theme.tipFont
          horizontalAlignment: Text.AlignLeft
          verticalAlignment: Text.AlignVCenter
          elide: Text.ElideRight
        }

        background: Item {
          implicitWidth: 120 * dp
          implicitHeight: 36 * dp

          Rectangle {
            anchors.fill: parent
            id: backgroundRect
            border.color: mapThemeComboBox.pressed ? "#17a81a" : "#21be2b"
            border.width: mapThemeComboBox.visualFocus ? 2 : 1
            //color: Theme.lightGray
            radius: 2
          }
        }
        // [/hidpi fixes]
      }
    }

    Legend {
      id: legend
      Layout.fillWidth: true
      Layout.fillHeight: true
    }
  }
}
