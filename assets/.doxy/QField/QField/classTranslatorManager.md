

# Class TranslatorManager



[**ClassList**](annotated.md) **>** [**TranslatorManager**](classTranslatorManager.md)



_Provides access to shared translator instances for runtime language switching._ [More...](#detailed-description)

* `#include <translatormanager.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**TranslatorManager**](#function-translatormanager-12) () <br>_Constructor._  |
|   | [**TranslatorManager**](#function-translatormanager-22) (const [**TranslatorManager**](classTranslatorManager.md) &) = delete<br>_Copy constructor is deleted (singleton pattern)_  |
|  [**TranslatorManager**](classTranslatorManager.md) & | [**operator=**](#function-operator) (const [**TranslatorManager**](classTranslatorManager.md) &) = delete<br>_Assignment operator is deleted (singleton pattern)_  |
|  QTranslator \* | [**qfieldTranslator**](#function-qfieldtranslator) () <br>_Returns the global QField translator instance._  |
|  QTranslator \* | [**qtTranslator**](#function-qttranslator) () <br>_Returns the global Qt framework translator instance._  |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  [**TranslatorManager**](classTranslatorManager.md) \* | [**instance**](#function-instance) () <br>_Returns the singleton instance of_ [_**TranslatorManager**_](classTranslatorManager.md) _._ |


























## Detailed Description


Translators are shared between startup (main.cpp) and runtime switching ([**AppInterface::changeLanguage**](classAppInterface.md#function-changelanguage)). 


    
## Public Functions Documentation




### function TranslatorManager [1/2]

_Constructor._ 
```C++
TranslatorManager::TranslatorManager () 
```




<hr>



### function TranslatorManager [2/2]

_Copy constructor is deleted (singleton pattern)_ 
```C++
TranslatorManager::TranslatorManager (
    const TranslatorManager &
) = delete
```




<hr>



### function operator= 

_Assignment operator is deleted (singleton pattern)_ 
```C++
TranslatorManager & TranslatorManager::operator= (
    const TranslatorManager &
) = delete
```




<hr>



### function qfieldTranslator 

_Returns the global QField translator instance._ 
```C++
QTranslator * TranslatorManager::qfieldTranslator () 
```




<hr>



### function qtTranslator 

_Returns the global Qt framework translator instance._ 
```C++
QTranslator * TranslatorManager::qtTranslator () 
```




<hr>
## Public Static Functions Documentation




### function instance 

_Returns the singleton instance of_ [_**TranslatorManager**_](classTranslatorManager.md) _._
```C++
static TranslatorManager * TranslatorManager::instance () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/translatormanager.h`

