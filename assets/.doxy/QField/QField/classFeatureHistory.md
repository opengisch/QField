

# Class FeatureHistory



[**ClassList**](annotated.md) **>** [**FeatureHistory**](classFeatureHistory.md)








Inherits the following classes: QObject












## Classes

| Type | Name |
| ---: | :--- |
| struct | [**FeatureModifications**](structFeatureHistory_1_1FeatureModifications.md) <br> |














## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**isRedoAvailable**](classFeatureHistory.md#property-isredoavailable-12)  <br> |
| property bool | [**isUndoAvailable**](classFeatureHistory.md#property-isundoavailable-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**isRedoAvailableChanged**](classFeatureHistory.md#signal-isredoavailablechanged)  <br> |
| signal void | [**isUndoAvailableChanged**](classFeatureHistory.md#signal-isundoavailablechanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**FeatureHistory**](#function-featurehistory) (const QgsProject \* project, [**TrackingModel**](classTrackingModel.md) \* trackingModel=nullptr) <br> |
|  bool | [**isRedoAvailable**](#function-isredoavailable-22) () <br> |
|  bool | [**isUndoAvailable**](#function-isundoavailable-22) () <br> |
|  Q\_INVOKABLE bool | [**redo**](#function-redo) () <br>_Perform redo of the most recent modification step._  |
|  Q\_INVOKABLE const QString | [**redoMessage**](#function-redomessage) () <br>_Get the redo message to be show in the UI. NOTE should be called before calling_ _redo_ _._ |
|  Q\_INVOKABLE bool | [**undo**](#function-undo) () <br>_Perform undo of the most recent modification step._  |
|  Q\_INVOKABLE const QString | [**undoMessage**](#function-undomessage) () <br>_Get the undo message to be show in the UI. NOTE should be called before calling_ _undo_ _._ |




























## Public Properties Documentation




### property isRedoAvailable [1/2]

```C++
bool FeatureHistory::isRedoAvailable;
```




<hr>



### property isUndoAvailable [1/2]

```C++
bool FeatureHistory::isUndoAvailable;
```




<hr>
## Public Signals Documentation




### signal isRedoAvailableChanged 

```C++
void FeatureHistory::isRedoAvailableChanged;
```




<hr>



### signal isUndoAvailableChanged 

```C++
void FeatureHistory::isUndoAvailableChanged;
```




<hr>
## Public Functions Documentation




### function FeatureHistory 

```C++
explicit FeatureHistory::FeatureHistory (
    const QgsProject * project,
    TrackingModel * trackingModel=nullptr
) 
```



Construct a new Feature history object




**Parameters:**


* `project` the current project instance 
* `trackingModel` the tracking model 




        

<hr>



### function isRedoAvailable [2/2]

```C++
bool FeatureHistory::isRedoAvailable () 
```




<hr>



### function isUndoAvailable [2/2]

```C++
bool FeatureHistory::isUndoAvailable () 
```




<hr>



### function redo 

_Perform redo of the most recent modification step._ 
```C++
Q_INVOKABLE bool FeatureHistory::redo () 
```




<hr>



### function redoMessage 

_Get the redo message to be show in the UI. NOTE should be called before calling_ _redo_ _._
```C++
Q_INVOKABLE const QString FeatureHistory::redoMessage () 
```




<hr>



### function undo 

_Perform undo of the most recent modification step._ 
```C++
Q_INVOKABLE bool FeatureHistory::undo () 
```




<hr>



### function undoMessage 

_Get the undo message to be show in the UI. NOTE should be called before calling_ _undo_ _._
```C++
Q_INVOKABLE const QString FeatureHistory::undoMessage () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/featurehistory.h`

