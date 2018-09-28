import QtQuick 2.0
import QtPositioning 5.3
import org.qfield 1.0

PositionSource {
  id: positionSource

  property alias destinationCrs: _ct.destinationCrs
  property alias projectedPosition: _ct.projectedPosition

  property CoordinateTransformer ct: CoordinateTransformer {
    id: _ct
    sourceCrs: CrsFactory.fromEpsgId(4326)
    sourcePosition: Utils.coordinateToPoint(_pos.coordinate)
    transformContext: qgisProject.transformContext

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
