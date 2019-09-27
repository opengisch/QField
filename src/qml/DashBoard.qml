import QtQuick 2.11
import org.qgis 1.0
import QtQuick.Controls 2.4 as Controls
import QtQuick.Layouts 1.1
import "js/style.js" as Style

Controls.Pane {
  objectName: "dashBoard"

  signal showMenu

  property alias allowLayerChange: legend.enabled
  property alias currentLayer: legend.currentLayer
  property MapSettings mapSettings

  property color mainColor: "#80CC28"
  padding: 0

  anchors { left: parent.left; bottom: parent.bottom; top: parent.top; }

  property bool preventFromOpening: overlayFeatureFormDrawer.visible
  readonly property bool open: dashBoard.visible && !preventFromOpening

  visible: false
  focus: visible
  clip: true

  Keys.onReleased: {
    if ( event.key === Qt.Key_Back ||
      event.key === Qt.Key_Escape ) {
      visible=false
      event.accepted = true
    }
  }

  Behavior on width {
    NumberAnimation {
      duration: 200
      easing.type: Easing.InOutQuad
    }
  }

  /* Workaround for menu position, will need to be adjusted when updating menu to QuickControls.2 */
  onShowMenu: mainMenu.__popup(Qt.rect(menuButton.x + 2 * menuButton.width, menuButton.y, mainMenu.__popupGeometry.width, mainMenu.__popupGeometry.height), 0, 0)

  onCurrentLayerChanged: {
    if ( currentLayer && currentLayer.readOnly && stateMachine.state == "digitize" )
      displayToast( qsTr( "The layer %1 is read only." ).arg( currentLayer.name ) )
  }

  Component.onCompleted: focusstack.addFocusTaker( this )

  ColumnLayout {
    anchors.fill: parent

    Rectangle {
      Layout.fillWidth: true
      height: childrenRect.height

      color: mainColor

      Row {
        height: childrenRect.height
        spacing: 4 * dp

        Controls.ToolButton {
          height: 48 * dp
          width: 48 * dp

          contentItem: Rectangle {
            anchors.fill: parent
            color: mainColor
            Image {
              anchors.fill: parent
              fillMode: Image.Pad
              horizontalAlignment: Image.AlignHCenter
              verticalAlignment: Image.AlignVCenter
              source: Style.getThemeIcon( 'ic_menu_white_24dp' )

            }
          }
        }

        Controls.ToolButton {
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
              source: Style.getThemeIcon( 'ic_settings_white_24dp' )
            }
          }

          onClicked: showMenu()
        }
      }


      Controls.Switch {
        id: modeswitch
        height: 56 * dp
        width: ( 56 + 36 )  * dp
        anchors.right: parent.right
        indicator: Rectangle {
          implicitHeight: 36 * dp
          implicitWidth: 36 * 2 * dp
          x: modeswitch.leftPadding
          radius: 4 * dp
          color:  "#80CC28"
          border.color: "white"
          anchors.verticalCenter: parent.verticalCenter
          Image {
            height: parent.height
            width: parent.width / 2
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            source: Style.getThemeIcon( 'ic_map_white_48dp' )
          }
          Image {
            height: parent.height
            width: parent.width / 2
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            source: Style.getThemeIcon( 'ic_create_white_24dp' )
          }
          Rectangle {
            x: modeswitch.checked ? parent.width - width : 0
            width: 36 * dp
            height: 36 * dp
            radius: 4 * dp
            color:  "#64B5F6"
            border.color: "white"
            Image {
              height: parent.height
              width: parent.height
              anchors.right:  modeswitch.checked ? parent.right : undefined
              anchors.left:  modeswitch.checked ? undefined : parent.left
              anchors.verticalCenter: parent.verticalCenter
              source:  modeswitch.checked ? Style.getThemeIcon( 'ic_create_white_24dp' ) : Style.getThemeIcon( 'ic_map_white_24dp' )
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

    Controls.GroupBox {
      id: mapThemeContainer
      Layout.fillWidth: true
      property bool isLoading: false

      Controls.ComboBox {
        id: mapThemeComboBox
        anchors { left: parent.left; right: parent.right }

        Connections {
          target: iface

          onLoadProjectEnded: {
            mapThemeContainer.isLoading = true
            var themes = qgisProject.mapThemeCollection.mapThemes
            mapThemeComboBox.model = themes
            mapThemeContainer.visible = themes.length > 1
            layerTree.updateCurrentMapTheme()
            mapThemeComboBox.currentIndex = mapThemeComboBox.find( layerTree.mapTheme )
            mapThemeContainer.isLoading = false
          }
        }

        onCurrentTextChanged: {
          if ( !mapThemeContainer.isLoading && qgisProject.mapThemeCollection.mapThemes.length > 1 ) {
            layerTree.mapTheme = mapThemeComboBox.currentText
          }
        }

        // [hidpi fixes]
        delegate: Controls.ItemDelegate {
          width: mapThemeComboBox.width
          height: 36 * dp
          text: modelData
          font.weight: mapThemeComboBox.currentIndex === index ? Font.DemiBold : Font.Normal
          font.pointSize: 12
          highlighted: mapThemeComboBox.highlightedIndex == index
        }

        contentItem: Text {
          height: 36 * dp
          leftPadding: 8 * dp
          text: mapThemeComboBox.displayText
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
            //color: "#dddddd"
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
