import QtQuick 2.11
import QtQuick.Controls 2.11
import QtPositioning 5.8
import QtQuick.Layouts 1.3
import org.qgis 1.0
import org.qfield 1.0
import Utils 1.0
import Theme 1.0

Rectangle {
  id: positionInformationView
  property TransformedPositionSource positionSource
  property double rowHeight: 30*dp
  property double antennaHeight: NaN
  color: "yellow"

  height: grid.rows * positionInformationView.rowHeight + 2 * border.width
  width: parent.width
  anchors.margins: 20

  Grid {
    id: grid
    flow: GridLayout.TopToBottom
    rows: parent.width > 800*dp ? 1: 2
    width: parent.width
    property double cellWidth: grid.width / ( 3 * ( grid.rows === 1 ? 2 : 1 ) )

    Rectangle {
      id: x
      height: rowHeight
      width: grid.cellWidth
      color: Theme.lightGray

      Text {
        anchors.margins:  10*dp
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        font: Theme.defaultFont
        text: positionSource.destinationCrs.isGeographic ?
                  qsTr( "Lat." ) + ': ' + ( positionSource.position.latitudeValid  ? Number( positionSource.projectedPosition.y ).toLocaleString( Qt.locale(), 'f', 3 ) : qsTr( "N/A" ) )
                : qsTr( "X" )    + ': ' + ( positionSource.position.longitudeValid ? Number( positionSource.projectedPosition.x ).toLocaleString( Qt.locale(), 'f', 3 ) : qsTr( "N/A" ) )
      }
    }

    Rectangle {
      height: rowHeight
      width: grid.cellWidth
      color: "white"

      Text {
        anchors.margins:  10*dp
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        font: Theme.defaultFont
        text: positionSource.destinationCrs.isGeographic ?
                  qsTr( "Lon." ) + ': ' + ( positionSource.position.longitudeValid ? Number( positionSource.projectedPosition.x ).toLocaleString( Qt.locale(), 'f', 3 ) : qsTr( "N/A" ) )
                : qsTr( "Y" )    + ': ' + ( positionSource.position.latitudeValid  ? Number( positionSource.projectedPosition.y ).toLocaleString( Qt.locale(), 'f', 3 ) : qsTr( "N/A" ) )

      }
    }

    Rectangle {
      height: rowHeight
      width: grid.cellWidth
      color: grid.rows === 2 ? "white" : Theme.lightGray

      Text {
        anchors.margins:  10*dp
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        font: Theme.defaultFont
        text: {
          var altitude
          if ( positionSource.position.altitudeValid ) {
            altitude = positionSource.position.coordinate.altitude.toFixed(3)
            if ( !isNaN( parseFloat( antennaHeight ) ) ) {
              altitude += '<font color="#2f2f2f"><i>%1</i></font>'.arg((antennaHeight > 0 ? "+" : "") + Math.abs(antennaHeight).toFixed(2))
            }
          }
          else
            altitude = qsTr( "N/A" )
          altitude = qsTr( "Altitude" ) + ': ' + altitude
          return altitude
        }
      }
    }

    Rectangle {
      height: rowHeight
      width: grid.cellWidth
      color: grid.rows === 2 ? Theme.lightGray : "white"

      Text {
        anchors.margins:  10*dp
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        font: Theme.defaultFont
        text: qsTr( "Speed" ) + ': ' + ( positionSource.position.speedValid ? positionSource.position.speed.toFixed(3) + " m/s" : qsTr( "N/A" ) )
      }
    }

    Rectangle {
      height: rowHeight
      width: grid.cellWidth
      color: Theme.lightGray

      Text {
        anchors.margins:  10*dp
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        font: Theme.defaultFont
        text: qsTr( "H. Accuracy" ) + ': ' + ( positionSource.position.horizontalAccuracyValid ? positionSource.position.horizontalAccuracy.toFixed(3) + " m" : qsTr( "N/A" ) )
      }
    }

    Rectangle {
      height: rowHeight
      width: grid.cellWidth
      color: "white"

      Text {
        anchors.margins:  10*dp
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        font: Theme.defaultFont
        text: qsTr( "V. Accuracy" ) + ': ' + ( positionSource.position.verticalAccuracyValid ? positionSource.position.verticalAccuracy.toFixed(3) + " m" : qsTr( "N/A" ) )
      }
    }
  }
}
