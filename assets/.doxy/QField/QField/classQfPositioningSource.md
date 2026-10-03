

# Class QfPositioningSource



[**ClassList**](annotated.md) **>** [**QfPositioningSource**](classQfPositioningSource.md)



[More...](#detailed-description)

* `#include <qfpositioningsource.h>`



Inherits the following classes: QObject














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**ElevationCorrectionMode**](#enum-elevationcorrectionmode)  <br> |
| enum  | [**NtripState**](#enum-ntripstate)  <br> |






## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  QString | [**backgroundFilePath**](#variable-backgroundfilepath)   = `QStringLiteral( "%1/positioning.background" ).arg( QStandardPaths::writableLocation( QStandardPaths::AppDataLocation ) )`<br> |


## Public Slots

| Type | Name |
| ---: | :--- |
| slot void | [**triggerConnectDevice**](classQfPositioningSource.md#slot-triggerconnectdevice)  <br> |
| slot void | [**triggerDisconnectDevice**](classQfPositioningSource.md#slot-triggerdisconnectdevice)  <br> |




## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**active**](classQfPositioningSource.md#property-active-12)  <br> |
| property double | [**antennaHeight**](classQfPositioningSource.md#property-antennaheight-12)  <br> |
| property bool | [**backgroundMode**](classQfPositioningSource.md#property-backgroundmode-12)  <br> |
| property double | [**deviceBatteryLevel**](classQfPositioningSource.md#property-devicebatterylevel-12)  <br> |
| property int | [**deviceCapabilities**](classQfPositioningSource.md#property-devicecapabilities-12)  <br> |
| property [**QfGnssPositionDetails**](classQfGnssPositionDetails.md) | [**deviceDetails**](classQfPositioningSource.md#property-devicedetails-12)  <br> |
| property QString | [**deviceId**](classQfPositioningSource.md#property-deviceid-12)  <br> |
| property QString | [**deviceLastError**](classQfPositioningSource.md#property-devicelasterror-12)  <br> |
| property QAbstractSocket::SocketState | [**deviceSocketState**](classQfPositioningSource.md#property-devicesocketstate-12)  <br> |
| property QString | [**deviceSocketStateString**](classQfPositioningSource.md#property-devicesocketstatestring-12)  <br> |
| property [**ElevationCorrectionMode**](classQfPositioningSource.md#enum-elevationcorrectionmode) | [**elevationCorrectionMode**](classQfPositioningSource.md#property-elevationcorrectionmode-12)  <br> |
| property bool | [**enableNtrip**](classQfPositioningSource.md#property-enablentrip-12)  <br> |
| property bool | [**logging**](classQfPositioningSource.md#property-logging-12)  <br> |
| property QString | [**loggingPath**](classQfPositioningSource.md#property-loggingpath-12)  <br> |
| property qint64 | [**ntripBytesReceived**](classQfPositioningSource.md#property-ntripbytesreceived-12)  <br> |
| property qint64 | [**ntripBytesSent**](classQfPositioningSource.md#property-ntripbytessent-12)  <br> |
| property QDateTime | [**ntripLastBytesReceivedUtcDateTime**](classQfPositioningSource.md#property-ntriplastbytesreceivedutcdatetime-12)  <br> |
| property QString | [**ntripLastError**](classQfPositioningSource.md#property-ntriplasterror-12)  <br> |
| property [**QfNtripSettings**](classQfNtripSettings.md) | [**ntripSettings**](classQfPositioningSource.md#property-ntripsettings-12)  <br> |
| property [**NtripState**](classQfPositioningSource.md#enum-ntripstate) | [**ntripState**](classQfPositioningSource.md#property-ntripstate-12)  <br> |
| property double | [**orientation**](classQfPositioningSource.md#property-orientation-12)  <br> |
| property [**QfGnssPositionInformation**](classQfGnssPositionInformation.md) | [**positionInformation**](classQfPositioningSource.md#property-positioninformation-12)  <br> |
| property bool | [**valid**](classQfPositioningSource.md#property-valid-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**activeChanged**](classQfPositioningSource.md#signal-activechanged)  <br> |
| signal void | [**antennaHeightChanged**](classQfPositioningSource.md#signal-antennaheightchanged)  <br> |
| signal void | [**backgroundModeChanged**](classQfPositioningSource.md#signal-backgroundmodechanged)  <br> |
| signal void | [**deviceBatteryLevelChanged**](classQfPositioningSource.md#signal-devicebatterylevelchanged)  <br> |
| signal void | [**deviceChanged**](classQfPositioningSource.md#signal-devicechanged)  <br> |
| signal void | [**deviceIdChanged**](classQfPositioningSource.md#signal-deviceidchanged)  <br> |
| signal void | [**deviceLastErrorChanged**](classQfPositioningSource.md#signal-devicelasterrorchanged)  <br> |
| signal void | [**deviceSocketStateChanged**](classQfPositioningSource.md#signal-devicesocketstatechanged)  <br> |
| signal void | [**deviceSocketStateStringChanged**](classQfPositioningSource.md#signal-devicesocketstatestringchanged)  <br> |
| signal void | [**elevationCorrectionModeChanged**](classQfPositioningSource.md#signal-elevationcorrectionmodechanged)  <br> |
| signal void | [**enableNtripChanged**](classQfPositioningSource.md#signal-enablentripchanged)  <br> |
| signal void | [**loggingChanged**](classQfPositioningSource.md#signal-loggingchanged)  <br> |
| signal void | [**loggingPathChanged**](classQfPositioningSource.md#signal-loggingpathchanged)  <br> |
| signal void | [**ntripBytesReceivedChanged**](classQfPositioningSource.md#signal-ntripbytesreceivedchanged)  <br> |
| signal void | [**ntripBytesSentChanged**](classQfPositioningSource.md#signal-ntripbytessentchanged)  <br> |
| signal void | [**ntripLastBytesReceivedUtcDateTimeChanged**](classQfPositioningSource.md#signal-ntriplastbytesreceivedutcdatetimechanged)  <br> |
| signal void | [**ntripLastErrorChanged**](classQfPositioningSource.md#signal-ntriplasterrorchanged)  <br> |
| signal void | [**ntripSettingsChanged**](classQfPositioningSource.md#signal-ntripsettingschanged)  <br> |
| signal void | [**ntripStateChanged**](classQfPositioningSource.md#signal-ntripstatechanged)  <br> |
| signal void | [**orientationChanged**](classQfPositioningSource.md#signal-orientationchanged)  <br> |
| signal void | [**positionInformationChanged**](classQfPositioningSource.md#signal-positioninformationchanged)  <br> |
| signal void | [**validChanged**](classQfPositioningSource.md#signal-validchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfPositioningSource**](#function-qfpositioningsource) (QObject \* parent=nullptr) <br> |
|  bool | [**active**](#function-active-22) () const<br> |
|  double | [**antennaHeight**](#function-antennaheight-22) () const<br> |
|  bool | [**backgroundMode**](#function-backgroundmode-22) () const<br> |
|  [**QfAbstractGnssReceiver**](classQfAbstractGnssReceiver.md) \* | [**device**](#function-device) () const<br> |
|  double | [**deviceBatteryLevel**](#function-devicebatterylevel-22) () const<br> |
|  int | [**deviceCapabilities**](#function-devicecapabilities-22) () const<br> |
|  [**QfGnssPositionDetails**](classQfGnssPositionDetails.md) | [**deviceDetails**](#function-devicedetails-22) () const<br> |
|  QString | [**deviceId**](#function-deviceid-22) () const<br> |
|  QString | [**deviceLastError**](#function-devicelasterror-22) () const<br> |
|  QAbstractSocket::SocketState | [**deviceSocketState**](#function-devicesocketstate-22) () const<br> |
|  QString | [**deviceSocketStateString**](#function-devicesocketstatestring-22) () const<br> |
|  [**ElevationCorrectionMode**](classQfPositioningSource.md#enum-elevationcorrectionmode) | [**elevationCorrectionMode**](#function-elevationcorrectionmode-22) () const<br> |
|  bool | [**enableNtrip**](#function-enablentrip-22) () const<br> |
|  Q\_INVOKABLE QList&lt; [**QfGnssPositionInformation**](classQfGnssPositionInformation.md) &gt; | [**getBackgroundPositionInformation**](#function-getbackgroundpositioninformation) () const<br> |
|  bool | [**logging**](#function-logging-22) () const<br> |
|  QString | [**loggingPath**](#function-loggingpath-22) () const<br> |
|  qint64 | [**ntripBytesReceived**](#function-ntripbytesreceived-22) () const<br> |
|  qint64 | [**ntripBytesSent**](#function-ntripbytessent-22) () const<br> |
|  QDateTime | [**ntripLastBytesReceivedUtcDateTime**](#function-ntriplastbytesreceivedutcdatetime-22) () const<br> |
|  QString | [**ntripLastError**](#function-ntriplasterror-22) () const<br> |
|  [**QfNtripSettings**](classQfNtripSettings.md) | [**ntripSettings**](#function-ntripsettings-22) () const<br> |
|  [**NtripState**](classQfPositioningSource.md#enum-ntripstate) | [**ntripState**](#function-ntripstate-22) () const<br> |
|  double | [**orientation**](#function-orientation-22) () const<br> |
|  [**QfGnssPositionInformation**](classQfGnssPositionInformation.md) | [**positionInformation**](#function-positioninformation-22) () const<br> |
|  void | [**setActive**](#function-setactive) (bool active) <br> |
|  void | [**setAntennaHeight**](#function-setantennaheight) (double antennaHeight) <br> |
|  void | [**setBackgroundMode**](#function-setbackgroundmode) (bool backgroundMode) <br> |
|  void | [**setDeviceId**](#function-setdeviceid) (const QString & id) <br> |
|  void | [**setElevationCorrectionMode**](#function-setelevationcorrectionmode) ([**ElevationCorrectionMode**](classQfPositioningSource.md#enum-elevationcorrectionmode) elevationCorrectionMode) <br> |
|  void | [**setEnableNtrip**](#function-setenablentrip) (bool enableNtrip) <br> |
|  void | [**setLogging**](#function-setlogging) (bool logging) <br> |
|  void | [**setLoggingPath**](#function-setloggingpath) (const QString & path) <br> |
|  void | [**setNtripSettings**](#function-setntripsettings) (const [**QfNtripSettings**](classQfNtripSettings.md) & ntripSettings) <br> |
|  void | [**setValid**](#function-setvalid) (bool valid) <br> |
|  bool | [**valid**](#function-valid-22) () const<br> |
| virtual  | [**~QfPositioningSource**](#function-qfpositioningsource) () = default<br> |




























## Detailed Description


This class connects to GNSS devices (internal or bluetooth NMEA) and provides positioning details. 


    
## Public Types Documentation




### enum ElevationCorrectionMode 

```C++
enum QfPositioningSource::ElevationCorrectionMode {
    None,
    OrthometricFromDevice,
    OrthometricFromGeoidFile
};
```



Elevation correction modes 


        

<hr>



### enum NtripState 

```C++
enum QfPositioningSource::NtripState {
    Disconnected,
    Connecting,
    Connected
};
```



NTRIP client connection states 


        

<hr>
## Public Static Attributes Documentation




### variable backgroundFilePath 

```C++
QString QfPositioningSource::backgroundFilePath;
```




<hr>
## Public Properties Documentation




### property active [1/2]

```C++
bool QfPositioningSource::active;
```




<hr>



### property antennaHeight [1/2]

```C++
double QfPositioningSource::antennaHeight;
```




<hr>



### property backgroundMode [1/2]

```C++
bool QfPositioningSource::backgroundMode;
```




<hr>



### property deviceBatteryLevel [1/2]

```C++
double QfPositioningSource::deviceBatteryLevel;
```




<hr>



### property deviceCapabilities [1/2]

```C++
int QfPositioningSource::deviceCapabilities;
```




<hr>



### property deviceDetails [1/2]

```C++
QfGnssPositionDetails QfPositioningSource::deviceDetails;
```




<hr>



### property deviceId [1/2]

```C++
QString QfPositioningSource::deviceId;
```




<hr>



### property deviceLastError [1/2]

```C++
QString QfPositioningSource::deviceLastError;
```




<hr>



### property deviceSocketState [1/2]

```C++
QAbstractSocket::SocketState QfPositioningSource::deviceSocketState;
```




<hr>



### property deviceSocketStateString [1/2]

```C++
QString QfPositioningSource::deviceSocketStateString;
```




<hr>



### property elevationCorrectionMode [1/2]

```C++
ElevationCorrectionMode QfPositioningSource::elevationCorrectionMode;
```




<hr>



### property enableNtrip [1/2]

```C++
bool QfPositioningSource::enableNtrip;
```




<hr>



### property logging [1/2]

```C++
bool QfPositioningSource::logging;
```




<hr>



### property loggingPath [1/2]

```C++
QString QfPositioningSource::loggingPath;
```




<hr>



### property ntripBytesReceived [1/2]

```C++
qint64 QfPositioningSource::ntripBytesReceived;
```




<hr>



### property ntripBytesSent [1/2]

```C++
qint64 QfPositioningSource::ntripBytesSent;
```




<hr>



### property ntripLastBytesReceivedUtcDateTime [1/2]

```C++
QDateTime QfPositioningSource::ntripLastBytesReceivedUtcDateTime;
```




<hr>



### property ntripLastError [1/2]

```C++
QString QfPositioningSource::ntripLastError;
```




<hr>



### property ntripSettings [1/2]

```C++
QfNtripSettings QfPositioningSource::ntripSettings;
```




<hr>



### property ntripState [1/2]

```C++
NtripState QfPositioningSource::ntripState;
```




<hr>



### property orientation [1/2]

```C++
double QfPositioningSource::orientation;
```




<hr>



### property positionInformation [1/2]

```C++
QfGnssPositionInformation QfPositioningSource::positionInformation;
```




<hr>



### property valid [1/2]

```C++
bool QfPositioningSource::valid;
```




<hr>
## Public Slots Documentation




### slot triggerConnectDevice 

```C++
void QfPositioningSource::triggerConnectDevice;
```




<hr>



### slot triggerDisconnectDevice 

```C++
void QfPositioningSource::triggerDisconnectDevice;
```




<hr>
## Public Signals Documentation




### signal activeChanged 

```C++
void QfPositioningSource::activeChanged;
```




<hr>



### signal antennaHeightChanged 

```C++
void QfPositioningSource::antennaHeightChanged;
```




<hr>



### signal backgroundModeChanged 

```C++
void QfPositioningSource::backgroundModeChanged;
```




<hr>



### signal deviceBatteryLevelChanged 

```C++
void QfPositioningSource::deviceBatteryLevelChanged;
```




<hr>



### signal deviceChanged 

```C++
void QfPositioningSource::deviceChanged;
```




<hr>



### signal deviceIdChanged 

```C++
void QfPositioningSource::deviceIdChanged;
```




<hr>



### signal deviceLastErrorChanged 

```C++
void QfPositioningSource::deviceLastErrorChanged;
```




<hr>



### signal deviceSocketStateChanged 

```C++
void QfPositioningSource::deviceSocketStateChanged;
```




<hr>



### signal deviceSocketStateStringChanged 

```C++
void QfPositioningSource::deviceSocketStateStringChanged;
```




<hr>



### signal elevationCorrectionModeChanged 

```C++
void QfPositioningSource::elevationCorrectionModeChanged;
```




<hr>



### signal enableNtripChanged 

```C++
void QfPositioningSource::enableNtripChanged;
```




<hr>



### signal loggingChanged 

```C++
void QfPositioningSource::loggingChanged;
```




<hr>



### signal loggingPathChanged 

```C++
void QfPositioningSource::loggingPathChanged;
```




<hr>



### signal ntripBytesReceivedChanged 

```C++
void QfPositioningSource::ntripBytesReceivedChanged;
```




<hr>



### signal ntripBytesSentChanged 

```C++
void QfPositioningSource::ntripBytesSentChanged;
```




<hr>



### signal ntripLastBytesReceivedUtcDateTimeChanged 

```C++
void QfPositioningSource::ntripLastBytesReceivedUtcDateTimeChanged;
```




<hr>



### signal ntripLastErrorChanged 

```C++
void QfPositioningSource::ntripLastErrorChanged;
```




<hr>



### signal ntripSettingsChanged 

```C++
void QfPositioningSource::ntripSettingsChanged;
```




<hr>



### signal ntripStateChanged 

```C++
void QfPositioningSource::ntripStateChanged;
```




<hr>



### signal orientationChanged 

```C++
void QfPositioningSource::orientationChanged;
```




<hr>



### signal positionInformationChanged 

```C++
void QfPositioningSource::positionInformationChanged;
```




<hr>



### signal validChanged 

```C++
void QfPositioningSource::validChanged;
```




<hr>
## Public Functions Documentation




### function QfPositioningSource 

```C++
explicit QfPositioningSource::QfPositioningSource (
    QObject * parent=nullptr
) 
```




<hr>



### function active [2/2]

```C++
inline bool QfPositioningSource::active () const
```



Returns TRUE when positioning is active. 

**See also:** [**setActive**](classQfPositioningSource.md#function-setactive) 



        

<hr>



### function antennaHeight [2/2]

```C++
inline double QfPositioningSource::antennaHeight () const
```



Sets the GNSS device antenna height. This should be the pole height + sensore phase height. 

**Note:**

When IMU is active this value is ignored as the device does the correction internally. 





        

<hr>



### function backgroundMode [2/2]

```C++
inline bool QfPositioningSource::backgroundMode () const
```



Returns TRUE if the background mode is active. When activated, position information details will not be signaled but instead saved to disk until deactivated. 

**See also:** [**getBackgroundPositionInformation()**](classQfPositioningSource.md#function-getbackgroundpositioninformation) 



        

<hr>



### function device 

```C++
inline QfAbstractGnssReceiver * QfPositioningSource::device () const
```



Returns the current positioning device. 

**See also:** deviceId 



        

<hr>



### function deviceBatteryLevel [2/2]

```C++
inline double QfPositioningSource::deviceBatteryLevel () const
```



Returns the device current battery level as a 0.0 to 1.0 double range. 


        

<hr>



### function deviceCapabilities [2/2]

```C++
int QfPositioningSource::deviceCapabilities () const
```



Returns device capabiities. 

**Note:**

Due to Qt Remote Object issues with flags, we use an int property. 





        

<hr>



### function deviceDetails [2/2]

```C++
inline QfGnssPositionDetails QfPositioningSource::deviceDetails () const
```



Returns extra details (such as hdop, vdop, pdop) provided by the positioning device. 


        

<hr>



### function deviceId [2/2]

```C++
inline QString QfPositioningSource::deviceId () const
```



Returns the current positioning device _id_ used to fetch position information. 

**See also:** setDevice 



        

<hr>



### function deviceLastError [2/2]

```C++
inline QString QfPositioningSource::deviceLastError () const
```



Returns positioning device's last error string. 


        

<hr>



### function deviceSocketState [2/2]

```C++
inline QAbstractSocket::SocketState QfPositioningSource::deviceSocketState () const
```



Returns positioning device's socket state. 

**See also:** deviceSocketStateString 



        

<hr>



### function deviceSocketStateString [2/2]

```C++
inline QString QfPositioningSource::deviceSocketStateString () const
```



Returns a string representation of the positioning device's socket state. 

**See also:** deviceSocketState 



        

<hr>



### function elevationCorrectionMode [2/2]

```C++
inline ElevationCorrectionMode QfPositioningSource::elevationCorrectionMode () const
```



Returns the current elevation correction mode. 

**Note:**

Some modes depends on device capabilities. 





        

<hr>



### function enableNtrip [2/2]

```C++
inline bool QfPositioningSource::enableNtrip () const
```



Returns TRUE if the NTRIP client is enabled. 


        

<hr>



### function getBackgroundPositionInformation 

```C++
Q_INVOKABLE QList< QfGnssPositionInformation > QfPositioningSource::getBackgroundPositionInformation () const
```



Returns a list of position information collected while background mode is active. 

**See also:** backgroundMode() 


**See also:** [**setBackgroundMode()**](classQfPositioningSource.md#function-setbackgroundmode) 



        

<hr>



### function logging [2/2]

```C++
inline bool QfPositioningSource::logging () const
```



Returns whether GNSS devices will log their incoming position stream into a logfile. 

**Note:**

Requires a device type with logging capability 





        

<hr>



### function loggingPath [2/2]

```C++
inline QString QfPositioningSource::loggingPath () const
```



Returns the path where GNSS devices will log their incoming position stream into logfiles. 

**Note:**

Requires a device type with logging capability 





        

<hr>



### function ntripBytesReceived [2/2]

```C++
inline qint64 QfPositioningSource::ntripBytesReceived () const
```



Returns the number of bytes received via NTRIP. 


        

<hr>



### function ntripBytesSent [2/2]

```C++
inline qint64 QfPositioningSource::ntripBytesSent () const
```



Returns the number of bytes sent via NTRIP. 


        

<hr>



### function ntripLastBytesReceivedUtcDateTime [2/2]

```C++
inline QDateTime QfPositioningSource::ntripLastBytesReceivedUtcDateTime () const
```



Returns the last received bytes time in UTC. 


        

<hr>



### function ntripLastError [2/2]

```C++
inline QString QfPositioningSource::ntripLastError () const
```



Returns the last NTRIP error string. 


        

<hr>



### function ntripSettings [2/2]

```C++
inline QfNtripSettings QfPositioningSource::ntripSettings () const
```



Returns the NTRIP settings. 


        

<hr>



### function ntripState [2/2]

```C++
inline NtripState QfPositioningSource::ntripState () const
```



Returns the current NTRIP connection state. 


        

<hr>



### function orientation [2/2]

```C++
inline double QfPositioningSource::orientation () const
```



Returns the current device orientation 


        

<hr>



### function positionInformation [2/2]

```C++
inline QfGnssPositionInformation QfPositioningSource::positionInformation () const
```



Returns a [**QfGnssPositionInformation**](classQfGnssPositionInformation.md) position information object. 


        

<hr>



### function setActive 

```C++
void QfPositioningSource::setActive (
    bool active
) 
```



Sets the positioning _active_ status. 

**See also:** active 



        

<hr>



### function setAntennaHeight 

```C++
void QfPositioningSource::setAntennaHeight (
    double antennaHeight
) 
```



Returns the GNSS device antenna height. This should be the pole height + sensore phase height. 

**Note:**

When IMU is active this value is ignored as the device does the correction internally. 





        

<hr>



### function setBackgroundMode 

```C++
void QfPositioningSource::setBackgroundMode (
    bool backgroundMode
) 
```



Sets whether the background mode is active. When activated, position information details will not be signaled but instead saved to disk until deactivated. 

**See also:** [**getBackgroundPositionInformation()**](classQfPositioningSource.md#function-getbackgroundpositioninformation) 



        

<hr>



### function setDeviceId 

```C++
void QfPositioningSource::setDeviceId (
    const QString & id
) 
```



Sets the positioning device _id_ used to fetch position information. 

**Note:**

A blank string will connect the internal positioning device; bluetooth addresses will trigger an NMEA connection to external devices. 





        

<hr>



### function setElevationCorrectionMode 

```C++
void QfPositioningSource::setElevationCorrectionMode (
    ElevationCorrectionMode elevationCorrectionMode
) 
```



Sets the current elevation correction mode. 

**Note:**

Some modes depends on device capabilities. 





        

<hr>



### function setEnableNtrip 

```C++
void QfPositioningSource::setEnableNtrip (
    bool enableNtrip
) 
```



Sets whether the NTRIP client is enabled. 


        

<hr>



### function setLogging 

```C++
void QfPositioningSource::setLogging (
    bool logging
) 
```



Sets whether GNSS devices will log their incoming position stream into a logfile. 

**Note:**

Requires a device type with logging capability 





        

<hr>



### function setLoggingPath 

```C++
void QfPositioningSource::setLoggingPath (
    const QString & path
) 
```



Sets the path where GNSS devices will log their incoming position stream into logfiles. 

**Note:**

Requires a device type with logging capability 





        

<hr>



### function setNtripSettings 

```C++
void QfPositioningSource::setNtripSettings (
    const QfNtripSettings & ntripSettings
) 
```



Sets the NTRIP settings. 


        

<hr>



### function setValid 

```C++
void QfPositioningSource::setValid (
    bool valid
) 
```



Sets whether the positioning device is valid. 


        

<hr>



### function valid [2/2]

```C++
inline bool QfPositioningSource::valid () const
```



Returns TRUE when the positioning device is valid. 


        

<hr>



### function ~QfPositioningSource 

```C++
virtual QfPositioningSource::~QfPositioningSource () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/positioning/qfpositioningsource.h`

