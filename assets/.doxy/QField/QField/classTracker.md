

# Class Tracker



[**ClassList**](annotated.md) **>** [**Tracker**](classTracker.md)








Inherits the following classes: QObject














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**MeasureType**](#enum-measuretype)  <br> |












## Public Properties

| Type | Name |
| ---: | :--- |
| property QColor | [**color**](classTracker.md#property-color-12)  <br> |
| property bool | [**conjunction**](classTracker.md#property-conjunction-12)  <br> |
| property QgsFeature | [**feature**](classTracker.md#property-feature-12)  <br> |
| property [**FeatureModel**](classFeatureModel.md) \* | [**featureModel**](classTracker.md#property-featuremodel-12)  <br> |
| property bool | [**filterAccuracy**](classTracker.md#property-filteraccuracy-12)  <br> |
| property bool | [**isActive**](classTracker.md#property-isactive-12)  <br> |
| property bool | [**isReplaying**](classTracker.md#property-isreplaying-12)  <br> |
| property bool | [**isSuspended**](classTracker.md#property-issuspended-12)  <br> |
| property double | [**maximumDistance**](classTracker.md#property-maximumdistance-12)  <br> |
| property MeasureType | [**measureType**](classTracker.md#property-measuretype-12)  <br> |
| property double | [**minimumDistance**](classTracker.md#property-minimumdistance-12)  <br> |
| property [**RubberbandModel**](classRubberbandModel.md) \* | [**rubberbandModel**](classTracker.md#property-rubberbandmodel-12)  <br> |
| property bool | [**sensorCapture**](classTracker.md#property-sensorcapture-12)  <br> |
| property QDateTime | [**startPositionTimestamp**](classTracker.md#property-startpositiontimestamp-12)  <br> |
| property double | [**timeInterval**](classTracker.md#property-timeinterval-12)  <br> |
| property QgsVectorLayer \* | [**vectorLayer**](classTracker.md#property-vectorlayer-12)  <br> |
| property bool | [**visible**](classTracker.md#property-visible-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**colorChanged**](classTracker.md#signal-colorchanged)  <br> |
| signal void | [**conjunctionChanged**](classTracker.md#signal-conjunctionchanged)  <br> |
| signal void | [**featureChanged**](classTracker.md#signal-featurechanged)  <br> |
| signal void | [**featureCreated**](classTracker.md#signal-featurecreated)  <br> |
| signal void | [**featureModelChanged**](classTracker.md#signal-featuremodelchanged)  <br> |
| signal void | [**filterAccuracyChanged**](classTracker.md#signal-filteraccuracychanged)  <br> |
| signal void | [**isActiveChanged**](classTracker.md#signal-isactivechanged)  <br> |
| signal void | [**isReplayingChanged**](classTracker.md#signal-isreplayingchanged)  <br> |
| signal void | [**isSuspendedChanged**](classTracker.md#signal-issuspendedchanged)  <br> |
| signal void | [**maximumDistanceChanged**](classTracker.md#signal-maximumdistancechanged)  <br> |
| signal void | [**measureTypeChanged**](classTracker.md#signal-measuretypechanged)  <br> |
| signal void | [**minimumDistanceChanged**](classTracker.md#signal-minimumdistancechanged)  <br> |
| signal void | [**rubberbandModelChanged**](classTracker.md#signal-rubberbandmodelchanged)  <br> |
| signal void | [**sensorCaptureChanged**](classTracker.md#signal-sensorcapturechanged)  <br> |
| signal void | [**startPositionTimestampChanged**](classTracker.md#signal-startpositiontimestampchanged)  <br> |
| signal void | [**timeIntervalChanged**](classTracker.md#signal-timeintervalchanged)  <br> |
| signal void | [**vectorLayerChanged**](classTracker.md#signal-vectorlayerchanged)  <br> |
| signal void | [**visibleChanged**](classTracker.md#signal-visiblechanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Tracker**](#function-tracker) (QgsVectorLayer \* vectorLayer) <br> |
|  QColor | [**color**](#function-color-22) () const<br>_Returns the track rubberband color._  |
|  bool | [**conjunction**](#function-conjunction-22) () const<br>_Returns TRUE if all constraints need to be fulfilled between each tracked point._  |
|  QgsFeature | [**feature**](#function-feature-22) () const<br>_Returns the created feature._  |
|  [**FeatureModel**](classFeatureModel.md) \* | [**featureModel**](#function-featuremodel-22) () const<br>_Returns the feature model used to generate the track attributes._  |
|  bool | [**filterAccuracy**](#function-filteraccuracy-22) () const<br>_Returns TRUE if GNSS accuracy filtering is enabled._  |
|  bool | [**isActive**](#function-isactive-22) () const<br>_Returns whether the tracker has been started._  |
|  bool | [**isReplaying**](#function-isreplaying-22) () const<br>_Returns whether the tracker is replaying positions._  |
|  bool | [**isSuspended**](#function-issuspended-22) () const<br>_Returns whether the track has been suspended._  |
|  double | [**maximumDistance**](#function-maximumdistance-22) () const<br>_Returns the maximum distance tolerated beyond which a position will be considered errenous._  |
|  MeasureType | [**measureType**](#function-measuretype-22) () const<br>_Returns the measure type used with the tracker geometry's M dimension when available._  |
|  double | [**minimumDistance**](#function-minimumdistance-22) () const<br>_Returns the minimum distance constraint between each tracked point._  |
|  Q\_INVOKABLE void | [**processPositionInformation**](#function-processpositioninformation) (const [**GnssPositionInformation**](classGnssPositionInformation.md) & positionInformation, const QgsPoint & projectedPosition) <br>_Process the given position information and projected position passed onto the tracker._  |
|  void | [**replayPositionInformationList**](#function-replaypositioninformationlist) (const QList&lt; [**GnssPositionInformation**](classGnssPositionInformation.md) &gt; & positionInformationList, [**QgsQuickCoordinateTransformer**](classQgsQuickCoordinateTransformer.md) \* coordinateTransformer=nullptr) <br>_Replays a list of position information taking into account the tracker settings._  |
|  [**RubberbandModel**](classRubberbandModel.md) \* | [**rubberbandModel**](#function-rubberbandmodel-22) () const<br>_Returns the rubber band model used to generate the track geometry._  |
|  bool | [**sensorCapture**](#function-sensorcapture-22) () const<br>_Returns if TRUE, newly captured sensor data is needed between each tracked point._  |
|  void | [**setColor**](#function-setcolor) (const QColor & color) <br>_Sets the track rubberband color._  |
|  void | [**setConjunction**](#function-setconjunction) (bool conjunction) <br>_Sets where all constraints need to be fulfilled between each tracked point._  |
|  void | [**setFeature**](#function-setfeature) (const QgsFeature & feature) <br>_Sets the created feature._  |
|  void | [**setFeatureModel**](#function-setfeaturemodel) ([**FeatureModel**](classFeatureModel.md) \* featureModel) <br>_Sets the feature model used to generate the track attributes._  |
|  void | [**setFilterAccuracy**](#function-setfilteraccuracy) (bool enabled) <br>_Sets whether GNSS accuracy filtering is enabled._  |
|  void | [**setMaximumDistance**](#function-setmaximumdistance) (double maximumDistance) <br>_Sets the maximum distance tolerated beyond which a position will be considered errenous._  |
|  void | [**setMeasureType**](#function-setmeasuretype) (MeasureType type) <br>_Sets the measure type used with the tracker geometry's M dimension when available._  |
|  void | [**setMinimumDistance**](#function-setminimumdistance) (double minimumDistance) <br>_Sets the minimum distance constraint between each tracked point._  |
|  void | [**setRubberbandModel**](#function-setrubberbandmodel) ([**RubberbandModel**](classRubberbandModel.md) \* rubberbandModel) <br>_Sets the rubber band model used to generate the track geometry._  |
|  void | [**setSensorCapture**](#function-setsensorcapture) (bool capture) <br>_Sets whether newly captured sensor data is needed between each tracked point._  |
|  void | [**setStartPositionTimestamp**](#function-setstartpositiontimestamp) (const QDateTime & startPositionTimestamp) <br>_Sets the timestamp of the first recorded point._  |
|  void | [**setTimeInterval**](#function-settimeinterval) (double timeInterval) <br>_Sets the minimum time interval constraint between each tracked point._  |
|  void | [**setVectorLayer**](#function-setvectorlayer) (QgsVectorLayer \* vectorLayer) <br>_Sets the current layer._  |
|  void | [**setVisible**](#function-setvisible) (bool visible) <br>_Sets whether the tracker rubberband is visible._  |
|  void | [**start**](#function-start) (const [**GnssPositionInformation**](classGnssPositionInformation.md) & positionInformation=[**GnssPositionInformation**](classGnssPositionInformation.md)(), const QgsPoint & projectedPosition=QgsPoint()) <br>_Starts tracking._  |
|  QDateTime | [**startPositionTimestamp**](#function-startpositiontimestamp-22) () const<br>_Returns the timestamp of the first recorded point._  |
|  void | [**stop**](#function-stop) () <br>_Stops tracking._  |
|  void | [**suspendUntilReplay**](#function-suspenduntilreplay) () <br> |
|  double | [**timeInterval**](#function-timeinterval-22) () const<br>_Returns the minimum time interval constraint between each tracked point._  |
|  QgsVectorLayer \* | [**vectorLayer**](#function-vectorlayer-22) () const<br>_Returns the current layer._  |
|  bool | [**visible**](#function-visible-22) () const<br>_Returns TRUE if the tracker rubberband is visible._  |




























## Public Types Documentation




### enum MeasureType 

```C++
enum Tracker::MeasureType {
    SecondsSinceStart = 0,
    Timestamp,
    GroundSpeed,
    Bearing,
    HorizontalAccuracy,
    VerticalAccuracy,
    PDOP,
    HDOP,
    VDOP
};
```




<hr>
## Public Properties Documentation




### property color [1/2]

```C++
QColor Tracker::color;
```




<hr>



### property conjunction [1/2]

```C++
bool Tracker::conjunction;
```




<hr>



### property feature [1/2]

```C++
QgsFeature Tracker::feature;
```




<hr>



### property featureModel [1/2]

```C++
FeatureModel * Tracker::featureModel;
```




<hr>



### property filterAccuracy [1/2]

```C++
bool Tracker::filterAccuracy;
```




<hr>



### property isActive [1/2]

```C++
bool Tracker::isActive;
```




<hr>



### property isReplaying [1/2]

```C++
bool Tracker::isReplaying;
```




<hr>



### property isSuspended [1/2]

```C++
bool Tracker::isSuspended;
```




<hr>



### property maximumDistance [1/2]

```C++
double Tracker::maximumDistance;
```




<hr>



### property measureType [1/2]

```C++
MeasureType Tracker::measureType;
```




<hr>



### property minimumDistance [1/2]

```C++
double Tracker::minimumDistance;
```




<hr>



### property rubberbandModel [1/2]

```C++
RubberbandModel * Tracker::rubberbandModel;
```




<hr>



### property sensorCapture [1/2]

```C++
bool Tracker::sensorCapture;
```




<hr>



### property startPositionTimestamp [1/2]

```C++
QDateTime Tracker::startPositionTimestamp;
```




<hr>



### property timeInterval [1/2]

```C++
double Tracker::timeInterval;
```




<hr>



### property vectorLayer [1/2]

```C++
QgsVectorLayer * Tracker::vectorLayer;
```




<hr>



### property visible [1/2]

```C++
bool Tracker::visible;
```




<hr>
## Public Signals Documentation




### signal colorChanged 

```C++
void Tracker::colorChanged;
```




<hr>



### signal conjunctionChanged 

```C++
void Tracker::conjunctionChanged;
```




<hr>



### signal featureChanged 

```C++
void Tracker::featureChanged;
```




<hr>



### signal featureCreated 

```C++
void Tracker::featureCreated;
```




<hr>



### signal featureModelChanged 

```C++
void Tracker::featureModelChanged;
```




<hr>



### signal filterAccuracyChanged 

```C++
void Tracker::filterAccuracyChanged;
```




<hr>



### signal isActiveChanged 

```C++
void Tracker::isActiveChanged;
```




<hr>



### signal isReplayingChanged 

```C++
void Tracker::isReplayingChanged;
```




<hr>



### signal isSuspendedChanged 

```C++
void Tracker::isSuspendedChanged;
```




<hr>



### signal maximumDistanceChanged 

```C++
void Tracker::maximumDistanceChanged;
```




<hr>



### signal measureTypeChanged 

```C++
void Tracker::measureTypeChanged;
```




<hr>



### signal minimumDistanceChanged 

```C++
void Tracker::minimumDistanceChanged;
```




<hr>



### signal rubberbandModelChanged 

```C++
void Tracker::rubberbandModelChanged;
```




<hr>



### signal sensorCaptureChanged 

```C++
void Tracker::sensorCaptureChanged;
```




<hr>



### signal startPositionTimestampChanged 

```C++
void Tracker::startPositionTimestampChanged;
```




<hr>



### signal timeIntervalChanged 

```C++
void Tracker::timeIntervalChanged;
```




<hr>



### signal vectorLayerChanged 

```C++
void Tracker::vectorLayerChanged;
```




<hr>



### signal visibleChanged 

```C++
void Tracker::visibleChanged;
```




<hr>
## Public Functions Documentation




### function Tracker 

```C++
explicit Tracker::Tracker (
    QgsVectorLayer * vectorLayer
) 
```




<hr>



### function color [2/2]

_Returns the track rubberband color._ 
```C++
inline QColor Tracker::color () const
```




<hr>



### function conjunction [2/2]

_Returns TRUE if all constraints need to be fulfilled between each tracked point._ 
```C++
inline bool Tracker::conjunction () const
```




<hr>



### function feature [2/2]

_Returns the created feature._ 
```C++
QgsFeature Tracker::feature () const
```




<hr>



### function featureModel [2/2]

_Returns the feature model used to generate the track attributes._ 
```C++
FeatureModel * Tracker::featureModel () const
```




<hr>



### function filterAccuracy [2/2]

_Returns TRUE if GNSS accuracy filtering is enabled._ 
```C++
bool Tracker::filterAccuracy () const
```




<hr>



### function isActive [2/2]

_Returns whether the tracker has been started._ 
```C++
inline bool Tracker::isActive () const
```




<hr>



### function isReplaying [2/2]

_Returns whether the tracker is replaying positions._ 
```C++
inline bool Tracker::isReplaying () const
```




<hr>



### function isSuspended [2/2]

_Returns whether the track has been suspended._ 
```C++
inline bool Tracker::isSuspended () const
```




<hr>



### function maximumDistance [2/2]

_Returns the maximum distance tolerated beyond which a position will be considered errenous._ 
```C++
inline double Tracker::maximumDistance () const
```




<hr>



### function measureType [2/2]

_Returns the measure type used with the tracker geometry's M dimension when available._ 
```C++
inline MeasureType Tracker::measureType () const
```




<hr>



### function minimumDistance [2/2]

_Returns the minimum distance constraint between each tracked point._ 
```C++
inline double Tracker::minimumDistance () const
```




<hr>



### function processPositionInformation 

_Process the given position information and projected position passed onto the tracker._ 
```C++
Q_INVOKABLE void Tracker::processPositionInformation (
    const GnssPositionInformation & positionInformation,
    const QgsPoint & projectedPosition
) 
```




<hr>



### function replayPositionInformationList 

_Replays a list of position information taking into account the tracker settings._ 
```C++
void Tracker::replayPositionInformationList (
    const QList< GnssPositionInformation > & positionInformationList,
    QgsQuickCoordinateTransformer * coordinateTransformer=nullptr
) 
```




<hr>



### function rubberbandModel [2/2]

_Returns the rubber band model used to generate the track geometry._ 
```C++
RubberbandModel * Tracker::rubberbandModel () const
```




<hr>



### function sensorCapture [2/2]

_Returns if TRUE, newly captured sensor data is needed between each tracked point._ 
```C++
inline bool Tracker::sensorCapture () const
```




<hr>



### function setColor 

_Sets the track rubberband color._ 
```C++
void Tracker::setColor (
    const QColor & color
) 
```




<hr>



### function setConjunction 

_Sets where all constraints need to be fulfilled between each tracked point._ 
```C++
void Tracker::setConjunction (
    bool conjunction
) 
```




<hr>



### function setFeature 

_Sets the created feature._ 
```C++
void Tracker::setFeature (
    const QgsFeature & feature
) 
```




<hr>



### function setFeatureModel 

_Sets the feature model used to generate the track attributes._ 
```C++
void Tracker::setFeatureModel (
    FeatureModel * featureModel
) 
```




<hr>



### function setFilterAccuracy 

_Sets whether GNSS accuracy filtering is enabled._ 
```C++
void Tracker::setFilterAccuracy (
    bool enabled
) 
```




<hr>



### function setMaximumDistance 

_Sets the maximum distance tolerated beyond which a position will be considered errenous._ 
```C++
void Tracker::setMaximumDistance (
    double maximumDistance
) 
```




<hr>



### function setMeasureType 

_Sets the measure type used with the tracker geometry's M dimension when available._ 
```C++
void Tracker::setMeasureType (
    MeasureType type
) 
```




<hr>



### function setMinimumDistance 

_Sets the minimum distance constraint between each tracked point._ 
```C++
void Tracker::setMinimumDistance (
    double minimumDistance
) 
```




<hr>



### function setRubberbandModel 

_Sets the rubber band model used to generate the track geometry._ 
```C++
void Tracker::setRubberbandModel (
    RubberbandModel * rubberbandModel
) 
```




<hr>



### function setSensorCapture 

_Sets whether newly captured sensor data is needed between each tracked point._ 
```C++
void Tracker::setSensorCapture (
    bool capture
) 
```




<hr>



### function setStartPositionTimestamp 

_Sets the timestamp of the first recorded point._ 
```C++
inline void Tracker::setStartPositionTimestamp (
    const QDateTime & startPositionTimestamp
) 
```




<hr>



### function setTimeInterval 

_Sets the minimum time interval constraint between each tracked point._ 
```C++
void Tracker::setTimeInterval (
    double timeInterval
) 
```




<hr>



### function setVectorLayer 

_Sets the current layer._ 
```C++
void Tracker::setVectorLayer (
    QgsVectorLayer * vectorLayer
) 
```




<hr>



### function setVisible 

_Sets whether the tracker rubberband is visible._ 
```C++
void Tracker::setVisible (
    bool visible
) 
```




<hr>



### function start 

_Starts tracking._ 
```C++
void Tracker::start (
    const GnssPositionInformation & positionInformation=GnssPositionInformation (),
    const QgsPoint & projectedPosition=QgsPoint()
) 
```




<hr>



### function startPositionTimestamp [2/2]

_Returns the timestamp of the first recorded point._ 
```C++
inline QDateTime Tracker::startPositionTimestamp () const
```




<hr>



### function stop 

_Stops tracking._ 
```C++
void Tracker::stop () 
```




<hr>



### function suspendUntilReplay 

```C++
void Tracker::suspendUntilReplay () 
```




<hr>



### function timeInterval [2/2]

_Returns the minimum time interval constraint between each tracked point._ 
```C++
inline double Tracker::timeInterval () const
```




<hr>



### function vectorLayer [2/2]

_Returns the current layer._ 
```C++
inline QgsVectorLayer * Tracker::vectorLayer () const
```




<hr>



### function visible [2/2]

_Returns TRUE if the tracker rubberband is visible._ 
```C++
inline bool Tracker::visible () const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/tracker.h`

