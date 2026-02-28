

# Class LayerResolver



[**ClassList**](annotated.md) **>** [**LayerResolver**](classLayerResolver.md)



[More...](#detailed-description)

* `#include <layerresolver.h>`



Inherits the following classes: QObject


























## Public Properties

| Type | Name |
| ---: | :--- |
| property QgsVectorLayer \* | [**currentLayer**](classLayerResolver.md#property-currentlayer-12)  <br> |
| property QString | [**layerId**](classLayerResolver.md#property-layerid-12)  <br> |
| property QString | [**layerName**](classLayerResolver.md#property-layername-12)  <br> |
| property QString | [**layerProviderName**](classLayerResolver.md#property-layerprovidername-12)  <br> |
| property QString | [**layerSource**](classLayerResolver.md#property-layersource-12)  <br> |
| property QgsProject \* | [**project**](classLayerResolver.md#property-project-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**currentLayerChanged**](classLayerResolver.md#signal-currentlayerchanged)  <br> |
| signal void | [**layerIdChanged**](classLayerResolver.md#signal-layeridchanged)  <br> |
| signal void | [**layerNameChanged**](classLayerResolver.md#signal-layernamechanged)  <br> |
| signal void | [**layerProviderNameChanged**](classLayerResolver.md#signal-layerprovidernamechanged)  <br> |
| signal void | [**layerSourceChanged**](classLayerResolver.md#signal-layersourcechanged)  <br> |
| signal void | [**projectChanged**](classLayerResolver.md#signal-projectchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**LayerResolver**](#function-layerresolver) (QObject \* parent=nullptr) <br> |
|  QgsVectorLayer \* | [**currentLayer**](#function-currentlayer-22) () const<br> |
|  QString | [**layerId**](#function-layerid-22) () const<br> |
|  QString | [**layerName**](#function-layername-22) () const<br> |
|  QString | [**layerProviderName**](#function-layerprovidername-22) () const<br> |
|  QString | [**layerSource**](#function-layersource-22) () const<br> |
|  QgsProject \* | [**project**](#function-project-22) () const<br> |
|  void | [**resolve**](#function-resolve) () <br> |
|  void | [**setLayerId**](#function-setlayerid) (const QString & layerId) <br> |
|  void | [**setLayerName**](#function-setlayername) (const QString & layerName) <br> |
|  void | [**setLayerProviderName**](#function-setlayerprovidername) (const QString & layerProviderName) <br> |
|  void | [**setLayerSource**](#function-setlayersource) (const QString & layerSource) <br> |
|  void | [**setProject**](#function-setproject) (QgsProject \* project) <br> |




























## Detailed Description


Helper class to resolve layers by id, name, source and provider. 


    
## Public Properties Documentation




### property currentLayer [1/2]

```C++
QgsVectorLayer * LayerResolver::currentLayer;
```



Contains the layer after a successful resolving process 


        

<hr>



### property layerId [1/2]

```C++
QString LayerResolver::layerId;
```



The layer id to resolve 


        

<hr>



### property layerName [1/2]

```C++
QString LayerResolver::layerName;
```



The layer name to resolve 


        

<hr>



### property layerProviderName [1/2]

```C++
QString LayerResolver::layerProviderName;
```



The layer provider name to resolve 


        

<hr>



### property layerSource [1/2]

```C++
QString LayerResolver::layerSource;
```



The layer source to resolve 


        

<hr>



### property project [1/2]

```C++
QgsProject * LayerResolver::project;
```



The qgis project from which layers will be acquired 


        

<hr>
## Public Signals Documentation




### signal currentLayerChanged 

```C++
void LayerResolver::currentLayerChanged;
```




<hr>



### signal layerIdChanged 

```C++
void LayerResolver::layerIdChanged;
```




<hr>



### signal layerNameChanged 

```C++
void LayerResolver::layerNameChanged;
```




<hr>



### signal layerProviderNameChanged 

```C++
void LayerResolver::layerProviderNameChanged;
```




<hr>



### signal layerSourceChanged 

```C++
void LayerResolver::layerSourceChanged;
```




<hr>



### signal projectChanged 

```C++
void LayerResolver::projectChanged;
```




<hr>
## Public Functions Documentation




### function LayerResolver 

```C++
explicit LayerResolver::LayerResolver (
    QObject * parent=nullptr
) 
```




<hr>



### function currentLayer [2/2]

```C++
QgsVectorLayer * LayerResolver::currentLayer () const
```




<hr>



### function layerId [2/2]

```C++
QString LayerResolver::layerId () const
```




<hr>



### function layerName [2/2]

```C++
QString LayerResolver::layerName () const
```




<hr>



### function layerProviderName [2/2]

```C++
QString LayerResolver::layerProviderName () const
```




<hr>



### function layerSource [2/2]

```C++
QString LayerResolver::layerSource () const
```




<hr>



### function project [2/2]

```C++
QgsProject * LayerResolver::project () const
```




<hr>



### function resolve 

```C++
void LayerResolver::resolve () 
```




<hr>



### function setLayerId 

```C++
void LayerResolver::setLayerId (
    const QString & layerId
) 
```




<hr>



### function setLayerName 

```C++
void LayerResolver::setLayerName (
    const QString & layerName
) 
```




<hr>



### function setLayerProviderName 

```C++
void LayerResolver::setLayerProviderName (
    const QString & layerProviderName
) 
```




<hr>



### function setLayerSource 

```C++
void LayerResolver::setLayerSource (
    const QString & layerSource
) 
```




<hr>



### function setProject 

```C++
void LayerResolver::setProject (
    QgsProject * project
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/layerresolver.h`

