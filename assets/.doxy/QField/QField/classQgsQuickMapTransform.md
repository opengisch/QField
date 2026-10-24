

# Class QgsQuickMapTransform



[**ClassList**](annotated.md) **>** [**QgsQuickMapTransform**](classQgsQuickMapTransform.md)



[More...](#detailed-description)

* `#include <qgsquickmaptransform.h>`



Inherits the following classes: QQuickTransform


























## Public Properties

| Type | Name |
| ---: | :--- |
| property [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](classQgsQuickMapTransform.md#property-mapsettings-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**mapSettingsChanged**](classQgsQuickMapTransform.md#signal-mapsettingschanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QgsQuickMapTransform**](#function-qgsquickmaptransform) () = default<br>_Creates a new map transform._  |
|  void | [**applyTo**](#function-applyto) (QMatrix4x4 \* matrix) const<br> |
|  [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](#function-mapsettings-22) () const<br> |
|  void | [**setMapSettings**](#function-setmapsettings) ([**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* mapSettings) <br> |
|   | [**~QgsQuickMapTransform**](#function-qgsquickmaptransform) () = default<br> |




























## Detailed Description


The [**QgsQuickMapTransform**](classQgsQuickMapTransform.md) is transformation that can be attached to any QQuickItem.


If the item is based on the map coordinates, [**QgsQuickMapTransform**](classQgsQuickMapTransform.md) will transform it to the device coordintes based on the attached map settings.




**Note:**

QML Type: MapTransform 





    
## Public Properties Documentation




### property mapSettings [1/2]

```C++
QgsQuickMapSettings * QgsQuickMapTransform::mapSettings;
```



Associated map settings. Should be initialized before the first use from mapcanvas map settings. 


        

<hr>
## Public Signals Documentation




### signal mapSettingsChanged 

```C++
void QgsQuickMapTransform::mapSettingsChanged;
```



Associated map settings. Should be initialized before the first use from mapcanvas map settings.   


        

<hr>
## Public Functions Documentation




### function QgsQuickMapTransform 

_Creates a new map transform._ 
```C++
QgsQuickMapTransform::QgsQuickMapTransform () = default
```




<hr>



### function applyTo 

```C++
void QgsQuickMapTransform::applyTo (
    QMatrix4x4 * matrix
) const
```



Applies transformation based on current map settings to a matrix.


Also optimize resulting matrix after transformation 

**Parameters:**


* `matrix` Matrix to be transformed 




        

<hr>



### function mapSettings [2/2]

```C++
QgsQuickMapSettings * QgsQuickMapTransform::mapSettings () const
```



Associated map settings. Should be initialized before the first use from mapcanvas map settings.   


        

<hr>



### function setMapSettings 

```C++
void QgsQuickMapTransform::setMapSettings (
    QgsQuickMapSettings * mapSettings
) 
```



Associated map settings. Should be initialized before the first use from mapcanvas map settings.   


        

<hr>



### function ~QgsQuickMapTransform 

```C++
QgsQuickMapTransform::~QgsQuickMapTransform () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qgsquick/qgsquickmaptransform.h`

