

# Class QfIosResourceSource



[**ClassList**](annotated.md) **>** [**QfIosResourceSource**](classQfIosResourceSource.md)








Inherits the following classes: [QfResourceSource](classQfResourceSource.md)






























## Public Slots

| Type | Name |
| ---: | :--- |
| slot void | [**pickGalleryPicture**](classQfIosResourceSource.md#slot-pickgallerypicture)  <br> |
| slot void | [**pickGalleryVideo**](classQfIosResourceSource.md#slot-pickgalleryvideo)  <br> |
| slot void | [**takePicture**](classQfIosResourceSource.md#slot-takepicture)  <br> |
| slot void | [**takeVideo**](classQfIosResourceSource.md#slot-takevideo)  <br> |
















## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**resourceReceived**](classQfIosResourceSource.md#signal-resourcereceived) (const QString & path) <br> |


## Public Signals inherited from QfResourceSource

See [QfResourceSource](classQfResourceSource.md)

| Type | Name |
| ---: | :--- |
| signal void | [**resourceReceived**](classQfResourceSource.md#signal-resourcereceived) (const QString & path) <br> |






## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfIosResourceSource**](#function-qfiosresourcesource) (const QString & prefix=QString(), const QString & resourceFilePath=QString(), QObject \* parent=nullptr) <br> |
|  QString | [**prefixPath**](#function-prefixpath) () const<br> |
|  QString | [**resourceFilePath**](#function-resourcefilepath) () const<br> |


## Public Functions inherited from QfResourceSource

See [QfResourceSource](classQfResourceSource.md)

| Type | Name |
| ---: | :--- |
|   | [**QfResourceSource**](classQfResourceSource.md#function-qfresourcesource) (QObject \* parent=nullptr, const QString & prefix=QString(), const QString & resourceFilePath=QString()) <br> |
| virtual  | [**~QfResourceSource**](classQfResourceSource.md#function-qfresourcesource) () <br> |






















































## Public Slots Documentation




### slot pickGalleryPicture 

```C++
void QfIosResourceSource::pickGalleryPicture;
```




<hr>



### slot pickGalleryVideo 

```C++
void QfIosResourceSource::pickGalleryVideo;
```




<hr>



### slot takePicture 

```C++
void QfIosResourceSource::takePicture;
```




<hr>



### slot takeVideo 

```C++
void QfIosResourceSource::takeVideo;
```




<hr>
## Public Signals Documentation




### signal resourceReceived 

```C++
void QfIosResourceSource::resourceReceived;
```




<hr>
## Public Functions Documentation




### function QfIosResourceSource 

```C++
explicit QfIosResourceSource::QfIosResourceSource (
    const QString & prefix=QString(),
    const QString & resourceFilePath=QString(),
    QObject * parent=nullptr
) 
```




<hr>



### function prefixPath 

```C++
inline QString QfIosResourceSource::prefixPath () const
```




<hr>



### function resourceFilePath 

```C++
inline QString QfIosResourceSource::resourceFilePath () const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/platforms/ios/qfiosresourcesource.h`

