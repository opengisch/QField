

# Class Positioning



[**ClassList**](annotated.md) **>** [**Positioning**](classPositioning.md)



[More...](#detailed-description)

* `#include <positioning.h>`



Inherits the following classes: QObject


























## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**active**](classPositioning.md#property-active-12)  <br> |
| property double | [**antennaHeight**](classPositioning.md#property-antennaheight-12)  <br> |
| property bool | [**averagedPosition**](classPositioning.md#property-averagedposition-12)  <br> |
| property int | [**averagedPositionCount**](classPositioning.md#property-averagedpositioncount-12)  <br> |
| property bool | [**averagedPositionFilterAccuracy**](classPositioning.md#property-averagedpositionfilteraccuracy-12)  <br> |
| property bool | [**backgroundMode**](classPositioning.md#property-backgroundmode-12)  <br> |
| property double | [**badAccuracyThreshold**](classPositioning.md#property-badaccuracythreshold-12)  <br> |
| property [**QgsQuickCoordinateTransformer**](classQgsQuickCoordinateTransformer.md) \* | [**coordinateTransformer**](classPositioning.md#property-coordinatetransformer-12)  <br> |
| property AbstractGnssReceiver::Capabilities | [**deviceCapabilities**](classPositioning.md#property-devicecapabilities-12)  <br> |
| property QString | [**deviceId**](classPositioning.md#property-deviceid-12)  <br> |
| property QString | [**deviceLastError**](classPositioning.md#property-devicelasterror-12)  <br> |
| property QAbstractSocket::SocketState | [**deviceSocketState**](classPositioning.md#property-devicesocketstate-12)  <br> |
| property QString | [**deviceSocketStateString**](classPositioning.md#property-devicesocketstatestring-12)  <br> |
| property [**PositioningSource::ElevationCorrectionMode**](classPositioningSource.md#enum-elevationcorrectionmode) | [**elevationCorrectionMode**](classPositioning.md#property-elevationcorrectionmode-12)  <br> |
| property double | [**excellentAccuracyThreshold**](classPositioning.md#property-excellentaccuracythreshold-12)  <br> |
| property bool | [**logging**](classPositioning.md#property-logging-12)  <br> |
| property QString | [**loggingPath**](classPositioning.md#property-loggingpath-12)  <br> |
| property double | [**orientation**](classPositioning.md#property-orientation-12)  <br> |
| property [**GnssPositionInformation**](classGnssPositionInformation.md) | [**positionInformation**](classPositioning.md#property-positioninformation-12)  <br> |
| property double | [**projectedHorizontalAccuracy**](classPositioning.md#property-projectedhorizontalaccuracy-12)  <br> |
| property QgsPoint | [**projectedPosition**](classPositioning.md#property-projectedposition-12)  <br> |
| property bool | [**serviceMode**](classPositioning.md#property-servicemode-12)  <br> |
| property QgsPoint | [**sourcePosition**](classPositioning.md#property-sourceposition-12)  <br> |
| property bool | [**valid**](classPositioning.md#property-valid-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**activeChanged**](classPositioning.md#signal-activechanged)  <br> |
| signal void | [**antennaHeightChanged**](classPositioning.md#signal-antennaheightchanged)  <br> |
| signal void | [**averagedPositionChanged**](classPositioning.md#signal-averagedpositionchanged)  <br> |
| signal void | [**averagedPositionCountChanged**](classPositioning.md#signal-averagedpositioncountchanged)  <br> |
| signal void | [**averagedPositionFilterAccuracyChanged**](classPositioning.md#signal-averagedpositionfilteraccuracychanged)  <br> |
| signal void | [**backgroundModeChanged**](classPositioning.md#signal-backgroundmodechanged)  <br> |
| signal void | [**badAccuracyThresholdChanged**](classPositioning.md#signal-badaccuracythresholdchanged)  <br> |
| signal void | [**coordinateTransformerChanged**](classPositioning.md#signal-coordinatetransformerchanged)  <br> |
| signal void | [**deviceIdChanged**](classPositioning.md#signal-deviceidchanged)  <br> |
| signal void | [**deviceLastErrorChanged**](classPositioning.md#signal-devicelasterrorchanged)  <br> |
| signal void | [**deviceSocketStateChanged**](classPositioning.md#signal-devicesocketstatechanged)  <br> |
| signal void | [**deviceSocketStateStringChanged**](classPositioning.md#signal-devicesocketstatestringchanged)  <br> |
| signal void | [**elevationCorrectionModeChanged**](classPositioning.md#signal-elevationcorrectionmodechanged)  <br> |
| signal void | [**excellentAccuracyThresholdChanged**](classPositioning.md#signal-excellentaccuracythresholdchanged)  <br> |
| signal void | [**loggingChanged**](classPositioning.md#signal-loggingchanged)  <br> |
| signal void | [**loggingPathChanged**](classPositioning.md#signal-loggingpathchanged)  <br> |
| signal void | [**orientationChanged**](classPositioning.md#signal-orientationchanged)  <br> |
| signal void | [**positionInformationChanged**](classPositioning.md#signal-positioninformationchanged)  <br> |
| signal void | [**projectedPositionChanged**](classPositioning.md#signal-projectedpositionchanged)  <br> |
| signal void | [**serviceModeChanged**](classPositioning.md#signal-servicemodechanged)  <br> |
| signal void | [**triggerConnectDevice**](classPositioning.md#signal-triggerconnectdevice)  <br> |
| signal void | [**triggerDisconnectDevice**](classPositioning.md#signal-triggerdisconnectdevice)  <br> |
| signal void | [**validChanged**](classPositioning.md#signal-validchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Positioning**](#function-positioning) (QObject \* parent=nullptr) <br> |
|  bool | [**active**](#function-active-22) () const<br> |
|  double | [**antennaHeight**](#function-antennaheight-22) () const<br> |
|  bool | [**averagedPosition**](#function-averagedposition-22) () const<br> |
|  int | [**averagedPositionCount**](#function-averagedpositioncount-22) () const<br> |
|  bool | [**averagedPositionFilterAccuracy**](#function-averagedpositionfilteraccuracy-22) () const<br> |
|  bool | [**backgroundMode**](#function-backgroundmode-22) () const<br> |
|  double | [**badAccuracyThreshold**](#function-badaccuracythreshold-22) () const<br> |
|  [**QgsQuickCoordinateTransformer**](classQgsQuickCoordinateTransformer.md) \* | [**coordinateTransformer**](#function-coordinatetransformer-22) () const<br> |
|  AbstractGnssReceiver::Capabilities | [**deviceCapabilities**](#function-devicecapabilities-22) () const<br> |
|  [**GnssPositionDetails**](classGnssPositionDetails.md) | [**deviceDetails**](#function-devicedetails) () const<br> |
|  QString | [**deviceId**](#function-deviceid-22) () const<br> |
|  QString | [**deviceLastError**](#function-devicelasterror-22) () const<br> |
|  QAbstractSocket::SocketState | [**deviceSocketState**](#function-devicesocketstate-22) () const<br> |
|  QString | [**deviceSocketStateString**](#function-devicesocketstatestring-22) () const<br> |
|  [**PositioningSource::ElevationCorrectionMode**](classPositioningSource.md#enum-elevationcorrectionmode) | [**elevationCorrectionMode**](#function-elevationcorrectionmode-22) () const<br> |
|  double | [**excellentAccuracyThreshold**](#function-excellentaccuracythreshold-22) () const<br> |
|  Q\_INVOKABLE QList&lt; [**GnssPositionInformation**](classGnssPositionInformation.md) &gt; | [**getBackgroundPositionInformation**](#function-getbackgroundpositioninformation) () const<br> |
|  bool | [**logging**](#function-logging-22) () const<br> |
|  QString | [**loggingPath**](#function-loggingpath-22) () const<br> |
|  double | [**orientation**](#function-orientation-22) () const<br> |
|  [**GnssPositionInformation**](classGnssPositionInformation.md) | [**positionInformation**](#function-positioninformation-22) () const<br> |
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
|  void | [**setElevationCorrectionMode**](#function-setelevationcorrectionmode) ([**PositioningSource::ElevationCorrectionMode**](classPositioningSource.md#enum-elevationcorrectionmode) elevationCorrectionMode) <br> |
|  void | [**setExcellentAccuracyThreshold**](#function-setexcellentaccuracythreshold) (double threshold) <br> |
|  void | [**setLogging**](#function-setlogging) (bool logging) <br> |
|  void | [**setLoggingPath**](#function-setloggingpath) (const QString & path) <br> |
|  void | [**setServiceMode**](#function-setservicemode) (bool enabled) <br> |
|  void | [**setValid**](#function-setvalid) (bool valid) <br> |
|  QgsPoint | [**sourcePosition**](#function-sourceposition-22) () const<br> |
|  bool | [**valid**](#function-valid-22) () const<br> |
| virtual  | [**~Positioning**](#function-positioning) () = default<br> |




























## Detailed Description


This class manages the positioning source and offers positioning details. 


    
## Public Properties Documentation




### property active [1/2]

```C++
bool Positioning::active;
```




<hr>



### property antennaHeight [1/2]

```C++
double Positioning::antennaHeight;
```




<hr>



### property averagedPosition [1/2]

```C++
bool Positioning::averagedPosition;
```




<hr>



### property averagedPositionCount [1/2]

```C++
int Positioning::averagedPositionCount;
```




<hr>



### property averagedPositionFilterAccuracy [1/2]

```C++
bool Positioning::averagedPositionFilterAccuracy;
```




<hr>



### property backgroundMode [1/2]

```C++
bool Positioning::backgroundMode;
```




<hr>



### property badAccuracyThreshold [1/2]

```C++
double Positioning::badAccuracyThreshold;
```




<hr>



### property coordinateTransformer [1/2]

```C++
QgsQuickCoordinateTransformer * Positioning::coordinateTransformer;
```




<hr>



### property deviceCapabilities [1/2]

```C++
AbstractGnssReceiver::Capabilities Positioning::deviceCapabilities;
```




<hr>



### property deviceId [1/2]

```C++
QString Positioning::deviceId;
```




<hr>



### property deviceLastError [1/2]

```C++
QString Positioning::deviceLastError;
```




<hr>



### property deviceSocketState [1/2]

```C++
QAbstractSocket::SocketState Positioning::deviceSocketState;
```




<hr>



### property deviceSocketStateString [1/2]

```C++
QString Positioning::deviceSocketStateString;
```




<hr>



### property elevationCorrectionMode [1/2]

```C++
PositioningSource::ElevationCorrectionMode Positioning::elevationCorrectionMode;
```




<hr>



### property excellentAccuracyThreshold [1/2]

```C++
double Positioning::excellentAccuracyThreshold;
```




<hr>



### property logging [1/2]

```C++
bool Positioning::logging;
```




<hr>



### property loggingPath [1/2]

```C++
QString Positioning::loggingPath;
```




<hr>



### property orientation [1/2]

```C++
double Positioning::orientation;
```




<hr>



### property positionInformation [1/2]

```C++
GnssPositionInformation Positioning::positionInformation;
```




<hr>



### property projectedHorizontalAccuracy [1/2]

```C++
double Positioning::projectedHorizontalAccuracy;
```




<hr>



### property projectedPosition [1/2]

```C++
QgsPoint Positioning::projectedPosition;
```




<hr>



### property serviceMode [1/2]

```C++
bool Positioning::serviceMode;
```




<hr>



### property sourcePosition [1/2]

```C++
QgsPoint Positioning::sourcePosition;
```




<hr>



### property valid [1/2]

```C++
bool Positioning::valid;
```




<hr>
## Public Signals Documentation




### signal activeChanged 

```C++
void Positioning::activeChanged;
```




<hr>



### signal antennaHeightChanged 

```C++
void Positioning::antennaHeightChanged;
```




<hr>



### signal averagedPositionChanged 

```C++
void Positioning::averagedPositionChanged;
```




<hr>



### signal averagedPositionCountChanged 

```C++
void Positioning::averagedPositionCountChanged;
```




<hr>



### signal averagedPositionFilterAccuracyChanged 

```C++
void Positioning::averagedPositionFilterAccuracyChanged;
```




<hr>



### signal backgroundModeChanged 

```C++
void Positioning::backgroundModeChanged;
```




<hr>



### signal badAccuracyThresholdChanged 

```C++
void Positioning::badAccuracyThresholdChanged;
```




<hr>



### signal coordinateTransformerChanged 

```C++
void Positioning::coordinateTransformerChanged;
```




<hr>



### signal deviceIdChanged 

```C++
void Positioning::deviceIdChanged;
```




<hr>



### signal deviceLastErrorChanged 

```C++
void Positioning::deviceLastErrorChanged;
```




<hr>



### signal deviceSocketStateChanged 

```C++
void Positioning::deviceSocketStateChanged;
```




<hr>



### signal deviceSocketStateStringChanged 

```C++
void Positioning::deviceSocketStateStringChanged;
```




<hr>



### signal elevationCorrectionModeChanged 

```C++
void Positioning::elevationCorrectionModeChanged;
```




<hr>



### signal excellentAccuracyThresholdChanged 

```C++
void Positioning::excellentAccuracyThresholdChanged;
```




<hr>



### signal loggingChanged 

```C++
void Positioning::loggingChanged;
```




<hr>



### signal loggingPathChanged 

```C++
void Positioning::loggingPathChanged;
```




<hr>



### signal orientationChanged 

```C++
void Positioning::orientationChanged;
```




<hr>



### signal positionInformationChanged 

```C++
void Positioning::positionInformationChanged;
```




<hr>



### signal projectedPositionChanged 

```C++
void Positioning::projectedPositionChanged;
```




<hr>



### signal serviceModeChanged 

```C++
void Positioning::serviceModeChanged;
```




<hr>



### signal triggerConnectDevice 

```C++
void Positioning::triggerConnectDevice;
```




<hr>



### signal triggerDisconnectDevice 

```C++
void Positioning::triggerDisconnectDevice;
```




<hr>



### signal validChanged 

```C++
void Positioning::validChanged;
```




<hr>
## Public Functions Documentation




### function Positioning 

```C++
explicit Positioning::Positioning (
    QObject * parent=nullptr
) 
```




<hr>



### function active [2/2]

```C++
bool Positioning::active () const
```



Returns TRUE when positioning is active. 

**See also:** [**setActive**](classPositioning.md#function-setactive) 



        

<hr>



### function antennaHeight [2/2]

```C++
double Positioning::antennaHeight () const
```



Sets the GNSS device antenna height. This should be the pole height + sensore phase height. 

**Note:**

When IMU is active this value is ignored as the device does the correction internally. 





        

<hr>



### function averagedPosition [2/2]

```C++
bool Positioning::averagedPosition () const
```



Returns whether the position information is averaged from an ongoing stream of incoming positions from the device. 


        

<hr>



### function averagedPositionCount [2/2]

```C++
int Positioning::averagedPositionCount () const
```



Returns the current number of collected position informations from which the averaged position is calculated. 

**Note:**

When averaged position is off, the value is zero. 





        

<hr>



### function averagedPositionFilterAccuracy [2/2]

```C++
bool Positioning::averagedPositionFilterAccuracy () const
```



Returns whether the average position filter accuracy is enabled. 


        

<hr>



### function backgroundMode [2/2]

```C++
bool Positioning::backgroundMode () const
```



Returns TRUE if the background mode is active. When activated, position information details will not be signaled but instead saved to disk until deactivated. 

**See also:** [**getBackgroundPositionInformation()**](classPositioning.md#function-getbackgroundpositioninformation) 



        

<hr>



### function badAccuracyThreshold [2/2]

```C++
inline double Positioning::badAccuracyThreshold () const
```



Returns the threshold above which accuracy is considered bad. 


        

<hr>



### function coordinateTransformer [2/2]

```C++
inline QgsQuickCoordinateTransformer * Positioning::coordinateTransformer () const
```



Returns the coordinate transformer object used to reproject the position location. 


        

<hr>



### function deviceCapabilities [2/2]

```C++
AbstractGnssReceiver::Capabilities Positioning::deviceCapabilities () const
```




<hr>



### function deviceDetails 

```C++
GnssPositionDetails Positioning::deviceDetails () const
```



Returns extra details (such as hdop, vdop, pdop) provided by the positioning device. 


        

<hr>



### function deviceId [2/2]

```C++
QString Positioning::deviceId () const
```



Returns the current positioning device _id_ used to fetch position information. 

**See also:** setDevice 



        

<hr>



### function deviceLastError [2/2]

```C++
QString Positioning::deviceLastError () const
```



Returns positioning device's last error string. 


        

<hr>



### function deviceSocketState [2/2]

```C++
QAbstractSocket::SocketState Positioning::deviceSocketState () const
```



Returns positioning device's socket state. 

**See also:** deviceSocketStateString 



        

<hr>



### function deviceSocketStateString [2/2]

```C++
QString Positioning::deviceSocketStateString () const
```



Returns a string representation of the positioning device's socket state. 

**See also:** deviceSocketState 



        

<hr>



### function elevationCorrectionMode [2/2]

```C++
PositioningSource::ElevationCorrectionMode Positioning::elevationCorrectionMode () const
```



Returns the current elevation correction mode. 

**Note:**

Some modes depends on device capabilities. 





        

<hr>



### function excellentAccuracyThreshold [2/2]

```C++
inline double Positioning::excellentAccuracyThreshold () const
```



Returns the threshold below which accuracy is considered excellent. 


        

<hr>



### function getBackgroundPositionInformation 

```C++
Q_INVOKABLE QList< GnssPositionInformation > Positioning::getBackgroundPositionInformation () const
```



Returns a list of position information collected while background mode is active. 

**See also:** backgroundMode() 


**See also:** [**setBackgroundMode()**](classPositioning.md#function-setbackgroundmode) 



        

<hr>



### function logging [2/2]

```C++
bool Positioning::logging () const
```



Returns whether GNSS devices will log their incoming position stream into a logfile. 

**Note:**

Requires a device type with logging capability 





        

<hr>



### function loggingPath [2/2]

```C++
QString Positioning::loggingPath () const
```



Returns the path where GNSS devices will log their incoming position stream into logfiles. 

**Note:**

Requires a device type with logging capability 





        

<hr>



### function orientation [2/2]

```C++
double Positioning::orientation () const
```



Returns the current device orientation 


        

<hr>



### function positionInformation [2/2]

```C++
GnssPositionInformation Positioning::positionInformation () const
```



Returns a [**GnssPositionInformation**](classGnssPositionInformation.md) position information object. 


        

<hr>



### function projectedHorizontalAccuracy [2/2]

```C++
double Positioning::projectedHorizontalAccuracy () const
```



Returns the position horizontal accuracy in the destination CRS' map units. 


        

<hr>



### function projectedPosition [2/2]

```C++
QgsPoint Positioning::projectedPosition () const
```



Returns the position point in its destination CRS defined in the coordinate transformer. 


        

<hr>



### function serviceMode [2/2]

```C++
bool Positioning::serviceMode () const
```



Returns TRUE if the service mode is active. On supported platform, positioning runs into a dedicated service that is not suspended when devices are locked or the application is put suspended. 

**See also:** [**setServiceMode()**](classPositioning.md#function-setservicemode) 



        

<hr>



### function setActive 

```C++
void Positioning::setActive (
    bool active
) 
```



Sets the positioning _active_ status. 

**See also:** active 



        

<hr>



### function setAntennaHeight 

```C++
void Positioning::setAntennaHeight (
    double antennaHeight
) 
```



Returns the GNSS device antenna height. This should be the pole height + sensore phase height. 

**Note:**

When IMU is active this value is ignored as the device does the correction internally. 





        

<hr>



### function setAveragedPosition 

```C++
void Positioning::setAveragedPosition (
    bool averaged
) 
```



Sets whether the position information is _averaged_ from an ongoing stream of incoming positions from the device. 


        

<hr>



### function setAveragedPositionFilterAccuracy 

```C++
void Positioning::setAveragedPositionFilterAccuracy (
    bool enabled
) 
```



Enables or disables the average position filter accuracy. 


        

<hr>



### function setBackgroundMode 

```C++
void Positioning::setBackgroundMode (
    bool enabled
) 
```



Sets whether the background mode is active. When activated, position information details will not be signaled but instead saved to disk until deactivated. 

**See also:** [**getBackgroundPositionInformation()**](classPositioning.md#function-getbackgroundpositioninformation) 



        

<hr>



### function setBadAccuracyThreshold 

```C++
void Positioning::setBadAccuracyThreshold (
    double threshold
) 
```



Sets the threshold above which accuracy is considered bad. 


        

<hr>



### function setCoordinateTransformer 

```C++
void Positioning::setCoordinateTransformer (
    QgsQuickCoordinateTransformer * coordinateTransformer
) 
```



Sets the coordinate transformer object used to reproject the position location. 

**Parameters:**


* `coordinateTransformer` A [**QgsQuickCoordinateTransformer**](classQgsQuickCoordinateTransformer.md) object. 




        

<hr>



### function setDeviceId 

```C++
void Positioning::setDeviceId (
    const QString & id
) 
```



Sets the positioning device _id_ used to fetch position information. 

**Note:**

A blank string will connect the internal positioning device; bluetooth addresses will trigger an NMEA connection to external devices. 





        

<hr>



### function setElevationCorrectionMode 

```C++
void Positioning::setElevationCorrectionMode (
    PositioningSource::ElevationCorrectionMode elevationCorrectionMode
) 
```



Sets the current elevation correction mode. 

**Note:**

Some modes depends on device capabilities. 





        

<hr>



### function setExcellentAccuracyThreshold 

```C++
void Positioning::setExcellentAccuracyThreshold (
    double threshold
) 
```



Sets the threshold below which accuracy is considered excellent. 


        

<hr>



### function setLogging 

```C++
void Positioning::setLogging (
    bool logging
) 
```



Sets whether GNSS devices will log their incoming position stream into a logfile. 

**Note:**

Requires a device type with logging capability 





        

<hr>



### function setLoggingPath 

```C++
void Positioning::setLoggingPath (
    const QString & path
) 
```



Sets the path where GNSS devices will log their incoming position stream into logfiles. 

**Note:**

Requires a device type with logging capability 





        

<hr>



### function setServiceMode 

```C++
void Positioning::setServiceMode (
    bool enabled
) 
```



Sets whether the service mode is active. On supported platform, positioning runs into a dedicated service that is not suspended when devices are locked or the application is put suspended. 

**See also:** serviceMode() 



        

<hr>



### function setValid 

```C++
void Positioning::setValid (
    bool valid
) 
```



Sets whether the positioning device is valid. 


        

<hr>



### function sourcePosition [2/2]

```C++
QgsPoint Positioning::sourcePosition () const
```



Returns the position point in its original WGS84 projection. 


        

<hr>



### function valid [2/2]

```C++
bool Positioning::valid () const
```



Returns TRUE when the positioning device is valid. 


        

<hr>



### function ~Positioning 

```C++
virtual Positioning::~Positioning () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/positioning/positioning.h`

