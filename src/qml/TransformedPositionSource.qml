import QtQuick 2.12
import QtPositioning 5.3
//nmea-todo: import QtBluetooth 5.12

import org.qfield 1.0
import org.qgis 1.0
import Utils 1.0

Item{
    id: positionSource

    property var positionInfo //GnssPositionInformation
    property alias destinationCrs: _ct.destinationCrs
    property alias projectedPosition: _ct.projectedPosition
    property real projectedHorizontalAccuracy: positionInfo.haccValid && destinationCrs.mapUnits !== QgsUnitTypes.DistanceUnknownUnit ? positionInfo.hacc * Utils.distanceFromUnitToUnitFactor( QgsUnitTypes.DistanceMeters, destinationCrs.mapUnits ) : 0.0
    property alias deltaZ: _ct.deltaZ
    property alias skipAltitudeTransformation: _ct.skipAltitudeTransformation

    // this sets as well the mode (internal/bluetooth)
    property string device

    // proxy variables
    property bool active
    property string name
    property bool valid

    property CoordinateTransformer ct: CoordinateTransformer {
      id: _ct
      sourceCrs: CrsFactory.fromEpsgId(4326)
      transformContext: qgisProject.transformContext
    }

    onPositionInfoChanged: {
        _ct.sourcePosition = Utils.coordinateToPoint(QtPositioning.coordinate( positionInfo.latitude, positionInfo.longitude, positionInfo.elevation ) )
    }

    PositionSource {
      id: qtPositionSource

      active: device === 'internal' && positionSource.active

      preferredPositioningMethods: PositionSource.AllPositioningMethods

      onValidChanged: {
          if( active ) {
              positionSource.valid = valid
          }
      }

      onActiveChanged: {
          if( active ){
              positionSource.name = name
              console.log( "me, position source, is active")
          }else{
              console.log( "oh! me, position source, is NOT active")
          }
      }

      onPositionChanged: {
        positionSource.positionInfo = bluetoothPositionSource.fromQGeoPositionInfo(name)
      }
    }

    BluetoothReceiver {
      id: bluetoothPositionSource

      property bool active: device !== 'internal' && positionSource.active

      onSocketStateChanged: {
          //if( socketState === BluetoothSocket.Connected ) {
          //    displayToast( "It's connected ")
          //    positionSource.valid = true
          //} else {
          //    displayToast( "It's not connected ")
          //    positionSource.valid = false
          //}
      }

      onActiveChanged: {
          if( active ){
              positionSource.name = device
              console.log( "me, bluetooth source, is active with device "+device)
              connectDevice(device)
          }else{
              disconnectDevice()
              console.log( "oh! me, bluetooth source, is NOT active")
          }
      }

      onLastGnssPositionInformationChanged:
      {
        positionSource.positionInfo = lastGnssPositionInformation
      }
    }
}
