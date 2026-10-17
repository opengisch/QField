

# Class QfCogoRegistry



[**ClassList**](annotated.md) **>** [**QfCogoRegistry**](classQfCogoRegistry.md)



_A registry managing available COGO operations._ 

* `#include <qfcogoregistry.h>`



Inherits the following classes: QObject






























## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**operationAdded**](classQfCogoRegistry.md#signal-operationadded)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfCogoRegistry**](#function-qfcogoregistry) (QObject \* parent=nullptr) <br> |
|  QStringList | [**availableOperations**](#function-availableoperations) () const<br> |
|  [**QfCogoOperation**](classQfCogoOperation.md) \* | [**operation**](#function-operation) (const QString & name) <br> |
|  bool | [**registerOperation**](#function-registeroperation) ([**QfCogoOperation**](classQfCogoOperation.md) \* operation) <br> |
|   | [**~QfCogoRegistry**](#function-qfcogoregistry) () <br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  [**QfCogoRegistry**](classQfCogoRegistry.md) \* | [**instance**](#function-instance) () <br> |
|  void | [**setInstance**](#function-setinstance) ([**QfCogoRegistry**](classQfCogoRegistry.md) \* instance) <br> |


























## Public Signals Documentation




### signal operationAdded 

```C++
void QfCogoRegistry::operationAdded;
```



Emitted when a COGO has been added into the registry. 


        

<hr>
## Public Functions Documentation




### function QfCogoRegistry 

```C++
explicit QfCogoRegistry::QfCogoRegistry (
    QObject * parent=nullptr
) 
```



COGO registry constructor. 


        

<hr>



### function availableOperations 

```C++
inline QStringList QfCogoRegistry::availableOperations () const
```



Returns the available COGO operations within the registry. 


        

<hr>



### function operation 

```C++
QfCogoOperation * QfCogoRegistry::operation (
    const QString & name
) 
```



Returns a COGO operation matching a given _name_. 


        

<hr>



### function registerOperation 

```C++
bool QfCogoRegistry::registerOperation (
    QfCogoOperation * operation
) 
```



Registers a COGO _operation_. 

**Note:**

the ownership of the operation pointer is transferred to the registry. 





        

<hr>



### function ~QfCogoRegistry 

```C++
QfCogoRegistry::~QfCogoRegistry () 
```




<hr>
## Public Static Functions Documentation




### function instance 

```C++
static inline QfCogoRegistry * QfCogoRegistry::instance () 
```



Returns the current COGO registry singleton instance. 


        

<hr>



### function setInstance 

```C++
static inline void QfCogoRegistry::setInstance (
    QfCogoRegistry * instance
) 
```



Set the current COGO registry singleton _instance_. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/cogo/qfcogoregistry.h`

