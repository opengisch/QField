

# Class AndroidResourceSource



[**ClassList**](annotated.md) **>** [**AndroidResourceSource**](classAndroidResourceSource.md)








Inherits the following classes: [ResourceSource](classResourceSource.md)
















































## Public Signals inherited from ResourceSource

See [ResourceSource](classResourceSource.md)

| Type | Name |
| ---: | :--- |
| signal void | [**resourceReceived**](classResourceSource.md#signal-resourcereceived) (const QString & path) <br> |






## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**AndroidResourceSource**](#function-androidresourcesource) (const QString & prefix, QObject \* parent) <br> |


## Public Functions inherited from ResourceSource

See [ResourceSource](classResourceSource.md)

| Type | Name |
| ---: | :--- |
|   | [**ResourceSource**](classResourceSource.md#function-resourcesource) (QObject \* parent=nullptr, const QString & prefix=QString(), const QString & resourceFilePath=QString()) <br> |
| virtual  | [**~ResourceSource**](classResourceSource.md#function-resourcesource) () <br> |






















































## Public Functions Documentation




### function AndroidResourceSource 

```C++
explicit AndroidResourceSource::AndroidResourceSource (
    const QString & prefix,
    QObject * parent
) 
```



Construct a new [**AndroidResourceSource**](classAndroidResourceSource.md) object




**Parameters:**


* `prefix` The project folder. Base directory path for all relative paths. 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/platforms/android/androidresourcesource.h`

