

# Class TrackingModel



[**ClassList**](annotated.md) **>** [**TrackingModel**](classTrackingModel.md)








Inherits the following classes: QAbstractItemModel














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**TrackingRoles**](#enum-trackingroles)  <br> |
















## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**layerInTrackingChanged**](classTrackingModel.md#signal-layerintrackingchanged) (QgsVectorLayer \* layer, bool tracking) <br> |
| signal void | [**trackingSetupRequested**](classTrackingModel.md#signal-trackingsetuprequested) (QModelIndex trackerIndex, bool skipSettings) <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**TrackingModel**](#function-trackingmodel) (QObject \* parent=nullptr) <br> |
|  Q\_INVOKABLE QList&lt; QgsVectorLayer \* &gt; | [**availableLayers**](#function-availablelayers) (QgsProject \* project) const<br> |
|  Q\_INVOKABLE QgsVectorLayer \* | [**bestAvailableLayer**](#function-bestavailablelayer) (QgsProject \* project) const<br> |
|  int | [**columnCount**](#function-columncount) (const QModelIndex & parent=QModelIndex()) override const<br> |
|  Q\_INVOKABLE void | [**createProjectTrackers**](#function-createprojecttrackers) (QgsProject \* project) <br>_Creates tracking sessions defined in a project being opened._  |
|  Q\_INVOKABLE QModelIndex | [**createTracker**](#function-createtracker) (QgsVectorLayer \* layer) <br>_Creates a tracking session for the provided vector_ _layer_ _._ |
|  QVariant | [**data**](#function-data) (const QModelIndex & index, int role=Qt::DisplayRole) override const<br> |
|  Q\_INVOKABLE bool | [**featureInTracking**](#function-featureintracking) (QgsVectorLayer \* layer, QgsFeatureId featureId) <br>_Returns TRUE if the_ _featureId_ _is attached to a vector__layer_ _tracking session._ |
|  Q\_INVOKABLE bool | [**featuresInTracking**](#function-featuresintracking) (QgsVectorLayer \* layer, const QList&lt; QgsFeature &gt; & features) <br>_Returns TRUE if the list of_ _features_ _is attached to a vector__layer_ _tracking session._ |
|  QModelIndex | [**index**](#function-index) (int row, int column, const QModelIndex & parent=QModelIndex()) override const<br> |
|  Q\_INVOKABLE bool | [**layerInActiveTracking**](#function-layerinactivetracking) (QgsVectorLayer \* layer) const<br>_Returns TRUE if the vector_ _layer_ _has an active tracking session._ |
|  Q\_INVOKABLE bool | [**layerInTracking**](#function-layerintracking) (QgsVectorLayer \* layer) const<br>_Returns TRUE if the vector_ _layer_ _has a tracking session._ |
|  QModelIndex | [**parent**](#function-parent) (const QModelIndex & index) override const<br> |
|  Q\_INVOKABLE void | [**replayPositionInformationList**](#function-replaypositioninformationlist) (const QList&lt; [**GnssPositionInformation**](classGnssPositionInformation.md) &gt; & positionInformationList, [**QgsQuickCoordinateTransformer**](classQgsQuickCoordinateTransformer.md) \* coordinateTransformer=nullptr) <br>_Replays a list of position information for all active trackers._  |
|  Q\_INVOKABLE void | [**requestTrackingSetup**](#function-requesttrackingsetup) (QgsVectorLayer \* layer, bool skipSettings=false) <br> |
|  void | [**reset**](#function-reset) () <br> |
|  QHash&lt; int, QByteArray &gt; | [**roleNames**](#function-rolenames) () override const<br> |
|  int | [**rowCount**](#function-rowcount) (const QModelIndex & parent=QModelIndex()) override const<br> |
| virtual bool | [**setData**](#function-setdata) (const QModelIndex & index, const QVariant & value, int role) override<br> |
|  Q\_INVOKABLE void | [**setTrackerVisibility**](#function-settrackervisibility) (QgsVectorLayer \* layer, bool visible) <br>_Sets whether the tracking session rubber band is_ _visible_ _._ |
|  Q\_INVOKABLE void | [**startTracker**](#function-starttracker) (QgsVectorLayer \* layer, const [**GnssPositionInformation**](classGnssPositionInformation.md) & positionInformation=[**GnssPositionInformation**](classGnssPositionInformation.md)(), const QgsPoint & projectedPosition=QgsPoint()) <br>_Starts tracking for the provided vector_ _layer_ _provided it has a tracking session created._ |
|  Q\_INVOKABLE void | [**stopTracker**](#function-stoptracker) (QgsVectorLayer \* layer) <br>_Stops the tracking session of the provided vector_ _layer_ _._ |
|  Q\_INVOKABLE void | [**stopTrackers**](#function-stoptrackers) () <br>_Stops all tracking sessions._  |
|  Q\_INVOKABLE void | [**suspendUntilReplay**](#function-suspenduntilreplay) () <br> |
|  Q\_INVOKABLE [**Tracker**](classTracker.md) \* | [**trackerForLayer**](#function-trackerforlayer) (QgsVectorLayer \* layer) const<br>_Returns the tracker for the vector_ _layer_ _if a tracking session is present, otherwise returns NULLPTR._ |
|  Q\_INVOKABLE void | [**trackingSetupDone**](#function-trackingsetupdone) () <br> |
|   | [**~TrackingModel**](#function-trackingmodel) () override<br> |




























## Public Types Documentation




### enum TrackingRoles 

```C++
enum TrackingModel::TrackingRoles {
    DisplayString = Qt::UserRole,
    TrackerPointer
};
```




<hr>
## Public Signals Documentation




### signal layerInTrackingChanged 

```C++
void TrackingModel::layerInTrackingChanged;
```




<hr>



### signal trackingSetupRequested 

```C++
void TrackingModel::trackingSetupRequested;
```




<hr>
## Public Functions Documentation




### function TrackingModel 

```C++
explicit TrackingModel::TrackingModel (
    QObject * parent=nullptr
) 
```




<hr>



### function availableLayers 

```C++
Q_INVOKABLE QList< QgsVectorLayer * > TrackingModel::availableLayers (
    QgsProject * project
) const
```



Returns a list of available _project_ vector layers with which a tracking session can be started. 


        

<hr>



### function bestAvailableLayer 

```C++
Q_INVOKABLE QgsVectorLayer * TrackingModel::bestAvailableLayer (
    QgsProject * project
) const
```



Returns the best available _project_ vector layer with which a tracking session can be started. 


        

<hr>



### function columnCount 

```C++
int TrackingModel::columnCount (
    const QModelIndex & parent=QModelIndex()
) override const
```




<hr>



### function createProjectTrackers 

_Creates tracking sessions defined in a project being opened._ 
```C++
Q_INVOKABLE void TrackingModel::createProjectTrackers (
    QgsProject * project
) 
```




<hr>



### function createTracker 

_Creates a tracking session for the provided vector_ _layer_ _._
```C++
Q_INVOKABLE QModelIndex TrackingModel::createTracker (
    QgsVectorLayer * layer
) 
```




<hr>



### function data 

```C++
QVariant TrackingModel::data (
    const QModelIndex & index,
    int role=Qt::DisplayRole
) override const
```




<hr>



### function featureInTracking 

_Returns TRUE if the_ _featureId_ _is attached to a vector__layer_ _tracking session._
```C++
Q_INVOKABLE bool TrackingModel::featureInTracking (
    QgsVectorLayer * layer,
    QgsFeatureId featureId
) 
```




<hr>



### function featuresInTracking 

_Returns TRUE if the list of_ _features_ _is attached to a vector__layer_ _tracking session._
```C++
Q_INVOKABLE bool TrackingModel::featuresInTracking (
    QgsVectorLayer * layer,
    const QList< QgsFeature > & features
) 
```




<hr>



### function index 

```C++
QModelIndex TrackingModel::index (
    int row,
    int column,
    const QModelIndex & parent=QModelIndex()
) override const
```




<hr>



### function layerInActiveTracking 

_Returns TRUE if the vector_ _layer_ _has an active tracking session._
```C++
Q_INVOKABLE bool TrackingModel::layerInActiveTracking (
    QgsVectorLayer * layer
) const
```




<hr>



### function layerInTracking 

_Returns TRUE if the vector_ _layer_ _has a tracking session._
```C++
Q_INVOKABLE bool TrackingModel::layerInTracking (
    QgsVectorLayer * layer
) const
```




<hr>



### function parent 

```C++
QModelIndex TrackingModel::parent (
    const QModelIndex & index
) override const
```




<hr>



### function replayPositionInformationList 

_Replays a list of position information for all active trackers._ 
```C++
Q_INVOKABLE void TrackingModel::replayPositionInformationList (
    const QList< GnssPositionInformation > & positionInformationList,
    QgsQuickCoordinateTransformer * coordinateTransformer=nullptr
) 
```




<hr>



### function requestTrackingSetup 

```C++
Q_INVOKABLE void TrackingModel::requestTrackingSetup (
    QgsVectorLayer * layer,
    bool skipSettings=false
) 
```



Forwards a tracking setup request to the user interface consisting of a settings panel followed by a feature form (unless suppressed by the project configuration). _layer_ the vector layer associated with the tracking _skipSettings_ set to TRUE if the settings panel should be omitted and only show the feature form 


        

<hr>



### function reset 

```C++
void TrackingModel::reset () 
```




<hr>



### function roleNames 

```C++
QHash< int, QByteArray > TrackingModel::roleNames () override const
```




<hr>



### function rowCount 

```C++
int TrackingModel::rowCount (
    const QModelIndex & parent=QModelIndex()
) override const
```




<hr>



### function setData 

```C++
virtual bool TrackingModel::setData (
    const QModelIndex & index,
    const QVariant & value,
    int role
) override
```




<hr>



### function setTrackerVisibility 

_Sets whether the tracking session rubber band is_ _visible_ _._
```C++
Q_INVOKABLE void TrackingModel::setTrackerVisibility (
    QgsVectorLayer * layer,
    bool visible
) 
```




<hr>



### function startTracker 

_Starts tracking for the provided vector_ _layer_ _provided it has a tracking session created._
```C++
Q_INVOKABLE void TrackingModel::startTracker (
    QgsVectorLayer * layer,
    const GnssPositionInformation & positionInformation=GnssPositionInformation (),
    const QgsPoint & projectedPosition=QgsPoint()
) 
```




<hr>



### function stopTracker 

_Stops the tracking session of the provided vector_ _layer_ _._
```C++
Q_INVOKABLE void TrackingModel::stopTracker (
    QgsVectorLayer * layer
) 
```




<hr>



### function stopTrackers 

_Stops all tracking sessions._ 
```C++
Q_INVOKABLE void TrackingModel::stopTrackers () 
```




<hr>



### function suspendUntilReplay 

```C++
Q_INVOKABLE void TrackingModel::suspendUntilReplay () 
```




<hr>



### function trackerForLayer 

_Returns the tracker for the vector_ _layer_ _if a tracking session is present, otherwise returns NULLPTR._
```C++
Q_INVOKABLE Tracker * TrackingModel::trackerForLayer (
    QgsVectorLayer * layer
) const
```




<hr>



### function trackingSetupDone 

```C++
Q_INVOKABLE void TrackingModel::trackingSetupDone () 
```




<hr>



### function ~TrackingModel 

```C++
TrackingModel::~TrackingModel () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/trackingmodel.h`

