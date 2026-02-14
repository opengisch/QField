

# Class LayerUtils



[**ClassList**](annotated.md) **>** [**LayerUtils**](classLayerUtils.md)








Inherits the following classes: QObject


































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**LayerUtils**](#function-layerutils) (QObject \* parent=nullptr) <br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  Q\_INVOKABLE bool | [**addFeature**](#function-addfeature) (QgsVectorLayer \* layer, QgsFeature feature) <br> |
|  QgsMapLayer \* | [**createBasemap**](#function-createbasemap) (const QString & style=QString()) <br> |
|  Q\_INVOKABLE [**FeatureIterator**](classFeatureIterator.md) | [**createFeatureIteratorFromExpression**](#function-createfeatureiteratorfromexpression) (QgsVectorLayer \* layer, const QString & expression) <br> |
|  Q\_INVOKABLE [**FeatureIterator**](classFeatureIterator.md) | [**createFeatureIteratorFromRectangle**](#function-createfeatureiteratorfromrectangle) (QgsVectorLayer \* layer, const QgsRectangle & rectangle) <br> |
|  Q\_INVOKABLE QgsVectorLayer \* | [**createMemoryLayer**](#function-creatememorylayer) (const QString & name, const QgsFields & fields=QgsFields(), Qgis::WkbType geometryType=Qgis::WkbType::NoGeometry, const QgsCoordinateReferenceSystem & crs=QgsCoordinateReferenceSystem()) <br> |
|  QgsRasterLayer \* | [**createOnlineElevationLayer**](#function-createonlineelevationlayer) () <br> |
|  QgsAbstractVectorLayerLabeling \* | [**defaultLabeling**](#function-defaultlabeling) (QgsVectorLayer \* layer, QgsTextFormat textFormat=QgsTextFormat()) <br> |
|  QgsSymbol \* | [**defaultSymbol**](#function-defaultsymbol) (QgsVectorLayer \* layer, const QString & attachmentField=QString(), const QString & colorField=QString()) <br> |
|  Q\_INVOKABLE bool | [**deleteFeature**](#function-deletefeature) (QgsProject \* project, QgsVectorLayer \* layer, const QgsFeatureId fid, bool flushBuffer=true) <br> |
|  Q\_INVOKABLE QgsFeature | [**duplicateFeature**](#function-duplicatefeature) (QgsVectorLayer \* layer, QgsFeature feature) <br> |
|  Q\_INVOKABLE QString | [**fieldType**](#function-fieldtype) (const QgsField & field) <br> |
|  Q\_INVOKABLE bool | [**hasMValue**](#function-hasmvalue) (QgsVectorLayer \* layer) <br> |
|  Q\_INVOKABLE bool | [**isAtlasCoverageLayer**](#function-isatlascoveragelayer) (QgsVectorLayer \* layer) <br> |
|  Q\_INVOKABLE bool | [**isFeatureAdditionLocked**](#function-isfeatureadditionlocked) (QgsMapLayer \* layer) <br> |
|  Q\_INVOKABLE QgsRasterLayer \* | [**loadRasterLayer**](#function-loadrasterlayer) (const QString & uri, const QString & name=QString(), const QString & provider=QStringLiteral("gdal")) <br> |
|  Q\_INVOKABLE QgsVectorLayer \* | [**loadVectorLayer**](#function-loadvectorlayer) (const QString & uri, const QString & name=QString(), const QString & provider=QStringLiteral("ogr")) <br> |
|  Q\_INVOKABLE QgsVectorLayer \* | [**memoryLayerFromJsonString**](#function-memorylayerfromjsonstring) (const QString & name, const QString & string, const QgsCoordinateReferenceSystem & crs=QgsCoordinateReferenceSystem()) <br> |
|  Q\_INVOKABLE void | [**selectFeaturesInLayer**](#function-selectfeaturesinlayer) (QgsVectorLayer \* layer, const QList&lt; int &gt; & fids, Qgis::SelectBehavior behavior=Qgis::SelectBehavior::SetSelection) <br> |
|  void | [**setDefaultLabeling**](#function-setdefaultlabeling) (QgsVectorLayer \* layer, QgsProject \* project=nullptr) <br> |
|  void | [**setDefaultRenderer**](#function-setdefaultrenderer) (QgsVectorLayer \* layer, QgsProject \* project=nullptr, const QString & attachmentField=QString(), const QString & colorField=QString()) <br> |


























## Public Functions Documentation




### function LayerUtils 

```C++
explicit LayerUtils::LayerUtils (
    QObject * parent=nullptr
) 
```




<hr>
## Public Static Functions Documentation




### function addFeature 

```C++
static Q_INVOKABLE bool LayerUtils::addFeature (
    QgsVectorLayer * layer,
    QgsFeature feature
) 
```



Adds a _feature_ into the _layer_. 

**Note:**

The function will not call startEditing() and commitChanges() 





        

<hr>



### function createBasemap 

```C++
static QgsMapLayer * LayerUtils::createBasemap (
    const QString & style=QString()
) 
```



Creates an online basemap layer. 


        

<hr>



### function createFeatureIteratorFromExpression 

```C++
static Q_INVOKABLE FeatureIterator LayerUtils::createFeatureIteratorFromExpression (
    QgsVectorLayer * layer,
    const QString & expression
) 
```



Returns a feature iterator to get features matching a given _expression_ within the provided _layer_. 


        

<hr>



### function createFeatureIteratorFromRectangle 

```C++
static Q_INVOKABLE FeatureIterator LayerUtils::createFeatureIteratorFromRectangle (
    QgsVectorLayer * layer,
    const QgsRectangle & rectangle
) 
```



Returns a feature iterator to get features overlapping a given _rectangle_ within the provided _layer_. 


        

<hr>



### function createMemoryLayer 

```C++
static Q_INVOKABLE QgsVectorLayer * LayerUtils::createMemoryLayer (
    const QString & name,
    const QgsFields & fields=QgsFields(),
    Qgis::WkbType geometryType=Qgis::WkbType::NoGeometry,
    const QgsCoordinateReferenceSystem & crs=QgsCoordinateReferenceSystem()
) 
```



Creates a new memory layer using the specified parameters. 

**Parameters:**


* `name` layer name 
* `fields` fields for layer 
* `geometryType` optional layer geometry type 
* `crs` optional layer CRS for layers with geometry 




        

<hr>



### function createOnlineElevationLayer 

```C++
static QgsRasterLayer * LayerUtils::createOnlineElevationLayer () 
```



Creats an online raster elevation layer. 


        

<hr>



### function defaultLabeling 

```C++
static QgsAbstractVectorLayerLabeling * LayerUtils::defaultLabeling (
    QgsVectorLayer * layer,
    QgsTextFormat textFormat=QgsTextFormat()
) 
```



Returns the default vector layer labeling for a given _layer_ and _textFormat_. 


        

<hr>



### function defaultSymbol 

```C++
static QgsSymbol * LayerUtils::defaultSymbol (
    QgsVectorLayer * layer,
    const QString & attachmentField=QString(),
    const QString & colorField=QString()
) 
```



Returns the default symbol for a given layer. 

**Parameters:**


* `layer` the vector layer used to create the default symbol 




        

<hr>



### function deleteFeature 

```C++
static Q_INVOKABLE bool LayerUtils::deleteFeature (
    QgsProject * project,
    QgsVectorLayer * layer,
    const QgsFeatureId fid,
    bool flushBuffer=true
) 
```



Deletes a vector layer feature, including related features tied to relationships. 

**Parameters:**


* `project` the project holding information on relationships 
* `layer` the layer from which the feature will be deleted 
* `fid` the feature ID to be deleted 
* `flushBuffer` set to TRUE to immediately save the edit buffer 




        

<hr>



### function duplicateFeature 

```C++
static Q_INVOKABLE QgsFeature LayerUtils::duplicateFeature (
    QgsVectorLayer * layer,
    QgsFeature feature
) 
```



Duplicates a given _feature_ within the provided vector _layer_. If successful, the function will return the duplicated feature with attribute values saved updated to match what was saved into the layer dataset. 


        

<hr>



### function fieldType 

```C++
static Q_INVOKABLE QString LayerUtils::fieldType (
    const QgsField & field
) 
```



Returns the QVariant typeName of a _field_. This is a stable identifier (compared to the provider field name). 


        

<hr>



### function hasMValue 

```C++
static Q_INVOKABLE bool LayerUtils::hasMValue (
    QgsVectorLayer * layer
) 
```



Returns TRUE if the vector _layer_ geometry has an M value. 


        

<hr>



### function isAtlasCoverageLayer 

```C++
static Q_INVOKABLE bool LayerUtils::isAtlasCoverageLayer (
    QgsVectorLayer * layer
) 
```



Returns TRUE if the vector layer is used as an atlas coverage layer in any of the print layouts of the currently opened project. 

**Parameters:**


* `layer` the vector layer to check against print layouts 




        

<hr>



### function isFeatureAdditionLocked 

```C++
static Q_INVOKABLE bool LayerUtils::isFeatureAdditionLocked (
    QgsMapLayer * layer
) 
```



Returns TRUE if the _layer_ permission state prevents feature addition. 


        

<hr>



### function loadRasterLayer 

```C++
static Q_INVOKABLE QgsRasterLayer * LayerUtils::loadRasterLayer (
    const QString & uri,
    const QString & name=QString(),
    const QString & provider=QStringLiteral("gdal")
) 
```



Loads a raster layer. 

**Parameters:**


* `uri` the data source uri 
* `name` the layer name 
* `provider` the data provider name 




        

<hr>



### function loadVectorLayer 

```C++
static Q_INVOKABLE QgsVectorLayer * LayerUtils::loadVectorLayer (
    const QString & uri,
    const QString & name=QString(),
    const QString & provider=QStringLiteral("ogr")
) 
```



Loads a vector layer. 

**Parameters:**


* `uri` the data source uri 
* `name` the layer name 
* `provider` the data provider name 




        

<hr>



### function memoryLayerFromJsonString 

```C++
static Q_INVOKABLE QgsVectorLayer * LayerUtils::memoryLayerFromJsonString (
    const QString & name,
    const QString & string,
    const QgsCoordinateReferenceSystem & crs=QgsCoordinateReferenceSystem()
) 
```



Attempts to parse a GeoJSON string to a memory vector layer containing the collection of features. The geometry type will be taken from the first parsed feature. 

**Parameters:**


* `name` layer name 
* `string` the GeoJSON string 
* `crs` optional layer CRS for layers with geometry 




        

<hr>



### function selectFeaturesInLayer 

```C++
static Q_INVOKABLE void LayerUtils::selectFeaturesInLayer (
    QgsVectorLayer * layer,
    const QList< int > & fids,
    Qgis::SelectBehavior behavior=Qgis::SelectBehavior::SetSelection
) 
```



Selects features in a layer This method is required since QML cannot perform the conversion of a feature ID to a QgsFeatureId 

**Parameters:**


* `layer` the vector layer 
* `fids` the list of feature IDs 
* `behavior` the selection behavior 




        

<hr>



### function setDefaultLabeling 

```C++
static void LayerUtils::setDefaultLabeling (
    QgsVectorLayer * layer,
    QgsProject * project=nullptr
) 
```



Sets the default labeling for a given _layer_. 


        

<hr>



### function setDefaultRenderer 

```C++
static void LayerUtils::setDefaultRenderer (
    QgsVectorLayer * layer,
    QgsProject * project=nullptr,
    const QString & attachmentField=QString(),
    const QString & colorField=QString()
) 
```



Sets the default symbology render for a given _layer_. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/utils/layerutils.h`

