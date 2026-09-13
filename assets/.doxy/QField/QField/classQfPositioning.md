

# Class QfPositioning



[**ClassList**](annotated.md) **>** [**QfPositioning**](classQfPositioning.md)



[More...](#detailed-description)

* `#include <qfpositioning.h>`



Inherits the following classes: QObject


























## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**active**](classQfPositioning.md#property-active-12)  <br> |
| property double | [**antennaHeight**](classQfPositioning.md#property-antennaheight-12)  <br> |
| property bool | [**averagedPosition**](classQfPositioning.md#property-averagedposition-12)  <br> |
| property int | [**averagedPositionCount**](classQfPositioning.md#property-averagedpositioncount-12)  <br> |
| property bool | [**averagedPositionFilterAccuracy**](classQfPositioning.md#property-averagedpositionfilteraccuracy-12)  <br> |
| property bool | [**backgroundMode**](classQfPositioning.md#property-backgroundmode-12)  <br> |
| property double | [**badAccuracyThreshold**](classQfPositioning.md#property-badaccuracythreshold-12)  <br> |
| property [**QgsQuickCoordinateTransformer**](classQgsQuickCoordinateTransformer.md) \* | [**coordinateTransformer**](classQfPositioning.md#property-coordinatetransformer-12)  <br> |
| property double | [**deviceBatteryLevel**](classQfPositioning.md#property-devicebatterylevel-12)  <br> |
| property QfAbstractGnssReceiver::Capabilities | [**deviceCapabilities**](classQfPositioning.md#property-devicecapabilities-12)  <br> |
| property QString | [**deviceId**](classQfPositioning.md#property-deviceid-12)  <br> |
| property QString | [**deviceLastError**](classQfPositioning.md#property-devicelasterror-12)  <br> |
| property QAbstractSocket::SocketState | [**deviceSocketState**](classQfPositioning.md#property-devicesocketstate-12)  <br> |
| property QString | [**deviceSocketStateString**](classQfPositioning.md#property-devicesocketstatestring-12)  <br> |
| property [**QfPositioningSource::ElevationCorrectionMode**](classQfPositioningSource.md#enum-elevationcorrectionmode) | [**elevationCorrectionMode**](classQfPositioning.md#property-elevationcorrectionmode-12)  <br> |
| property bool | [**enableNtrip**](classQfPositioning.md#property-enablentrip-12)  <br> |
| property double | [**excellentAccuracyThreshold**](classQfPositioning.md#property-excellentaccuracythreshold-12)  <br> |
| property bool | [**logging**](classQfPositioning.md#property-logging-12)  <br> |
| property QString | [**loggingPath**](classQfPositioning.md#property-loggingpath-12)  <br> |
| property qint64 | [**ntripBytesReceived**](classQfPositioning.md#property-ntripbytesreceived-12)  <br> |
| property qint64 | [**ntripBytesSent**](classQfPositioning.md#property-ntripbytessent-12)  <br> |
| property QDateTime | [**ntripLastBytesReceivedUtcDateTime**](classQfPositioning.md#property-ntriplastbytesreceivedutcdatetime-12)  <br> |
| property [**QfNtripSettings**](classQfNtripSettings.md) | [**ntripSettings**](classQfPositioning.md#property-ntripsettings-12)  <br> |
| property [**QfPositioningSource::NtripState**](classQfPositioningSource.md#enum-ntripstate) | [**ntripState**](classQfPositioning.md#property-ntripstate-12)  <br> |
| property double | [**orientation**](classQfPositioning.md#property-orientation-12)  <br> |
| property [**QfGnssPositionInformation**](classQfGnssPositionInformation.md) | [**positionInformation**](classQfPositioning.md#property-positioninformation-12)  <br> |
| property double | [**projectedHorizontalAccuracy**](classQfPositioning.md#property-projectedhorizontalaccuracy-12)  <br> |
| property QgsPoint | [**projectedPosition**](classQfPositioning.md#property-projectedposition-12)  <br> |
| property bool | [**serviceMode**](classQfPositioning.md#property-servicemode-12)  <br> |
| property QgsPoint | [**sourcePosition**](classQfPositioning.md#property-sourceposition-12)  <br> |
| property bool | [**valid**](classQfPositioning.md#property-valid-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**activeChanged**](classQfPositioning.md#signal-activechanged)  <br> |
| signal void | [**antennaHeightChanged**](classQfPositioning.md#signal-antennaheightchanged)  <br> |
| signal void | [**averagedPositionChanged**](classQfPositioning.md#signal-averagedpositionchanged)  <br> |
| signal void | [**averagedPositionCountChanged**](classQfPositioning.md#signal-averagedpositioncountchanged)  <br> |
| signal void | [**averagedPositionFilterAccuracyChanged**](classQfPositioning.md#signal-averagedpositionfilteraccuracychanged)  <br> |
| signal void | [**backgroundModeChanged**](classQfPositioning.md#signal-backgroundmodechanged)  <br> |
| signal void | [**badAccuracyThresholdChanged**](classQfPositioning.md#signal-badaccuracythresholdchanged)  <br> |
| signal void | [**coordinateTransformerChanged**](classQfPositioning.md#signal-coordinatetransformerchanged)  <br> |
| signal void | [**deviceBatteryLevelChanged**](classQfPositioning.md#signal-devicebatterylevelchanged)  <br> |
| signal void | [**deviceChanged**](classQfPositioning.md#signal-devicechanged)  <br> |
| signal void | [**deviceIdChanged**](classQfPositioning.md#signal-deviceidchanged)  <br> |
| signal void | [**deviceLastErrorChanged**](classQfPositioning.md#signal-devicelasterrorchanged)  <br> |
| signal void | [**deviceSocketStateChanged**](classQfPositioning.md#signal-devicesocketstatechanged)  <br> |
| signal void | [**deviceSocketStateStringChanged**](classQfPositioning.md#signal-devicesocketstatestringchanged)  <br> |
| signal void | [**elevationCorrectionModeChanged**](classQfPositioning.md#signal-elevationcorrectionmodechanged)  <br> |
| signal void | [**enableNtripChanged**](classQfPositioning.md#signal-enablentripchanged)  <br> |
| signal void | [**excellentAccuracyThresholdChanged**](classQfPositioning.md#signal-excellentaccuracythresholdchanged)  <br> |
| signal void | [**loggingChanged**](classQfPositioning.md#signal-loggingchanged)  <br> |
| signal void | [**loggingPathChanged**](classQfPositioning.md#signal-loggingpathchanged)  <br> |
| signal void | [**ntripBytesReceivedChanged**](classQfPositioning.md#signal-ntripbytesreceivedchanged)  <br> |
| signal void | [**ntripBytesSentChanged**](classQfPositioning.md#signal-ntripbytessentchanged)  <br> |
| signal void | [**ntripLastBytesReceivedUtcDateTimeChanged**](classQfPositioning.md#signal-ntriplastbytesreceivedutcdatetimechanged)  <br> |
| signal void | [**ntripSettingsChanged**](classQfPositioning.md#signal-ntripsettingschanged)  <br> |
| signal void | [**ntripStateChanged**](classQfPositioning.md#signal-ntripstatechanged)  <br> |
| signal void | [**orientationChanged**](classQfPositioning.md#signal-orientationchanged)  <br> |
| signal void | [**positionInformationChanged**](classQfPositioning.md#signal-positioninformationchanged)  <br> |
| signal void | [**projectedPositionChanged**](classQfPositioning.md#signal-projectedpositionchanged)  <br> |
| signal void | [**serviceModeChanged**](classQfPositioning.md#signal-servicemodechanged)  <br> |
| signal void | [**triggerConnectDevice**](classQfPositioning.md#signal-triggerconnectdevice)  <br> |
| signal void | [**triggerDisconnectDevice**](classQfPositioning.md#signal-triggerdisconnectdevice)  <br> |
| signal void | [**validChanged**](classQfPositioning.md#signal-validchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfPositioning**](#function-qfpositioning) (QObject \* parent=nullptr) <br> |
|  bool | [**active**](#function-active-22) () const<br> |
|  double | [**antennaHeight**](#function-antennaheight-22) () const<br> |
|  bool | [**averagedPosition**](#function-averagedposition-22) () const<br> |
|  int | [**averagedPositionCount**](#function-averagedpositioncount-22) () const<br> |
|  bool | [**averagedPositionFilterAccuracy**](#function-averagedpositionfilteraccuracy-22) () const<br> |
|  bool | [**backgroundMode**](#function-backgroundmode-22) () const<br> |
|  double | [**badAccuracyThreshold**](#function-badaccuracythreshold-22) () const<br> |
|  [**QgsQuickCoordinateTransformer**](classQgsQuickCoordinateTransformer.md) \* | [**coordinateTransformer**](#function-coordinatetransformer-22) () const<br> |
|  double | [**deviceBatteryLevel**](#function-devicebatterylevel-22) () const<br> |
|  QfAbstractGnssReceiver::Capabilities | [**deviceCapabilities**](#function-devicecapabilities-22) () const<br> |
|  [**QfGnssPositionDetails**](classQfGnssPositionDetails.md) | [**deviceDetails**](#function-devicedetails) () const<br> |
|  QString | [**deviceId**](#function-deviceid-22) () const<br> |
|  QString | [**deviceLastError**](#function-devicelasterror-22) () const<br> |
|  QAbstractSocket::SocketState | [**deviceSocketState**](#function-devicesocketstate-22) () const<br> |
|  QString | [**deviceSocketStateString**](#function-devicesocketstatestring-22) () const<br> |
|  [**QfPositioningSource::ElevationCorrectionMode**](classQfPositioningSource.md#enum-elevationcorrectionmode) | [**elevationCorrectionMode**](#function-elevationcorrectionmode-22) () const<br> |
|  bool | [**enableNtrip**](#function-enablentrip-22) () const<br> |
|  double | [**excellentAccuracyThreshold**](#function-excellentaccuracythreshold-22) () const<br> |
|  Q\_INVOKABLE QList&lt; [**QfGnssPositionInformation**](classQfGnssPositionInformation.md) &gt; | [**getBackgroundPositionInformation**](#function-getbackgroundpositioninformation) () const<br> |
|  bool | [**logging**](#function-logging-22) () const<br> |
|  QString | [**loggingPath**](#function-loggingpath-22) () const<br> |
|  qint64 | [**ntripBytesReceived**](#function-ntripbytesreceived-22) () const<br> |
|  qint64 | [**ntripBytesSent**](#function-ntripbytessent-22) () const<br> |
|  QDateTime | [**ntripLastBytesReceivedUtcDateTime**](#function-ntriplastbytesreceivedutcdatetime-22) () const<br> |
|  [**QfNtripSettings**](classQfNtripSettings.md) | [**ntripSettings**](#function-ntripsettings-22) () const<br> |
|  [**QfPositioningSource::NtripState**](classQfPositioningSource.md#enum-ntripstate) | [**ntripState**](#function-ntripstate-22) () const<br> |
|  double | [**orientation**](#function-orientation-22) () const<br> |
|  [**QfGnssPositionInformation**](classQfGnssPositionInformation.md) | [**positionInformation**](#function-positioninformation-22) () const<br> |
|  double | [**projectedHorizontalAccuracy**](#function-projectedhorizontalaccuracy-22) () const<br> |
|  QgsPoint | [**projectedPosition**](#function-projectedposition-22) () const<br> |
|  bool | [**serviceMode**](#function-servicemode-22) () const<br> |
|  void | [**setActive**](#function-setactive) (bool active) <br> |
|  void | [**setAntennaHeight**](#function-setantennaheight) (double antennaHeight) <br> |
|  void | [**setAveragedPosition**](#function-setaveragedposition) (bool averaged) <br> |
|  void | [**setAveragedPositionFilterAccuracy**](#function-setaveragedpositionfilteraccuracy) (bool enabled) <br> |
|  void | [**setBackgroundMode**](#function-setbackgroundmode) (bool enabled) <br> |
|  void | [**setBadAccuracyThreshold**](#function-setbadaccuracythreshold) (double threshold) <br> |
|  void | [**setCoordinateTransformer**](#function-setcoordinatetransformer) ([**QgsQuickCoordinateTransformer**](classQgsQuickCoordinateTransformer.md) \* coordinateTransformer) <br> |
|  void | [**setDeviceId**](#function-setdeviceid) (const QString & id) <br> |
|  void | [**setElevationCorrectionMode**](#function-setelevationcorrectionmode) ([**QfPositioningSource::ElevationCorrectionMode**](classQfPositioningSource.md#enum-elevationcorrectionmode) elevationCorrectionMode) <br> |
|  void | [**setEnableNtrip**](#function-setenablentrip) (bool enableNtrip) <br> |
|  void | [**setExcellentAccuracyThreshold**](#function-setexcellentaccuracythreshold) (double threshold) <br> |
|  void | [**setLogging**](#function-setlogging) (bool logging) <br> |
|  void | [**setLoggingPath**](#function-setloggingpath) (const QString & path) <br> |
|  void | [**setNtripSettings**](#function-setntripsettings) (const [**QfNtripSettings**](classQfNtripSettings.md) & ntripSettings) <br> |
|  void | [**setServiceMode**](#function-setservicemode) (bool enabled) <br> |
|  void | [**setValid**](#function-setvalid) (bool valid) <br> |
|  QgsPoint | [**sourcePosition**](#function-sourceposition-22) () const<br> |
|  bool | [**valid**](#function-valid-22) () const<br> |
| virtual  | [**~QfPositioning**](#function-qfpositioning) () = default<br> |




























## Detailed Description


This class manages the positioning source and offers positioning details. 


    
## Public Properties Documentation




### property active [1/2]

```C++
bool QfPositioning::active;
```




<hr>



### property antennaHeight [1/2]

```C++
double QfPositioning::antennaHeight;
```




<hr>



### property averagedPosition [1/2]

```C++
bool QfPositioning::averagedPosition;
```




<hr>



### property averagedPositionCount [1/2]

```C++
int QfPositioning::averagedPositionCount;
```




<hr>



### property averagedPositionFilterAccuracy [1/2]

```C++
bool QfPositioning::averagedPositionFilterAccuracy;
```




<hr>



### property backgroundMode [1/2]

```C++
bool QfPositioning::backgroundMode;
```




<hr>



### property badAccuracyThreshold [1/2]

```C++
double QfPositioning::badAccuracyThreshold;
```




<hr>



### property coordinateTransformer [1/2]

```C++
QgsQuickCoordinateTransformer * QfPositioning::coordinateTransformer;
```




<hr>



### property deviceBatteryLevel [1/2]

```C++
double QfPositioning::deviceBatteryLevel;
```




<hr>



### property deviceCapabilities [1/2]

```C++
QfAbstractGnssReceiver::Capabilities QfPositioning::deviceCapabilities;
```




<hr>



### property deviceId [1/2]

```C++
QString QfPositioning::deviceId;
```




<hr>



### property deviceLastError [1/2]

```C++
QString QfPositioning::deviceLastError;
```




<hr>



### property deviceSocketState [1/2]

```C++
QAbstractSocket::SocketState QfPositioning::deviceSocketState;
```




<hr>



### property deviceSocketStateString [1/2]

```C++
QString QfPositioning::deviceSocketStateString;
```




<hr>



### property elevationCorrectionMode [1/2]

```C++
QfPositioningSource::ElevationCorrectionMode QfPositioning::elevationCorrectionMode;
```




<hr>



### property enableNtrip [1/2]

```C++
bool QfPositioning::enableNtrip;
```




<hr>



### property excellentAccuracyThreshold [1/2]

```C++
double QfPositioning::excellentAccuracyThreshold;
```




<hr>



### property logging [1/2]

```C++
bool QfPositioning::logging;
```




<hr>



### property loggingPath [1/2]

```C++
QString QfPositioning::loggingPath;
```




<hr>



### property ntripBytesReceived [1/2]

```C++
qint64 QfPositioning::ntripBytesReceived;
```




<hr>



### property ntripBytesSent [1/2]

```C++
qint64 QfPositioning::ntripBytesSent;
```




<hr>



### property ntripLastBytesReceivedUtcDateTime [1/2]

```C++
QDateTime QfPositioning::ntripLastBytesReceivedUtcDateTime;
```




<hr>



### property ntripSettings [1/2]

```C++
QfNtripSettings QfPositioning::ntripSettings;
```




<hr>



### property ntripState [1/2]

```C++
QfPositioningSource::NtripState QfPositioning::ntripState;
```




<hr>



### property orientation [1/2]

```C++
double QfPositioning::orientation;
```




<hr>



### property positionInformation [1/2]

```C++
QfGnssPositionInformation QfPositioning::positionInformation;
```




<hr>



### property projectedHorizontalAccuracy [1/2]

```C++
double QfPositioning::projectedHorizontalAccuracy;
```




<hr>



### property projectedPosition [1/2]

```C++
QgsPoint QfPositioning::projectedPosition;
```




<hr>



### property serviceMode [1/2]

```C++
bool QfPositioning::serviceMode;
```




<hr>



### property sourcePosition [1/2]

```C++
QgsPoint QfPositioning::sourcePosition;
```




<hr>



### property valid [1/2]

```C++
bool QfPositioning::valid;
```




<hr>
## Public Signals Documentation




### signal activeChanged 

```C++
void QfPositioning::activeChanged;
```




<hr>



### signal antennaHeightChanged 

```C++
void QfPositioning::antennaHeightChanged;
```




<hr>



### signal averagedPositionChanged 

```C++
void QfPositioning::averagedPositionChanged;
```




<hr>



### signal averagedPositionCountChanged 

```C++
void QfPositioning::averagedPositionCountChanged;
```




<hr>



### signal averagedPositionFilterAccuracyChanged 

```C++
void QfPositioning::averagedPositionFilterAccuracyChanged;
```




<hr>



### signal backgroundModeChanged 

```C++
void QfPositioning::backgroundModeChanged;
```




<hr>



### signal badAccuracyThresholdChanged 

```C++
void QfPositioning::badAccuracyThresholdChanged;
```




<hr>



### signal coordinateTransformerChanged 

```C++
void QfPositioning::coordinateTransformerChanged;
```




<hr>



### signal deviceBatteryLevelChanged 

```C++
void QfPositioning::deviceBatteryLevelChanged;
```




<hr>



### signal deviceChanged 

```C++
void QfPositioning::deviceChanged;
```




<hr>



### signal deviceIdChanged 

```C++
void QfPositioning::deviceIdChanged;
```




<hr>



### signal deviceLastErrorChanged 

```C++
void QfPositioning::deviceLastErrorChanged;
```




<hr>



### signal deviceSocketStateChanged 

```C++
void QfPositioning::deviceSocketStateChanged;
```




<hr>



### signal deviceSocketStateStringChanged 

```C++
void QfPositioning::deviceSocketStateStringChanged;
```




<hr>



### signal elevationCorrectionModeChanged 

```C++
void QfPositioning::elevationCorrectionModeChanged;
```




<hr>



### signal enableNtripChanged 

```C++
void QfPositioning::enableNtripChanged;
```




<hr>



### signal excellentAccuracyThresholdChanged 

```C++
void QfPositioning::excellentAccuracyThresholdChanged;
```




<hr>



### signal loggingChanged 

```C++
void QfPositioning::loggingChanged;
```




<hr>



### signal loggingPathChanged 

```C++
void QfPositioning::loggingPathChanged;
```




<hr>



### signal ntripBytesReceivedChanged 

```C++
void QfPositioning::ntripBytesReceivedChanged;
```




<hr>



### signal ntripBytesSentChanged 

```C++
void QfPositioning::ntripBytesSentChanged;
```




<hr>



### signal ntripLastBytesReceivedUtcDateTimeChanged 

```C++
void QfPositioning::ntripLastBytesReceivedUtcDateTimeChanged;
```




<hr>



### signal ntripSettingsChanged 

```C++
void QfPositioning::ntripSettingsChanged;
```




<hr>



### signal ntripStateChanged 

```C++
void QfPositioning::ntripStateChanged;
```




<hr>



### signal orientationChanged 

```C++
void QfPositioning::orientationChanged;
```




<hr>



### signal positionInformationChanged 

```C++
void QfPositioning::positionInformationChanged;
```




<hr>



### signal projectedPositionChanged 

```C++
void QfPositioning::projectedPositionChanged;
```




<hr>



### signal serviceModeChanged 

```C++
void QfPositioning::serviceModeChanged;
```




<hr>



### signal triggerConnectDevice 

```C++
void QfPositioning::triggerConnectDevice;
```




<hr>



### signal triggerDisconnectDevice 

```C++
void QfPositioning::triggerDisconnectDevice;
```




<hr>



### signal validChanged 

```C++
void QfPositioning::validChanged;
```




<hr>
## Public Functions Documentation




### function QfPositioning 

```C++
explicit QfPositioning::QfPositioning (
    QObject * parent=nullptr
) 
```




<hr>



### function active [2/2]

```C++
bool QfPositioning::active () const
```



Returns TRUE when positioning is active. 

**See also:** [**setActive**](classQfPositioning.md#function-setactive) 



        

<hr>



### function antennaHeight [2/2]

```C++
double QfPositioning::antennaHeight () const
```



Sets the GNSS device antenna height. This should be the pole height + sensore phase height. 

**Note:**

When IMU is active this value is ignored as the device does the correction internally. 





        

<hr>



### function averagedPosition [2/2]

```C++
bool QfPositioning::averagedPosition () const
```



Returns whether the position information is averaged from an ongoing stream of incoming positions from the device. 


        

<hr>



### function averagedPositionCount [2/2]

```C++
int QfPositioning::averagedPositionCount () const
```



Returns the current number of collected position informations from which the averaged position is calculated. 

**Note:**

When averaged position is off, the value is zero. 





        

<hr>



### function averagedPositionFilterAccuracy [2/2]

```C++
bool QfPositioning::averagedPositionFilterAccuracy () const
```



Returns whether the average position filter accuracy is enabled. 


        

<hr>



### function backgroundMode [2/2]

```C++
bool QfPositioning::backgroundMode () const
```



Returns TRUE if the background mode is active. When activated, position information details will not be signaled but instead saved to disk until deactivated. 

**See also:** [**getBackgroundPositionInformation()**](classQfPositioning.md#function-getbackgroundpositioninformation) 



        

<hr>



### function badAccuracyThreshold [2/2]

```C++
inline double QfPositioning::badAccuracyThreshold () const
```



Returns the threshold above which accuracy is considered bad. 


        

<hr>



### function coordinateTransformer [2/2]

```C++
inline QgsQuickCoordinateTransformer * QfPositioning::coordinateTransformer () const
```



Returns the coordinate transformer object used to reproject the position location. 


        

<hr>



### function deviceBatteryLevel [2/2]

```C++
double QfPositioning::deviceBatteryLevel () const
```



Returns the device current battery level as a 0.0 to 1.0 double range. 


        

<hr>



### function deviceCapabilities [2/2]

```C++
QfAbstractGnssReceiver::Capabilities QfPositioning::deviceCapabilities () const
```




<hr>



### function deviceDetails 

```C++
QfGnssPositionDetails QfPositioning::deviceDetails () const
```



Returns extra details (such as hdop, vdop, pdop) provided by the positioning device. 


        

<hr>



### function deviceId [2/2]

```C++
QString QfPositioning::deviceId () const
```



Returns the current positioning device _id_ used to fetch position information. 

**See also:** setDevice 



        

<hr>



### function deviceLastError [2/2]

```C++
QString QfPositioning::deviceLastError () const
```



Returns positioning device's last error string. 


        

<hr>



### function deviceSocketState [2/2]

```C++
QAbstractSocket::SocketState QfPositioning::deviceSocketState () const
```



Returns positioning device's socket state. 

**See also:** deviceSocketStateString 



        

<hr>



### function deviceSocketStateString [2/2]

```C++
QString QfPositioning::deviceSocketStateString () const
```



Returns a string representation of the positioning device's socket state. 

**See also:** deviceSocketState 



        

<hr>



### function elevationCorrectionMode [2/2]

```C++
QfPositioningSource::ElevationCorrectionMode QfPositioning::elevationCorrectionMode () const
```



Returns the current elevation correction mode. 

**Note:**

Some modes depends on device capabilities. 





        

<hr>



### function enableNtrip [2/2]

```C++
bool QfPositioning::enableNtrip () const
```



Returns TRUE if the NTRIP client is enabled. 


        

<hr>



### function excellentAccuracyThreshold [2/2]

```C++
inline double QfPositioning::excellentAccuracyThreshold () const
```



Returns the threshold below which accuracy is considered excellent. 


        

<hr>



### function getBackgroundPositionInformation 

```C++
Q_INVOKABLE QList< QfGnssPositionInformation > QfPositioning::getBackgroundPositionInformation () const
```



Returns a list of position information collected while background mode is active. 

**See also:** backgroundMode() 


**See also:** [**setBackgroundMode()**](classQfPositioning.md#function-setbackgroundmode) 



        

<hr>



### function logging [2/2]

```C++
bool QfPositioning::logging () const
```



Returns whether GNSS devices will log their incoming position stream into a logfile. 

**Note:**

Requires a device type with logging capability 





        

<hr>



### function loggingPath [2/2]

```C++
QString QfPositioning::loggingPath () const
```



Returns the path where GNSS devices will log their incoming position stream into logfiles. 

**Note:**

Requires a device type with logging capability 





        

<hr>



### function ntripBytesReceived [2/2]

```C++
qint64 QfPositioning::ntripBytesReceived () const
```



Returns the number of bytes received via NTRIP. 


        

<hr>



### function ntripBytesSent [2/2]

```C++
qint64 QfPositioning::ntripBytesSent () const
```



Returns the number of bytes sent via NTRIP. 


        

<hr>



### function ntripLastBytesReceivedUtcDateTime [2/2]

```C++
QDateTime QfPositioning::ntripLastBytesReceivedUtcDateTime () const
```



Returns the last received bytes time in UTC. 


        

<hr>



### function ntripSettings [2/2]

```C++
QfNtripSettings QfPositioning::ntripSettings () const
```



Returns the NTRIP settings. 


        

<hr>



### function ntripState [2/2]

```C++
QfPositioningSource::NtripState QfPositioning::ntripState () const
```



Returns the current NTRIP connection status. 


        

<hr>



### function orientation [2/2]

```C++
double QfPositioning::orientation () const
```



Returns the current device orientation 


        

<hr>



### function positionInformation [2/2]

```C++
QfGnssPositionInformation QfPositioning::positionInformation () const
```



Returns a [**QfGnssPositionInformation**](classQfGnssPositionInformation.md) position information object. 


        

<hr>



### function projectedHorizontalAccuracy [2/2]

```C++
double QfPositioning::projectedHorizontalAccuracy () const
```



Returns the position horizontal accuracy in the destination CRS' map units. 


        

<hr>



### function projectedPosition [2/2]

```C++
QgsPoint QfPositioning::projectedPosition () const
```



Returns the position point in its destination CRS defined in the coordinate transformer. 


        

<hr>



### function serviceMode [2/2]

```C++
bool QfPositioning::serviceMode () const
```



Returns TRUE if the service mode is active. On supported platform, positioning runs into a dedicated service that is not suspended when devices are locked or the application is put suspended. 

**See also:** [**setServiceMode()**](classQfPositioning.md#function-setservicemode) 



        

<hr>



### function setActive 

```C++
void QfPositioning::setActive (
    bool active
) 
```



Sets the positioning _active_ status. 

**See also:** active 



        

<hr>



### function setAntennaHeight 

```C++
void QfPositioning::setAntennaHeight (
    double antennaHeight
) 
```



Returns the GNSS device antenna height. This should be the pole height + sensore phase height. 

**Note:**

When IMU is active this value is ignored as the device does the correction internally. 





        

<hr>



### function setAveragedPosition 

```C++
void QfPositioning::setAveragedPosition (
    bool averaged
) 
```



Sets whether the position information is _averaged_ from an ongoing stream of incoming positions from the device. 


        

<hr>



### function setAveragedPositionFilterAccuracy 

```C++
void QfPositioning::setAveragedPositionFilterAccuracy (
    bool enabled
) 
```



Enables or disables the average position filter accuracy. 


        

<hr>



### function setBackgroundMode 

```C++
void QfPositioning::setBackgroundMode (
    bool enabled
) 
```



Sets whether the background mode is active. When activated, position information details will not be signaled but instead saved to disk until deactivated. 

**See also:** [**getBackgroundPositionInformation()**](classQfPositioning.md#function-getbackgroundpositioninformation) 



        

<hr>



### function setBadAccuracyThreshold 

```C++
void QfPositioning::setBadAccuracyThreshold (
    double threshold
) 
```



Sets the threshold above which accuracy is considered bad. 


        

<hr>



### function setCoordinateTransformer 

```C++
void QfPositioning::setCoordinateTransformer (
    QgsQuickCoordinateTransformer * coordinateTransformer
) 
```



Sets the coordinate transformer object used to reproject the position location. 

**Parameters:**


* `coordinateTransformer` A [**QgsQuickCoordinateTransformer**](classQgsQuickCoordinateTransformer.md) object. 




        

<hr>



### function setDeviceId 

```C++
void QfPositioning::setDeviceId (
    const QString & id
) 
```



Sets the positioning device _id_ used to fetch position information. 

**Note:**

A blank string will connect the internal positioning device; bluetooth addresses will trigger an NMEA connection to external devices. 





        

<hr>



### function setElevationCorrectionMode 

```C++
void QfPositioning::setElevationCorrectionMode (
    QfPositioningSource::ElevationCorrectionMode elevationCorrectionMode
) 
```



Sets the current elevation correction mode. 

**Note:**

Some modes depends on device capabilities. 





        

<hr>



### function setEnableNtrip 

```C++
void QfPositioning::setEnableNtrip (
    bool enableNtrip
) 
```



Sets whether the NTRIP client is enabled. 


        

<hr>



### function setExcellentAccuracyThreshold 

```C++
void QfPositioning::setExcellentAccuracyThreshold (
    double threshold
) 
```



Sets the threshold below which accuracy is considered excellent. 


        

<hr>



### function setLogging 

```C++
void QfPositioning::setLogging (
    bool logging
) 
```



Sets whether GNSS devices will log their incoming position stream into a logfile. 

**Note:**

Requires a device type with logging capability 





        

<hr>



### function setLoggingPath 

```C++
void QfPositioning::setLoggingPath (
    const QString & path
) 
```



Sets the path where GNSS devices will log their incoming position stream into logfiles. 

**Note:**

Requires a device type with logging capability 





        

<hr>



### function setNtripSettings 

```C++
void QfPositioning::setNtripSettings (
    const QfNtripSettings & ntripSettings
) 
```



Sets the NTRIP settings. 


        

<hr>



### function setServiceMode 

```C++
void QfPositioning::setServiceMode (
    bool enabled
) 
```



Sets whether the service mode is active. On supported platform, positioning runs into a dedicated service that is not suspended when devices are locked or the application is put suspended. 

**See also:** serviceMode() 



        

<hr>



### function setValid 

```C++
void QfPositioning::setValid (
    bool valid
) 
```



Sets whether the positioning device is valid. 


        

<hr>



### function sourcePosition [2/2]

```C++
QgsPoint QfPositioning::sourcePosition () const
```



Returns the position point in its original WGS84 projection. 


        

<hr>



### function valid [2/2]

```C++
bool QfPositioning::valid () const
```



Returns TRUE when the positioning device is valid. 


        

<hr>



### function ~QfPositioning 

```C++
virtual QfPositioning::~QfPositioning () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/positioning/qfpositioning.h`

