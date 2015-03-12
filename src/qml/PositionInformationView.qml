import QtQuick 2.0
import QtQuick.Controls 1.2
import QtPositioning 5.3
import QtQuick.Layouts 1.1

Item {
  property PositionSource positionSource

  width: 300
  height: 300

  GridLayout {
    columns: 2

    Text {
      text: qsTr( "Altitude" )
    }
    Text {
      text: positionSource.position.altitudeValid ? positionSource.position.coordinate.altitude.toFixed(3) : qsTr( "N/A" )
    }
    Text {
      text: qsTr( "Accuracy" )
    }
    Text {
      text: positionSource.position.horizontalAccuracyValid ? positionSource.position.horizontalAccuracy.toFixed(3) + " m" : qsTr( "N/A" )
    }
    Text {
      text: qsTr( "Coordinate" )
    }
    Text {
      text: positionSource.position.latitudeValid ? positionSource.position.coordinate.latitude.toFixed(3) + " / " + positionSource.position.coordinate.longitude.toFixed(3) : qsTr( "N/A" )
    }
    Text {
      text: qsTr( "Speed" )
    }
    Text {
      text: positionSource.position.speedValid ? positionSource.position.speed.toFixed(3) + " m/s" : qsTr( "N/A" )
    }
  }
}
