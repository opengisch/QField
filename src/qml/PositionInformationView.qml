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
    rows: ( positionSource.device === '' ? 1 : 2 ) * ( parent.width > 1000? 1 : parent.width > 620? 2 : 3 )
    width: parent.width
    property double cellWidth: grid.width / ( ( positionSource.device === '' ? 1 : 2 ) * 6 / grid.rows )

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
                  qsTr( "Lat." ) + ': ' + ( positionSource.positionInfo && positionSource.positionInfo.latitudeValid  ? Number( positionSource.projectedPosition.y ).toLocaleString( Qt.locale(), 'f', 5 ) : qsTr( "N/A" ) )
                : qsTr( "X" )    + ': ' + ( positionSource.positionInfo && positionSource.positionInfo.longitudeValid ? Number( positionSource.projectedPosition.x ).toLocaleString( Qt.locale(), 'f', 2 ) : qsTr( "N/A" ) )
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
                  qsTr( "Lon." ) + ': ' + ( positionSource.positionInfo && positionSource.positionInfo.longitudeValid ? Number( positionSource.projectedPosition.x ).toLocaleString( Qt.locale(), 'f', 5 ) : qsTr( "N/A" ) )
                : qsTr( "Y" )    + ': ' + ( positionSource.positionInfo && positionSource.positionInfo.latitudeValid  ? Number( positionSource.projectedPosition.y ).toLocaleString( Qt.locale(), 'f', 2 ) : qsTr( "N/A" ) )

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
            var altitude = qsTr( "Altitude" ) + ': ';
            if ( positionSource.positionInfo && positionSource.positionInfo.elevationValid ) {
                altitude += Number( positionSource.projectedPosition.z ).toLocaleString( Qt.locale(), 'f', 2 ) + ' m'
                if ( !isNaN( parseFloat( antennaHeight ) ) ) {
                    altitude = ' <font color="#2f2f2f"><i>(%1)</i></font>'.arg( ( antennaHeight > 0 ? "+" : "-" ) + Math.abs( antennaHeight ).toLocaleString(Qt.locale(), 'f', 2) );
                }
            }
            else
            {
                altitude += 'N/A';
            }
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
        text: qsTr( "Speed" ) + ': ' + ( positionSource.positionInfo && positionSource.positionInfo.speedValid ? positionSource.positionInfo.speed.toLocaleString(Qt.locale(), 'f', 2) + " m/s" : qsTr( "N/A" ) )
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
        text: qsTr( "H. Accuracy" ) + ': ' + ( positionSource.positionInfo && positionSource.positionInfo.haccValid ? positionSource.positionInfo.hacc.toLocaleString(Qt.locale(), 'f', 2) + " m" : qsTr( "N/A" ) )
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
        text: qsTr( "V. Accuracy" ) + ': ' + ( positionSource.positionInfo && positionSource.positionInfo.vaccValid ? positionSource.positionInfo.vacc.toLocaleString(Qt.locale(), 'f', 2) + " m" : qsTr( "N/A" ) )
      }
    }

    Rectangle {
      height: rowHeight
      width: grid.cellWidth
      color: grid.rows % 2 === 0 ? "white" : "#e6f2fd"
      visible: positionSource.device !== ''

      Text {
        anchors.margins:  10
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        font: Theme.tipFont
        color: textColor
        text: qsTr( "PDOP" ) + ': ' + ( positionSource.positionInfo ? positionSource.positionInfo.pdop.toLocaleString(Qt.locale(), 'f', 2) : '-' )
      }
    }

    Rectangle {
      height: rowHeight
      width: grid.cellWidth
      color: grid.rows % 2 === 0 ? "#e6f2fd" : "white"
      visible: positionSource.device !== ''

      Text {
        anchors.margins:  10
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        font: Theme.tipFont
        color: textColor
        text: qsTr( "HDOP" ) + ': ' + ( positionSource.positionInfo ? positionSource.positionInfo.hdop.toLocaleString(Qt.locale(), 'f', 2) : '-' )
      }
    }

    Rectangle {
      height: rowHeight
      width: grid.cellWidth
      color: grid.rows === 6 ? "white" : "#e6f2fd"
      visible: positionSource.device !== ''

      Text {
        anchors.margins:  10
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        font: Theme.tipFont
        color: textColor
        text: qsTr( "VDOP" ) + ': ' + ( positionSource.positionInfo ? positionSource.positionInfo.vdop.toLocaleString(Qt.locale(), 'f', 2) : '-' )
      }
    }

    Rectangle {
      height: rowHeight
      width: grid.cellWidth
      color: grid.rows === 6 ? "#e6f2fd" : "white"
      visible: positionSource.device !== ''

      Text {
        anchors.margins:  10
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        font: Theme.tipFont
        color: textColor
        text: qsTr( "Valid" ) + ': ' + ( positionSource.positionInfo && positionSource.positionInfo.isValid ? 'True' : 'False' )
      }
    }

    Rectangle {
      height: rowHeight
      width: grid.cellWidth
      color: grid.rows === 2 || grid.rows === 6 ? "white" : "#e6f2fd"
      visible: positionSource.device !== ''

      Text {
        anchors.margins:  10
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        font: Theme.tipFont
        color: textColor
        text: qsTr( "Fix" ) + ': ' + ( positionSource.positionInfo ? positionSource.positionInfo.fixStatusDescription : '-' )
      }
    }

    Rectangle {
      height: rowHeight
      width: grid.cellWidth
      color: grid.rows === 2 || grid.rows === 6 ? "#e6f2fd" : "white"
      visible: positionSource.device !== ''

      Text {
        anchors.margins:  10
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        font: Theme.tipFont
        color: textColor
        text: qsTr( "Quality" ) + ': ' + ( positionSource.positionInfo ? positionSource.positionInfo.qualityDescription : '-' )
      }
    }

  }
}
