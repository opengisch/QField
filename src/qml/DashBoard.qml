import QtQuick 2.0
import org.qgis 1.0
import QtQuick.Controls 2.0 as Controls
import QtQuick.Layouts 1.1
import "js/style.js" as Style

Controls.Pane {
  id: dashBoard

  signal showMenu

  property alias allowLayerChange: legend.enabled
  property alias currentLayer: legend.currentLayer
  property MapSettings mapSettings

  property color mainColor: "#C8E6C9"
  padding: 0

  ColumnLayout {
    anchors.fill: parent

    Rectangle {
      anchors { left: parent.left; right: parent.right }
      height: childrenRect.height

      color: mainColor

      Row {
        height: childrenRect.height

        Controls.ToolButton {
          // dummy button, is hidden behind main button
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
              source: Style.getThemeIcon( 'ic_settings_white_24dp' )
            }
          }

          onClicked: showMenu()
        }
      }
    }

    Controls.GroupBox {
      id: mapThemeContainer
      title: qsTr( "Map Theme" )
      anchors { left: parent.left; right: parent.right }

      Controls.ComboBox {
        id: mapThemeComboBox
        anchors { left: parent.left; right: parent.right }

        Connections {
          target: iface

          onLoadProjectEnded: {
            var themes = qgisProject.mapThemeCollection.mapThemes.slice()
            themes.reverse()
            mapThemeComboBox.model = themes
            mapThemeContainer.visible = themes.length > 1
          }
        }

        onCurrentIndexChanged: {
          if ( qgisProject.mapThemeCollection.mapThemes.length > 1 ) {
            layerTree.mapTheme = mapThemeComboBox.currentText
          }
        }
      }
    }

    Legend {
      id: legend
      anchors { left: parent.left; right: parent.right }
      Layout.fillHeight: true
    }
  }
}
