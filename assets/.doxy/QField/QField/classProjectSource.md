

# Class ProjectSource



[**ClassList**](annotated.md) **>** [**ProjectSource**](classProjectSource.md)



[More...](#detailed-description)

* `#include <projectsource.h>`



Inherits the following classes: QObject


Inherited by the following classes: [AndroidProjectSource](classAndroidProjectSource.md),  [IosProjectSource](classIosProjectSource.md)




























## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**projectOpened**](classProjectSource.md#signal-projectopened) (const QString & path) <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ProjectSource**](#function-projectsource) (QObject \* parent=nullptr) <br> |
| virtual  | [**~ProjectSource**](#function-projectsource) () = default<br> |




























## Detailed Description


This class represents an ongoing open project intent. It will notify the system with the [**projectOpened()**](classProjectSource.md#signal-projectopened) signal once the user has selected a project to open.


The path can either be a QGIS project file (.qgs, .qgz) or a supported vector/raster dataset. The latter will trigger the creation of a project within which the dataset will be loaded.


The default implementation does nothing. You probably want to have a look at the [**AndroidProjectSource**](classAndroidProjectSource.md) subclass. 


    
## Public Signals Documentation




### signal projectOpened 

```C++
void ProjectSource::projectOpened;
```



This signal communicates, when a project has been opened. 


        

<hr>
## Public Functions Documentation




### function ProjectSource 

```C++
explicit ProjectSource::ProjectSource (
    QObject * parent=nullptr
) 
```




<hr>



### function ~ProjectSource 

```C++
virtual ProjectSource::~ProjectSource () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/projectsource.h`

