

# Class CogoRegistry



[**ClassList**](annotated.md) **>** [**CogoRegistry**](classCogoRegistry.md)



_A registry managing available COGO operations._ 

* `#include <cogoregistry.h>`



Inherits the following classes: QObject






























## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**operationAdded**](classCogoRegistry.md#signal-operationadded)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**CogoRegistry**](#function-cogoregistry) (QObject \* parent=nullptr) <br> |
|  QStringList | [**availableOperations**](#function-availableoperations) () const<br> |
|  [**CogoOperation**](classCogoOperation.md) \* | [**operation**](#function-operation) (const QString & name) <br> |
|  bool | [**registerOperation**](#function-registeroperation) ([**CogoOperation**](classCogoOperation.md) \* operation) <br> |
|   | [**~CogoRegistry**](#function-cogoregistry) () <br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  [**CogoRegistry**](classCogoRegistry.md) \* | [**instance**](#function-instance) () <br> |
|  void | [**setInstance**](#function-setinstance) ([**CogoRegistry**](classCogoRegistry.md) \* instance) <br> |


























## Public Signals Documentation




### signal operationAdded 

```C++
void CogoRegistry::operationAdded;
```



Emitted when a COGO has been added into the registry. 


        

<hr>
## Public Functions Documentation




### function CogoRegistry 

```C++
explicit CogoRegistry::CogoRegistry (
    QObject * parent=nullptr
) 
```



COGO registry constructor. 


        

<hr>



### function availableOperations 

```C++
inline QStringList CogoRegistry::availableOperations () const
```



Returns the available COGO operations within the registry. 


        

<hr>



### function operation 

```C++
CogoOperation * CogoRegistry::operation (
    const QString & name
) 
```



Returns a COGO operation matching a given _name_. 


        

<hr>



### function registerOperation 

```C++
bool CogoRegistry::registerOperation (
    CogoOperation * operation
) 
```



Registers a COGO _operation_. 

**Note:**

the ownership of the operation pointer is transferred to the registry. 





        

<hr>



### function ~CogoRegistry 

```C++
CogoRegistry::~CogoRegistry () 
```




<hr>
## Public Static Functions Documentation




### function instance 

```C++
static inline CogoRegistry * CogoRegistry::instance () 
```



Returns the current COGO registry singleton instance. 


        

<hr>



### function setInstance 

```C++
static inline void CogoRegistry::setInstance (
    CogoRegistry * instance
) 
```



Set the current COGO registry singleton _instance_. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/cogo/cogoregistry.h`

