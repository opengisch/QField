import QtQuick 2.0
import org.qgis 1.0

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
    anchors.bottom: mainLine.top
    anchors.horizontalCenter: mainLine.horizontalCenter
    font.pointSize: 12 * dp
    color: "darkslategrey"

    text: if (vars.units === QgsUnitTypes.DistanceMeters && vars.adjustedMagnitude >= 1000 ) {
            vars.adjustedMagnitude/1000 + ' ' + UnitTypes.toAbbreviatedString( QgsUnitTypes.DistanceKilometers )
          } else {
            vars.adjustedMagnitude + ' ' + UnitTypes.toAbbreviatedString( mapSettings.destinationCrs.mapUnits )
          }
  }
}
