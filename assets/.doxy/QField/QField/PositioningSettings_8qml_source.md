

# File PositioningSettings.qml

[**File List**](files.md) **>** [**qml**](dir_1d81f7f4362e4398c0a695c4e466fcb0.md) **>** [**PositioningSettings.qml**](PositioningSettings_8qml.md)

[Go to the documentation of this file](PositioningSettings_8qml.md)


```C++
import QtCore
import org.qfield

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
```


