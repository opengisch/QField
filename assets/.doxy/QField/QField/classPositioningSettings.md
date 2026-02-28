

# Class PositioningSettings



[**ClassList**](annotated.md) **>** [**PositioningSettings**](classPositioningSettings.md)








Inherits the following classes: [Settings](classSettings.md)














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**FollowMode**](#enum-followmode)  <br> |
























## Public Properties

| Type | Name |
| ---: | :--- |
| property real | [**accuracyBad**](classPositioningSettings.md#property-accuracybad)  <br> |
| property real | [**accuracyExcellent**](classPositioningSettings.md#property-accuracyexcellent)  <br> |
| property bool | [**accuracyIndicator**](classPositioningSettings.md#property-accuracyindicator)  <br> |
| property bool | [**accuracyRequirement**](classPositioningSettings.md#property-accuracyrequirement)  <br> |
| property bool | [**alwaysShowPreciseView**](classPositioningSettings.md#property-alwaysshowpreciseview)  <br> |
| property real | [**antennaHeight**](classPositioningSettings.md#property-antennaheight)  <br> |
| property bool | [**antennaHeightActivated**](classPositioningSettings.md#property-antennaheightactivated)  <br> |
| property bool | [**averagedPositioning**](classPositioningSettings.md#property-averagedpositioning)  <br> |
| property bool | [**averagedPositioningAutomaticStop**](classPositioningSettings.md#property-averagedpositioningautomaticstop)  <br> |
| property int | [**averagedPositioningMinimumCount**](classPositioningSettings.md#property-averagedpositioningminimumcount)  <br> |
| property int | [**digitizingMeasureType**](classPositioningSettings.md#property-digitizingmeasuretype)  <br> |
| property bool | [**egenioussEnabled**](classPositioningSettings.md#property-egenioussenabled)  <br> |
| property int | [**elevationCorrectionMode**](classPositioningSettings.md#property-elevationcorrectionmode)  <br> |
| property bool | [**geofencingPreventDigitizingDuringAlert**](classPositioningSettings.md#property-geofencingpreventdigitizingduringalert)  <br> |
| property bool | [**logging**](classPositioningSettings.md#property-logging)  <br> |
| property int | [**positionFollowMode**](classPositioningSettings.md#property-positionfollowmode)  <br> |
| property bool | [**positioningActivated**](classPositioningSettings.md#property-positioningactivated)  <br> |
| property bool | [**positioningCoordinateLock**](classPositioningSettings.md#property-positioningcoordinatelock)  <br> |
| property string | [**positioningDevice**](classPositioningSettings.md#property-positioningdevice)  <br> |
| property string | [**positioningDeviceName**](classPositioningSettings.md#property-positioningdevicename)  <br> |
| property real | [**preciseViewPrecision**](classPositioningSettings.md#property-preciseviewprecision)  <br> |
| property bool | [**preciseViewProximityAlarm**](classPositioningSettings.md#property-preciseviewproximityalarm)  <br> |
| property bool | [**showPositionInformation**](classPositioningSettings.md#property-showpositioninformation)  <br> |
| property bool | [**skipAltitudeCorrection**](classPositioningSettings.md#property-skipaltitudecorrection)  <br> |
| property double | [**trackerErroneousDistance**](classPositioningSettings.md#property-trackererroneousdistance)  <br> |
| property bool | [**trackerErroneousDistanceSafeguard**](classPositioningSettings.md#property-trackererroneousdistancesafeguard)  <br> |
| property int | [**trackerMeasureType**](classPositioningSettings.md#property-trackermeasuretype)  <br> |
| property bool | [**trackerMeetAllConstraints**](classPositioningSettings.md#property-trackermeetallconstraints)  <br> |
| property double | [**trackerMinimumDistance**](classPositioningSettings.md#property-trackerminimumdistance)  <br> |
| property bool | [**trackerMinimumDistanceConstraint**](classPositioningSettings.md#property-trackerminimumdistanceconstraint)  <br> |
| property bool | [**trackerSensorCaptureConstraint**](classPositioningSettings.md#property-trackersensorcaptureconstraint)  <br> |
| property double | [**trackerTimeInterval**](classPositioningSettings.md#property-trackertimeinterval)  <br> |
| property bool | [**trackerTimeIntervalConstraint**](classPositioningSettings.md#property-trackertimeintervalconstraint)  <br> |
| property string | [**verticalGrid**](classPositioningSettings.md#property-verticalgrid)  <br> |










## Public Signals inherited from Settings

See [Settings](classSettings.md)

| Type | Name |
| ---: | :--- |
| signal void | [**settingChanged**](classSettings.md#signal-settingchanged) (const QString & key) <br> |








## Public Functions inherited from Settings

See [Settings](classSettings.md)

| Type | Name |
| ---: | :--- |
|   | [**Settings**](classSettings.md#function-settings) (QObject \* parent=nullptr) <br> |
|  Q\_INVOKABLE void | [**remove**](classSettings.md#function-remove) (const QString & key) <br> |
|  Q\_INVOKABLE void | [**setValue**](classSettings.md#function-setvalue) (const QString & key, const QVariant & value) <br> |
|  Q\_INVOKABLE void | [**sync**](classSettings.md#function-sync) () <br> |
|  Q\_INVOKABLE QVariant | [**value**](classSettings.md#function-value) (const QString & key, const QVariant & defaultValue) <br> |
|  Q\_INVOKABLE bool | [**valueBool**](classSettings.md#function-valuebool) (const QString & key, bool defaultValue) <br> |
|  Q\_INVOKABLE int | [**valueInt**](classSettings.md#function-valueint) (const QString & key, int defaultValue) <br> |






















































## Public Types Documentation




### enum FollowMode 

```C++
enum PositioningSettings::FollowMode {
    PositionOnly,
    PositionAndCompass,
    PositionAndDirection
};
```




<hr>
## Public Properties Documentation




### property accuracyBad 

```C++
real PositioningSettings::accuracyBad;
```




<hr>



### property accuracyExcellent 

```C++
real PositioningSettings::accuracyExcellent;
```




<hr>



### property accuracyIndicator 

```C++
bool PositioningSettings::accuracyIndicator;
```




<hr>



### property accuracyRequirement 

```C++
bool PositioningSettings::accuracyRequirement;
```




<hr>



### property alwaysShowPreciseView 

```C++
bool PositioningSettings::alwaysShowPreciseView;
```




<hr>



### property antennaHeight 

```C++
real PositioningSettings::antennaHeight;
```




<hr>



### property antennaHeightActivated 

```C++
bool PositioningSettings::antennaHeightActivated;
```




<hr>



### property averagedPositioning 

```C++
bool PositioningSettings::averagedPositioning;
```




<hr>



### property averagedPositioningAutomaticStop 

```C++
bool PositioningSettings::averagedPositioningAutomaticStop;
```




<hr>



### property averagedPositioningMinimumCount 

```C++
int PositioningSettings::averagedPositioningMinimumCount;
```




<hr>



### property digitizingMeasureType 

```C++
int PositioningSettings::digitizingMeasureType;
```




<hr>



### property egenioussEnabled 

```C++
bool PositioningSettings::egenioussEnabled;
```




<hr>



### property elevationCorrectionMode 

```C++
int PositioningSettings::elevationCorrectionMode;
```




<hr>



### property geofencingPreventDigitizingDuringAlert 

```C++
bool PositioningSettings::geofencingPreventDigitizingDuringAlert;
```




<hr>



### property logging 

```C++
bool PositioningSettings::logging;
```




<hr>



### property positionFollowMode 

```C++
int PositioningSettings::positionFollowMode;
```




<hr>



### property positioningActivated 

```C++
bool PositioningSettings::positioningActivated;
```




<hr>



### property positioningCoordinateLock 

```C++
bool PositioningSettings::positioningCoordinateLock;
```




<hr>



### property positioningDevice 

```C++
string PositioningSettings::positioningDevice;
```




<hr>



### property positioningDeviceName 

```C++
string PositioningSettings::positioningDeviceName;
```




<hr>



### property preciseViewPrecision 

```C++
real PositioningSettings::preciseViewPrecision;
```




<hr>



### property preciseViewProximityAlarm 

```C++
bool PositioningSettings::preciseViewProximityAlarm;
```




<hr>



### property showPositionInformation 

```C++
bool PositioningSettings::showPositionInformation;
```




<hr>



### property skipAltitudeCorrection 

```C++
bool PositioningSettings::skipAltitudeCorrection;
```




<hr>



### property trackerErroneousDistance 

```C++
double PositioningSettings::trackerErroneousDistance;
```




<hr>



### property trackerErroneousDistanceSafeguard 

```C++
bool PositioningSettings::trackerErroneousDistanceSafeguard;
```




<hr>



### property trackerMeasureType 

```C++
int PositioningSettings::trackerMeasureType;
```




<hr>



### property trackerMeetAllConstraints 

```C++
bool PositioningSettings::trackerMeetAllConstraints;
```




<hr>



### property trackerMinimumDistance 

```C++
double PositioningSettings::trackerMinimumDistance;
```




<hr>



### property trackerMinimumDistanceConstraint 

```C++
bool PositioningSettings::trackerMinimumDistanceConstraint;
```




<hr>



### property trackerSensorCaptureConstraint 

```C++
bool PositioningSettings::trackerSensorCaptureConstraint;
```




<hr>



### property trackerTimeInterval 

```C++
double PositioningSettings::trackerTimeInterval;
```




<hr>



### property trackerTimeIntervalConstraint 

```C++
bool PositioningSettings::trackerTimeIntervalConstraint;
```




<hr>



### property verticalGrid 

```C++
string PositioningSettings::verticalGrid;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/qml/PositioningSettings.qml`

