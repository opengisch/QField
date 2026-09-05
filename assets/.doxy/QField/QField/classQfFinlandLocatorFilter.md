

# Class QfFinlandLocatorFilter



[**ClassList**](annotated.md) **>** [**QfFinlandLocatorFilter**](classQfFinlandLocatorFilter.md)



[More...](#detailed-description)

* `#include <qffinlandlocatorfilter.h>`



Inherits the following classes: QgsAbstractGeocoderLocatorFilter


































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfFinlandLocatorFilter**](#function-qffinlandlocatorfilter) (QgsGeocoderInterface \* geocoder, [**QfLocatorModelSuperBridge**](classQfLocatorModelSuperBridge.md) \* locatorBridge) <br> |
|  const QgsRectangle | [**boundingBox**](#function-boundingbox) () const<br> |
|  [**QfFinlandLocatorFilter**](classQfFinlandLocatorFilter.md) \* | [**clone**](#function-clone) () override const<br> |
|  void | [**setBoundingBox**](#function-setboundingbox) (const QgsRectangle boundingBox) <br> |




























## Detailed Description


[**QfFinlandLocatorFilter**](classQfFinlandLocatorFilter.md) is a locator filter to search for addresses in Finland through Digitrans's API. 


    
## Public Functions Documentation




### function QfFinlandLocatorFilter 

```C++
explicit QfFinlandLocatorFilter::QfFinlandLocatorFilter (
    QgsGeocoderInterface * geocoder,
    QfLocatorModelSuperBridge * locatorBridge
) 
```




<hr>



### function boundingBox 

```C++
inline const QgsRectangle QfFinlandLocatorFilter::boundingBox () const
```




<hr>



### function clone 

```C++
QfFinlandLocatorFilter * QfFinlandLocatorFilter::clone () override const
```




<hr>



### function setBoundingBox 

```C++
inline void QfFinlandLocatorFilter::setBoundingBox (
    const QgsRectangle boundingBox
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/locator/qffinlandlocatorfilter.h`

