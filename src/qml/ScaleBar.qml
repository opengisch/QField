import QtQuick 2.0
import org.qgis 1.0

Row {
  property MapSettings mapSettings
  property int referenceWidth: 192 * dp

  QtObject {
    id: vars

    property real range: referenceWidth * mapSettings.mapUnitsPerPixel
    property int exponent: Math.log(range) /  Math.LN10
    property int magnitude: Math.pow(10, exponent)
  }

  Repeater {
    model: 4

    Item {
      width: rect.width
      height: rect.height

      Rectangle {
        id: rect
        width: vars.magnitude / mapSettings.mapUnitsPerPixel
        height: 20
        color: index % 2 ? "black" : "white"
      }

      Text {
        anchors.right: rect.right
        anchors.rightMargin: 1 * dp
        anchors.verticalCenter: rect.verticalCenter

        color: index % 2 ? "white" : "black"

        text: Math.floor( ( vars.magnitude / mapSettings.mapUnitsPerPixel ) * ( 1 + index ) * mapSettings.mapUnitsPerPixel )
      }
    }
  }
}
