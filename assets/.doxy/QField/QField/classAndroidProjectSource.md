

# Class AndroidProjectSource



[**ClassList**](annotated.md) **>** [**AndroidProjectSource**](classAndroidProjectSource.md)








Inherits the following classes: [ProjectSource](classProjectSource.md),  QAndroidActivityResultReceiver
















































## Public Signals inherited from ProjectSource

See [ProjectSource](classProjectSource.md)

| Type | Name |
| ---: | :--- |
| signal void | [**projectOpened**](classProjectSource.md#signal-projectopened) (const QString & path) <br> |






## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**AndroidProjectSource**](#function-androidprojectsource) (QObject \* parent=nullptr) <br> |
|  void | [**handleActivityResult**](#function-handleactivityresult) (int receiverRequestCode, int resultCode, const QJniObject & data) override<br> |


## Public Functions inherited from ProjectSource

See [ProjectSource](classProjectSource.md)

| Type | Name |
| ---: | :--- |
|   | [**ProjectSource**](classProjectSource.md#function-projectsource) (QObject \* parent=nullptr) <br> |
| virtual  | [**~ProjectSource**](classProjectSource.md#function-projectsource) () = default<br> |






















































## Public Functions Documentation




### function AndroidProjectSource 

```C++
explicit AndroidProjectSource::AndroidProjectSource (
    QObject * parent=nullptr
) 
```




<hr>



### function handleActivityResult 

```C++
void AndroidProjectSource::handleActivityResult (
    int receiverRequestCode,
    int resultCode,
    const QJniObject & data
) override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/platforms/android/androidprojectsource.h`

