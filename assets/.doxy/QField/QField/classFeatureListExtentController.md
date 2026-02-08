

# Class FeatureListExtentController



[**ClassList**](annotated.md) **>** [**FeatureListExtentController**](classFeatureListExtentController.md)








Inherits the following classes: QObject






















## Public Slots

| Type | Name |
| ---: | :--- |
| slot QgsPoint | [**getCentroidFromSelected**](classFeatureListExtentController.md#slot-getcentroidfromselected)  <br> |
| slot void | [**zoomToAllFeatures**](classFeatureListExtentController.md#slot-zoomtoallfeatures)  <br>_Zoom to the combined extent of all features in the model._  |
| slot void | [**zoomToSelected**](classFeatureListExtentController.md#slot-zoomtoselected) (bool skipIfIntersects=false) <br> |




## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**autoZoom**](classFeatureListExtentController.md#property-autozoom)  <br> |
| property bool | [**keepScale**](classFeatureListExtentController.md#property-keepscale)  <br> |
| property [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](classFeatureListExtentController.md#property-mapsettings)  <br> |
| property [**MultiFeatureListModel**](classMultiFeatureListModel.md) \* | [**model**](classFeatureListExtentController.md#property-model-12)  <br> |
| property [**FeatureListModelSelection**](classFeatureListModelSelection.md) \* | [**selection**](classFeatureListExtentController.md#property-selection-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**autoZoomChanged**](classFeatureListExtentController.md#signal-autozoomchanged)  <br> |
| signal void | [**featureFormStateRequested**](classFeatureListExtentController.md#signal-featureformstaterequested)  <br> |
| signal void | [**keepScaleChanged**](classFeatureListExtentController.md#signal-keepscalechanged)  <br> |
| signal void | [**mapSettingsChanged**](classFeatureListExtentController.md#signal-mapsettingschanged)  <br> |
| signal void | [**modelChanged**](classFeatureListExtentController.md#signal-modelchanged)  <br> |
| signal void | [**requestJumpToPoint**](classFeatureListExtentController.md#signal-requestjumptopoint) (const QgsPoint & center, const double & scale=-1.0, bool handleMargins=false) <br> |
| signal void | [**selectionChanged**](classFeatureListExtentController.md#signal-selectionchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**FeatureListExtentController**](#function-featurelistextentcontroller) (QObject \* parent=nullptr) <br> |
|  [**MultiFeatureListModel**](classMultiFeatureListModel.md) \* | [**model**](#function-model-22) () const<br>_Returns the feature list model._  |
|  void | [**requestFeatureFormState**](#function-requestfeatureformstate) () <br>_This will emit a signal to request a state change in the feature form._  |
|  [**FeatureListModelSelection**](classFeatureListModelSelection.md) \* | [**selection**](#function-selection-22) () const<br>_Returns the selection of the feature list model._  |
|   | [**~FeatureListExtentController**](#function-featurelistextentcontroller) () <br> |




























## Public Properties Documentation




### property autoZoom 

```C++
bool FeatureListExtentController::autoZoom;
```




<hr>



### property keepScale 

```C++
bool FeatureListExtentController::keepScale;
```




<hr>



### property mapSettings 

```C++
QgsQuickMapSettings * FeatureListExtentController::mapSettings;
```




<hr>



### property model [1/2]

```C++
MultiFeatureListModel * FeatureListExtentController::model;
```




<hr>



### property selection [1/2]

```C++
FeatureListModelSelection * FeatureListExtentController::selection;
```




<hr>
## Public Slots Documentation




### slot getCentroidFromSelected 

```C++
QgsPoint FeatureListExtentController::getCentroidFromSelected;
```




<hr>



### slot zoomToAllFeatures 

_Zoom to the combined extent of all features in the model._ 
```C++
void FeatureListExtentController::zoomToAllFeatures;
```




<hr>



### slot zoomToSelected 

```C++
void FeatureListExtentController::zoomToSelected;
```



zoom to the selected features. If _skipIfIntersects_ is true, no change will be applied if bounding box intersects with canvas extent 


        

<hr>
## Public Signals Documentation




### signal autoZoomChanged 

```C++
void FeatureListExtentController::autoZoomChanged;
```




<hr>



### signal featureFormStateRequested 

```C++
void FeatureListExtentController::featureFormStateRequested;
```




<hr>



### signal keepScaleChanged 

```C++
void FeatureListExtentController::keepScaleChanged;
```




<hr>



### signal mapSettingsChanged 

```C++
void FeatureListExtentController::mapSettingsChanged;
```




<hr>



### signal modelChanged 

```C++
void FeatureListExtentController::modelChanged;
```




<hr>



### signal requestJumpToPoint 

```C++
void FeatureListExtentController::requestJumpToPoint;
```




<hr>



### signal selectionChanged 

```C++
void FeatureListExtentController::selectionChanged;
```




<hr>
## Public Functions Documentation




### function FeatureListExtentController 

```C++
explicit FeatureListExtentController::FeatureListExtentController (
    QObject * parent=nullptr
) 
```




<hr>



### function model [2/2]

_Returns the feature list model._ 
```C++
MultiFeatureListModel * FeatureListExtentController::model () const
```




<hr>



### function requestFeatureFormState 

_This will emit a signal to request a state change in the feature form._ 
```C++
void FeatureListExtentController::requestFeatureFormState () 
```




<hr>



### function selection [2/2]

_Returns the selection of the feature list model._ 
```C++
FeatureListModelSelection * FeatureListExtentController::selection () const
```




<hr>



### function ~FeatureListExtentController 

```C++
FeatureListExtentController::~FeatureListExtentController () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/featurelistextentcontroller.h`

