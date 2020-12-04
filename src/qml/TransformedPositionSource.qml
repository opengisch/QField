import QtQuick 2.12
import QtPositioning 5.3
//nmea-todo: import QtBluetooth 5.15

import org.qfield 1.0
import org.qgis 1.0
import Utils 1.0

Item{
    id: positionSource

    //by source generated properties
    property var positionInfo //ehemals position //as object would be GnssPositionInformation
    property alias destinationCrs: _ct.destinationCrs
    property alias projectedPosition: _ct.projectedPosition
    property real projectedHorizontalAccuracy: positionInfo.haccValid && destinationCrs.mapUnits !== QgsUnitTypes.DistanceUnknownUnit ? positionInfo.hacc * Utils.distanceFromUnitToUnitFactor( QgsUnitTypes.DistanceMeters, destinationCrs.mapUnits ) : 0.0
    property alias deltaZ: _ct.deltaZ
    property alias skipAltitudeTransformation: _ct.skipAltitudeTransformation

    property bool active

    // HUDSUCKER PROXY
    property alias preferredPositioningMethods: qtPositionSource.preferredPositioningMethods //todo
    property alias name: qtPositionSource.name //todo
    property bool valid: true //todo

    property string device

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

      onValidChanged: {
          if( active ) {
              positionSource.valid = valid
          }
      }

      onActiveChanged: {
          if( active ){
              console.log( "me, position source, is active")
          }else{

              console.log( "oh! me, position source, is NOT active")
          }
      }

      onPositionChanged: {
          position.latitudeValid
        positionInfo = GnssPositionConverter.fromQGeoPositionInfo(name) //pass the correct here...
      }
    }

    BluetoothReceiver {
      id: bluetoothPositionSource

      property bool active: device !== 'internal' && positionSource.active

      onSocketStateChanged: {
          //nmea-todo:if( socketState === BluetoothSocket.Connected ) {
          //nmea-todo:    displayToast( "It's connected ")
          //nmea-todo:    positionSource.valid = true
          //nmea-todo:} else {
          //nmea-todo:    displayToast( "It's not connected ")
          //nmea-todo:    positionSource.valid = false
          //nmea-todo:}
      }

      onActiveChanged: {
          if( active ){
              console.log( "me, bluetooth source, is active with device "+device)
              connectDevice(device)
          }else{
              disconnectDevice()
              console.log( "oh! me, bluetooth source, is NOT active")
          }
      }

      //implement this in c as proerty: property GnssPositionInformation positionInfo
      //property string name: "bluetooth device" //todo
      //property var preferredPositioningMethods //unused maybe todo

      onLastGnssPositionInformationChanged:
      {
        positionInfo = lastGnssPositionInformation
      }
    }
}
