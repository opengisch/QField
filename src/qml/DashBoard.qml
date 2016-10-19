import QtQuick 2.0
import org.qgis 1.0
import "js/style.js" as Style

Item {
  signal showMenu

  property alias showLayerSelector: layerSelector.visible
  property alias currentLayer: layerSelector.currentLayer

  Rectangle {
    anchors.fill: parent
    color: "white"
  }

  Column {
    anchors { left: parent.left; right: parent.right }
    Rectangle {
      anchors { left: parent.left; right: parent.right }
      height: childrenRect.height

      color: "#C8E6C9"

      Row {
        height: childrenRect.height

        Button {
          // dummy button, is hidden behind main button
          iconSource: Style.getThemeIcon( 'ic_menu_white_24dp' )
        }

        Button {
          iconSource: Style.getThemeIcon( 'ic_settings_white_24dp' )

          onClicked: showMenu()
        }
      }
    }

    LayerSelector {
      id: layerSelector
      anchors { left: parent.left; right: parent.right }
    }
  }
}
