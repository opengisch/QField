

# Class FinlandLocatorFilter



[**ClassList**](annotated.md) **>** [**FinlandLocatorFilter**](classFinlandLocatorFilter.md)



[More...](#detailed-description)

* `#include <finlandlocatorfilter.h>`



Inherits the following classes: QgsAbstractGeocoderLocatorFilter


































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**FinlandLocatorFilter**](#function-finlandlocatorfilter) (QgsGeocoderInterface \* geocoder, [**LocatorModelSuperBridge**](classLocatorModelSuperBridge.md) \* locatorBridge) <br> |
|  const QgsRectangle | [**boundingBox**](#function-boundingbox) () const<br> |
|  [**FinlandLocatorFilter**](classFinlandLocatorFilter.md) \* | [**clone**](#function-clone) () override const<br> |
|  void | [**setBoundingBox**](#function-setboundingbox) (const QgsRectangle boundingBox) <br> |




























## Detailed Description


[**FinlandLocatorFilter**](classFinlandLocatorFilter.md) is a locator filter to search for addresses in Finland through Digitrans's API. 


    
## Public Functions Documentation




### function FinlandLocatorFilter 

```C++
explicit FinlandLocatorFilter::FinlandLocatorFilter (
    QgsGeocoderInterface * geocoder,
    LocatorModelSuperBridge * locatorBridge
) 
```




<hr>



### function boundingBox 

```C++
inline const QgsRectangle FinlandLocatorFilter::boundingBox () const
```




<hr>



### function clone 

```C++
FinlandLocatorFilter * FinlandLocatorFilter::clone () override const
```




<hr>



### function setBoundingBox 

```C++
inline void FinlandLocatorFilter::setBoundingBox (
    const QgsRectangle boundingBox
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/locator/finlandlocatorfilter.h`

