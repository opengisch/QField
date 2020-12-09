import QtQuick 2.12
import QtQuick.Controls 2.12
import QtPositioning 5.8
import QtQuick.Layouts 1.12

import org.qgis 1.0
import org.qfield 1.0
import Utils 1.0
import Theme 1.0

Rectangle {
  id: positionInformationView
  property TransformedPositionSource positionSource
  property double rowHeight: 30
  property double antennaHeight: NaN
  color: "yellow"
  property color textColor: positionSource.currentness ? "black" : "gray"

  height: grid.rows * positionInformationView.rowHeight
  width: parent.width
  anchors.margins: 20

  Grid {
    id: grid
    flow: GridLayout.TopToBottom
    rows: ( positionSource.device === 'internal' ? 1 : 2 ) * ( parent.width > 1000? 1 : parent.width > 620? 2 : 3 )
    width: parent.width
    property double cellWidth: grid.width / ( ( positionSource.device === 'internal' ? 1 : 2 ) * 6 / grid.rows )

    Rectangle {
      id: x
      height: rowHeight
      width: grid.cellWidth
      color: "#e6f2fd"

      Text {
        anchors.margins:  10
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        font: Theme.tipFont
        color: textColor
        text: positionSource.destinationCrs.isGeographic ?
                  qsTr( "Lat." ) + ': ' + ( positionSource.positionInfo.latitudeValid  ? Number( positionSource.projectedPosition.y ).toLocaleString( Qt.locale(), 'f', 5 ) : qsTr( "N/A" ) )
                : qsTr( "X" )    + ': ' + ( positionSource.positionInfo.longitudeValid ? Number( positionSource.projectedPosition.x ).toLocaleString( Qt.locale(), 'f', 2 ) : qsTr( "N/A" ) )
      }
    }

    Rectangle {
      height: rowHeight
      width: grid.cellWidth
      color: "white"

      Text {
        anchors.margins:  10
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        font: Theme.tipFont
        color: textColor
        text: positionSource.destinationCrs.isGeographic ?
                  qsTr( "Lon." ) + ': ' + ( positionSource.positionInfo.longitudeValid ? Number( positionSource.projectedPosition.x ).toLocaleString( Qt.locale(), 'f', 5 ) : qsTr( "N/A" ) )
                : qsTr( "Y" )    + ': ' + ( positionSource.positionInfo.latitudeValid  ? Number( positionSource.projectedPosition.y ).toLocaleString( Qt.locale(), 'f', 2 ) : qsTr( "N/A" ) )

      }
    }

    Rectangle {
      height: rowHeight
      width: grid.cellWidth
      color: grid.rows === 2 ? "white" : "#e6f2fd"
      Text {
        anchors.margins:  10
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        font: Theme.tipFont
        color: textColor
        text: {
          var altitude
          if ( positionSource.positionInfo.elevationValid ) {
            altitude = Number( positionSource.projectedPosition.z ).toLocaleString( Qt.locale(), 'f', 2 )
            if ( !isNaN( parseFloat( antennaHeight ) ) ) {
              altitude += ' <font color="#2f2f2f"><i>(%1)</i></font>'.arg((antennaHeight > 0 ? "+" : "-") + Math.abs(antennaHeight).toLocaleString(Qt.locale(), 'f', 2))
            }
          }
          else
          {
            altitude = qsTr( "N/A" )
          }
          altitude = qsTr( "Altitude: %1 m" ).arg( altitude )
          return altitude
        }
      }
    }

    Rectangle {
      height: rowHeight
      width: grid.cellWidth
      color: grid.rows === 2 ? "#e6f2fd" : "white"

      Text {
        anchors.margins:  10
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        font: Theme.tipFont
        color: textColor
        text: qsTr( "Speed" ) + ': ' + ( positionSource.positionInfo.speedValid ? positionSource.positionInfo.speed.toLocaleString(Qt.locale(), 'f', 2) + " m/s" : qsTr( "N/A" ) )
      }
    }

    Rectangle {
      height: rowHeight
      width: grid.cellWidth
      color: grid.rows === 4 ? "white" : "#e6f2fd"

      Text {
        anchors.margins:  10
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        font: Theme.tipFont
        color: textColor
        text: qsTr( "H. Accuracy" ) + ': ' + ( positionSource.positionInfo.haccValid ? positionSource.positionInfo.hacc.toLocaleString(Qt.locale(), 'f', 2) + " m" : qsTr( "N/A" ) )
      }
    }

    Rectangle {
      height: rowHeight
      width: grid.cellWidth
      color: grid.rows === 4 ? "#e6f2fd" : "white"

      Text {
        anchors.margins:  10
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        font: Theme.tipFont
        color: textColor
        text: qsTr( "V. Accuracy" ) + ': ' + ( positionSource.positionInfo.vaccValid ? positionSource.positionInfo.vacc.toLocaleString(Qt.locale(), 'f', 2) + " m" : qsTr( "N/A" ) )
      }
    }

    Rectangle {
      height: rowHeight
      width: grid.cellWidth
      color: grid.rows % 2 === 0 ? "white" : "#e6f2fd"
      visible: positionSource.device !== 'internal'

      Text {
        anchors.margins:  10
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        font: Theme.tipFont
        color: textColor
        text: qsTr( "pdop" ) + ': ' + positionSource.positionInfo.pdop.toLocaleString(Qt.locale(), 'f', 2)
      }
    }

    Rectangle {
      height: rowHeight
      width: grid.cellWidth
      color: grid.rows % 2 === 0 ? "#e6f2fd" : "white"
      visible: positionSource.device !== 'internal'

      Text {
        anchors.margins:  10
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        font: Theme.tipFont
        color: textColor
        text: qsTr( "hdop" ) + ': ' + positionSource.positionInfo.hdop.toLocaleString(Qt.locale(), 'f', 2)
      }
    }

    Rectangle {
      height: rowHeight
      width: grid.cellWidth
      color: grid.rows === 6 ? "white" : "#e6f2fd"
      visible: positionSource.device !== 'internal'

      Text {
        anchors.margins:  10
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        font: Theme.tipFont
        color: textColor
        text: qsTr( "vdop" ) + ': ' + positionSource.positionInfo.vdop.toLocaleString(Qt.locale(), 'f', 2)
      }
    }

    Rectangle {
      height: rowHeight
      width: grid.cellWidth
      color: grid.rows === 6 ? "#e6f2fd" : "white"
      visible: positionSource.device !== 'internal'

      Text {
        anchors.margins:  10
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        font: Theme.tipFont
        color: textColor
        text: qsTr( "valid" ) + ': ' + ( positionSource.positionInfo.isValid ? 'true' : 'false' )
      }
    }

    Rectangle {
      height: rowHeight
      width: grid.cellWidth
      color: grid.rows === 2 || grid.rows === 6 ? "white" : "#e6f2fd"
      visible: positionSource.device !== 'internal'

      Text {
        anchors.margins:  10
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        font: Theme.tipFont
        color: textColor
        text: qsTr( "fix" ) + ': ' + ( positionSource.positionInfo.fixStatus === GnssPositionInformation.NoFix ? 'NoFix' :
                                     positionSource.positionInfo.fixStatus === GnssPositionInformation.Fix2D ? 'Fix2D' :
                                     positionSource.positionInfo.fixStatus === GnssPositionInformation.Fix3D ? 'Fix3D' :
                                     'NoData' )
      }
    }

    Rectangle {
      height: rowHeight
      width: grid.cellWidth
      color: grid.rows === 2 || grid.rows === 6 ? "#e6f2fd" : "white"
      visible: positionSource.device !== 'internal'

      Text {
        anchors.margins:  10
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        font: Theme.tipFont
        color: textColor
        text: qsTr( "quality" ) + ': ' + positionSource.positionInfo.qualityDescription
      }
    }

  }
}
