

# Class QfPositioningSettings



[**ClassList**](annotated.md) **>** [**QfPositioningSettings**](classQfPositioningSettings.md)








Inherits the following classes: Settings














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**FollowMode**](#enum-followmode)  <br> |












## Public Properties

| Type | Name |
| ---: | :--- |
| property real | [**accuracyBad**](classQfPositioningSettings.md#property-accuracybad)  <br> |
| property real | [**accuracyExcellent**](classQfPositioningSettings.md#property-accuracyexcellent)  <br> |
| property bool | [**accuracyIndicator**](classQfPositioningSettings.md#property-accuracyindicator)  <br> |
| property bool | [**accuracyRequirement**](classQfPositioningSettings.md#property-accuracyrequirement)  <br> |
| property bool | [**alwaysShowPreciseView**](classQfPositioningSettings.md#property-alwaysshowpreciseview)  <br> |
| property real | [**antennaHeight**](classQfPositioningSettings.md#property-antennaheight)  <br> |
| property bool | [**antennaHeightActivated**](classQfPositioningSettings.md#property-antennaheightactivated)  <br> |
| property bool | [**averagedPositioning**](classQfPositioningSettings.md#property-averagedpositioning)  <br> |
| property bool | [**averagedPositioningAutomaticStop**](classQfPositioningSettings.md#property-averagedpositioningautomaticstop)  <br> |
| property int | [**averagedPositioningMinimumCount**](classQfPositioningSettings.md#property-averagedpositioningminimumcount)  <br> |
| property int | [**digitizingMeasureType**](classQfPositioningSettings.md#property-digitizingmeasuretype)  <br> |
| property bool | [**egenioussEnabled**](classQfPositioningSettings.md#property-egenioussenabled)  <br> |
| property int | [**elevationCorrectionMode**](classQfPositioningSettings.md#property-elevationcorrectionmode)  <br> |
| property bool | [**enableNtrip**](classQfPositioningSettings.md#property-enablentrip)  <br> |
| property bool | [**geofencingPreventDigitizingDuringAlert**](classQfPositioningSettings.md#property-geofencingpreventdigitizingduringalert)  <br> |
| property bool | [**logging**](classQfPositioningSettings.md#property-logging)  <br> |
| property var | [**ntripSettings**](classQfPositioningSettings.md#property-ntripsettings)  <br> |
| property int | [**positionFollowMode**](classQfPositioningSettings.md#property-positionfollowmode)  <br> |
| property bool | [**positioningActivated**](classQfPositioningSettings.md#property-positioningactivated)  <br> |
| property bool | [**positioningCoordinateLock**](classQfPositioningSettings.md#property-positioningcoordinatelock)  <br> |
| property string | [**positioningDevice**](classQfPositioningSettings.md#property-positioningdevice)  <br> |
| property string | [**positioningDeviceName**](classQfPositioningSettings.md#property-positioningdevicename)  <br> |
| property bool | [**preciseViewAutoRotate**](classQfPositioningSettings.md#property-preciseviewautorotate)  <br> |
| property real | [**preciseViewPrecision**](classQfPositioningSettings.md#property-preciseviewprecision)  <br> |
| property bool | [**preciseViewProximityAlarm**](classQfPositioningSettings.md#property-preciseviewproximityalarm)  <br> |
| property int | [**preciseViewRotationSource**](classQfPositioningSettings.md#property-preciseviewrotationsource)  <br> |
| property bool | [**showPositionInformation**](classQfPositioningSettings.md#property-showpositioninformation)  <br> |
| property bool | [**skipAltitudeCorrection**](classQfPositioningSettings.md#property-skipaltitudecorrection)  <br> |
| property double | [**trackerErroneousDistance**](classQfPositioningSettings.md#property-trackererroneousdistance)  <br> |
| property bool | [**trackerErroneousDistanceSafeguard**](classQfPositioningSettings.md#property-trackererroneousdistancesafeguard)  <br> |
| property int | [**trackerMeasureType**](classQfPositioningSettings.md#property-trackermeasuretype)  <br> |
| property bool | [**trackerMeetAllConstraints**](classQfPositioningSettings.md#property-trackermeetallconstraints)  <br> |
| property double | [**trackerMinimumDistance**](classQfPositioningSettings.md#property-trackerminimumdistance)  <br> |
| property bool | [**trackerMinimumDistanceConstraint**](classQfPositioningSettings.md#property-trackerminimumdistanceconstraint)  <br> |
| property bool | [**trackerSensorCaptureConstraint**](classQfPositioningSettings.md#property-trackersensorcaptureconstraint)  <br> |
| property double | [**trackerTimeInterval**](classQfPositioningSettings.md#property-trackertimeinterval)  <br> |
| property bool | [**trackerTimeIntervalConstraint**](classQfPositioningSettings.md#property-trackertimeintervalconstraint)  <br> |
| property string | [**verticalGrid**](classQfPositioningSettings.md#property-verticalgrid)  <br> |




































## Public Types Documentation




### enum FollowMode 

```C++
enum QfPositioningSettings::FollowMode {
    PositionOnly,
    PositionAndCompass,
    PositionAndDirection
};
```




<hr>
## Public Properties Documentation




### property accuracyBad 

```C++
real QfPositioningSettings::accuracyBad;
```




<hr>



### property accuracyExcellent 

```C++
real QfPositioningSettings::accuracyExcellent;
```




<hr>



### property accuracyIndicator 

```C++
bool QfPositioningSettings::accuracyIndicator;
```




<hr>



### property accuracyRequirement 

```C++
bool QfPositioningSettings::accuracyRequirement;
```




<hr>



### property alwaysShowPreciseView 

```C++
bool QfPositioningSettings::alwaysShowPreciseView;
```




<hr>



### property antennaHeight 

```C++
real QfPositioningSettings::antennaHeight;
```




<hr>



### property antennaHeightActivated 

```C++
bool QfPositioningSettings::antennaHeightActivated;
```




<hr>



### property averagedPositioning 

```C++
bool QfPositioningSettings::averagedPositioning;
```




<hr>



### property averagedPositioningAutomaticStop 

```C++
bool QfPositioningSettings::averagedPositioningAutomaticStop;
```




<hr>



### property averagedPositioningMinimumCount 

```C++
int QfPositioningSettings::averagedPositioningMinimumCount;
```




<hr>



### property digitizingMeasureType 

```C++
int QfPositioningSettings::digitizingMeasureType;
```




<hr>



### property egenioussEnabled 

```C++
bool QfPositioningSettings::egenioussEnabled;
```




<hr>



### property elevationCorrectionMode 

```C++
int QfPositioningSettings::elevationCorrectionMode;
```




<hr>



### property enableNtrip 

```C++
bool QfPositioningSettings::enableNtrip;
```




<hr>



### property geofencingPreventDigitizingDuringAlert 

```C++
bool QfPositioningSettings::geofencingPreventDigitizingDuringAlert;
```




<hr>



### property logging 

```C++
bool QfPositioningSettings::logging;
```




<hr>



### property ntripSettings 

```C++
var QfPositioningSettings::ntripSettings;
```




<hr>



### property positionFollowMode 

```C++
int QfPositioningSettings::positionFollowMode;
```




<hr>



### property positioningActivated 

```C++
bool QfPositioningSettings::positioningActivated;
```




<hr>



### property positioningCoordinateLock 

```C++
bool QfPositioningSettings::positioningCoordinateLock;
```




<hr>



### property positioningDevice 

```C++
string QfPositioningSettings::positioningDevice;
```




<hr>



### property positioningDeviceName 

```C++
string QfPositioningSettings::positioningDeviceName;
```




<hr>



### property preciseViewAutoRotate 

```C++
bool QfPositioningSettings::preciseViewAutoRotate;
```




<hr>



### property preciseViewPrecision 

```C++
real QfPositioningSettings::preciseViewPrecision;
```




<hr>



### property preciseViewProximityAlarm 

```C++
bool QfPositioningSettings::preciseViewProximityAlarm;
```




<hr>



### property preciseViewRotationSource 

```C++
int QfPositioningSettings::preciseViewRotationSource;
```




<hr>



### property showPositionInformation 

```C++
bool QfPositioningSettings::showPositionInformation;
```




<hr>



### property skipAltitudeCorrection 

```C++
bool QfPositioningSettings::skipAltitudeCorrection;
```




<hr>



### property trackerErroneousDistance 

```C++
double QfPositioningSettings::trackerErroneousDistance;
```




<hr>



### property trackerErroneousDistanceSafeguard 

```C++
bool QfPositioningSettings::trackerErroneousDistanceSafeguard;
```




<hr>



### property trackerMeasureType 

```C++
int QfPositioningSettings::trackerMeasureType;
```




<hr>



### property trackerMeetAllConstraints 

```C++
bool QfPositioningSettings::trackerMeetAllConstraints;
```




<hr>



### property trackerMinimumDistance 

```C++
double QfPositioningSettings::trackerMinimumDistance;
```




<hr>



### property trackerMinimumDistanceConstraint 

```C++
bool QfPositioningSettings::trackerMinimumDistanceConstraint;
```




<hr>



### property trackerSensorCaptureConstraint 

```C++
bool QfPositioningSettings::trackerSensorCaptureConstraint;
```




<hr>



### property trackerTimeInterval 

```C++
double QfPositioningSettings::trackerTimeInterval;
```




<hr>



### property trackerTimeIntervalConstraint 

```C++
bool QfPositioningSettings::trackerTimeIntervalConstraint;
```




<hr>



### property verticalGrid 

```C++
string QfPositioningSettings::verticalGrid;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/app/qml/QfPositioningSettings.qml`

