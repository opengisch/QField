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

  height: grid.rows * positionInformationView.rowHeight
  width: parent.width
  anchors.margins: 20

  Grid {
    id: grid
    flow: GridLayout.TopToBottom
    rows: parent.width > 1000*dp ? 1 : parent.width > 620*dp ? 2 : 3
    width: parent.width
    property double cellWidth: grid.width / ( 6 / grid.rows )

    Rectangle {
      id: x
      height: rowHeight
      width: grid.cellWidth
      color: "#e6f2fd"

      Text {
        anchors.margins:  10*dp
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        font: Theme.tipFont
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
        font: Theme.tipFont
        text: positionSource.destinationCrs.isGeographic ?
                  qsTr( "Lon." ) + ': ' + ( positionSource.position.longitudeValid ? Number( positionSource.projectedPosition.x ).toLocaleString( Qt.locale(), 'f', 3 ) : qsTr( "N/A" ) )
                : qsTr( "Y" )    + ': ' + ( positionSource.position.latitudeValid  ? Number( positionSource.projectedPosition.y ).toLocaleString( Qt.locale(), 'f', 3 ) : qsTr( "N/A" ) )

      }
    }

    Rectangle {
      height: rowHeight
      width: grid.cellWidth
      color: grid.rows === 2 ? "white" : "#e6f2fd"

      Text {
        anchors.margins:  10*dp
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        font: Theme.tipFont
        text: {
          var altitude
          if ( positionSource.position.altitudeValid ) {
            altitude = Number( positionSource.projectedPosition.z ).toLocaleString( Qt.locale(), 'f', 3 )
            if ( !isNaN( parseFloat( antennaHeight ) ) ) {
              altitude += '<font color="#2f2f2f"><i>(%1)</i></font>'.arg((antennaHeight > 0 ? "+" : "") + Math.abs(antennaHeight).toFixed(2))
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
      color: grid.rows === 2 ? "#e6f2fd" : "white"

      Text {
        anchors.margins:  10*dp
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        font: Theme.tipFont
        text: qsTr( "Speed" ) + ': ' + ( positionSource.position.speedValid ? positionSource.position.speed.toFixed(3) + " m/s" : qsTr( "N/A" ) )
      }
    }

    Rectangle {
      height: rowHeight
      width: grid.cellWidth
      color: "#e6f2fd"

      Text {
        anchors.margins:  10*dp
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        font: Theme.tipFont
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
        font: Theme.tipFont
        text: qsTr( "V. Accuracy" ) + ': ' + ( positionSource.position.verticalAccuracyValid ? positionSource.position.verticalAccuracy.toFixed(3) + " m" : qsTr( "N/A" ) )
      }
    }
  }
}
