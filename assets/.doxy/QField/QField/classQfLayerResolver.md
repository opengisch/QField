

# Class QfLayerResolver



[**ClassList**](annotated.md) **>** [**QfLayerResolver**](classQfLayerResolver.md)



[More...](#detailed-description)

* `#include <qflayerresolver.h>`



Inherits the following classes: QObject


























## Public Properties

| Type | Name |
| ---: | :--- |
| property QgsVectorLayer \* | [**currentLayer**](classQfLayerResolver.md#property-currentlayer-12)  <br> |
| property QString | [**layerId**](classQfLayerResolver.md#property-layerid-12)  <br> |
| property QString | [**layerName**](classQfLayerResolver.md#property-layername-12)  <br> |
| property QString | [**layerProviderName**](classQfLayerResolver.md#property-layerprovidername-12)  <br> |
| property QString | [**layerSource**](classQfLayerResolver.md#property-layersource-12)  <br> |
| property QgsProject \* | [**project**](classQfLayerResolver.md#property-project-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**currentLayerChanged**](classQfLayerResolver.md#signal-currentlayerchanged)  <br> |
| signal void | [**layerIdChanged**](classQfLayerResolver.md#signal-layeridchanged)  <br> |
| signal void | [**layerNameChanged**](classQfLayerResolver.md#signal-layernamechanged)  <br> |
| signal void | [**layerProviderNameChanged**](classQfLayerResolver.md#signal-layerprovidernamechanged)  <br> |
| signal void | [**layerSourceChanged**](classQfLayerResolver.md#signal-layersourcechanged)  <br> |
| signal void | [**projectChanged**](classQfLayerResolver.md#signal-projectchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfLayerResolver**](#function-qflayerresolver) (QObject \* parent=nullptr) <br> |
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
QgsVectorLayer * QfLayerResolver::currentLayer;
```



Contains the layer after a successful resolving process 


        

<hr>



### property layerId [1/2]

```C++
QString QfLayerResolver::layerId;
```



The layer id to resolve 


        

<hr>



### property layerName [1/2]

```C++
QString QfLayerResolver::layerName;
```



The layer name to resolve 


        

<hr>



### property layerProviderName [1/2]

```C++
QString QfLayerResolver::layerProviderName;
```



The layer provider name to resolve 


        

<hr>



### property layerSource [1/2]

```C++
QString QfLayerResolver::layerSource;
```



The layer source to resolve 


        

<hr>



### property project [1/2]

```C++
QgsProject * QfLayerResolver::project;
```



The qgis project from which layers will be acquired 


        

<hr>
## Public Signals Documentation




### signal currentLayerChanged 

```C++
void QfLayerResolver::currentLayerChanged;
```




<hr>



### signal layerIdChanged 

```C++
void QfLayerResolver::layerIdChanged;
```




<hr>



### signal layerNameChanged 

```C++
void QfLayerResolver::layerNameChanged;
```




<hr>



### signal layerProviderNameChanged 

```C++
void QfLayerResolver::layerProviderNameChanged;
```




<hr>



### signal layerSourceChanged 

```C++
void QfLayerResolver::layerSourceChanged;
```




<hr>



### signal projectChanged 

```C++
void QfLayerResolver::projectChanged;
```




<hr>
## Public Functions Documentation




### function QfLayerResolver 

```C++
explicit QfLayerResolver::QfLayerResolver (
    QObject * parent=nullptr
) 
```




<hr>



### function currentLayer [2/2]

```C++
QgsVectorLayer * QfLayerResolver::currentLayer () const
```




<hr>



### function layerId [2/2]

```C++
QString QfLayerResolver::layerId () const
```




<hr>



### function layerName [2/2]

```C++
QString QfLayerResolver::layerName () const
```




<hr>



### function layerProviderName [2/2]

```C++
QString QfLayerResolver::layerProviderName () const
```




<hr>



### function layerSource [2/2]

```C++
QString QfLayerResolver::layerSource () const
```




<hr>



### function project [2/2]

```C++
QgsProject * QfLayerResolver::project () const
```




<hr>



### function resolve 

```C++
void QfLayerResolver::resolve () 
```




<hr>



### function setLayerId 

```C++
void QfLayerResolver::setLayerId (
    const QString & layerId
) 
```




<hr>



### function setLayerName 

```C++
void QfLayerResolver::setLayerName (
    const QString & layerName
) 
```




<hr>



### function setLayerProviderName 

```C++
void QfLayerResolver::setLayerProviderName (
    const QString & layerProviderName
) 
```




<hr>



### function setLayerSource 

```C++
void QfLayerResolver::setLayerSource (
    const QString & layerSource
) 
```




<hr>



### function setProject 

```C++
void QfLayerResolver::setProject (
    QgsProject * project
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qflayerresolver.h`

