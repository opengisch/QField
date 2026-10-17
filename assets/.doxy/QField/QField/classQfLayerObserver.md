

# Class QfLayerObserver



[**ClassList**](annotated.md) **>** [**QfLayerObserver**](classQfLayerObserver.md)



[More...](#detailed-description)

* `#include <qflayerobserver.h>`



Inherits the following classes: QObject


























## Public Properties

| Type | Name |
| ---: | :--- |
| property [**QfDeltaFileWrapper**](classQfDeltaFileWrapper.md) \* | [**deltaFileWrapper**](classQfLayerObserver.md#property-deltafilewrapper-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**deltaFileWrapperChanged**](classQfLayerObserver.md#signal-deltafilewrapperchanged)  <br> |
| signal void | [**layerEdited**](classQfLayerObserver.md#signal-layeredited) (const QString & layerId) <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfLayerObserver**](#function-qflayerobserver) (const QgsProject \* project) <br> |
|  void | [**addLayerListeners**](#function-addlayerlisteners) () <br> |
|  [**QfDeltaFileWrapper**](classQfDeltaFileWrapper.md) \* | [**deltaFileWrapper**](#function-deltafilewrapper-22) () const<br> |
|  void | [**setDeltaFileWrapper**](#function-setdeltafilewrapper) ([**QfDeltaFileWrapper**](classQfDeltaFileWrapper.md) \* wrapper) <br> |




























## Detailed Description


Monitors all layers for changes and writes those changes to a delta file 


    
## Public Properties Documentation




### property deltaFileWrapper [1/2]

```C++
QfDeltaFileWrapper * QfLayerObserver::deltaFileWrapper;
```




<hr>
## Public Signals Documentation




### signal deltaFileWrapperChanged 

```C++
void QfLayerObserver::deltaFileWrapperChanged;
```




<hr>



### signal layerEdited 

```C++
void QfLayerObserver::layerEdited;
```




<hr>
## Public Functions Documentation




### function QfLayerObserver 

```C++
explicit QfLayerObserver::QfLayerObserver (
    const QgsProject * project
) 
```



Construct a new Layer Observer object




**Parameters:**


* `project` 




        

<hr>



### function addLayerListeners 

```C++
void QfLayerObserver::addLayerListeners () 
```



Add the needed event listeners to monitor for changes. Assigns listeners only for layer actions of `cloud` and `offline`. 


        

<hr>



### function deltaFileWrapper [2/2]

```C++
QfDeltaFileWrapper * QfLayerObserver::deltaFileWrapper () const
```



Gets the current delta file




**Returns:**

current delta file 





        

<hr>



### function setDeltaFileWrapper 

```C++
void QfLayerObserver::setDeltaFileWrapper (
    QfDeltaFileWrapper * wrapper
) 
```



Sets the current delta file




**Returns:**

current delta file 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfieldcloud/qflayerobserver.h`

