import QtQuick 2.11
import org.qgis 1.0
import Theme 1.0

Item {
  property MapSettings mapSettings
  property int referenceWidth: 300 * dp
  property double lineWidth: 2*dp

  QtObject {
    id: vars

    property real range: referenceWidth * mapSettings.mapUnitsPerPixel
    property int exponent: Math.floor(Math.log(range) / Math.LN10)
    property int magnitude: Math.pow(10, exponent)
    property int adjustedMagnitude: magnitude / (1 + Math.round((magnitude / mapSettings.mapUnitsPerPixel) / referenceWidth))
    property int units: mapSettings.destinationCrs.mapUnits
  }

  Rectangle {
    id: mainLine
    width: vars.adjustedMagnitude / mapSettings.mapUnitsPerPixel
    height: lineWidth
    color: "darkslategrey"
  }

  Rectangle {
    width: lineWidth
    height: 3*lineWidth
    color: "darkslategrey"
    anchors.left: mainLine.left
    anchors.bottom: mainLine.top
  }

  Rectangle {
    width: lineWidth
    height: 3*lineWidth
    color: "darkslategrey"
    anchors.right: mainLine.right
    anchors.bottom: mainLine.top
  }

  Text {
    id: label
    anchors.bottomMargin: 4 * dp
    anchors.bottom: mainLine.top
    anchors.horizontalCenter: mainLine.horizontalCenter
    anchors.left: undefined // The value will be set to mainLine.left is the label is wider than the mainLine
    font: Theme.defaultFont
    color: "darkslategrey"

    states: State {
        name: "narrow"; when: label.width > mainLine.width
        AnchorChanges {
            target: label
            anchors.horizontalCenter: undefined
            anchors.left: mainLine.left
        }
    }

    text: if (vars.units === QgsUnitTypes.DistanceMeters && vars.adjustedMagnitude >= 1000 ) {
            vars.adjustedMagnitude/1000 + ' ' + UnitTypes.toAbbreviatedString( QgsUnitTypes.DistanceKilometers )
          } else {
            vars.adjustedMagnitude + ' ' + UnitTypes.toAbbreviatedString( mapSettings.destinationCrs.mapUnits )
          }
  }
}
