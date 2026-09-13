

# Class QfFeatureListExtentController



[**ClassList**](annotated.md) **>** [**QfFeatureListExtentController**](classQfFeatureListExtentController.md)








Inherits the following classes: QObject






















## Public Slots

| Type | Name |
| ---: | :--- |
| slot QgsPoint | [**getCentroidFromSelected**](classQfFeatureListExtentController.md#slot-getcentroidfromselected)  <br> |
| slot void | [**zoomToAllFeatures**](classQfFeatureListExtentController.md#slot-zoomtoallfeatures)  <br>_Zoom to the combined extent of all features in the model._  |
| slot void | [**zoomToSelected**](classQfFeatureListExtentController.md#slot-zoomtoselected) (bool skipIfIntersects=false) <br> |




## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**autoZoom**](classQfFeatureListExtentController.md#property-autozoom)  <br> |
| property bool | [**keepScale**](classQfFeatureListExtentController.md#property-keepscale)  <br> |
| property [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](classQfFeatureListExtentController.md#property-mapsettings)  <br> |
| property [**QfMultiFeatureListModel**](classQfMultiFeatureListModel.md) \* | [**model**](classQfFeatureListExtentController.md#property-model-12)  <br> |
| property [**QfFeatureListModelSelection**](classQfFeatureListModelSelection.md) \* | [**selection**](classQfFeatureListExtentController.md#property-selection-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**autoZoomChanged**](classQfFeatureListExtentController.md#signal-autozoomchanged)  <br> |
| signal void | [**featureFormStateRequested**](classQfFeatureListExtentController.md#signal-featureformstaterequested)  <br> |
| signal void | [**keepScaleChanged**](classQfFeatureListExtentController.md#signal-keepscalechanged)  <br> |
| signal void | [**mapSettingsChanged**](classQfFeatureListExtentController.md#signal-mapsettingschanged)  <br> |
| signal void | [**modelChanged**](classQfFeatureListExtentController.md#signal-modelchanged)  <br> |
| signal void | [**requestJumpToPoint**](classQfFeatureListExtentController.md#signal-requestjumptopoint) (const QgsPoint & center, const double & scale=-1.0, bool handleMargins=false) <br> |
| signal void | [**selectionChanged**](classQfFeatureListExtentController.md#signal-selectionchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfFeatureListExtentController**](#function-qffeaturelistextentcontroller) (QObject \* parent=nullptr) <br> |
|  [**QfMultiFeatureListModel**](classQfMultiFeatureListModel.md) \* | [**model**](#function-model-22) () const<br>_Returns the feature list model._  |
|  void | [**requestFeatureFormState**](#function-requestfeatureformstate) () <br>_This will emit a signal to request a state change in the feature form._  |
|  [**QfFeatureListModelSelection**](classQfFeatureListModelSelection.md) \* | [**selection**](#function-selection-22) () const<br>_Returns the selection of the feature list model._  |
|   | [**~QfFeatureListExtentController**](#function-qffeaturelistextentcontroller) () <br> |




























## Public Properties Documentation




### property autoZoom 

```C++
bool QfFeatureListExtentController::autoZoom;
```




<hr>



### property keepScale 

```C++
bool QfFeatureListExtentController::keepScale;
```




<hr>



### property mapSettings 

```C++
QgsQuickMapSettings * QfFeatureListExtentController::mapSettings;
```




<hr>



### property model [1/2]

```C++
QfMultiFeatureListModel * QfFeatureListExtentController::model;
```




<hr>



### property selection [1/2]

```C++
QfFeatureListModelSelection * QfFeatureListExtentController::selection;
```




<hr>
## Public Slots Documentation




### slot getCentroidFromSelected 

```C++
QgsPoint QfFeatureListExtentController::getCentroidFromSelected;
```




<hr>



### slot zoomToAllFeatures 

_Zoom to the combined extent of all features in the model._ 
```C++
void QfFeatureListExtentController::zoomToAllFeatures;
```




<hr>



### slot zoomToSelected 

```C++
void QfFeatureListExtentController::zoomToSelected;
```



zoom to the selected features. If _skipIfIntersects_ is true, no change will be applied if bounding box intersects with canvas extent 


        

<hr>
## Public Signals Documentation




### signal autoZoomChanged 

```C++
void QfFeatureListExtentController::autoZoomChanged;
```




<hr>



### signal featureFormStateRequested 

```C++
void QfFeatureListExtentController::featureFormStateRequested;
```




<hr>



### signal keepScaleChanged 

```C++
void QfFeatureListExtentController::keepScaleChanged;
```




<hr>



### signal mapSettingsChanged 

```C++
void QfFeatureListExtentController::mapSettingsChanged;
```




<hr>



### signal modelChanged 

```C++
void QfFeatureListExtentController::modelChanged;
```




<hr>



### signal requestJumpToPoint 

```C++
void QfFeatureListExtentController::requestJumpToPoint;
```




<hr>



### signal selectionChanged 

```C++
void QfFeatureListExtentController::selectionChanged;
```




<hr>
## Public Functions Documentation




### function QfFeatureListExtentController 

```C++
explicit QfFeatureListExtentController::QfFeatureListExtentController (
    QObject * parent=nullptr
) 
```




<hr>



### function model [2/2]

_Returns the feature list model._ 
```C++
QfMultiFeatureListModel * QfFeatureListExtentController::model () const
```




<hr>



### function requestFeatureFormState 

_This will emit a signal to request a state change in the feature form._ 
```C++
void QfFeatureListExtentController::requestFeatureFormState () 
```




<hr>



### function selection [2/2]

_Returns the selection of the feature list model._ 
```C++
QfFeatureListModelSelection * QfFeatureListExtentController::selection () const
```




<hr>



### function ~QfFeatureListExtentController 

```C++
QfFeatureListExtentController::~QfFeatureListExtentController () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qffeaturelistextentcontroller.h`

