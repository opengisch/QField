

# Class QfTracker



[**ClassList**](annotated.md) **>** [**QfTracker**](classQfTracker.md)








Inherits the following classes: QObject














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**MeasureType**](#enum-measuretype)  <br> |












## Public Properties

| Type | Name |
| ---: | :--- |
| property QColor | [**color**](classQfTracker.md#property-color-12)  <br> |
| property bool | [**conjunction**](classQfTracker.md#property-conjunction-12)  <br> |
| property QgsFeature | [**feature**](classQfTracker.md#property-feature-12)  <br> |
| property [**QfFeatureModel**](classQfFeatureModel.md) \* | [**featureModel**](classQfTracker.md#property-featuremodel-12)  <br> |
| property bool | [**filterAccuracy**](classQfTracker.md#property-filteraccuracy-12)  <br> |
| property bool | [**isActive**](classQfTracker.md#property-isactive-12)  <br> |
| property bool | [**isReplaying**](classQfTracker.md#property-isreplaying-12)  <br> |
| property bool | [**isSuspended**](classQfTracker.md#property-issuspended-12)  <br> |
| property double | [**maximumDistance**](classQfTracker.md#property-maximumdistance-12)  <br> |
| property MeasureType | [**measureType**](classQfTracker.md#property-measuretype-12)  <br> |
| property double | [**minimumDistance**](classQfTracker.md#property-minimumdistance-12)  <br> |
| property [**QfRubberbandModel**](classQfRubberbandModel.md) \* | [**rubberbandModel**](classQfTracker.md#property-rubberbandmodel-12)  <br> |
| property bool | [**sensorCapture**](classQfTracker.md#property-sensorcapture-12)  <br> |
| property QDateTime | [**startPositionTimestamp**](classQfTracker.md#property-startpositiontimestamp-12)  <br> |
| property double | [**timeInterval**](classQfTracker.md#property-timeinterval-12)  <br> |
| property QgsVectorLayer \* | [**vectorLayer**](classQfTracker.md#property-vectorlayer-12)  <br> |
| property bool | [**visible**](classQfTracker.md#property-visible-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**colorChanged**](classQfTracker.md#signal-colorchanged)  <br> |
| signal void | [**conjunctionChanged**](classQfTracker.md#signal-conjunctionchanged)  <br> |
| signal void | [**featureChanged**](classQfTracker.md#signal-featurechanged)  <br> |
| signal void | [**featureCreated**](classQfTracker.md#signal-featurecreated)  <br> |
| signal void | [**featureModelChanged**](classQfTracker.md#signal-featuremodelchanged)  <br> |
| signal void | [**filterAccuracyChanged**](classQfTracker.md#signal-filteraccuracychanged)  <br> |
| signal void | [**isActiveChanged**](classQfTracker.md#signal-isactivechanged)  <br> |
| signal void | [**isReplayingChanged**](classQfTracker.md#signal-isreplayingchanged)  <br> |
| signal void | [**isSuspendedChanged**](classQfTracker.md#signal-issuspendedchanged)  <br> |
| signal void | [**maximumDistanceChanged**](classQfTracker.md#signal-maximumdistancechanged)  <br> |
| signal void | [**measureTypeChanged**](classQfTracker.md#signal-measuretypechanged)  <br> |
| signal void | [**minimumDistanceChanged**](classQfTracker.md#signal-minimumdistancechanged)  <br> |
| signal void | [**rubberbandModelChanged**](classQfTracker.md#signal-rubberbandmodelchanged)  <br> |
| signal void | [**sensorCaptureChanged**](classQfTracker.md#signal-sensorcapturechanged)  <br> |
| signal void | [**startPositionTimestampChanged**](classQfTracker.md#signal-startpositiontimestampchanged)  <br> |
| signal void | [**timeIntervalChanged**](classQfTracker.md#signal-timeintervalchanged)  <br> |
| signal void | [**vectorLayerChanged**](classQfTracker.md#signal-vectorlayerchanged)  <br> |
| signal void | [**visibleChanged**](classQfTracker.md#signal-visiblechanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfTracker**](#function-qftracker) (QgsVectorLayer \* vectorLayer) <br> |
|  QColor | [**color**](#function-color-22) () const<br>_Returns the track rubberband color._  |
|  bool | [**conjunction**](#function-conjunction-22) () const<br>_Returns TRUE if all constraints need to be fulfilled between each tracked point._  |
|  QgsFeature | [**feature**](#function-feature-22) () const<br>_Returns the created feature._  |
|  [**QfFeatureModel**](classQfFeatureModel.md) \* | [**featureModel**](#function-featuremodel-22) () const<br>_Returns the feature model used to generate the track attributes._  |
|  bool | [**filterAccuracy**](#function-filteraccuracy-22) () const<br>_Returns TRUE if GNSS accuracy filtering is enabled._  |
|  bool | [**isActive**](#function-isactive-22) () const<br>_Returns whether the tracker has been started._  |
|  bool | [**isReplaying**](#function-isreplaying-22) () const<br>_Returns whether the tracker is replaying positions._  |
|  bool | [**isSuspended**](#function-issuspended-22) () const<br>_Returns whether the track has been suspended._  |
|  double | [**maximumDistance**](#function-maximumdistance-22) () const<br>_Returns the maximum distance tolerated beyond which a position will be considered errenous._  |
|  MeasureType | [**measureType**](#function-measuretype-22) () const<br>_Returns the measure type used with the tracker geometry's M dimension when available._  |
|  double | [**minimumDistance**](#function-minimumdistance-22) () const<br>_Returns the minimum distance constraint between each tracked point._  |
|  Q\_INVOKABLE void | [**processPositionInformation**](#function-processpositioninformation) (const [**QfGnssPositionInformation**](classQfGnssPositionInformation.md) & positionInformation, const QgsPoint & projectedPosition) <br>_Process the given position information and projected position passed onto the tracker._  |
|  void | [**replayPositionInformationList**](#function-replaypositioninformationlist) (const QList&lt; [**QfGnssPositionInformation**](classQfGnssPositionInformation.md) &gt; & positionInformationList, [**QgsQuickCoordinateTransformer**](classQgsQuickCoordinateTransformer.md) \* coordinateTransformer=nullptr) <br>_Replays a list of position information taking into account the tracker settings._  |
|  [**QfRubberbandModel**](classQfRubberbandModel.md) \* | [**rubberbandModel**](#function-rubberbandmodel-22) () const<br>_Returns the rubber band model used to generate the track geometry._  |
|  bool | [**sensorCapture**](#function-sensorcapture-22) () const<br>_Returns if TRUE, newly captured sensor data is needed between each tracked point._  |
|  void | [**setColor**](#function-setcolor) (const QColor & color) <br>_Sets the track rubberband color._  |
|  void | [**setConjunction**](#function-setconjunction) (bool conjunction) <br>_Sets where all constraints need to be fulfilled between each tracked point._  |
|  void | [**setFeature**](#function-setfeature) (const QgsFeature & feature) <br>_Sets the created feature._  |
|  void | [**setFeatureModel**](#function-setfeaturemodel) ([**QfFeatureModel**](classQfFeatureModel.md) \* featureModel) <br>_Sets the feature model used to generate the track attributes._  |
|  void | [**setFilterAccuracy**](#function-setfilteraccuracy) (bool enabled) <br>_Sets whether GNSS accuracy filtering is enabled._  |
|  void | [**setMaximumDistance**](#function-setmaximumdistance) (double maximumDistance) <br>_Sets the maximum distance tolerated beyond which a position will be considered errenous._  |
|  void | [**setMeasureType**](#function-setmeasuretype) (MeasureType type) <br>_Sets the measure type used with the tracker geometry's M dimension when available._  |
|  void | [**setMinimumDistance**](#function-setminimumdistance) (double minimumDistance) <br>_Sets the minimum distance constraint between each tracked point._  |
|  void | [**setRubberbandModel**](#function-setrubberbandmodel) ([**QfRubberbandModel**](classQfRubberbandModel.md) \* rubberbandModel) <br>_Sets the rubber band model used to generate the track geometry._  |
|  void | [**setSensorCapture**](#function-setsensorcapture) (bool capture) <br>_Sets whether newly captured sensor data is needed between each tracked point._  |
|  void | [**setStartPositionTimestamp**](#function-setstartpositiontimestamp) (const QDateTime & startPositionTimestamp) <br>_Sets the timestamp of the first recorded point._  |
|  void | [**setTimeInterval**](#function-settimeinterval) (double timeInterval) <br>_Sets the minimum time interval constraint between each tracked point._  |
|  void | [**setVectorLayer**](#function-setvectorlayer) (QgsVectorLayer \* vectorLayer) <br>_Sets the current layer._  |
|  void | [**setVisible**](#function-setvisible) (bool visible) <br>_Sets whether the tracker rubberband is visible._  |
|  void | [**start**](#function-start) (const [**QfGnssPositionInformation**](classQfGnssPositionInformation.md) & positionInformation=[**QfGnssPositionInformation**](classQfGnssPositionInformation.md)(), const QgsPoint & projectedPosition=QgsPoint()) <br>_Starts tracking._  |
|  QDateTime | [**startPositionTimestamp**](#function-startpositiontimestamp-22) () const<br>_Returns the timestamp of the first recorded point._  |
|  void | [**stop**](#function-stop) () <br>_Stops tracking._  |
|  void | [**suspendUntilReplay**](#function-suspenduntilreplay) () <br> |
|  double | [**timeInterval**](#function-timeinterval-22) () const<br>_Returns the minimum time interval constraint between each tracked point._  |
|  QgsVectorLayer \* | [**vectorLayer**](#function-vectorlayer-22) () const<br>_Returns the current layer._  |
|  bool | [**visible**](#function-visible-22) () const<br>_Returns TRUE if the tracker rubberband is visible._  |




























## Public Types Documentation




### enum MeasureType 

```C++
enum QfTracker::MeasureType {
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
QColor QfTracker::color;
```




<hr>



### property conjunction [1/2]

```C++
bool QfTracker::conjunction;
```




<hr>



### property feature [1/2]

```C++
QgsFeature QfTracker::feature;
```




<hr>



### property featureModel [1/2]

```C++
QfFeatureModel * QfTracker::featureModel;
```




<hr>



### property filterAccuracy [1/2]

```C++
bool QfTracker::filterAccuracy;
```




<hr>



### property isActive [1/2]

```C++
bool QfTracker::isActive;
```




<hr>



### property isReplaying [1/2]

```C++
bool QfTracker::isReplaying;
```




<hr>



### property isSuspended [1/2]

```C++
bool QfTracker::isSuspended;
```




<hr>



### property maximumDistance [1/2]

```C++
double QfTracker::maximumDistance;
```




<hr>



### property measureType [1/2]

```C++
MeasureType QfTracker::measureType;
```




<hr>



### property minimumDistance [1/2]

```C++
double QfTracker::minimumDistance;
```




<hr>



### property rubberbandModel [1/2]

```C++
QfRubberbandModel * QfTracker::rubberbandModel;
```




<hr>



### property sensorCapture [1/2]

```C++
bool QfTracker::sensorCapture;
```




<hr>



### property startPositionTimestamp [1/2]

```C++
QDateTime QfTracker::startPositionTimestamp;
```




<hr>



### property timeInterval [1/2]

```C++
double QfTracker::timeInterval;
```




<hr>



### property vectorLayer [1/2]

```C++
QgsVectorLayer * QfTracker::vectorLayer;
```




<hr>



### property visible [1/2]

```C++
bool QfTracker::visible;
```




<hr>
## Public Signals Documentation




### signal colorChanged 

```C++
void QfTracker::colorChanged;
```




<hr>



### signal conjunctionChanged 

```C++
void QfTracker::conjunctionChanged;
```




<hr>



### signal featureChanged 

```C++
void QfTracker::featureChanged;
```




<hr>



### signal featureCreated 

```C++
void QfTracker::featureCreated;
```




<hr>



### signal featureModelChanged 

```C++
void QfTracker::featureModelChanged;
```




<hr>



### signal filterAccuracyChanged 

```C++
void QfTracker::filterAccuracyChanged;
```




<hr>



### signal isActiveChanged 

```C++
void QfTracker::isActiveChanged;
```




<hr>



### signal isReplayingChanged 

```C++
void QfTracker::isReplayingChanged;
```




<hr>



### signal isSuspendedChanged 

```C++
void QfTracker::isSuspendedChanged;
```




<hr>



### signal maximumDistanceChanged 

```C++
void QfTracker::maximumDistanceChanged;
```




<hr>



### signal measureTypeChanged 

```C++
void QfTracker::measureTypeChanged;
```




<hr>



### signal minimumDistanceChanged 

```C++
void QfTracker::minimumDistanceChanged;
```




<hr>



### signal rubberbandModelChanged 

```C++
void QfTracker::rubberbandModelChanged;
```




<hr>



### signal sensorCaptureChanged 

```C++
void QfTracker::sensorCaptureChanged;
```




<hr>



### signal startPositionTimestampChanged 

```C++
void QfTracker::startPositionTimestampChanged;
```




<hr>



### signal timeIntervalChanged 

```C++
void QfTracker::timeIntervalChanged;
```




<hr>



### signal vectorLayerChanged 

```C++
void QfTracker::vectorLayerChanged;
```




<hr>



### signal visibleChanged 

```C++
void QfTracker::visibleChanged;
```




<hr>
## Public Functions Documentation




### function QfTracker 

```C++
explicit QfTracker::QfTracker (
    QgsVectorLayer * vectorLayer
) 
```




<hr>



### function color [2/2]

_Returns the track rubberband color._ 
```C++
inline QColor QfTracker::color () const
```




<hr>



### function conjunction [2/2]

_Returns TRUE if all constraints need to be fulfilled between each tracked point._ 
```C++
inline bool QfTracker::conjunction () const
```




<hr>



### function feature [2/2]

_Returns the created feature._ 
```C++
QgsFeature QfTracker::feature () const
```




<hr>



### function featureModel [2/2]

_Returns the feature model used to generate the track attributes._ 
```C++
QfFeatureModel * QfTracker::featureModel () const
```




<hr>



### function filterAccuracy [2/2]

_Returns TRUE if GNSS accuracy filtering is enabled._ 
```C++
bool QfTracker::filterAccuracy () const
```




<hr>



### function isActive [2/2]

_Returns whether the tracker has been started._ 
```C++
inline bool QfTracker::isActive () const
```




<hr>



### function isReplaying [2/2]

_Returns whether the tracker is replaying positions._ 
```C++
inline bool QfTracker::isReplaying () const
```




<hr>



### function isSuspended [2/2]

_Returns whether the track has been suspended._ 
```C++
inline bool QfTracker::isSuspended () const
```




<hr>



### function maximumDistance [2/2]

_Returns the maximum distance tolerated beyond which a position will be considered errenous._ 
```C++
inline double QfTracker::maximumDistance () const
```




<hr>



### function measureType [2/2]

_Returns the measure type used with the tracker geometry's M dimension when available._ 
```C++
inline MeasureType QfTracker::measureType () const
```




<hr>



### function minimumDistance [2/2]

_Returns the minimum distance constraint between each tracked point._ 
```C++
inline double QfTracker::minimumDistance () const
```




<hr>



### function processPositionInformation 

_Process the given position information and projected position passed onto the tracker._ 
```C++
Q_INVOKABLE void QfTracker::processPositionInformation (
    const QfGnssPositionInformation & positionInformation,
    const QgsPoint & projectedPosition
) 
```




<hr>



### function replayPositionInformationList 

_Replays a list of position information taking into account the tracker settings._ 
```C++
void QfTracker::replayPositionInformationList (
    const QList< QfGnssPositionInformation > & positionInformationList,
    QgsQuickCoordinateTransformer * coordinateTransformer=nullptr
) 
```




<hr>



### function rubberbandModel [2/2]

_Returns the rubber band model used to generate the track geometry._ 
```C++
QfRubberbandModel * QfTracker::rubberbandModel () const
```




<hr>



### function sensorCapture [2/2]

_Returns if TRUE, newly captured sensor data is needed between each tracked point._ 
```C++
inline bool QfTracker::sensorCapture () const
```




<hr>



### function setColor 

_Sets the track rubberband color._ 
```C++
void QfTracker::setColor (
    const QColor & color
) 
```




<hr>



### function setConjunction 

_Sets where all constraints need to be fulfilled between each tracked point._ 
```C++
void QfTracker::setConjunction (
    bool conjunction
) 
```




<hr>



### function setFeature 

_Sets the created feature._ 
```C++
void QfTracker::setFeature (
    const QgsFeature & feature
) 
```




<hr>



### function setFeatureModel 

_Sets the feature model used to generate the track attributes._ 
```C++
void QfTracker::setFeatureModel (
    QfFeatureModel * featureModel
) 
```




<hr>



### function setFilterAccuracy 

_Sets whether GNSS accuracy filtering is enabled._ 
```C++
void QfTracker::setFilterAccuracy (
    bool enabled
) 
```




<hr>



### function setMaximumDistance 

_Sets the maximum distance tolerated beyond which a position will be considered errenous._ 
```C++
void QfTracker::setMaximumDistance (
    double maximumDistance
) 
```




<hr>



### function setMeasureType 

_Sets the measure type used with the tracker geometry's M dimension when available._ 
```C++
void QfTracker::setMeasureType (
    MeasureType type
) 
```




<hr>



### function setMinimumDistance 

_Sets the minimum distance constraint between each tracked point._ 
```C++
void QfTracker::setMinimumDistance (
    double minimumDistance
) 
```




<hr>



### function setRubberbandModel 

_Sets the rubber band model used to generate the track geometry._ 
```C++
void QfTracker::setRubberbandModel (
    QfRubberbandModel * rubberbandModel
) 
```




<hr>



### function setSensorCapture 

_Sets whether newly captured sensor data is needed between each tracked point._ 
```C++
void QfTracker::setSensorCapture (
    bool capture
) 
```




<hr>



### function setStartPositionTimestamp 

_Sets the timestamp of the first recorded point._ 
```C++
inline void QfTracker::setStartPositionTimestamp (
    const QDateTime & startPositionTimestamp
) 
```




<hr>



### function setTimeInterval 

_Sets the minimum time interval constraint between each tracked point._ 
```C++
void QfTracker::setTimeInterval (
    double timeInterval
) 
```




<hr>



### function setVectorLayer 

_Sets the current layer._ 
```C++
void QfTracker::setVectorLayer (
    QgsVectorLayer * vectorLayer
) 
```




<hr>



### function setVisible 

_Sets whether the tracker rubberband is visible._ 
```C++
void QfTracker::setVisible (
    bool visible
) 
```




<hr>



### function start 

_Starts tracking._ 
```C++
void QfTracker::start (
    const QfGnssPositionInformation & positionInformation=QfGnssPositionInformation (),
    const QgsPoint & projectedPosition=QgsPoint()
) 
```




<hr>



### function startPositionTimestamp [2/2]

_Returns the timestamp of the first recorded point._ 
```C++
inline QDateTime QfTracker::startPositionTimestamp () const
```




<hr>



### function stop 

_Stops tracking._ 
```C++
void QfTracker::stop () 
```




<hr>



### function suspendUntilReplay 

```C++
void QfTracker::suspendUntilReplay () 
```




<hr>



### function timeInterval [2/2]

_Returns the minimum time interval constraint between each tracked point._ 
```C++
inline double QfTracker::timeInterval () const
```




<hr>



### function vectorLayer [2/2]

_Returns the current layer._ 
```C++
inline QgsVectorLayer * QfTracker::vectorLayer () const
```




<hr>



### function visible [2/2]

_Returns TRUE if the tracker rubberband is visible._ 
```C++
inline bool QfTracker::visible () const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qftracker.h`

