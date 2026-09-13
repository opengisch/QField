

# Class QfTranslatorManager



[**ClassList**](annotated.md) **>** [**QfTranslatorManager**](classQfTranslatorManager.md)



_Provides access to shared translator instances for runtime language switching._ [More...](#detailed-description)

* `#include <qftranslatormanager.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfTranslatorManager**](#function-qftranslatormanager-12) () <br>_Constructor._  |
|   | [**QfTranslatorManager**](#function-qftranslatormanager-22) (const [**QfTranslatorManager**](classQfTranslatorManager.md) &) = delete<br>_Copy constructor is deleted (singleton pattern)_  |
|  [**QfTranslatorManager**](classQfTranslatorManager.md) & | [**operator=**](#function-operator) (const [**QfTranslatorManager**](classQfTranslatorManager.md) &) = delete<br>_Assignment operator is deleted (singleton pattern)_  |
|  QTranslator \* | [**qfieldTranslator**](#function-qfieldtranslator) () <br>_Returns the global QField translator instance._  |
|  QTranslator \* | [**qtTranslator**](#function-qttranslator) () <br>_Returns the global Qt framework translator instance._  |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  [**QfTranslatorManager**](classQfTranslatorManager.md) \* | [**instance**](#function-instance) () <br>_Returns the singleton instance of_ [_**QfTranslatorManager**_](classQfTranslatorManager.md) _._ |


























## Detailed Description


Translators are shared between startup (main.cpp) and runtime switching ([**QfAppInterface::changeLanguage**](classQfAppInterface.md#function-changelanguage)). 


    
## Public Functions Documentation




### function QfTranslatorManager [1/2]

_Constructor._ 
```C++
QfTranslatorManager::QfTranslatorManager () 
```




<hr>



### function QfTranslatorManager [2/2]

_Copy constructor is deleted (singleton pattern)_ 
```C++
QfTranslatorManager::QfTranslatorManager (
    const QfTranslatorManager &
) = delete
```




<hr>



### function operator= 

_Assignment operator is deleted (singleton pattern)_ 
```C++
QfTranslatorManager & QfTranslatorManager::operator= (
    const QfTranslatorManager &
) = delete
```




<hr>



### function qfieldTranslator 

_Returns the global QField translator instance._ 
```C++
QTranslator * QfTranslatorManager::qfieldTranslator () 
```




<hr>



### function qtTranslator 

_Returns the global Qt framework translator instance._ 
```C++
QTranslator * QfTranslatorManager::qtTranslator () 
```




<hr>
## Public Static Functions Documentation




### function instance 

_Returns the singleton instance of_ [_**QfTranslatorManager**_](classQfTranslatorManager.md) _._
```C++
static QfTranslatorManager * QfTranslatorManager::instance () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qftranslatormanager.h`

