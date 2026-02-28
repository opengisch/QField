

# Class GnssPositionInformation



[**ClassList**](annotated.md) **>** [**GnssPositionInformation**](classGnssPositionInformation.md)






















## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**AccuracyQuality**](#enum-accuracyquality)  <br> |
| enum  | [**FixStatus**](#enum-fixstatus)  <br> |












## Public Properties

| Type | Name |
| ---: | :--- |
| property AccuracyQuality | [**accuracyQuality**](classGnssPositionInformation.md#property-accuracyquality-12)  <br> |
| property int | [**averagedCount**](classGnssPositionInformation.md#property-averagedcount-12)  <br> |
| property double | [**direction**](classGnssPositionInformation.md#property-direction-12)  <br> |
| property bool | [**directionValid**](classGnssPositionInformation.md#property-directionvalid-12)  <br> |
| property double | [**elevation**](classGnssPositionInformation.md#property-elevation-12)  <br> |
| property bool | [**elevationValid**](classGnssPositionInformation.md#property-elevationvalid-12)  <br> |
| property QChar | [**fixMode**](classGnssPositionInformation.md#property-fixmode-12)  <br> |
| property [**FixStatus**](classGnssPositionInformation.md#enum-fixstatus) | [**fixStatus**](classGnssPositionInformation.md#property-fixstatus-12)  <br> |
| property QString | [**fixStatusDescription**](classGnssPositionInformation.md#property-fixstatusdescription-12)  <br> |
| property int | [**fixType**](classGnssPositionInformation.md#property-fixtype-12)  <br> |
| property double | [**hacc**](classGnssPositionInformation.md#property-hacc-12)  <br> |
| property bool | [**haccValid**](classGnssPositionInformation.md#property-haccvalid-12)  <br> |
| property double | [**hdop**](classGnssPositionInformation.md#property-hdop-12)  <br> |
| property double | [**hvacc**](classGnssPositionInformation.md#property-hvacc-12)  <br> |
| property double | [**hvaccValid**](classGnssPositionInformation.md#property-hvaccvalid-12)  <br> |
| property bool | [**imuCorrection**](classGnssPositionInformation.md#property-imucorrection-12)  <br> |
| property double | [**imuHeading**](classGnssPositionInformation.md#property-imuheading-12)  <br> |
| property bool | [**imuHeadingValid**](classGnssPositionInformation.md#property-imuheadingvalid-12)  <br> |
| property double | [**imuPitch**](classGnssPositionInformation.md#property-imupitch-12)  <br> |
| property bool | [**imuPitchValid**](classGnssPositionInformation.md#property-imupitchvalid-12)  <br> |
| property double | [**imuRoll**](classGnssPositionInformation.md#property-imuroll-12)  <br> |
| property bool | [**imuRollValid**](classGnssPositionInformation.md#property-imurollvalid-12)  <br> |
| property double | [**imuSteering**](classGnssPositionInformation.md#property-imusteering-12)  <br> |
| property bool | [**imuSteeringValid**](classGnssPositionInformation.md#property-imusteeringvalid-12)  <br> |
| property bool | [**isValid**](classGnssPositionInformation.md#property-isvalid-12)  <br> |
| property double | [**latitude**](classGnssPositionInformation.md#property-latitude-12)  <br> |
| property bool | [**latitudeValid**](classGnssPositionInformation.md#property-latitudevalid-12)  <br> |
| property double | [**longitude**](classGnssPositionInformation.md#property-longitude-12)  <br> |
| property bool | [**longitudeValid**](classGnssPositionInformation.md#property-longitudevalid-12)  <br> |
| property double | [**magneticVariation**](classGnssPositionInformation.md#property-magneticvariation-12)  <br> |
| property double | [**orientation**](classGnssPositionInformation.md#property-orientation-12)  <br> |
| property bool | [**orientationValid**](classGnssPositionInformation.md#property-orientationvalid-12)  <br> |
| property double | [**pdop**](classGnssPositionInformation.md#property-pdop-12)  <br> |
| property int | [**quality**](classGnssPositionInformation.md#property-quality-12)  <br> |
| property QString | [**qualityDescription**](classGnssPositionInformation.md#property-qualitydescription-12)  <br> |
| property bool | [**satInfoComplete**](classGnssPositionInformation.md#property-satinfocomplete-12)  <br> |
| property QList&lt; int &gt; | [**satPrn**](classGnssPositionInformation.md#property-satprn-12)  <br> |
| property QList&lt; QgsSatelliteInfo &gt; | [**satellitesInView**](classGnssPositionInformation.md#property-satellitesinview-12)  <br> |
| property int | [**satellitesUsed**](classGnssPositionInformation.md#property-satellitesused-12)  <br> |
| property QString | [**sourceName**](classGnssPositionInformation.md#property-sourcename-12)  <br> |
| property double | [**speed**](classGnssPositionInformation.md#property-speed-12)  <br> |
| property bool | [**speedValid**](classGnssPositionInformation.md#property-speedvalid-12)  <br> |
| property QChar | [**status**](classGnssPositionInformation.md#property-status-12)  <br> |
| property QDateTime | [**utcDateTime**](classGnssPositionInformation.md#property-utcdatetime-12)  <br> |
| property double | [**vacc**](classGnssPositionInformation.md#property-vacc-12)  <br> |
| property bool | [**vaccValid**](classGnssPositionInformation.md#property-vaccvalid-12)  <br> |
| property double | [**vdop**](classGnssPositionInformation.md#property-vdop-12)  <br> |
| property double | [**verticalSpeed**](classGnssPositionInformation.md#property-verticalspeed-12)  <br> |








## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**GnssPositionInformation**](#function-gnsspositioninformation) (double latitude=std::numeric\_limits&lt; double &gt;::quiet\_NaN(), double longitude=std::numeric\_limits&lt; double &gt;::quiet\_NaN(), double elevation=std::numeric\_limits&lt; double &gt;::quiet\_NaN(), double speed=std::numeric\_limits&lt; double &gt;::quiet\_NaN(), double direction=std::numeric\_limits&lt; double &gt;::quiet\_NaN(), const QList&lt; QgsSatelliteInfo &gt; & satellitesInView=QList&lt; QgsSatelliteInfo &gt;(), double pdop=0, double hdop=0, double vdop=0, double hacc=std::numeric\_limits&lt; double &gt;::quiet\_NaN(), double vacc=std::numeric\_limits&lt; double &gt;::quiet\_NaN(), QDateTime utcDateTime=QDateTime(), QChar fixMode=QChar(), int fixType=0, int quality=-1, int satellitesUsed=0, QChar status=QChar(), const QList&lt; int &gt; & satPrn=QList&lt; int &gt;(), bool satInfoComplete=false, double verticalSpeed=std::numeric\_limits&lt; double &gt;::quiet\_NaN(), double magneticVariation=std::numeric\_limits&lt; double &gt;::quiet\_NaN(), int averagedCount=0, const QString & sourceName=QString(), bool imuCorrection=false, double imuRoll=std::numeric\_limits&lt; double &gt;::quiet\_NaN(), double imuPitch=std::numeric\_limits&lt; double &gt;::quiet\_NaN(), double imuHeading=std::numeric\_limits&lt; double &gt;::quiet\_NaN(), double imuSteering=std::numeric\_limits&lt; double &gt;::quiet\_NaN(), double orientation=std::numeric\_limits&lt; double &gt;::quiet\_NaN()) <br> |
|  AccuracyQuality | [**accuracyQuality**](#function-accuracyquality-22) () const<br> |
|  int | [**averagedCount**](#function-averagedcount-22) () const<br> |
|  double | [**direction**](#function-direction-22) () const<br> |
|  bool | [**directionValid**](#function-directionvalid-22) () const<br> |
|  double | [**elevation**](#function-elevation-22) () const<br> |
|  bool | [**elevationValid**](#function-elevationvalid-22) () const<br> |
|  QChar | [**fixMode**](#function-fixmode-22) () const<br> |
|  [**FixStatus**](classGnssPositionInformation.md#enum-fixstatus) | [**fixStatus**](#function-fixstatus-22) () const<br> |
|  QString | [**fixStatusDescription**](#function-fixstatusdescription-22) () const<br> |
|  int | [**fixType**](#function-fixtype-22) () const<br> |
|  double | [**hacc**](#function-hacc-22) () const<br> |
|  bool | [**haccValid**](#function-haccvalid-22) () const<br> |
|  double | [**hdop**](#function-hdop-22) () const<br> |
|  double | [**hvacc**](#function-hvacc-22) () const<br> |
|  bool | [**hvaccValid**](#function-hvaccvalid-22) () const<br> |
|  bool | [**imuCorrection**](#function-imucorrection-22) () const<br> |
|  double | [**imuHeading**](#function-imuheading-22) () const<br> |
|  bool | [**imuHeadingValid**](#function-imuheadingvalid-22) () const<br> |
|  double | [**imuPitch**](#function-imupitch-22) () const<br> |
|  bool | [**imuPitchValid**](#function-imupitchvalid-22) () const<br> |
|  double | [**imuRoll**](#function-imuroll-22) () const<br> |
|  bool | [**imuRollValid**](#function-imurollvalid-22) () const<br> |
|  double | [**imuSteering**](#function-imusteering-22) () const<br> |
|  bool | [**imuSteeringValid**](#function-imusteeringvalid-22) () const<br> |
|  bool | [**isValid**](#function-isvalid-22) () const<br> |
|  double | [**latitude**](#function-latitude-22) () const<br> |
|  bool | [**latitudeValid**](#function-latitudevalid-22) () const<br> |
|  double | [**longitude**](#function-longitude-22) () const<br> |
|  bool | [**longitudeValid**](#function-longitudevalid-22) () const<br> |
|  double | [**magneticVariation**](#function-magneticvariation-22) () const<br> |
|  bool | [**operator!=**](#function-operator) (const [**GnssPositionInformation**](classGnssPositionInformation.md) & other) const<br> |
|  bool | [**operator==**](#function-operator_1) (const [**GnssPositionInformation**](classGnssPositionInformation.md) & other) const<br> |
|  double | [**orientation**](#function-orientation-22) () const<br> |
|  bool | [**orientationValid**](#function-orientationvalid-22) () const<br> |
|  double | [**pdop**](#function-pdop-22) () const<br> |
|  int | [**quality**](#function-quality-22) () const<br> |
|  QString | [**qualityDescription**](#function-qualitydescription-22) () const<br> |
|  bool | [**satInfoComplete**](#function-satinfocomplete-22) () const<br> |
|  QList&lt; int &gt; | [**satPrn**](#function-satprn-22) () const<br> |
|  QList&lt; QgsSatelliteInfo &gt; | [**satellitesInView**](#function-satellitesinview-22) () const<br> |
|  int | [**satellitesUsed**](#function-satellitesused-22) () const<br> |
|  void | [**setAccuracyQuality**](#function-setaccuracyquality) (AccuracyQuality quality) <br> |
|  void | [**setAveragedCount**](#function-setaveragedcount) (int averagedCount) <br> |
|  void | [**setDirection**](#function-setdirection) (double direction) <br> |
|  void | [**setElevation**](#function-setelevation) (double elevation) <br> |
|  void | [**setFixMode**](#function-setfixmode) (QChar fixMode) <br> |
|  void | [**setFixType**](#function-setfixtype) (int fixType) <br> |
|  void | [**setHVacc**](#function-sethvacc) (double hvacc) <br> |
|  void | [**setHacc**](#function-sethacc) (double hacc) <br> |
|  void | [**setHdop**](#function-sethdop) (double hdop) <br> |
|  void | [**setImuCorrection**](#function-setimucorrection) (bool imuCorrection) <br> |
|  void | [**setImuHeading**](#function-setimuheading) (double heading) <br> |
|  void | [**setImuPitch**](#function-setimupitch) (double pitch) <br> |
|  void | [**setImuRoll**](#function-setimuroll) (double roll) <br> |
|  void | [**setImuSteering**](#function-setimusteering) (double steering) <br> |
|  void | [**setLatitude**](#function-setlatitude) (double latitude) <br> |
|  void | [**setLongitude**](#function-setlongitude) (double longitude) <br> |
|  void | [**setMagneticVaritation**](#function-setmagneticvaritation) (double magneticVariation) <br> |
|  void | [**setOrientation**](#function-setorientation) (double orientation) <br> |
|  void | [**setPdop**](#function-setpdop) (double pdop) <br> |
|  void | [**setQuality**](#function-setquality) (int quality) <br> |
|  void | [**setSatInfoComplete**](#function-setsatinfocomplete) (bool satInfoComplete) <br> |
|  void | [**setSatPrn**](#function-setsatprn) (const QList&lt; int &gt; & satPrn) <br> |
|  void | [**setSatellitesInView**](#function-setsatellitesinview) (const QList&lt; QgsSatelliteInfo &gt; & satellitesInView) <br> |
|  void | [**setSatellitesUsed**](#function-setsatellitesused) (int satellitesUsed) <br> |
|  void | [**setSourceName**](#function-setsourcename) (const QString & sourceName) <br> |
|  void | [**setSpeed**](#function-setspeed) (double speed) <br> |
|  void | [**setStatus**](#function-setstatus) (QChar status) <br> |
|  void | [**setUtcDateTime**](#function-setutcdatetime) (const QDateTime & utcDateTime) <br> |
|  void | [**setVacc**](#function-setvacc) (double vacc) <br> |
|  void | [**setVdop**](#function-setvdop) (double vdop) <br> |
|  void | [**setVerticalSpeed**](#function-setverticalspeed) (double verticalSpeed) <br> |
|  QString | [**sourceName**](#function-sourcename-22) () const<br> |
|  double | [**speed**](#function-speed-22) () const<br> |
|  bool | [**speedValid**](#function-speedvalid-22) () const<br> |
|  QChar | [**status**](#function-status-22) () const<br> |
|  QDateTime | [**utcDateTime**](#function-utcdatetime-22) () const<br> |
|  double | [**vacc**](#function-vacc-22) () const<br> |
|  bool | [**vaccValid**](#function-vaccvalid-22) () const<br> |
|  double | [**vdop**](#function-vdop-22) () const<br> |
|  double | [**verticalSpeed**](#function-verticalspeed-22) () const<br> |




























## Public Types Documentation




### enum AccuracyQuality 

```C++
enum GnssPositionInformation::AccuracyQuality {
    AccuracyUndetermined,
    AccuracyBad,
    AccuracyOk,
    AccuracyExcellent
};
```




<hr>



### enum FixStatus 

```C++
enum GnssPositionInformation::FixStatus {
    NoData,
    NoFix,
    Fix2D,
    Fix3D
};
```



GPS fix status 


        

<hr>
## Public Properties Documentation




### property accuracyQuality [1/2]

```C++
AccuracyQuality GnssPositionInformation::accuracyQuality;
```




<hr>



### property averagedCount [1/2]

```C++
int GnssPositionInformation::averagedCount;
```




<hr>



### property direction [1/2]

```C++
double GnssPositionInformation::direction;
```




<hr>



### property directionValid [1/2]

```C++
bool GnssPositionInformation::directionValid;
```




<hr>



### property elevation [1/2]

```C++
double GnssPositionInformation::elevation;
```




<hr>



### property elevationValid [1/2]

```C++
bool GnssPositionInformation::elevationValid;
```




<hr>



### property fixMode [1/2]

```C++
QChar GnssPositionInformation::fixMode;
```




<hr>



### property fixStatus [1/2]

```C++
FixStatus GnssPositionInformation::fixStatus;
```




<hr>



### property fixStatusDescription [1/2]

```C++
QString GnssPositionInformation::fixStatusDescription;
```




<hr>



### property fixType [1/2]

```C++
int GnssPositionInformation::fixType;
```




<hr>



### property hacc [1/2]

```C++
double GnssPositionInformation::hacc;
```




<hr>



### property haccValid [1/2]

```C++
bool GnssPositionInformation::haccValid;
```




<hr>



### property hdop [1/2]

```C++
double GnssPositionInformation::hdop;
```




<hr>



### property hvacc [1/2]

```C++
double GnssPositionInformation::hvacc;
```




<hr>



### property hvaccValid [1/2]

```C++
double GnssPositionInformation::hvaccValid;
```




<hr>



### property imuCorrection [1/2]

```C++
bool GnssPositionInformation::imuCorrection;
```




<hr>



### property imuHeading [1/2]

```C++
double GnssPositionInformation::imuHeading;
```




<hr>



### property imuHeadingValid [1/2]

```C++
bool GnssPositionInformation::imuHeadingValid;
```




<hr>



### property imuPitch [1/2]

```C++
double GnssPositionInformation::imuPitch;
```




<hr>



### property imuPitchValid [1/2]

```C++
bool GnssPositionInformation::imuPitchValid;
```




<hr>



### property imuRoll [1/2]

```C++
double GnssPositionInformation::imuRoll;
```




<hr>



### property imuRollValid [1/2]

```C++
bool GnssPositionInformation::imuRollValid;
```




<hr>



### property imuSteering [1/2]

```C++
double GnssPositionInformation::imuSteering;
```




<hr>



### property imuSteeringValid [1/2]

```C++
bool GnssPositionInformation::imuSteeringValid;
```




<hr>



### property isValid [1/2]

```C++
bool GnssPositionInformation::isValid;
```




<hr>



### property latitude [1/2]

```C++
double GnssPositionInformation::latitude;
```




<hr>



### property latitudeValid [1/2]

```C++
bool GnssPositionInformation::latitudeValid;
```




<hr>



### property longitude [1/2]

```C++
double GnssPositionInformation::longitude;
```




<hr>



### property longitudeValid [1/2]

```C++
bool GnssPositionInformation::longitudeValid;
```




<hr>



### property magneticVariation [1/2]

```C++
double GnssPositionInformation::magneticVariation;
```




<hr>



### property orientation [1/2]

```C++
double GnssPositionInformation::orientation;
```




<hr>



### property orientationValid [1/2]

```C++
bool GnssPositionInformation::orientationValid;
```




<hr>



### property pdop [1/2]

```C++
double GnssPositionInformation::pdop;
```




<hr>



### property quality [1/2]

```C++
int GnssPositionInformation::quality;
```




<hr>



### property qualityDescription [1/2]

```C++
QString GnssPositionInformation::qualityDescription;
```




<hr>



### property satInfoComplete [1/2]

```C++
bool GnssPositionInformation::satInfoComplete;
```




<hr>



### property satPrn [1/2]

```C++
QList<int> GnssPositionInformation::satPrn;
```




<hr>



### property satellitesInView [1/2]

```C++
QList<QgsSatelliteInfo> GnssPositionInformation::satellitesInView;
```




<hr>



### property satellitesUsed [1/2]

```C++
int GnssPositionInformation::satellitesUsed;
```




<hr>



### property sourceName [1/2]

```C++
QString GnssPositionInformation::sourceName;
```




<hr>



### property speed [1/2]

```C++
double GnssPositionInformation::speed;
```




<hr>



### property speedValid [1/2]

```C++
bool GnssPositionInformation::speedValid;
```




<hr>



### property status [1/2]

```C++
QChar GnssPositionInformation::status;
```




<hr>



### property utcDateTime [1/2]

```C++
QDateTime GnssPositionInformation::utcDateTime;
```




<hr>



### property vacc [1/2]

```C++
double GnssPositionInformation::vacc;
```




<hr>



### property vaccValid [1/2]

```C++
bool GnssPositionInformation::vaccValid;
```




<hr>



### property vdop [1/2]

```C++
double GnssPositionInformation::vdop;
```




<hr>



### property verticalSpeed [1/2]

```C++
double GnssPositionInformation::verticalSpeed;
```




<hr>
## Public Functions Documentation




### function GnssPositionInformation 

```C++
GnssPositionInformation::GnssPositionInformation (
    double latitude=std::numeric_limits< double >::quiet_NaN(),
    double longitude=std::numeric_limits< double >::quiet_NaN(),
    double elevation=std::numeric_limits< double >::quiet_NaN(),
    double speed=std::numeric_limits< double >::quiet_NaN(),
    double direction=std::numeric_limits< double >::quiet_NaN(),
    const QList< QgsSatelliteInfo > & satellitesInView=QList< QgsSatelliteInfo >(),
    double pdop=0,
    double hdop=0,
    double vdop=0,
    double hacc=std::numeric_limits< double >::quiet_NaN(),
    double vacc=std::numeric_limits< double >::quiet_NaN(),
    QDateTime utcDateTime=QDateTime(),
    QChar fixMode=QChar(),
    int fixType=0,
    int quality=-1,
    int satellitesUsed=0,
    QChar status=QChar(),
    const QList< int > & satPrn=QList< int >(),
    bool satInfoComplete=false,
    double verticalSpeed=std::numeric_limits< double >::quiet_NaN(),
    double magneticVariation=std::numeric_limits< double >::quiet_NaN(),
    int averagedCount=0,
    const QString & sourceName=QString(),
    bool imuCorrection=false,
    double imuRoll=std::numeric_limits< double >::quiet_NaN(),
    double imuPitch=std::numeric_limits< double >::quiet_NaN(),
    double imuHeading=std::numeric_limits< double >::quiet_NaN(),
    double imuSteering=std::numeric_limits< double >::quiet_NaN(),
    double orientation=std::numeric_limits< double >::quiet_NaN()
) 
```



GNSSPositionInformation constructor. 


        

<hr>



### function accuracyQuality [2/2]

```C++
inline AccuracyQuality GnssPositionInformation::accuracyQuality () const
```




<hr>



### function averagedCount [2/2]

```C++
inline int GnssPositionInformation::averagedCount () const
```




<hr>



### function direction [2/2]

```C++
inline double GnssPositionInformation::direction () const
```




<hr>



### function directionValid [2/2]

```C++
inline bool GnssPositionInformation::directionValid () const
```




<hr>



### function elevation [2/2]

```C++
inline double GnssPositionInformation::elevation () const
```




<hr>



### function elevationValid [2/2]

```C++
inline bool GnssPositionInformation::elevationValid () const
```




<hr>



### function fixMode [2/2]

```C++
inline QChar GnssPositionInformation::fixMode () const
```




<hr>



### function fixStatus [2/2]

```C++
FixStatus GnssPositionInformation::fixStatus () const
```



Returns the fix status 


        

<hr>



### function fixStatusDescription [2/2]

```C++
QString GnssPositionInformation::fixStatusDescription () const
```



Returns the fix status as string 


        

<hr>



### function fixType [2/2]

```C++
inline int GnssPositionInformation::fixType () const
```




<hr>



### function hacc [2/2]

```C++
inline double GnssPositionInformation::hacc () const
```




<hr>



### function haccValid [2/2]

```C++
inline bool GnssPositionInformation::haccValid () const
```




<hr>



### function hdop [2/2]

```C++
inline double GnssPositionInformation::hdop () const
```




<hr>



### function hvacc [2/2]

```C++
inline double GnssPositionInformation::hvacc () const
```




<hr>



### function hvaccValid [2/2]

```C++
inline bool GnssPositionInformation::hvaccValid () const
```




<hr>



### function imuCorrection [2/2]

```C++
inline bool GnssPositionInformation::imuCorrection () const
```




<hr>



### function imuHeading [2/2]

```C++
inline double GnssPositionInformation::imuHeading () const
```




<hr>



### function imuHeadingValid [2/2]

```C++
inline bool GnssPositionInformation::imuHeadingValid () const
```




<hr>



### function imuPitch [2/2]

```C++
inline double GnssPositionInformation::imuPitch () const
```




<hr>



### function imuPitchValid [2/2]

```C++
inline bool GnssPositionInformation::imuPitchValid () const
```




<hr>



### function imuRoll [2/2]

```C++
inline double GnssPositionInformation::imuRoll () const
```




<hr>



### function imuRollValid [2/2]

```C++
inline bool GnssPositionInformation::imuRollValid () const
```




<hr>



### function imuSteering [2/2]

```C++
inline double GnssPositionInformation::imuSteering () const
```




<hr>



### function imuSteeringValid [2/2]

```C++
inline bool GnssPositionInformation::imuSteeringValid () const
```




<hr>



### function isValid [2/2]

```C++
bool GnssPositionInformation::isValid () const
```



Returns whether the connection information is valid 


        

<hr>



### function latitude [2/2]

```C++
inline double GnssPositionInformation::latitude () const
```




<hr>



### function latitudeValid [2/2]

```C++
inline bool GnssPositionInformation::latitudeValid () const
```




<hr>



### function longitude [2/2]

```C++
inline double GnssPositionInformation::longitude () const
```




<hr>



### function longitudeValid [2/2]

```C++
inline bool GnssPositionInformation::longitudeValid () const
```




<hr>



### function magneticVariation [2/2]

```C++
inline double GnssPositionInformation::magneticVariation () const
```




<hr>



### function operator!= 

```C++
inline bool GnssPositionInformation::operator!= (
    const GnssPositionInformation & other
) const
```




<hr>



### function operator== 

```C++
bool GnssPositionInformation::operator== (
    const GnssPositionInformation & other
) const
```




<hr>



### function orientation [2/2]

```C++
inline double GnssPositionInformation::orientation () const
```




<hr>



### function orientationValid [2/2]

```C++
inline bool GnssPositionInformation::orientationValid () const
```




<hr>



### function pdop [2/2]

```C++
inline double GnssPositionInformation::pdop () const
```




<hr>



### function quality [2/2]

```C++
inline int GnssPositionInformation::quality () const
```




<hr>



### function qualityDescription [2/2]

```C++
QString GnssPositionInformation::qualityDescription () const
```



Returns a descriptive string for the signal quality 


        

<hr>



### function satInfoComplete [2/2]

```C++
inline bool GnssPositionInformation::satInfoComplete () const
```




<hr>



### function satPrn [2/2]

```C++
inline QList< int > GnssPositionInformation::satPrn () const
```




<hr>



### function satellitesInView [2/2]

```C++
inline QList< QgsSatelliteInfo > GnssPositionInformation::satellitesInView () const
```




<hr>



### function satellitesUsed [2/2]

```C++
inline int GnssPositionInformation::satellitesUsed () const
```




<hr>



### function setAccuracyQuality 

```C++
inline void GnssPositionInformation::setAccuracyQuality (
    AccuracyQuality quality
) 
```



Accuracy quality classification. Indicates if the position accuracy is bad, ok, or excellent, based on user-provided thresholds. 


        

<hr>



### function setAveragedCount 

```C++
inline void GnssPositionInformation::setAveragedCount (
    int averagedCount
) 
```



Returns the number of collected position from which the averaged positioning details were computed 

**Note:**

A value of zero means the position information isn't averaged 





        

<hr>



### function setDirection 

```C++
inline void GnssPositionInformation::setDirection (
    double direction
) 
```



The bearing measured in degrees clockwise from true north to the direction of travel. 


        

<hr>



### function setElevation 

```C++
inline void GnssPositionInformation::setElevation (
    double elevation
) 
```



Altitude (in meters) above or below the mean sea level. 


        

<hr>



### function setFixMode 

```C++
inline void GnssPositionInformation::setFixMode (
    QChar fixMode
) 
```



Fix mode (where M = Manual, forced to operate in 2D or 3D or A = Automatic, 3D/2D) 


        

<hr>



### function setFixType 

```C++
inline void GnssPositionInformation::setFixType (
    int fixType
) 
```



Contains the fix type, where 1 = no fix, 2 = 2d fix, 3 = 3d fix 


        

<hr>



### function setHVacc 

```C++
inline void GnssPositionInformation::setHVacc (
    double hvacc
) 
```



3D accuracy in meters 3DRMS 


        

<hr>



### function setHacc 

```C++
inline void GnssPositionInformation::setHacc (
    double hacc
) 
```



Horizontal accuracy in meters. RMS 


        

<hr>



### function setHdop 

```C++
inline void GnssPositionInformation::setHdop (
    double hdop
) 
```



Horizontal dilution of precision. 


        

<hr>



### function setImuCorrection 

```C++
inline void GnssPositionInformation::setImuCorrection (
    bool imuCorrection
) 
```



Returns TRUE when IMU correction is active 


        

<hr>



### function setImuHeading 

```C++
inline void GnssPositionInformation::setImuHeading (
    double heading
) 
```



IMU heading 


        

<hr>



### function setImuPitch 

```C++
inline void GnssPositionInformation::setImuPitch (
    double pitch
) 
```



IMU pitch 


        

<hr>



### function setImuRoll 

```C++
inline void GnssPositionInformation::setImuRoll (
    double roll
) 
```



IMU roll 


        

<hr>



### function setImuSteering 

```C++
inline void GnssPositionInformation::setImuSteering (
    double steering
) 
```



IMU steering 


        

<hr>



### function setLatitude 

```C++
inline void GnssPositionInformation::setLatitude (
    double latitude
) 
```



Latitude in decimal degrees, using the WGS84 datum. A positive value indicates the Northern Hemisphere, and a negative value indicates the Southern Hemisphere. 


        

<hr>



### function setLongitude 

```C++
inline void GnssPositionInformation::setLongitude (
    double longitude
) 
```



Longitude in decimal degrees, using the WGS84 datum. A positive value indicates the Eastern Hemisphere, and a negative value indicates the Western Hemisphere. 


        

<hr>



### function setMagneticVaritation 

```C++
inline void GnssPositionInformation::setMagneticVaritation (
    double magneticVariation
) 
```



magnetic variation in degrees 


        

<hr>



### function setOrientation 

```C++
inline void GnssPositionInformation::setOrientation (
    double orientation
) 
```



Orientation (in degrees) 


        

<hr>



### function setPdop 

```C++
inline void GnssPositionInformation::setPdop (
    double pdop
) 
```



Dilution of precision. 


        

<hr>



### function setQuality 

```C++
inline void GnssPositionInformation::setQuality (
    int quality
) 
```



GPS quality indicator (0 = Invalid; 1 = Fix; 2 = Differential, 3 = Sensitive) 


        

<hr>



### function setSatInfoComplete 

```C++
inline void GnssPositionInformation::setSatInfoComplete (
    bool satInfoComplete
) 
```



TRUE if satellite information is complete. 


        

<hr>



### function setSatPrn 

```C++
inline void GnssPositionInformation::setSatPrn (
    const QList< int > & satPrn
) 
```



IDs of satellites used in the position fix. 


        

<hr>



### function setSatellitesInView 

```C++
inline void GnssPositionInformation::setSatellitesInView (
    const QList< QgsSatelliteInfo > & satellitesInView
) 
```



Contains a list of information relating to the current satellites in view. 


        

<hr>



### function setSatellitesUsed 

```C++
inline void GnssPositionInformation::setSatellitesUsed (
    int satellitesUsed
) 
```



Count of satellites used in obtaining the fix. 


        

<hr>



### function setSourceName 

```C++
inline void GnssPositionInformation::setSourceName (
    const QString & sourceName
) 
```



source name (used by QtPositioning) 


        

<hr>



### function setSpeed 

```C++
inline void GnssPositionInformation::setSpeed (
    double speed
) 
```



Ground speed, in km/h. 


        

<hr>



### function setStatus 

```C++
inline void GnssPositionInformation::setStatus (
    QChar status
) 
```



Status (A = active or V = void) 


        

<hr>



### function setUtcDateTime 

```C++
inline void GnssPositionInformation::setUtcDateTime (
    const QDateTime & utcDateTime
) 
```



The date and time at which this position was reported, in UTC time. 


        

<hr>



### function setVacc 

```C++
inline void GnssPositionInformation::setVacc (
    double vacc
) 
```



Vertical accuracy in meters VRMS 


        

<hr>



### function setVdop 

```C++
inline void GnssPositionInformation::setVdop (
    double vdop
) 
```



Vertical dilution of precision. 


        

<hr>



### function setVerticalSpeed 

```C++
inline void GnssPositionInformation::setVerticalSpeed (
    double verticalSpeed
) 
```



Vertical speed, in km/h. 


        

<hr>



### function sourceName [2/2]

```C++
inline QString GnssPositionInformation::sourceName () const
```




<hr>



### function speed [2/2]

```C++
inline double GnssPositionInformation::speed () const
```




<hr>



### function speedValid [2/2]

```C++
inline bool GnssPositionInformation::speedValid () const
```




<hr>



### function status [2/2]

```C++
inline QChar GnssPositionInformation::status () const
```




<hr>



### function utcDateTime [2/2]

```C++
inline QDateTime GnssPositionInformation::utcDateTime () const
```




<hr>



### function vacc [2/2]

```C++
inline double GnssPositionInformation::vacc () const
```




<hr>



### function vaccValid [2/2]

```C++
inline bool GnssPositionInformation::vaccValid () const
```




<hr>



### function vdop [2/2]

```C++
inline double GnssPositionInformation::vdop () const
```




<hr>



### function verticalSpeed [2/2]

```C++
inline double GnssPositionInformation::verticalSpeed () const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/positioning/gnsspositioninformation.h`

