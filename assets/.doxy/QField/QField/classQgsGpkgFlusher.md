

# Class QgsGpkgFlusher



[**ClassList**](annotated.md) **>** [**QgsGpkgFlusher**](classQgsGpkgFlusher.md)



[More...](#detailed-description)

* `#include <qgsgpkgflusher.h>`



Inherits the following classes: QObject






























## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**requestFlush**](classQgsGpkgFlusher.md#signal-requestflush) (const QString & filename) <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QgsGpkgFlusher**](#function-qgsgpkgflusher) (QgsProject \* project) <br> |
|  bool | [**isStopped**](#function-isstopped) (const QString & fileName) const<br> |
|  void | [**start**](#function-start) (const QString & fileName) <br> |
|  void | [**stop**](#function-stop) (const QString & fileName) <br> |
|   | [**~QgsGpkgFlusher**](#function-qgsgpkgflusher) () <br> |




























## Detailed Description


The [**QgsGpkgFlusher**](classQgsGpkgFlusher.md) class is attached to a QgsProject. It will make sure that all changes are regularly flushed from the wal file to the gpkg itself on all added layers. It will start a background thread and post an event to it whenever the gpkg has been changed. After a delay of 500ms without any changes the wal file will be flushed. The flusher does not need to be started after initialization. 


    
## Public Signals Documentation




### signal requestFlush 

```C++
void QgsGpkgFlusher::requestFlush;
```



Emitted when a file has changed and a flush should be scheduled. 


        

<hr>
## Public Functions Documentation




### function QgsGpkgFlusher 

```C++
explicit QgsGpkgFlusher::QgsGpkgFlusher (
    QgsProject * project
) 
```




<hr>



### function isStopped 

```C++
bool QgsGpkgFlusher::isStopped (
    const QString & fileName
) const
```



Returns whether the flusher is stopped for a given _fileName_ 


        

<hr>



### function start 

```C++
void QgsGpkgFlusher::start (
    const QString & fileName
) 
```



Reenables scheduling a flush for a given _fileName_. 


        

<hr>



### function stop 

```C++
void QgsGpkgFlusher::stop (
    const QString & fileName
) 
```



Immediately performs a flush for a given _fileName_ and returns. If the flusher is stopped, flush for that _fileName_ would be ignored. 


        

<hr>



### function ~QgsGpkgFlusher 

```C++
QgsGpkgFlusher::~QgsGpkgFlusher () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qgsgpkgflusher.h`

