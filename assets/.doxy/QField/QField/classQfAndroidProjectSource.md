

# Class QfAndroidProjectSource



[**ClassList**](annotated.md) **>** [**QfAndroidProjectSource**](classQfAndroidProjectSource.md)








Inherits the following classes: [QfProjectSource](classQfProjectSource.md),  QAndroidActivityResultReceiver
















































## Public Signals inherited from QfProjectSource

See [QfProjectSource](classQfProjectSource.md)

| Type | Name |
| ---: | :--- |
| signal void | [**projectOpened**](classQfProjectSource.md#signal-projectopened) (const QString & path) <br> |






## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfAndroidProjectSource**](#function-qfandroidprojectsource) (QObject \* parent=nullptr) <br> |
|  void | [**handleActivityResult**](#function-handleactivityresult) (int receiverRequestCode, int resultCode, const QJniObject & data) override<br> |


## Public Functions inherited from QfProjectSource

See [QfProjectSource](classQfProjectSource.md)

| Type | Name |
| ---: | :--- |
|   | [**QfProjectSource**](classQfProjectSource.md#function-qfprojectsource) (QObject \* parent=nullptr) <br> |
| virtual  | [**~QfProjectSource**](classQfProjectSource.md#function-qfprojectsource) () = default<br> |






















































## Public Functions Documentation




### function QfAndroidProjectSource 

```C++
explicit QfAndroidProjectSource::QfAndroidProjectSource (
    QObject * parent=nullptr
) 
```




<hr>



### function handleActivityResult 

```C++
void QfAndroidProjectSource::handleActivityResult (
    int receiverRequestCode,
    int resultCode,
    const QJniObject & data
) override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/platforms/android/qfandroidprojectsource.h`

