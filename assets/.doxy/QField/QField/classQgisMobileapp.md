

# Class QgisMobileapp



[**ClassList**](annotated.md) **>** [**QgisMobileapp**](classQgisMobileapp.md)








Inherits the following classes: QQmlApplicationEngine






























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
|  void | [**clearProject**](#function-clearproject) () <br> |
|  bool | [**event**](#function-event) (QEvent \* event) override<br> |
|  bool | [**loadProjectFile**](#function-loadprojectfile) (const QString & path, const QString & name=QString()) <br> |
|  bool | [**print**](#function-print) (const QString & layoutName) <br> |
|  bool | [**printAtlasFeatures**](#function-printatlasfeatures) (const QString & layoutName, const QList&lt; long long &gt; & featureIds) <br> |
|  bool | [**readProjectBoolEntry**](#function-readprojectboolentry) (const QString & scope, const QString & key, bool def=false) const<br> |
|  double | [**readProjectDoubleEntry**](#function-readprojectdoubleentry) (const QString & scope, const QString & key, double def=0.0) const<br> |
|  QString | [**readProjectEntry**](#function-readprojectentry) (const QString & scope, const QString & key, const QString & def=QString()) const<br> |
|  void | [**readProjectFile**](#function-readprojectfile) () <br> |
|  int | [**readProjectNumEntry**](#function-readprojectnumentry) (const QString & scope, const QString & key, int def=0) const<br> |
|  QList&lt; QPair&lt; QString, QString &gt; &gt; | [**recentProjects**](#function-recentprojects) () <br> |
|  void | [**reloadProjectFile**](#function-reloadprojectfile) () <br> |
|  void | [**removeRecentProject**](#function-removerecentproject) (const QString & path) <br> |
|  void | [**saveRecentProjects**](#function-saverecentprojects) (const QList&lt; QPair&lt; QString, QString &gt; &gt; & projects) <br> |
|  void | [**setScreenDimmerTimeout**](#function-setscreendimmertimeout) (int timeoutSeconds) <br> |
|   | [**~QgisMobileapp**](#function-qgismobileapp) () override<br> |


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
void QgisMobileapp::clearProject () 
```



Clear the currently opened project back to a blank project 


        

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
bool QgisMobileapp::loadProjectFile (
    const QString & path,
    const QString & name=QString()
) 
```



Set the project or dataset file path to be loaded.




**Parameters:**


* `path` The project or dataset file to load 
* `name` The project name 



**Note:**

The actual loading is done in readProjectFile 





        

<hr>



### function print 

```C++
bool QgisMobileapp::print (
    const QString & layoutName
) 
```



Prints a given layout from the currently opened project to a PDF file 

**Parameters:**


* `layoutName` the layout name that will be printed 



**Returns:**

TRUE if the layout was successfully printed 





        

<hr>



### function printAtlasFeatures 

```C++
bool QgisMobileapp::printAtlasFeatures (
    const QString & layoutName,
    const QList< long long > & featureIds
) 
```



Prints a given atlas-driven layout from the currently opened project to one or more PDF files 

**Parameters:**


* `layoutName` the layout name that will be printed 
* `featureIds` the features from the atlas coverage vector layer that will be used to print the layout 



**Returns:**

TRUE if the layout was successfully printed 





        

<hr>



### function readProjectBoolEntry 

```C++
bool QgisMobileapp::readProjectBoolEntry (
    const QString & scope,
    const QString & key,
    bool def=false
) const
```



Reads a boolean from the specified _scope_ and _key_ from the currently opened project




**Parameters:**


* `scope` entry scope (group) name 
* `key` entry key name. Keys are '/'-delimited entries, implying a hierarchy of keys and corresponding values 
* `def` default value to return if the specified _key_ does not exist within the _scope_ 



**Returns:**

entry value as boolean from _scope_ given its _key_ 





        

<hr>



### function readProjectDoubleEntry 

```C++
double QgisMobileapp::readProjectDoubleEntry (
    const QString & scope,
    const QString & key,
    double def=0.0
) const
```



Reads a double from the specified _scope_ and _key_ from the currently opened project




**Parameters:**


* `scope` entry scope (group) name 
* `key` entry key name. Keys are '/'-delimited entries, implying a hierarchy of keys and corresponding values 
* `def` default value to return if the specified _key_ does not exist within the _scope_ 



**Returns:**

entry value as double from _scope_ given its _key_ 





        

<hr>



### function readProjectEntry 

```C++
QString QgisMobileapp::readProjectEntry (
    const QString & scope,
    const QString & key,
    const QString & def=QString()
) const
```



Reads a string from the specified _scope_ and _key_ from the currently opened project




**Parameters:**


* `scope` entry scope (group) name 
* `key` entry key name. Keys are '/'-delimited entries, implying a hierarchy of keys and corresponding values 
* `def` default value to return if the specified _key_ does not exist within the _scope_ 



**Returns:**

entry value as string from _scope_ given its _key_ 





        

<hr>



### function readProjectFile 

```C++
void QgisMobileapp::readProjectFile () 
```



Reads and opens the project file set in the loadProjectFile function 


        

<hr>



### function readProjectNumEntry 

```C++
int QgisMobileapp::readProjectNumEntry (
    const QString & scope,
    const QString & key,
    int def=0
) const
```



Reads an integer from the specified _scope_ and _key_ from the currently opened project




**Parameters:**


* `scope` entry scope (group) name 
* `key` entry key name. Keys are '/'-delimited entries, implying a hierarchy of keys and corresponding values 
* `def` default value to return if the specified _key_ does not exist within the _scope_ 



**Returns:**

entry value as integer from _scope_ given its _key_ 





        

<hr>



### function recentProjects 

```C++
QList< QPair< QString, QString > > QgisMobileapp::recentProjects () 
```



Returns a list of recent projects. 


        

<hr>



### function reloadProjectFile 

```C++
void QgisMobileapp::reloadProjectFile () 
```



Reloads the current project




**Note:**

It does not reset the Auth Request Handler. 




**Note:**

The actual loading is done in readProjectFile 





        

<hr>



### function removeRecentProject 

```C++
void QgisMobileapp::removeRecentProject (
    const QString & path
) 
```



Removes the project with a given _path_ from the list of recent projects 


        

<hr>



### function saveRecentProjects 

```C++
void QgisMobileapp::saveRecentProjects (
    const QList< QPair< QString, QString > > & projects
) 
```



Saves a list of recent _projects_. 


        

<hr>



### function setScreenDimmerTimeout 

```C++
void QgisMobileapp::setScreenDimmerTimeout (
    int timeoutSeconds
) 
```



Sets the screen dimmer timeout in seconds 

**Note:**

setting the timeout value to 0 will disable the screen dimmer 





        

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
The documentation for this class was generated from the following file `src/core/qgismobileapp.h`

