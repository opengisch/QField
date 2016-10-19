import QtQuick 2.0
import org.qgis 1.0
import QtQuick.Controls 1.4 as Controls
import "js/style.js" as Style

Item {
  id: dashBoard

  signal showMenu

  property alias showLayerSelector: layerSelectorContainer.visible
  property alias currentLayer: layerSelector.currentLayer

  property color mainColor: "#C8E6C9"

  Rectangle {
    anchors.fill: parent
    color: "white"
  }

  Column {
    anchors { left: parent.left; right: parent.right }
    Rectangle {
      anchors { left: parent.left; right: parent.right }
      height: childrenRect.height

      color: mainColor

      Row {
        height: childrenRect.height

        Button {
          // dummy button, is hidden behind main button
          iconSource: Style.getThemeIcon( 'ic_menu_white_24dp' )
          bgcolor: mainColor
        }

        Button {
          iconSource: Style.getThemeIcon( 'ic_settings_white_24dp' )

          bgcolor: mainColor

          onClicked: showMenu()
        }
      }
    }

    Controls.GroupBox {
      id: layerSelectorContainer
      title: qsTr( "Active Layer" )
      LayerSelector {
        id: layerSelector
        anchors { left: parent.left; right: parent.right }
      }
    }

    Controls.GroupBox {
      title: qsTr( "Map Theme" )

      Controls.ComboBox {
        id: mapThemeComboBox
        anchors { left: parent.left; right: parent.right }

        Connections {
          target: qgisProject.mapThemeCollection
          onMapThemesChanged:
              mapThemeComboBox.model = qgisProject.mapThemeCollection.mapThemes
        }
      }
    }
  }
}
