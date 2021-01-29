import QtQuick 2.12
import QtPositioning 5.3
import QtBluetooth 5.14

import org.qfield 1.0
import org.qgis 1.0

import Utils 1.0

Item{
    id: positionSource

    // GnssPositionInformation object
    property var positionInfo

    property alias destinationCrs: _ct.destinationCrs
    property alias projectedPosition: _ct.projectedPosition
    property real projectedHorizontalAccuracy: positionInfo && positionInfo.haccValid && destinationCrs.mapUnits !== QgsUnitTypes.DistanceUnknownUnit ? positionInfo.hacc * Utils.distanceFromUnitToUnitFactor( QgsUnitTypes.DistanceMeters, destinationCrs.mapUnits ) : 0.0
    property alias deltaZ: _ct.deltaZ
    property alias skipAltitudeTransformation: _ct.skipAltitudeTransformation

    // this sets as well the mode (empty is internal, otherwise bluetooth)
    property string device: ''

    // proxy variables
    property bool active: false
    property string name: ''
    property bool valid: qtPositionSource.valid || bluetoothPositionSource.valid
    property alias bluetoothSocketState: bluetoothPositionSource.socketState
    property bool currentness: false

    property CoordinateTransformer ct: CoordinateTransformer {
        id: _ct
        sourceCrs: CrsFactory.fromEpsgId(4326)
        transformContext: qgisProject.transformContext
    }

    onPositionInfoChanged: {
        _ct.sourcePosition = Utils.coordinateToPoint(QtPositioning.coordinate( positionInfo.latitude, positionInfo.longitude, positionInfo.elevation ) )
    }

    Timer {
        id: timer
        interval: 1000
        repeat: true
        running: true
        triggeredOnStart: true
        onTriggered: {
            if ( positionSource.positionInfo )
              currentness = ( ( new Date() - positionSource.positionInfo.utcDateTime ) / 1000 ) < 30
        }
    }

    onActiveChanged: {
        if (active) {
            connectBluetoothSource()
        } else {
            bluetoothPositionSource.disconnectDevice()
        }
    }

    onDeviceChanged: {
        connectBluetoothSource()
    }

    function connectBluetoothSource() {
        if( active && device !== '' ) {
            positionSource.name = device
            bluetoothPositionSource.connectDevice(device)
        }
    }

    PositionSource {
        id: qtPositionSource

        active: device === '' && positionSource.active

        preferredPositioningMethods: PositionSource.AllPositioningMethods

        onActiveChanged: {
            if( active )
            {
                positionSource.name = name
            }
        }

        onPositionChanged: {
            if (!bluetoothPositionSource.valid) {
              positionSource.positionInfo = bluetoothPositionSource.createGnssPositionInformation(
                        qtPositionSource.position.coordinate.latitude,
                        qtPositionSource.position.coordinate.longitude,
                        qtPositionSource.position.coordinate.altitude,
                        qtPositionSource.position.speed,
                        qtPositionSource.position.direction,
                        qtPositionSource.position.horizontalAccuracy,
                        qtPositionSource.position.verticalAccuracy,
                        qtPositionSource.position.verticalSpeed,
                        qtPositionSource.position.magneticVariation,
                        qtPositionSource.position.timestamp,
                        qtPositionSource.name)
            }
        }
    }

    BluetoothReceiver {
        id: bluetoothPositionSource
        ellipsoidalElevation: positioningSettings.ellipsoidalElevation

        property bool active: device !== '' && positionSource.active
        property bool valid: socketState === BluetoothSocket.Connected

        onSocketStateChanged: {
            displayToast( socketStateString )
        }

        onActiveChanged: {
            if( !active )
            {
                disconnectDevice()
            }
        }

        onLastGnssPositionInformationChanged: {
            if (valid) {
               positionSource.positionInfo = lastGnssPositionInformation
            }
        }
    }
}
