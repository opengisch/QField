import QtQuick 2.12
import Theme 1.0
import org.qgis 1.0

Item {
    property MapSettings mapSettings
    property int referenceWidth: 300
    property double lineWidth: 2

    QtObject {
        id: vars

        property int units: mapSettings.destinationCrs.mapUnits
        property real range: referenceWidth * mapSettings.mapUnitsPerPixel
        property real exponent: Math.floor(Math.log(range) / Math.LN10)
        property real magnitude: Math.pow(10, exponent)
        property real adjustedMagnitude: units == QgsUnitTypes.DistanceDegrees ? magnitude / (1 + (magnitude / mapSettings.mapUnitsPerPixel) / referenceWidth) : magnitude / (1 + Math.round((magnitude / mapSettings.mapUnitsPerPixel) / referenceWidth))
        property real decimalsAdjustment: units == QgsUnitTypes.DistanceDegrees ? adjustedMagnitude < 0.01 ? 4 : 3 : 0
    }

    Rectangle {
        id: mainLineBackground

        width: (vars.adjustedMagnitude / mapSettings.mapUnitsPerPixel) + 2
        height: lineWidth + 2
        color: "#AAFFFFFF"
    }

    Rectangle {
        width: lineWidth + 2
        height: (3 * lineWidth) + 1
        color: "#AAFFFFFF"
        anchors.left: mainLineBackground.left
        anchors.bottom: mainLineBackground.top
    }

    Rectangle {
        width: lineWidth + 2
        height: (3 * lineWidth) + 1
        color: "#AAFFFFFF"
        anchors.right: mainLineBackground.right
        anchors.bottom: mainLineBackground.top
    }

    Rectangle {
        id: mainLine

        width: vars.adjustedMagnitude / mapSettings.mapUnitsPerPixel
        height: lineWidth
        color: Theme.darkGray

        anchors {
            horizontalCenter: mainLineBackground.horizontalCenter
            verticalCenter: mainLineBackground.verticalCenter
        }

    }

    Rectangle {
        width: lineWidth
        height: 3 * lineWidth
        color: Theme.darkGray
        anchors.left: mainLine.left
        anchors.bottom: mainLine.top
    }

    Rectangle {
        width: lineWidth
        height: 3 * lineWidth
        color: Theme.darkGray
        anchors.right: mainLine.right
        anchors.bottom: mainLine.top
    }

    Text {
        id: label

        anchors.bottomMargin: 7
        anchors.bottom: mainLine.top
        anchors.horizontalCenter: mainLine.horizontalCenter
        anchors.left: undefined // The value will be set to mainLine.left is the label is wider than the mainLine
        font: Theme.defaultFont
        color: Theme.darkGray
        style: Text.Outline
        styleColor: "#AAFFFFFF"
        text: if (vars.units === QgsUnitTypes.DistanceMeters && vars.adjustedMagnitude >= 1000) {
            vars.adjustedMagnitude / 1000 + " " + UnitTypes.toAbbreviatedString(QgsUnitTypes.DistanceKilometers);
        } else {
            Math.round(vars.adjustedMagnitude * Math.pow(10, vars.decimalsAdjustment)) / Math.pow(10, vars.decimalsAdjustment) + " " + UnitTypes.toAbbreviatedString(mapSettings.destinationCrs.mapUnits);
        }

        states: State {
            name: "narrow"
            when: label.width > mainLine.width

            AnchorChanges {
                target: label
                anchors.horizontalCenter: undefined
                anchors.left: mainLine.left
            }

        }

    }

}
