

# Class QfProjectSource



[**ClassList**](annotated.md) **>** [**QfProjectSource**](classQfProjectSource.md)



[More...](#detailed-description)

* `#include <qfprojectsource.h>`



Inherits the following classes: QObject


Inherited by the following classes: [QfAndroidProjectSource](classQfAndroidProjectSource.md),  [QfIosProjectSource](classQfIosProjectSource.md)




























## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**projectOpened**](classQfProjectSource.md#signal-projectopened) (const QString & path) <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfProjectSource**](#function-qfprojectsource) (QObject \* parent=nullptr) <br> |
| virtual  | [**~QfProjectSource**](#function-qfprojectsource) () = default<br> |




























## Detailed Description


This class represents an ongoing open project intent. It will notify the system with the [**projectOpened()**](classQfProjectSource.md#signal-projectopened) signal once the user has selected a project to open.


The path can either be a QGIS project file (.qgs, .qgz) or a supported vector/raster dataset. The latter will trigger the creation of a project within which the dataset will be loaded.


The default implementation does nothing. You probably want to have a look at the [**QfAndroidProjectSource**](classQfAndroidProjectSource.md) subclass. 


    
## Public Signals Documentation




### signal projectOpened 

```C++
void QfProjectSource::projectOpened;
```



This signal communicates, when a project has been opened. 


        

<hr>
## Public Functions Documentation




### function QfProjectSource 

```C++
explicit QfProjectSource::QfProjectSource (
    QObject * parent=nullptr
) 
```




<hr>



### function ~QfProjectSource 

```C++
virtual QfProjectSource::~QfProjectSource () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfprojectsource.h`

