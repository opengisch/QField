

# Class QfResourceSource



[**ClassList**](annotated.md) **>** [**QfResourceSource**](classQfResourceSource.md)



[More...](#detailed-description)

* `#include <qfresourcesource.h>`



Inherits the following classes: QObject


Inherited by the following classes: [QfAndroidResourceSource](classQfAndroidResourceSource.md),  [QfIosResourceSource](classQfIosResourceSource.md)




























## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**resourceReceived**](classQfResourceSource.md#signal-resourcereceived) (const QString & path) <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfResourceSource**](#function-qfresourcesource) (QObject \* parent=nullptr, const QString & prefix=QString(), const QString & resourceFilePath=QString()) <br> |
| virtual  | [**~QfResourceSource**](#function-qfresourcesource) () <br> |




























## Detailed Description


A resource source object will be created when a new resource is requested from the system. It should be subclassed and filled with platform specific code. 


    
## Public Signals Documentation




### signal resourceReceived 

```C++
void QfResourceSource::resourceReceived;
```



Emit this signal when a resource really has been received.




**Note:**

When the constructor received a non-empty _resourceFilePath_, the signal is emitted in the constructor. 





        

<hr>
## Public Functions Documentation




### function QfResourceSource 

```C++
explicit QfResourceSource::QfResourceSource (
    QObject * parent=nullptr,
    const QString & prefix=QString(),
    const QString & resourceFilePath=QString()
) 
```



Construct a new [**QfResourceSource**](classQfResourceSource.md) object.




**Note:**

Subclasses which implement their own file dialog should provide an empty string for _resourceFilePath_ and emit _resourceReceived_ when appropriate. 




**Parameters:**


* `parent` Parent object 
* `prefix` The project folder. Base directory path for all relative paths. 
* `resourceFilePath` Suggested file path to permanently store the file. If the file is not existing yet, it must be an empty string. 




        

<hr>



### function ~QfResourceSource 

```C++
virtual QfResourceSource::~QfResourceSource () 
```



Destroy the [**QfResourceSource**](classQfResourceSource.md) object 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfresourcesource.h`

