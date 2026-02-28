

# Class IosProjectSource



[**ClassList**](annotated.md) **>** [**IosProjectSource**](classIosProjectSource.md)








Inherits the following classes: [ProjectSource](classProjectSource.md)






























## Public Slots

| Type | Name |
| ---: | :--- |
| slot void | [**pickProject**](classIosProjectSource.md#slot-pickproject)  <br> |


















## Public Signals inherited from ProjectSource

See [ProjectSource](classProjectSource.md)

| Type | Name |
| ---: | :--- |
| signal void | [**projectOpened**](classProjectSource.md#signal-projectopened) (const QString & path) <br> |






## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**IosProjectSource**](#function-iosprojectsource) (QObject \* parent=nullptr) <br> |
|  QString | [**projectFromFolder**](#function-projectfromfolder) (const QString & folder) const<br> |


## Public Functions inherited from ProjectSource

See [ProjectSource](classProjectSource.md)

| Type | Name |
| ---: | :--- |
|   | [**ProjectSource**](classProjectSource.md#function-projectsource) (QObject \* parent=nullptr) <br> |
| virtual  | [**~ProjectSource**](classProjectSource.md#function-projectsource) () = default<br> |






















































## Public Slots Documentation




### slot pickProject 

```C++
void IosProjectSource::pickProject;
```




<hr>
## Public Functions Documentation




### function IosProjectSource 

```C++
explicit IosProjectSource::IosProjectSource (
    QObject * parent=nullptr
) 
```




<hr>



### function projectFromFolder 

```C++
QString IosProjectSource::projectFromFolder (
    const QString & folder
) const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/platforms/ios/iosprojectsource.h`

