import QtQuick 2.0
import org.qgis 1.0

Row {
  property MapSettings mapSettings
  property int referenceWidth: 300 * dp

  QtObject {
    id: vars

    property real range: referenceWidth * mapSettings.mapUnitsPerPixel
    property int exponent: Math.floor(Math.log(range) /  Math.LN10)
    property int magnitude: Math.pow(10, exponent)
    property int adjustedMagnitude: magnitude / (1 + Math.round((magnitude / mapSettings.mapUnitsPerPixel) / referenceWidth))
  }

  Repeater {
    model: 1

    delegate: Loader {
      sourceComponent: scaleBarPart

      property int factor: 2
      property int xindex: index
      property string labelText: vars.adjustedMagnitude * factor + ' ' + UnitTypes.encodeUnit(mapSettings.mapUnits)
    }
  }

  Repeater {
    model: 2

    delegate: Loader {
      sourceComponent: scaleBarPart

      property int factor: 1
      property int xindex: index + 1
      property string labelText: undefined
    }
  }

  Component {
    id: scaleBarPart

    Item {
      width: rect.width
      height: rect.height

      Rectangle {
        id: rect
        width: factor * vars.adjustedMagnitude / mapSettings.mapUnitsPerPixel
        height: label.height + 8 * dp
        color: xindex % 2 ? "white" : "black"

        Behavior on width {
          SmoothedAnimation { duration: 200 }
        }
      }

      Text {
        id: label
        anchors.right: rect.right
        anchors.rightMargin: 1 * dp
        anchors.verticalCenter: rect.verticalCenter

        color: xindex % 2 ? "black" : "white"

        text: labelText || vars.adjustedMagnitude * factor
      }
    }
  }
}
