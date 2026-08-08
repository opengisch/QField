

# Class AppController



[**ClassList**](annotated.md) **>** [**AppController**](classAppController.md)



_Application operations that_ [_**AppInterface**_](classAppInterface.md) _delegates to the app._

* `#include <appcontroller.h>`





Inherited by the following classes: [QgisMobileapp](classQgisMobileapp.md)
































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual void | [**clearProject**](#function-clearproject) () = 0<br>_Clears the currently opened project._  |
| virtual bool | [**loadProjectFile**](#function-loadprojectfile) (const QString & path, const QString & name=QString()) = 0<br>_Loads a project file (.qgs/.qgz) or standalone dataset from_ _path_ _, labelled__name_ _._ |
| virtual bool | [**print**](#function-print) (const QString & layoutName) = 0<br>_Prints the project layout matching_ _layoutName_ _to PDF._ |
| virtual bool | [**printAtlasFeatures**](#function-printatlasfeatures) (const QString & layoutName, const QList&lt; long long &gt; & featureIds) = 0<br>_Prints the atlas-driven layout_ _layoutName_ _for__featureIds_ _to PDF._ |
| virtual bool | [**readProjectBoolEntry**](#function-readprojectboolentry) (const QString & scope, const QString & key, bool def=false) const = 0<br>_Reads a bool project entry at_ _scope_ _/__key_ _(__def_ _if absent)._ |
| virtual double | [**readProjectDoubleEntry**](#function-readprojectdoubleentry) (const QString & scope, const QString & key, double def=0.0) const = 0<br>_Reads a double project entry at_ _scope_ _/__key_ _(__def_ _if absent)._ |
| virtual QString | [**readProjectEntry**](#function-readprojectentry) (const QString & scope, const QString & key, const QString & def=QString()) const = 0<br>_Reads a string project entry at_ _scope_ _/__key_ _(__def_ _if absent)._ |
| virtual void | [**readProjectFile**](#function-readprojectfile) () = 0<br>_Reads the content of the loaded project._  |
| virtual int | [**readProjectNumEntry**](#function-readprojectnumentry) (const QString & scope, const QString & key, int def=0) const = 0<br>_Reads an int project entry at_ _scope_ _/__key_ _(__def_ _if absent)._ |
| virtual void | [**reloadProjectFile**](#function-reloadprojectfile) () = 0<br>_Reloads the currently opened project._  |
| virtual void | [**setScreenDimmerTimeout**](#function-setscreendimmertimeout) (int timeoutSeconds) = 0<br>_Sets the screen dimmer timeout in seconds (0 disables dimming)._  |
| virtual  | [**~AppController**](#function-appcontroller) () = default<br> |




























## Public Functions Documentation




### function clearProject 

_Clears the currently opened project._ 
```C++
virtual void AppController::clearProject () = 0
```




<hr>



### function loadProjectFile 

_Loads a project file (.qgs/.qgz) or standalone dataset from_ _path_ _, labelled__name_ _._
```C++
virtual bool AppController::loadProjectFile (
    const QString & path,
    const QString & name=QString()
) = 0
```




<hr>



### function print 

_Prints the project layout matching_ _layoutName_ _to PDF._
```C++
virtual bool AppController::print (
    const QString & layoutName
) = 0
```




<hr>



### function printAtlasFeatures 

_Prints the atlas-driven layout_ _layoutName_ _for__featureIds_ _to PDF._
```C++
virtual bool AppController::printAtlasFeatures (
    const QString & layoutName,
    const QList< long long > & featureIds
) = 0
```




<hr>



### function readProjectBoolEntry 

_Reads a bool project entry at_ _scope_ _/__key_ _(__def_ _if absent)._
```C++
virtual bool AppController::readProjectBoolEntry (
    const QString & scope,
    const QString & key,
    bool def=false
) const = 0
```




<hr>



### function readProjectDoubleEntry 

_Reads a double project entry at_ _scope_ _/__key_ _(__def_ _if absent)._
```C++
virtual double AppController::readProjectDoubleEntry (
    const QString & scope,
    const QString & key,
    double def=0.0
) const = 0
```




<hr>



### function readProjectEntry 

_Reads a string project entry at_ _scope_ _/__key_ _(__def_ _if absent)._
```C++
virtual QString AppController::readProjectEntry (
    const QString & scope,
    const QString & key,
    const QString & def=QString()
) const = 0
```




<hr>



### function readProjectFile 

_Reads the content of the loaded project._ 
```C++
virtual void AppController::readProjectFile () = 0
```




<hr>



### function readProjectNumEntry 

_Reads an int project entry at_ _scope_ _/__key_ _(__def_ _if absent)._
```C++
virtual int AppController::readProjectNumEntry (
    const QString & scope,
    const QString & key,
    int def=0
) const = 0
```




<hr>



### function reloadProjectFile 

_Reloads the currently opened project._ 
```C++
virtual void AppController::reloadProjectFile () = 0
```




<hr>



### function setScreenDimmerTimeout 

_Sets the screen dimmer timeout in seconds (0 disables dimming)._ 
```C++
virtual void AppController::setScreenDimmerTimeout (
    int timeoutSeconds
) = 0
```




<hr>



### function ~AppController 

```C++
virtual AppController::~AppController () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/appcontroller.h`

