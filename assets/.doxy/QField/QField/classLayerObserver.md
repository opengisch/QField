

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
|  [**DeltaFileWrapper**](classDeltaFileWrapper.md) \* | [**deltaFileWrapper**](#function-deltafilewrapper-22) () const<br> |
|  Q\_INVOKABLE void | [**reset**](#function-reset) (bool isHardReset=false) const<br> |
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



### function deltaFileWrapper [2/2]

```C++
DeltaFileWrapper * LayerObserver::deltaFileWrapper () const
```



Gets the current delta file




**Returns:**

current delta file 





        

<hr>



### function reset 

```C++
Q_INVOKABLE void LayerObserver::reset (
    bool isHardReset=false
) const
```



Clears the current delta file changes 


        

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

