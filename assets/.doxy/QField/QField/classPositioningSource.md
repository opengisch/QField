

# Class PositioningSource



[**ClassList**](annotated.md) **>** [**PositioningSource**](classPositioningSource.md)



[More...](#detailed-description)

* `#include <positioningsource.h>`



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
| slot void | [**triggerConnectDevice**](classPositioningSource.md#slot-triggerconnectdevice)  <br> |
| slot void | [**triggerDisconnectDevice**](classPositioningSource.md#slot-triggerdisconnectdevice)  <br> |




## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**active**](classPositioningSource.md#property-active-12)  <br> |
| property double | [**antennaHeight**](classPositioningSource.md#property-antennaheight-12)  <br> |
| property bool | [**backgroundMode**](classPositioningSource.md#property-backgroundmode-12)  <br> |
| property double | [**deviceBatteryLevel**](classPositioningSource.md#property-devicebatterylevel-12)  <br> |
| property int | [**deviceCapabilities**](classPositioningSource.md#property-devicecapabilities-12)  <br> |
| property [**GnssPositionDetails**](classGnssPositionDetails.md) | [**deviceDetails**](classPositioningSource.md#property-devicedetails-12)  <br> |
| property QString | [**deviceId**](classPositioningSource.md#property-deviceid-12)  <br> |
| property QString | [**deviceLastError**](classPositioningSource.md#property-devicelasterror-12)  <br> |
| property QAbstractSocket::SocketState | [**deviceSocketState**](classPositioningSource.md#property-devicesocketstate-12)  <br> |
| property QString | [**deviceSocketStateString**](classPositioningSource.md#property-devicesocketstatestring-12)  <br> |
| property [**ElevationCorrectionMode**](classPositioningSource.md#enum-elevationcorrectionmode) | [**elevationCorrectionMode**](classPositioningSource.md#property-elevationcorrectionmode-12)  <br> |
| property bool | [**enableNtrip**](classPositioningSource.md#property-enablentrip-12)  <br> |
| property bool | [**logging**](classPositioningSource.md#property-logging-12)  <br> |
| property QString | [**loggingPath**](classPositioningSource.md#property-loggingpath-12)  <br> |
| property qint64 | [**ntripBytesReceived**](classPositioningSource.md#property-ntripbytesreceived-12)  <br> |
| property qint64 | [**ntripBytesSent**](classPositioningSource.md#property-ntripbytessent-12)  <br> |
| property QDateTime | [**ntripLastBytesReceivedUtcDateTime**](classPositioningSource.md#property-ntriplastbytesreceivedutcdatetime-12)  <br> |
| property QString | [**ntripLastError**](classPositioningSource.md#property-ntriplasterror-12)  <br> |
| property [**NtripSettings**](classNtripSettings.md) | [**ntripSettings**](classPositioningSource.md#property-ntripsettings-12)  <br> |
| property [**NtripState**](classPositioningSource.md#enum-ntripstate) | [**ntripState**](classPositioningSource.md#property-ntripstate-12)  <br> |
| property double | [**orientation**](classPositioningSource.md#property-orientation-12)  <br> |
| property [**GnssPositionInformation**](classGnssPositionInformation.md) | [**positionInformation**](classPositioningSource.md#property-positioninformation-12)  <br> |
| property bool | [**valid**](classPositioningSource.md#property-valid-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**activeChanged**](classPositioningSource.md#signal-activechanged)  <br> |
| signal void | [**antennaHeightChanged**](classPositioningSource.md#signal-antennaheightchanged)  <br> |
| signal void | [**backgroundModeChanged**](classPositioningSource.md#signal-backgroundmodechanged)  <br> |
| signal void | [**deviceBatteryLevelChanged**](classPositioningSource.md#signal-devicebatterylevelchanged)  <br> |
| signal void | [**deviceChanged**](classPositioningSource.md#signal-devicechanged)  <br> |
| signal void | [**deviceIdChanged**](classPositioningSource.md#signal-deviceidchanged)  <br> |
| signal void | [**deviceLastErrorChanged**](classPositioningSource.md#signal-devicelasterrorchanged)  <br> |
| signal void | [**deviceSocketStateChanged**](classPositioningSource.md#signal-devicesocketstatechanged)  <br> |
| signal void | [**deviceSocketStateStringChanged**](classPositioningSource.md#signal-devicesocketstatestringchanged)  <br> |
| signal void | [**elevationCorrectionModeChanged**](classPositioningSource.md#signal-elevationcorrectionmodechanged)  <br> |
| signal void | [**enableNtripChanged**](classPositioningSource.md#signal-enablentripchanged)  <br> |
| signal void | [**loggingChanged**](classPositioningSource.md#signal-loggingchanged)  <br> |
| signal void | [**loggingPathChanged**](classPositioningSource.md#signal-loggingpathchanged)  <br> |
| signal void | [**ntripBytesReceivedChanged**](classPositioningSource.md#signal-ntripbytesreceivedchanged)  <br> |
| signal void | [**ntripBytesSentChanged**](classPositioningSource.md#signal-ntripbytessentchanged)  <br> |
| signal void | [**ntripLastBytesReceivedUtcDateTimeChanged**](classPositioningSource.md#signal-ntriplastbytesreceivedutcdatetimechanged)  <br> |
| signal void | [**ntripLastErrorChanged**](classPositioningSource.md#signal-ntriplasterrorchanged)  <br> |
| signal void | [**ntripSettingsChanged**](classPositioningSource.md#signal-ntripsettingschanged)  <br> |
| signal void | [**ntripStateChanged**](classPositioningSource.md#signal-ntripstatechanged)  <br> |
| signal void | [**orientationChanged**](classPositioningSource.md#signal-orientationchanged)  <br> |
| signal void | [**positionInformationChanged**](classPositioningSource.md#signal-positioninformationchanged)  <br> |
| signal void | [**validChanged**](classPositioningSource.md#signal-validchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PositioningSource**](#function-positioningsource) (QObject \* parent=nullptr) <br> |
|  bool | [**active**](#function-active-22) () const<br> |
|  double | [**antennaHeight**](#function-antennaheight-22) () const<br> |
|  bool | [**backgroundMode**](#function-backgroundmode-22) () const<br> |
|  [**AbstractGnssReceiver**](classAbstractGnssReceiver.md) \* | [**device**](#function-device) () const<br> |
|  double | [**deviceBatteryLevel**](#function-devicebatterylevel-22) () const<br> |
|  int | [**deviceCapabilities**](#function-devicecapabilities-22) () const<br> |
|  [**GnssPositionDetails**](classGnssPositionDetails.md) | [**deviceDetails**](#function-devicedetails-22) () const<br> |
|  QString | [**deviceId**](#function-deviceid-22) () const<br> |
|  QString | [**deviceLastError**](#function-devicelasterror-22) () const<br> |
|  QAbstractSocket::SocketState | [**deviceSocketState**](#function-devicesocketstate-22) () const<br> |
|  QString | [**deviceSocketStateString**](#function-devicesocketstatestring-22) () const<br> |
|  [**ElevationCorrectionMode**](classPositioningSource.md#enum-elevationcorrectionmode) | [**elevationCorrectionMode**](#function-elevationcorrectionmode-22) () const<br> |
|  bool | [**enableNtrip**](#function-enablentrip-22) () const<br> |
|  Q\_INVOKABLE QList&lt; [**GnssPositionInformation**](classGnssPositionInformation.md) &gt; | [**getBackgroundPositionInformation**](#function-getbackgroundpositioninformation) () const<br> |
|  bool | [**logging**](#function-logging-22) () const<br> |
|  QString | [**loggingPath**](#function-loggingpath-22) () const<br> |
|  qint64 | [**ntripBytesReceived**](#function-ntripbytesreceived-22) () const<br> |
|  qint64 | [**ntripBytesSent**](#function-ntripbytessent-22) () const<br> |
|  QDateTime | [**ntripLastBytesReceivedUtcDateTime**](#function-ntriplastbytesreceivedutcdatetime-22) () const<br> |
|  QString | [**ntripLastError**](#function-ntriplasterror-22) () const<br> |
|  [**NtripSettings**](classNtripSettings.md) | [**ntripSettings**](#function-ntripsettings-22) () const<br> |
|  [**NtripState**](classPositioningSource.md#enum-ntripstate) | [**ntripState**](#function-ntripstate-22) () const<br> |
|  double | [**orientation**](#function-orientation-22) () const<br> |
|  [**GnssPositionInformation**](classGnssPositionInformation.md) | [**positionInformation**](#function-positioninformation-22) () const<br> |
|  void | [**setActive**](#function-setactive) (bool active) <br> |
|  void | [**setAntennaHeight**](#function-setantennaheight) (double antennaHeight) <br> |
|  void | [**setBackgroundMode**](#function-setbackgroundmode) (bool backgroundMode) <br> |
|  void | [**setDeviceId**](#function-setdeviceid) (const QString & id) <br> |
|  void | [**setElevationCorrectionMode**](#function-setelevationcorrectionmode) ([**ElevationCorrectionMode**](classPositioningSource.md#enum-elevationcorrectionmode) elevationCorrectionMode) <br> |
|  void | [**setEnableNtrip**](#function-setenablentrip) (bool enableNtrip) <br> |
|  void | [**setLogging**](#function-setlogging) (bool logging) <br> |
|  void | [**setLoggingPath**](#function-setloggingpath) (const QString & path) <br> |
|  void | [**setNtripSettings**](#function-setntripsettings) (const [**NtripSettings**](classNtripSettings.md) & ntripSettings) <br> |
|  void | [**setValid**](#function-setvalid) (bool valid) <br> |
|  bool | [**valid**](#function-valid-22) () const<br> |
| virtual  | [**~PositioningSource**](#function-positioningsource) () = default<br> |




























## Detailed Description


This class connects to GNSS devices (internal or bluetooth NMEA) and provides positioning details. 


    
## Public Types Documentation




### enum ElevationCorrectionMode 

```C++
enum PositioningSource::ElevationCorrectionMode {
    None,
    OrthometricFromDevice,
    OrthometricFromGeoidFile
};
```



Elevation correction modes 


        

<hr>



### enum NtripState 

```C++
enum PositioningSource::NtripState {
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
QString PositioningSource::backgroundFilePath;
```




<hr>
## Public Properties Documentation




### property active [1/2]

```C++
bool PositioningSource::active;
```




<hr>



### property antennaHeight [1/2]

```C++
double PositioningSource::antennaHeight;
```




<hr>



### property backgroundMode [1/2]

```C++
bool PositioningSource::backgroundMode;
```




<hr>



### property deviceBatteryLevel [1/2]

```C++
double PositioningSource::deviceBatteryLevel;
```




<hr>



### property deviceCapabilities [1/2]

```C++
int PositioningSource::deviceCapabilities;
```




<hr>



### property deviceDetails [1/2]

```C++
GnssPositionDetails PositioningSource::deviceDetails;
```




<hr>



### property deviceId [1/2]

```C++
QString PositioningSource::deviceId;
```




<hr>



### property deviceLastError [1/2]

```C++
QString PositioningSource::deviceLastError;
```




<hr>



### property deviceSocketState [1/2]

```C++
QAbstractSocket::SocketState PositioningSource::deviceSocketState;
```




<hr>



### property deviceSocketStateString [1/2]

```C++
QString PositioningSource::deviceSocketStateString;
```




<hr>



### property elevationCorrectionMode [1/2]

```C++
ElevationCorrectionMode PositioningSource::elevationCorrectionMode;
```




<hr>



### property enableNtrip [1/2]

```C++
bool PositioningSource::enableNtrip;
```




<hr>



### property logging [1/2]

```C++
bool PositioningSource::logging;
```




<hr>



### property loggingPath [1/2]

```C++
QString PositioningSource::loggingPath;
```




<hr>



### property ntripBytesReceived [1/2]

```C++
qint64 PositioningSource::ntripBytesReceived;
```




<hr>



### property ntripBytesSent [1/2]

```C++
qint64 PositioningSource::ntripBytesSent;
```




<hr>



### property ntripLastBytesReceivedUtcDateTime [1/2]

```C++
QDateTime PositioningSource::ntripLastBytesReceivedUtcDateTime;
```




<hr>



### property ntripLastError [1/2]

```C++
QString PositioningSource::ntripLastError;
```




<hr>



### property ntripSettings [1/2]

```C++
NtripSettings PositioningSource::ntripSettings;
```




<hr>



### property ntripState [1/2]

```C++
NtripState PositioningSource::ntripState;
```




<hr>



### property orientation [1/2]

```C++
double PositioningSource::orientation;
```




<hr>



### property positionInformation [1/2]

```C++
GnssPositionInformation PositioningSource::positionInformation;
```




<hr>



### property valid [1/2]

```C++
bool PositioningSource::valid;
```




<hr>
## Public Slots Documentation




### slot triggerConnectDevice 

```C++
void PositioningSource::triggerConnectDevice;
```




<hr>



### slot triggerDisconnectDevice 

```C++
void PositioningSource::triggerDisconnectDevice;
```




<hr>
## Public Signals Documentation




### signal activeChanged 

```C++
void PositioningSource::activeChanged;
```




<hr>



### signal antennaHeightChanged 

```C++
void PositioningSource::antennaHeightChanged;
```




<hr>



### signal backgroundModeChanged 

```C++
void PositioningSource::backgroundModeChanged;
```




<hr>



### signal deviceBatteryLevelChanged 

```C++
void PositioningSource::deviceBatteryLevelChanged;
```




<hr>



### signal deviceChanged 

```C++
void PositioningSource::deviceChanged;
```




<hr>



### signal deviceIdChanged 

```C++
void PositioningSource::deviceIdChanged;
```




<hr>



### signal deviceLastErrorChanged 

```C++
void PositioningSource::deviceLastErrorChanged;
```




<hr>



### signal deviceSocketStateChanged 

```C++
void PositioningSource::deviceSocketStateChanged;
```




<hr>



### signal deviceSocketStateStringChanged 

```C++
void PositioningSource::deviceSocketStateStringChanged;
```




<hr>



### signal elevationCorrectionModeChanged 

```C++
void PositioningSource::elevationCorrectionModeChanged;
```




<hr>



### signal enableNtripChanged 

```C++
void PositioningSource::enableNtripChanged;
```




<hr>



### signal loggingChanged 

```C++
void PositioningSource::loggingChanged;
```




<hr>



### signal loggingPathChanged 

```C++
void PositioningSource::loggingPathChanged;
```




<hr>



### signal ntripBytesReceivedChanged 

```C++
void PositioningSource::ntripBytesReceivedChanged;
```




<hr>



### signal ntripBytesSentChanged 

```C++
void PositioningSource::ntripBytesSentChanged;
```




<hr>



### signal ntripLastBytesReceivedUtcDateTimeChanged 

```C++
void PositioningSource::ntripLastBytesReceivedUtcDateTimeChanged;
```




<hr>



### signal ntripLastErrorChanged 

```C++
void PositioningSource::ntripLastErrorChanged;
```




<hr>



### signal ntripSettingsChanged 

```C++
void PositioningSource::ntripSettingsChanged;
```




<hr>



### signal ntripStateChanged 

```C++
void PositioningSource::ntripStateChanged;
```




<hr>



### signal orientationChanged 

```C++
void PositioningSource::orientationChanged;
```




<hr>



### signal positionInformationChanged 

```C++
void PositioningSource::positionInformationChanged;
```




<hr>



### signal validChanged 

```C++
void PositioningSource::validChanged;
```




<hr>
## Public Functions Documentation




### function PositioningSource 

```C++
explicit PositioningSource::PositioningSource (
    QObject * parent=nullptr
) 
```




<hr>



### function active [2/2]

```C++
inline bool PositioningSource::active () const
```



Returns TRUE when positioning is active. 

**See also:** [**setActive**](classPositioningSource.md#function-setactive) 



        

<hr>



### function antennaHeight [2/2]

```C++
inline double PositioningSource::antennaHeight () const
```



Sets the GNSS device antenna height. This should be the pole height + sensore phase height. 

**Note:**

When IMU is active this value is ignored as the device does the correction internally. 





        

<hr>



### function backgroundMode [2/2]

```C++
inline bool PositioningSource::backgroundMode () const
```



Returns TRUE if the background mode is active. When activated, position information details will not be signaled but instead saved to disk until deactivated. 

**See also:** [**getBackgroundPositionInformation()**](classPositioningSource.md#function-getbackgroundpositioninformation) 



        

<hr>



### function device 

```C++
inline AbstractGnssReceiver * PositioningSource::device () const
```



Returns the current positioning device. 

**See also:** deviceId 



        

<hr>



### function deviceBatteryLevel [2/2]

```C++
inline double PositioningSource::deviceBatteryLevel () const
```



Returns the device current battery level as a 0.0 to 1.0 double range. 


        

<hr>



### function deviceCapabilities [2/2]

```C++
int PositioningSource::deviceCapabilities () const
```



Returns device capabiities. 

**Note:**

Due to Qt Remote Object issues with flags, we use an int property. 





        

<hr>



### function deviceDetails [2/2]

```C++
inline GnssPositionDetails PositioningSource::deviceDetails () const
```



Returns extra details (such as hdop, vdop, pdop) provided by the positioning device. 


        

<hr>



### function deviceId [2/2]

```C++
inline QString PositioningSource::deviceId () const
```



Returns the current positioning device _id_ used to fetch position information. 

**See also:** setDevice 



        

<hr>



### function deviceLastError [2/2]

```C++
inline QString PositioningSource::deviceLastError () const
```



Returns positioning device's last error string. 


        

<hr>



### function deviceSocketState [2/2]

```C++
inline QAbstractSocket::SocketState PositioningSource::deviceSocketState () const
```



Returns positioning device's socket state. 

**See also:** deviceSocketStateString 



        

<hr>



### function deviceSocketStateString [2/2]

```C++
inline QString PositioningSource::deviceSocketStateString () const
```



Returns a string representation of the positioning device's socket state. 

**See also:** deviceSocketState 



        

<hr>



### function elevationCorrectionMode [2/2]

```C++
inline ElevationCorrectionMode PositioningSource::elevationCorrectionMode () const
```



Returns the current elevation correction mode. 

**Note:**

Some modes depends on device capabilities. 





        

<hr>



### function enableNtrip [2/2]

```C++
inline bool PositioningSource::enableNtrip () const
```



Returns TRUE if the NTRIP client is enabled. 


        

<hr>



### function getBackgroundPositionInformation 

```C++
Q_INVOKABLE QList< GnssPositionInformation > PositioningSource::getBackgroundPositionInformation () const
```



Returns a list of position information collected while background mode is active. 

**See also:** backgroundMode() 


**See also:** [**setBackgroundMode()**](classPositioningSource.md#function-setbackgroundmode) 



        

<hr>



### function logging [2/2]

```C++
inline bool PositioningSource::logging () const
```



Returns whether GNSS devices will log their incoming position stream into a logfile. 

**Note:**

Requires a device type with logging capability 





        

<hr>



### function loggingPath [2/2]

```C++
inline QString PositioningSource::loggingPath () const
```



Returns the path where GNSS devices will log their incoming position stream into logfiles. 

**Note:**

Requires a device type with logging capability 





        

<hr>



### function ntripBytesReceived [2/2]

```C++
inline qint64 PositioningSource::ntripBytesReceived () const
```



Returns the number of bytes received via NTRIP. 


        

<hr>



### function ntripBytesSent [2/2]

```C++
inline qint64 PositioningSource::ntripBytesSent () const
```



Returns the number of bytes sent via NTRIP. 


        

<hr>



### function ntripLastBytesReceivedUtcDateTime [2/2]

```C++
inline QDateTime PositioningSource::ntripLastBytesReceivedUtcDateTime () const
```



Returns the last received bytes time in UTC. 


        

<hr>



### function ntripLastError [2/2]

```C++
inline QString PositioningSource::ntripLastError () const
```



Returns the last NTRIP error string. 


        

<hr>



### function ntripSettings [2/2]

```C++
inline NtripSettings PositioningSource::ntripSettings () const
```



Returns the NTRIP settings. 


        

<hr>



### function ntripState [2/2]

```C++
inline NtripState PositioningSource::ntripState () const
```



Returns the current NTRIP connection state. 


        

<hr>



### function orientation [2/2]

```C++
inline double PositioningSource::orientation () const
```



Returns the current device orientation 


        

<hr>



### function positionInformation [2/2]

```C++
inline GnssPositionInformation PositioningSource::positionInformation () const
```



Returns a [**GnssPositionInformation**](classGnssPositionInformation.md) position information object. 


        

<hr>



### function setActive 

```C++
void PositioningSource::setActive (
    bool active
) 
```



Sets the positioning _active_ status. 

**See also:** active 



        

<hr>



### function setAntennaHeight 

```C++
void PositioningSource::setAntennaHeight (
    double antennaHeight
) 
```



Returns the GNSS device antenna height. This should be the pole height + sensore phase height. 

**Note:**

When IMU is active this value is ignored as the device does the correction internally. 





        

<hr>



### function setBackgroundMode 

```C++
void PositioningSource::setBackgroundMode (
    bool backgroundMode
) 
```



Sets whether the background mode is active. When activated, position information details will not be signaled but instead saved to disk until deactivated. 

**See also:** [**getBackgroundPositionInformation()**](classPositioningSource.md#function-getbackgroundpositioninformation) 



        

<hr>



### function setDeviceId 

```C++
void PositioningSource::setDeviceId (
    const QString & id
) 
```



Sets the positioning device _id_ used to fetch position information. 

**Note:**

A blank string will connect the internal positioning device; bluetooth addresses will trigger an NMEA connection to external devices. 





        

<hr>



### function setElevationCorrectionMode 

```C++
void PositioningSource::setElevationCorrectionMode (
    ElevationCorrectionMode elevationCorrectionMode
) 
```



Sets the current elevation correction mode. 

**Note:**

Some modes depends on device capabilities. 





        

<hr>



### function setEnableNtrip 

```C++
void PositioningSource::setEnableNtrip (
    bool enableNtrip
) 
```



Sets whether the NTRIP client is enabled. 


        

<hr>



### function setLogging 

```C++
void PositioningSource::setLogging (
    bool logging
) 
```



Sets whether GNSS devices will log their incoming position stream into a logfile. 

**Note:**

Requires a device type with logging capability 





        

<hr>



### function setLoggingPath 

```C++
void PositioningSource::setLoggingPath (
    const QString & path
) 
```



Sets the path where GNSS devices will log their incoming position stream into logfiles. 

**Note:**

Requires a device type with logging capability 





        

<hr>



### function setNtripSettings 

```C++
void PositioningSource::setNtripSettings (
    const NtripSettings & ntripSettings
) 
```



Sets the NTRIP settings. 


        

<hr>



### function setValid 

```C++
void PositioningSource::setValid (
    bool valid
) 
```



Sets whether the positioning device is valid. 


        

<hr>



### function valid [2/2]

```C++
inline bool PositioningSource::valid () const
```



Returns TRUE when the positioning device is valid. 


        

<hr>



### function ~PositioningSource 

```C++
virtual PositioningSource::~PositioningSource () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/positioning/positioningsource.h`

