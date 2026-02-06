import QtCore
import org.qfield

/**
 * \ingroup qml
 */
Settings {
  enum FollowMode {
    PositionOnly,
    PositionAndCompass,
    PositionAndDirection
  }

  property bool positioningActivated: false
  property bool positioningCoordinateLock: false

  property string positioningDevice: ""
  property string positioningDeviceName: qsTr("Internal device")
  property int elevationCorrectionMode: Positioning.ElevationCorrectionMode.None
  property bool logging: false

  property bool showPositionInformation: false

  property bool enableNtripClient: false
  property bool ntripSendNmea: false
  property string ntripHost: ""
  property int ntripPort: 2101
  property string ntripMountpoint: ""
  property string ntripUsername: ""
  property string ntripPassword: ""
  property string ntripStatus: ""
  property int ntripBytesSent: 0
  property int ntripBytesReceived: 0

  property bool alwaysShowPreciseView: false
  property real preciseViewPrecision: 2.5
  property bool preciseViewProximityAlarm: true

  property bool accuracyIndicator: false
  property real accuracyBad: 5.0
  property real accuracyExcellent: 1.0
  property bool accuracyRequirement: false

  property bool averagedPositioning: false
  property int averagedPositioningMinimumCount: 1
  property bool averagedPositioningAutomaticStop: true

  property real antennaHeight: 0.0
  property bool antennaHeightActivated: false
  property bool skipAltitudeCorrection: false

  property string verticalGrid: ""

  property bool trackerTimeIntervalConstraint: false
  property double trackerTimeInterval: 30
  property bool trackerMinimumDistanceConstraint: false
  property double trackerMinimumDistance: 30
  property bool trackerSensorCaptureConstraint: false
  property bool trackerMeetAllConstraints: false

  property bool trackerErroneousDistanceSafeguard: false
  property double trackerErroneousDistance: 100

  property int trackerMeasureType: 0
  property int digitizingMeasureType: 1

  property bool geofencingPreventDigitizingDuringAlert: false
  property bool egenioussEnabled: false

  property int positionFollowMode: PositioningSettings.FollowMode.PositionOnly
}
