

# File QfPositioningSettings.qml

[**File List**](files.md) **>** [**app**](dir_5194e5fea318fda12687127c23f8aba1.md) **>** [**qml**](dir_408f7dc2ccecf14179dc3d37678eaa5b.md) **>** [**QfPositioningSettings.qml**](QfPositioningSettings_8qml.md)

[Go to the documentation of this file](QfPositioningSettings_8qml.md)


```C++
import QtCore
import org.qfield.core
import org.qfield.gui

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
  property int elevationCorrectionMode: QfPositioning.ElevationCorrectionMode.None
  property bool logging: false

  property bool showPositionInformation: false

  property bool enableNtrip: false
  property var ntripSettings: ({})

  property bool alwaysShowPreciseView: false
  property real preciseViewPrecision: 2.5
  property bool preciseViewProximityAlarm: true

  property bool preciseViewAutoRotate: false
  property int preciseViewRotationSource: QfPositioningPreciseView.RotationSource.Compass

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

  property int positionFollowMode: QfPositioningSettings.FollowMode.PositionOnly
}
```


