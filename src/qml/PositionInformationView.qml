import QtQuick 2.11
import QtQuick.Controls 1.4
import QtPositioning 5.8
import QtQuick.Layouts 1.3
import org.qgis 1.0
import org.qfield 1.0
import Utils 1.0

Rectangle {
  id: positionInformationView
  property PositionSource positionSource
  property alias crs: _ct.destinationCrs
  property double rowHeight: 30*dp
  border.color: "darkslategrey"
  border.width: 1*dp
  color: "yellow"

  CoordinateTransformer {
    id: _ct
    sourceCrs: CrsFactory.fromEpsgId(4326)
    sourcePosition: Utils.coordinateToPoint(positionSource.position.coordinate)
    transformContext: qgisProject.transformContext
  }

  height: grid.rows * positionInformationView.rowHeight + 2 * border.width
  width: parent.width
  anchors.margins: 20

  Grid {
    id: grid
    flow: GridLayout.TopToBottom
    rows: parent.width > 800*dp ? 1: 2
    width: parent.width - 2 * parent.border.width
    padding: parent.border.width
    property double cellWidth: grid.width / ( 3* ( grid.rows === 1 ? 2 : 1 ) )

    Rectangle {
      id: x
      height: rowHeight
      width: grid.cellWidth
      color: "#f2f2f2"

      Text {
        anchors.margins:  10*dp
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left

        text: crs.isGeographic ?
                  qsTr( "Lat." ) + ': ' + ( positionSource.position.latitudeValid  ? Number( _ct.projectedPosition.y ).toLocaleString( Qt.locale(), 'f', 3 ) : qsTr( "N/A" ) )
                : qsTr( "X" )    + ': ' + ( positionSource.position.longitudeValid ? Number( _ct.projectedPosition.x ).toLocaleString( Qt.locale(), 'f', 3 ) : qsTr( "N/A" ) )
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
        text: crs.isGeographic ?
                  qsTr( "Lon." ) + ': ' + ( positionSource.position.longitudeValid ? Number( _ct.projectedPosition.x ).toLocaleString( Qt.locale(), 'f', 3 ) : qsTr( "N/A" ) )
                : qsTr( "Y" )    + ': ' + ( positionSource.position.latitudeValid  ? Number( _ct.projectedPosition.y ).toLocaleString( Qt.locale(), 'f', 3 ) : qsTr( "N/A" ) )

      }
    }

    Rectangle {
      height: rowHeight
      width: grid.cellWidth
      color: grid.rows === 2 ? "white" : "#f2f2f2"

      Text {
        anchors.margins:  10*dp
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        text: qsTr( "Altitude" ) + ': ' + ( positionSource.position.altitudeValid ? positionSource.position.coordinate.altitude.toFixed(3) : qsTr( "N/A" ) )
      }
    }

    Rectangle {
      height: rowHeight
      width: grid.cellWidth
      color: grid.rows === 2 ? "#f2f2f2" : "white"

      Text {
        anchors.margins:  10*dp
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        text: "name: " + positionSource.name //qsTr( "Speed" ) + ': ' + ( positionSource.position.speedValid ? positionSource.position.speed.toFixed(3) + " m/s" : qsTr( "N/A" ) )
      }
    }

    Rectangle {
      height: rowHeight
      width: grid.cellWidth
      color: "#f2f2f2"

      Text {
        anchors.margins:  10*dp
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
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
        text: qsTr( "V. Accuracy" ) + ': ' + ( positionSource.position.verticalAccuracyValid ? positionSource.position.verticalAccuracy.toFixed(3) + " m" : qsTr( "N/A" ) )
      }
    }
  }
}
