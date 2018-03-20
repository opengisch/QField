import QtQuick 2.0
import QtPositioning 5.3
import org.qfield 1.0
import QgisQuick 0.1 as QgsQuick

PositionSource {
  id: positionSource

  property alias destinationCrs: _ct.destinationCrs
  property alias projectedPosition: _ct.projectedPosition
  property alias mapSettings: _ct.mapSettings

  property QgsQuick.CoordinateTransformer ct: QgsQuick.CoordinateTransformer {
    id: _ct
    sourceCrs: QgsQuick.Utils.coordinateReferenceSystemFromEpsgId(4326)
    sourcePosition: QgsQuick.Utils.coordinateToPoint(_pos.coordinate)

    property Position _pos: positionSource.position
  }
  // TODO:::: remove this block
  /*
  property Timer tm: Timer {
    interval: 500;
    running: true;
    repeat: true;

    onTriggered: {
      var coord = positionSource.position.coordinate;
      coord.latitude = 46.9483+ Math.random()/10;
      coord.longitude = 7.44225+ Math.random()/10;
      _ct.sourcePosition = coord;
    }
  }
  */
  // END TODO:::: remove this block
}
