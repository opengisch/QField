

# Class IosResourceSource



[**ClassList**](annotated.md) **>** [**IosResourceSource**](classIosResourceSource.md)








Inherits the following classes: [ResourceSource](classResourceSource.md)






























## Public Slots

| Type | Name |
| ---: | :--- |
| slot void | [**pickGalleryPicture**](classIosResourceSource.md#slot-pickgallerypicture)  <br> |
| slot void | [**pickGalleryVideo**](classIosResourceSource.md#slot-pickgalleryvideo)  <br> |
| slot void | [**takePicture**](classIosResourceSource.md#slot-takepicture)  <br> |
| slot void | [**takeVideo**](classIosResourceSource.md#slot-takevideo)  <br> |
















## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**resourceReceived**](classIosResourceSource.md#signal-resourcereceived) (const QString & path) <br> |


## Public Signals inherited from ResourceSource

See [ResourceSource](classResourceSource.md)

| Type | Name |
| ---: | :--- |
| signal void | [**resourceReceived**](classResourceSource.md#signal-resourcereceived) (const QString & path) <br> |






## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**IosResourceSource**](#function-iosresourcesource) (const QString & prefix=QString(), const QString & resourceFilePath=QString(), QObject \* parent=nullptr) <br> |
|  QString | [**prefixPath**](#function-prefixpath) () const<br> |
|  QString | [**resourceFilePath**](#function-resourcefilepath) () const<br> |


## Public Functions inherited from ResourceSource

See [ResourceSource](classResourceSource.md)

| Type | Name |
| ---: | :--- |
|   | [**ResourceSource**](classResourceSource.md#function-resourcesource) (QObject \* parent=nullptr, const QString & prefix=QString(), const QString & resourceFilePath=QString()) <br> |
| virtual  | [**~ResourceSource**](classResourceSource.md#function-resourcesource) () <br> |






















































## Public Slots Documentation




### slot pickGalleryPicture 

```C++
void IosResourceSource::pickGalleryPicture;
```




<hr>



### slot pickGalleryVideo 

```C++
void IosResourceSource::pickGalleryVideo;
```




<hr>



### slot takePicture 

```C++
void IosResourceSource::takePicture;
```




<hr>



### slot takeVideo 

```C++
void IosResourceSource::takeVideo;
```




<hr>
## Public Signals Documentation




### signal resourceReceived 

```C++
void IosResourceSource::resourceReceived;
```




<hr>
## Public Functions Documentation




### function IosResourceSource 

```C++
explicit IosResourceSource::IosResourceSource (
    const QString & prefix=QString(),
    const QString & resourceFilePath=QString(),
    QObject * parent=nullptr
) 
```




<hr>



### function prefixPath 

```C++
inline QString IosResourceSource::prefixPath () const
```




<hr>



### function resourceFilePath 

```C++
inline QString IosResourceSource::resourceFilePath () const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/platforms/ios/iosresourcesource.h`

