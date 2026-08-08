

# Class ProjectUtils



[**ClassList**](annotated.md) **>** [**ProjectUtils**](classProjectUtils.md)








Inherits the following classes: QObject


































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ProjectUtils**](#function-projectutils) (QObject \* parent=nullptr) <br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  Q\_INVOKABLE bool | [**addMapLayer**](#function-addmaplayer) (QgsProject \* project, QgsMapLayer \* layer) <br> |
|  Q\_INVOKABLE QString | [**createProject**](#function-createproject) (const QVariantMap & options, const [**GnssPositionInformation**](classGnssPositionInformation.md) & positionInformation=[**GnssPositionInformation**](classGnssPositionInformation.md)()) <br> |
|  Q\_INVOKABLE QVariantMap | [**mapLayers**](#function-maplayers) (QgsProject \* project) <br> |
|  Q\_INVOKABLE void | [**removeMapLayer**](#function-removemaplayer-12) (QgsProject \* project, QgsMapLayer \* layer) <br> |
|  Q\_INVOKABLE void | [**removeMapLayer**](#function-removemaplayer-22) (QgsProject \* project, const QString & layerId) <br> |
|  Q\_INVOKABLE QString | [**title**](#function-title) (QgsProject \* project=nullptr) <br> |
|  Q\_INVOKABLE Qgis::TransactionMode | [**transactionMode**](#function-transactionmode) (QgsProject \* project=nullptr) <br> |


























## Public Functions Documentation




### function ProjectUtils 

```C++
explicit ProjectUtils::ProjectUtils (
    QObject * parent=nullptr
) 
```




<hr>
## Public Static Functions Documentation




### function addMapLayer 

```C++
static Q_INVOKABLE bool ProjectUtils::addMapLayer (
    QgsProject * project,
    QgsMapLayer * layer
) 
```



Adds a map _layer_ to a _project_ layers registry. 


        

<hr>



### function createProject 

```C++
static Q_INVOKABLE QString ProjectUtils::createProject (
    const QVariantMap & options,
    const GnssPositionInformation & positionInformation=GnssPositionInformation ()
) 
```



Creates a new project and return the local path of the project file.


The available _options_ are:
* title: the project title
* basemap: the basemap type (color, lightgray, darkgray, custom, blank)
* basemap\_url: a custom basemap URL (XYZ raster layer or vector tile layer style JSON)
* notes: set to TRUE to add a notes layer
* camera\_capture: set to TRUE to add an image/video capture field to the notes layer
* tracks: set to TRUE to add a tracks layer
* track\_on\_launch: set to TRUE to start tracking position on project launch 




        

<hr>



### function mapLayers 

```C++
static Q_INVOKABLE QVariantMap ProjectUtils::mapLayers (
    QgsProject * project
) 
```



Returns a map of all registered layers by layer ID. 

**Note:**

This function mimics QgsProject::mapLayers with a return type that is QML compatible. 





        

<hr>



### function removeMapLayer [1/2]

```C++
static Q_INVOKABLE void ProjectUtils::removeMapLayer (
    QgsProject * project,
    QgsMapLayer * layer
) 
```



Removes a map _layer_ from a _project_ layers registry. 


        

<hr>



### function removeMapLayer [2/2]

```C++
static Q_INVOKABLE void ProjectUtils::removeMapLayer (
    QgsProject * project,
    const QString & layerId
) 
```



Removes a map layer from a project layers registry matching a given layer ID. 

**Parameters:**


* `project` the project 
* `layerId` the layer ID 




        

<hr>



### function title 

```C++
static Q_INVOKABLE QString ProjectUtils::title (
    QgsProject * project=nullptr
) 
```



Returns the _project_ title. 


        

<hr>



### function transactionMode 

```C++
static Q_INVOKABLE Qgis::TransactionMode ProjectUtils::transactionMode (
    QgsProject * project=nullptr
) 
```



Returns the transaction mode for a given _project_. 

**Note:**

To be removed when QField updates to QGIS 3.38. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/utils/projectutils.h`

