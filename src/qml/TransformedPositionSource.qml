import QtQuick 2.0
import QtPositioning 5.3
import org.qfield 1.0
import org.qgis 1.0
import Utils 1.0

PositionSource {
  id: positionSource
  property alias destinationCrs: _ct.destinationCrs
  property alias projectedPosition: _ct.projectedPosition
  property real projectedHorizontalAccuracy: if (positionSource.position.horizontalAccuracyValid && destinationCrs.mapUnits !== QgsUnitTypes.DistanceUnknownUnit) {
                                               positionSource.position.horizontalAccuracy * Utils.distanceFromUnitToUnitFactor( QgsUnitTypes.DistanceMeters, destinationCrs.mapUnits )
                                             } else {
                                               0.0
                                             }
  property alias deltaZ: _ct.deltaZ
  property alias skipAltitudeTransformation: _ct.skipAltitudeTransformation

  property CoordinateTransformer ct: CoordinateTransformer {
    id: _ct
    sourceCrs: CrsFactory.fromEpsgId(4326)
    transformContext: qgisProject.transformContext
  }

  onPositionChanged: {
    _ct.sourcePosition = Utils.coordinateToPoint(position.coordinate)
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
