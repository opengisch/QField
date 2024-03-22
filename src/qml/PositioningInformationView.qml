import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import org.qgis 1.0
import org.qfield 1.0
import Theme 1.0

Rectangle {
  id: positioningInformationView

  property Positioning positionSource
  property var coordinates: GeometryUtils.reprojectPoint(positionSource.sourcePosition, CoordinateReferenceSystemUtils.wgs84Crs(), projectInfo.coordinateDisplayCrs)
  property bool coordinatesIsXY: CoordinateReferenceSystemUtils.defaultCoordinateOrderForCrsIsXY(projectInfo.coordinateDisplayCrs)
  property bool coordinatesIsGeographic: projectInfo.coordinateDisplayCrs.isGeographic

  property double distanceUnitFactor: UnitTypes.fromUnitToUnitFactor(Qgis.DistanceUnit.Meters, projectInfo.distanceUnits)
  property string distanceUnitAbbreviation: UnitTypes.toAbbreviatedString(projectInfo.distanceUnits)

  property double antennaHeight: NaN
  property double rowHeight: 30
  property color backgroundColor: Theme.mainBackgroundColor
  property color alternateBackgroundColor: Theme.positionBackgroundColor
  property color textColor: positionSource.currentness ? Theme.mainTextColor : Theme.secondaryTextColor

  height: grid.rows * positioningInformationView.rowHeight
  width: parent.width
  anchors.margins: 20

  color: Theme.mainBackgroundColor

  Grid {
    id: grid
    flow: GridLayout.TopToBottom
    rows: ( positionSource.deviceId === '' ? 1 : 2 ) * ( parent.width > 1000? 1 : parent.width > 620? 2 : 3 )
    width: parent.width
    property double cellWidth: grid.width / ( ( positionSource.deviceId === '' ? 1 : 2 ) * 6 / grid.rows )

    Rectangle {
      id: x
      height: rowHeight
      width: grid.cellWidth
      color: alternateBackgroundColor

      Text {
        anchors.margins:  10
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        font: Theme.tipFont
        color: textColor
        text: coordinatesIsXY
              ? (coordinatesIsGeographic ? qsTr( "Lon" ) : qsTr( "X" )) + ': '
                + ( positionSource.positionInformation && positionSource.positionInformation.longitudeValid
                   ? Number( coordinates.x ).toLocaleString( Qt.locale(), 'f', coordinatesIsGeographic ? 7 : 3 )
                   : qsTr( "N/A" ) )
              : (coordinatesIsGeographic ? qsTr( "Lat" ) : qsTr( "Y" )) + ': '
                + ( positionSource.positionInformation && positionSource.positionInformation.latitudeValid
                   ? Number( coordinates.y ).toLocaleString( Qt.locale(), 'f', coordinatesIsGeographic ? 7 : 3 )
                   : qsTr( "N/A" ) )
      }
    }

    Rectangle {
      height: rowHeight
      width: grid.cellWidth
      color: backgroundColor

      Text {
        anchors.margins:  10
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        font: Theme.tipFont
        color: textColor
        text: coordinatesIsXY
              ? (coordinatesIsGeographic ? qsTr( "Lat" ) : qsTr( "Y" )) + ': '
                + ( positionSource.positionInformation && positionSource.positionInformation.longitudeValid
                   ? Number( coordinates.y ).toLocaleString( Qt.locale(), 'f', coordinatesIsGeographic ? 7 : 3 )
                   : qsTr( "N/A" ) )
              : (coordinatesIsGeographic ? qsTr( "Lon" ) : qsTr( "X" )) + ': '
                + ( positionSource.positionInformation && positionSource.positionInformation.latitudeValid
                   ? Number( coordinates.x ).toLocaleString( Qt.locale(), 'f', coordinatesIsGeographic ? 7 : 3 )
                   : qsTr( "N/A" ) )

      }
    }

    Rectangle {
      height: rowHeight
      width: grid.cellWidth
      color: grid.rows === 2 ? backgroundColor : alternateBackgroundColor
      Text {
        anchors.margins:  10
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        font: Theme.tipFont
        color: textColor
        text: {
          var altitude = qsTr( "Altitude" ) + ': '
          if (positionSource.positionInformation && positionSource.positionInformation.elevationValid) {
            altitude += Number(positionSource.projectedPosition.z * distanceUnitFactor).toLocaleString(Qt.locale(), 'f', 3) + ' ' + distanceUnitAbbreviation + ' '
            var details = []
            if (positionSource.elevationCorrectionMode === Positioning.ElevationCorrectionMode.OrthometricFromGeoidFile) {
              details.push('grid')
            } else if (positionSource.elevationCorrectionMode === Positioning.ElevationCorrectionMode.OrthometricFromDevice) {
              details.push('ortho.')
            }
            if (!isNaN(parseFloat(antennaHeight))) {
              details.push('ant.')
            }
            if (details.length > 0) {
              altitude += ' (%1)'.arg( details.join(', '))
            }
          }
          else
          {
            altitude += qsTr('N/A');
          }
          return altitude
        }
      }
    }

    Rectangle {
      height: rowHeight
      width: grid.cellWidth
      color: grid.rows === 2 ? alternateBackgroundColor : backgroundColor

      Text {
        anchors.margins:  10
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        font: Theme.tipFont
        color: textColor
        text: qsTr( "Speed" ) + ': ' + ( positionSource.positionInformation && positionSource.positionInformation.speedValid ? positionSource.positionInformation.speed.toLocaleString(Qt.locale(), 'f', 3) + " m/s" : qsTr( "N/A" ) )
      }
    }

    Rectangle {
      height: rowHeight
      width: grid.cellWidth
      color: grid.rows === 4 ? backgroundColor : alternateBackgroundColor

      Text {
        anchors.margins:  10
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        font: Theme.tipFont
        color: textColor
        text: qsTr("H. Accuracy") + ': ' + (positionSource.positionInformation && positionSource.positionInformation.haccValid
                                            ? Number(positionSource.positionInformation.hacc * distanceUnitFactor).toLocaleString(Qt.locale(), 'f', 3) + ' ' + distanceUnitAbbreviation
                                            : qsTr("N/A"))
      }
    }

    Rectangle {
      height: rowHeight
      width: grid.cellWidth
      color: grid.rows === 4 ? alternateBackgroundColor : backgroundColor

      Text {
        anchors.margins:  10
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        font: Theme.tipFont
        color: textColor
        text: qsTr( "V. Accuracy" ) + ': ' + (positionSource.positionInformation && positionSource.positionInformation.vaccValid
                                              ? Number(positionSource.positionInformation.vacc * distanceUnitFactor).toLocaleString(Qt.locale(), 'f', 3) + ' ' + distanceUnitAbbreviation
                                              : qsTr("N/A"))
      }
    }

    Rectangle {
      height: rowHeight
      width: grid.cellWidth
      color: grid.rows % 2 === 0 ? backgroundColor : alternateBackgroundColor
      visible: positionSource.deviceId !== ''

      Text {
        anchors.margins:  10
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        font: Theme.tipFont
        color: textColor
        text: qsTr( "PDOP" ) + ': ' +
              (positionSource.positionInformation && !isNaN(positionSource.positionInformation.pdop)
               ? positionSource.positionInformation.pdop.toLocaleString(Qt.locale(), 'f', 1)
               : qsTr('N/A'))
      }
    }

    Rectangle {
      height: rowHeight
      width: grid.cellWidth
      color: grid.rows % 2 === 0 ? alternateBackgroundColor : backgroundColor
      visible: positionSource.deviceId !== ''

      Text {
        anchors.margins:  10
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        font: Theme.tipFont
        color: textColor
        text: qsTr( "HDOP" ) + ': ' +
              (positionSource.positionInformation && !isNaN(positionSource.positionInformation.hdop)
               ? positionSource.positionInformation.hdop.toLocaleString(Qt.locale(), 'f', 1)
               : qsTr('N/A'))
      }
    }

    Rectangle {
      height: rowHeight
      width: grid.cellWidth
      color: grid.rows === 6 ? backgroundColor : alternateBackgroundColor
      visible: positionSource.deviceId !== ''

      Text {
        anchors.margins:  10
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        font: Theme.tipFont
        color: textColor
        text: qsTr( "VDOP" ) + ': ' +
              (positionSource.positionInformation && !isNaN(positionSource.positionInformation.vdop)
               ? positionSource.positionInformation.vdop.toLocaleString(Qt.locale(), 'f', 1)
               : qsTr('N/A'))
      }
    }

    Rectangle {
      height: rowHeight
      width: grid.cellWidth
      color: grid.rows === 6 ? alternateBackgroundColor : backgroundColor
      visible: positionSource.deviceId !== ''

      Text {
        anchors.margins:  10
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        font: Theme.tipFont
        color: textColor
        text: qsTr( "Valid" ) + ': ' + ( positionSource.positionInformation && positionSource.positionInformation.isValid ? 'True' : 'False' )
      }
    }

    Rectangle {
      height: rowHeight
      width: grid.cellWidth
      color: grid.rows === 2 || grid.rows === 6 ? backgroundColor : alternateBackgroundColor
      visible: positionSource.deviceId !== ''

      Text {
        anchors.margins:  10
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        font: Theme.tipFont
        color: textColor
        text: qsTr( "Fix" ) + ': ' + ( positionSource.positionInformation ? positionSource.positionInformation.fixStatusDescription : qsTr('N/A') )
      }
    }

    Rectangle {
      height: rowHeight
      width: grid.cellWidth
      color: grid.rows === 2 || grid.rows === 6 ? alternateBackgroundColor : backgroundColor
      visible: positionSource.deviceId !== ''

      Text {
        anchors.margins:  10
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        font: Theme.tipFont
        color: textColor
        text: qsTr( "Quality" ) + ': ' + ( positionSource.positionInformation ? positionSource.positionInformation.qualityDescription : qsTr('N/A') )
      }
    }
  }
}
