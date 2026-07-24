

# Class QgisMobileapp



[**ClassList**](annotated.md) **>** [**QgisMobileapp**](classQgisMobileapp.md)








Inherits the following classes: QQmlApplicationEngine,  [AppController](classAppController.md)














































## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**loadProjectEnded**](classQgisMobileapp.md#signal-loadprojectended) (const QString & filename, const QString & name) <br> |
| signal void | [**loadProjectTriggered**](classQgisMobileapp.md#signal-loadprojecttriggered) (const QString & filename, const QString & name) <br> |
| signal void | [**setMapExtent**](classQgisMobileapp.md#signal-setmapextent) (const QgsRectangle & extent) <br> |








## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QgisMobileapp**](#function-qgismobileapp) (QgsApplication \* app, QObject \* parent=nullptr) <br> |
| virtual void | [**clearProject**](#function-clearproject) () override<br> |
|  bool | [**event**](#function-event) (QEvent \* event) override<br> |
| virtual bool | [**loadProjectFile**](#function-loadprojectfile) (const QString & path, const QString & name=QString()) override<br> |
| virtual bool | [**print**](#function-print) (const QString & layoutName) override<br> |
| virtual bool | [**printAtlasFeatures**](#function-printatlasfeatures) (const QString & layoutName, const QList&lt; long long &gt; & featureIds) override<br> |
| virtual bool | [**readProjectBoolEntry**](#function-readprojectboolentry) (const QString & scope, const QString & key, bool def=false) override const<br> |
| virtual double | [**readProjectDoubleEntry**](#function-readprojectdoubleentry) (const QString & scope, const QString & key, double def=0.0) override const<br> |
| virtual QString | [**readProjectEntry**](#function-readprojectentry) (const QString & scope, const QString & key, const QString & def=QString()) override const<br> |
| virtual void | [**readProjectFile**](#function-readprojectfile) () override<br> |
| virtual int | [**readProjectNumEntry**](#function-readprojectnumentry) (const QString & scope, const QString & key, int def=0) override const<br> |
| virtual void | [**reloadProjectFile**](#function-reloadprojectfile) () override<br> |
| virtual void | [**setScreenDimmerTimeout**](#function-setscreendimmertimeout) (int timeoutSeconds) override<br> |
|   | [**~QgisMobileapp**](#function-qgismobileapp) () override<br> |


## Public Functions inherited from AppController

See [AppController](classAppController.md)

| Type | Name |
| ---: | :--- |
| virtual void | [**clearProject**](classAppController.md#function-clearproject) () = 0<br>_Clears the currently opened project._  |
| virtual bool | [**loadProjectFile**](classAppController.md#function-loadprojectfile) (const QString & path, const QString & name=QString()) = 0<br>_Loads a project file (.qgs/.qgz) or standalone dataset from_ _path_ _, labelled__name_ _._ |
| virtual bool | [**print**](classAppController.md#function-print) (const QString & layoutName) = 0<br>_Prints the project layout matching_ _layoutName_ _to PDF._ |
| virtual bool | [**printAtlasFeatures**](classAppController.md#function-printatlasfeatures) (const QString & layoutName, const QList&lt; long long &gt; & featureIds) = 0<br>_Prints the atlas-driven layout_ _layoutName_ _for__featureIds_ _to PDF._ |
| virtual bool | [**readProjectBoolEntry**](classAppController.md#function-readprojectboolentry) (const QString & scope, const QString & key, bool def=false) const = 0<br>_Reads a bool project entry at_ _scope_ _/__key_ _(__def_ _if absent)._ |
| virtual double | [**readProjectDoubleEntry**](classAppController.md#function-readprojectdoubleentry) (const QString & scope, const QString & key, double def=0.0) const = 0<br>_Reads a double project entry at_ _scope_ _/__key_ _(__def_ _if absent)._ |
| virtual QString | [**readProjectEntry**](classAppController.md#function-readprojectentry) (const QString & scope, const QString & key, const QString & def=QString()) const = 0<br>_Reads a string project entry at_ _scope_ _/__key_ _(__def_ _if absent)._ |
| virtual void | [**readProjectFile**](classAppController.md#function-readprojectfile) () = 0<br>_Reads the content of the loaded project._  |
| virtual int | [**readProjectNumEntry**](classAppController.md#function-readprojectnumentry) (const QString & scope, const QString & key, int def=0) const = 0<br>_Reads an int project entry at_ _scope_ _/__key_ _(__def_ _if absent)._ |
| virtual void | [**reloadProjectFile**](classAppController.md#function-reloadprojectfile) () = 0<br>_Reloads the currently opened project._  |
| virtual void | [**setScreenDimmerTimeout**](classAppController.md#function-setscreendimmertimeout) (int timeoutSeconds) = 0<br>_Sets the screen dimmer timeout in seconds (0 disables dimming)._  |
| virtual  | [**~AppController**](classAppController.md#function-appcontroller) () = default<br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  void | [**initDeclarative**](#function-initdeclarative) (QQmlEngine \* engine) <br> |




















































## Public Signals Documentation




### signal loadProjectEnded 

```C++
void QgisMobileapp::loadProjectEnded;
```



Emitted when the project is fully loaded 


        

<hr>



### signal loadProjectTriggered 

```C++
void QgisMobileapp::loadProjectTriggered;
```



Emitted when a project file is being loaded




**Parameters:**


* `filename` The filename of the project that is being loaded 
* `name` The project name that is being loaded 




        

<hr>



### signal setMapExtent 

```C++
void QgisMobileapp::setMapExtent;
```



Emitted when a map canvas extent change is needed 


        

<hr>
## Public Functions Documentation




### function QgisMobileapp 

```C++
explicit QgisMobileapp::QgisMobileapp (
    QgsApplication * app,
    QObject * parent=nullptr
) 
```




<hr>



### function clearProject 

```C++
virtual void QgisMobileapp::clearProject () override
```



Clear the currently opened project back to a blank project 


        
Implements [*AppController::clearProject*](classAppController.md#function-clearproject)


<hr>



### function event 

```C++
bool QgisMobileapp::event (
    QEvent * event
) override
```




<hr>



### function loadProjectFile 

```C++
virtual bool QgisMobileapp::loadProjectFile (
    const QString & path,
    const QString & name=QString()
) override
```



Set the project or dataset file path to be loaded.




**Parameters:**


* `path` The project or dataset file to load 
* `name` The project name 



**Note:**

The actual loading is done in readProjectFile 





        
Implements [*AppController::loadProjectFile*](classAppController.md#function-loadprojectfile)


<hr>



### function print 

```C++
virtual bool QgisMobileapp::print (
    const QString & layoutName
) override
```



Prints a given layout from the currently opened project to a PDF file 

**Parameters:**


* `layoutName` the layout name that will be printed 



**Returns:**

TRUE if the layout was successfully printed 





        
Implements [*AppController::print*](classAppController.md#function-print)


<hr>



### function printAtlasFeatures 

```C++
virtual bool QgisMobileapp::printAtlasFeatures (
    const QString & layoutName,
    const QList< long long > & featureIds
) override
```



Prints a given atlas-driven layout from the currently opened project to one or more PDF files 

**Parameters:**


* `layoutName` the layout name that will be printed 
* `featureIds` the features from the atlas coverage vector layer that will be used to print the layout 



**Returns:**

TRUE if the layout was successfully printed 





        
Implements [*AppController::printAtlasFeatures*](classAppController.md#function-printatlasfeatures)


<hr>



### function readProjectBoolEntry 

```C++
virtual bool QgisMobileapp::readProjectBoolEntry (
    const QString & scope,
    const QString & key,
    bool def=false
) override const
```



Reads a boolean from the specified _scope_ and _key_ from the currently opened project




**Parameters:**


* `scope` entry scope (group) name 
* `key` entry key name. Keys are '/'-delimited entries, implying a hierarchy of keys and corresponding values 
* `def` default value to return if the specified _key_ does not exist within the _scope_ 



**Returns:**

entry value as boolean from _scope_ given its _key_ 





        
Implements [*AppController::readProjectBoolEntry*](classAppController.md#function-readprojectboolentry)


<hr>



### function readProjectDoubleEntry 

```C++
virtual double QgisMobileapp::readProjectDoubleEntry (
    const QString & scope,
    const QString & key,
    double def=0.0
) override const
```



Reads a double from the specified _scope_ and _key_ from the currently opened project




**Parameters:**


* `scope` entry scope (group) name 
* `key` entry key name. Keys are '/'-delimited entries, implying a hierarchy of keys and corresponding values 
* `def` default value to return if the specified _key_ does not exist within the _scope_ 



**Returns:**

entry value as double from _scope_ given its _key_ 





        
Implements [*AppController::readProjectDoubleEntry*](classAppController.md#function-readprojectdoubleentry)


<hr>



### function readProjectEntry 

```C++
virtual QString QgisMobileapp::readProjectEntry (
    const QString & scope,
    const QString & key,
    const QString & def=QString()
) override const
```



Reads a string from the specified _scope_ and _key_ from the currently opened project




**Parameters:**


* `scope` entry scope (group) name 
* `key` entry key name. Keys are '/'-delimited entries, implying a hierarchy of keys and corresponding values 
* `def` default value to return if the specified _key_ does not exist within the _scope_ 



**Returns:**

entry value as string from _scope_ given its _key_ 





        
Implements [*AppController::readProjectEntry*](classAppController.md#function-readprojectentry)


<hr>



### function readProjectFile 

```C++
virtual void QgisMobileapp::readProjectFile () override
```



Reads and opens the project file set in the loadProjectFile function 


        
Implements [*AppController::readProjectFile*](classAppController.md#function-readprojectfile)


<hr>



### function readProjectNumEntry 

```C++
virtual int QgisMobileapp::readProjectNumEntry (
    const QString & scope,
    const QString & key,
    int def=0
) override const
```



Reads an integer from the specified _scope_ and _key_ from the currently opened project




**Parameters:**


* `scope` entry scope (group) name 
* `key` entry key name. Keys are '/'-delimited entries, implying a hierarchy of keys and corresponding values 
* `def` default value to return if the specified _key_ does not exist within the _scope_ 



**Returns:**

entry value as integer from _scope_ given its _key_ 





        
Implements [*AppController::readProjectNumEntry*](classAppController.md#function-readprojectnumentry)


<hr>



### function reloadProjectFile 

```C++
virtual void QgisMobileapp::reloadProjectFile () override
```



Reloads the current project




**Note:**

It does not reset the Auth Request Handler. 




**Note:**

The actual loading is done in readProjectFile 





        
Implements [*AppController::reloadProjectFile*](classAppController.md#function-reloadprojectfile)


<hr>



### function setScreenDimmerTimeout 

```C++
virtual void QgisMobileapp::setScreenDimmerTimeout (
    int timeoutSeconds
) override
```



Sets the screen dimmer timeout in seconds 

**Note:**

setting the timeout value to 0 will disable the screen dimmer 





        
Implements [*AppController::setScreenDimmerTimeout*](classAppController.md#function-setscreendimmertimeout)


<hr>



### function ~QgisMobileapp 

```C++
QgisMobileapp::~QgisMobileapp () override
```




<hr>
## Public Static Functions Documentation




### function initDeclarative 

```C++
static void QgisMobileapp::initDeclarative (
    QQmlEngine * engine
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/app/qgismobileapp.h`

