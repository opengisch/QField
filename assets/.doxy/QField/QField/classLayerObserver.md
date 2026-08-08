

# Class LayerObserver



[**ClassList**](annotated.md) **>** [**LayerObserver**](classLayerObserver.md)



[More...](#detailed-description)

* `#include <layerobserver.h>`



Inherits the following classes: QObject


























## Public Properties

| Type | Name |
| ---: | :--- |
| property [**DeltaFileWrapper**](classDeltaFileWrapper.md) \* | [**deltaFileWrapper**](classLayerObserver.md#property-deltafilewrapper-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**deltaFileWrapperChanged**](classLayerObserver.md#signal-deltafilewrapperchanged)  <br> |
| signal void | [**layerEdited**](classLayerObserver.md#signal-layeredited) (const QString & layerId) <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**LayerObserver**](#function-layerobserver) (const QgsProject \* project) <br> |
|  void | [**addLayerListeners**](#function-addlayerlisteners) () <br> |
|  [**DeltaFileWrapper**](classDeltaFileWrapper.md) \* | [**deltaFileWrapper**](#function-deltafilewrapper-22) () const<br> |
|  void | [**setDeltaFileWrapper**](#function-setdeltafilewrapper) ([**DeltaFileWrapper**](classDeltaFileWrapper.md) \* wrapper) <br> |




























## Detailed Description


Monitors all layers for changes and writes those changes to a delta file 


    
## Public Properties Documentation




### property deltaFileWrapper [1/2]

```C++
DeltaFileWrapper * LayerObserver::deltaFileWrapper;
```




<hr>
## Public Signals Documentation




### signal deltaFileWrapperChanged 

```C++
void LayerObserver::deltaFileWrapperChanged;
```




<hr>



### signal layerEdited 

```C++
void LayerObserver::layerEdited;
```




<hr>
## Public Functions Documentation




### function LayerObserver 

```C++
explicit LayerObserver::LayerObserver (
    const QgsProject * project
) 
```



Construct a new Layer Observer object




**Parameters:**


* `project` 




        

<hr>



### function addLayerListeners 

```C++
void LayerObserver::addLayerListeners () 
```



Add the needed event listeners to monitor for changes. Assigns listeners only for layer actions of `cloud` and `offline`. 


        

<hr>



### function deltaFileWrapper [2/2]

```C++
DeltaFileWrapper * LayerObserver::deltaFileWrapper () const
```



Gets the current delta file




**Returns:**

current delta file 





        

<hr>



### function setDeltaFileWrapper 

```C++
void LayerObserver::setDeltaFileWrapper (
    DeltaFileWrapper * wrapper
) 
```



Sets the current delta file




**Returns:**

current delta file 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfieldcloud/layerobserver.h`

