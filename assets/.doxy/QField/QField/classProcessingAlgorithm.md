

# Class ProcessingAlgorithm



[**ClassList**](annotated.md) **>** [**ProcessingAlgorithm**](classProcessingAlgorithm.md)



_A processing algorithm item capable of runnning a given algorithm._ 

* `#include <processingalgorithm.h>`



Inherits the following classes: QObject


























## Public Properties

| Type | Name |
| ---: | :--- |
| property QString | [**displayName**](classProcessingAlgorithm.md#property-displayname-12)  <br> |
| property QString | [**id**](classProcessingAlgorithm.md#property-id-12)  <br> |
| property QList&lt; QgsFeature &gt; | [**inPlaceFeatures**](classProcessingAlgorithm.md#property-inplacefeatures-12)  <br> |
| property QgsVectorLayer \* | [**inPlaceLayer**](classProcessingAlgorithm.md#property-inplacelayer-12)  <br> |
| property bool | [**isValid**](classProcessingAlgorithm.md#property-isvalid-12)  <br> |
| property QVariantMap | [**parameters**](classProcessingAlgorithm.md#property-parameters-12)  <br> |
| property bool | [**preview**](classProcessingAlgorithm.md#property-preview-12)  <br> |
| property QList&lt; QgsGeometry &gt; | [**previewGeometries**](classProcessingAlgorithm.md#property-previewgeometries-12)  <br> |
| property QString | [**shortHelp**](classProcessingAlgorithm.md#property-shorthelp-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**idChanged**](classProcessingAlgorithm.md#signal-idchanged) (const QString & id) <br> |
| signal void | [**inPlaceFeaturesChanged**](classProcessingAlgorithm.md#signal-inplacefeatureschanged)  <br> |
| signal void | [**inPlaceLayerChanged**](classProcessingAlgorithm.md#signal-inplacelayerchanged)  <br> |
| signal void | [**parametersChanged**](classProcessingAlgorithm.md#signal-parameterschanged)  <br> |
| signal void | [**previewChanged**](classProcessingAlgorithm.md#signal-previewchanged)  <br> |
| signal void | [**previewGeometriesChanged**](classProcessingAlgorithm.md#signal-previewgeometrieschanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ProcessingAlgorithm**](#function-processingalgorithm) (QObject \* parent=nullptr) <br> |
|  QString | [**displayName**](#function-displayname-22) () const<br> |
|  QString | [**id**](#function-id-22) () const<br> |
|  QList&lt; QgsFeature &gt; | [**inPlaceFeatures**](#function-inplacefeatures-22) () const<br> |
|  QgsVectorLayer \* | [**inPlaceLayer**](#function-inplacelayer-22) () const<br> |
|  bool | [**isValid**](#function-isvalid-22) () const<br> |
|  QVariantMap | [**parameters**](#function-parameters-22) () const<br> |
|  bool | [**preview**](#function-preview-22) () const<br> |
|  QList&lt; QgsGeometry &gt; | [**previewGeometries**](#function-previewgeometries-22) () const<br> |
|  Q\_INVOKABLE bool | [**run**](#function-run) (bool previewMode=false) <br> |
|  void | [**setId**](#function-setid) (const QString & id) <br> |
|  void | [**setInPlaceFeatures**](#function-setinplacefeatures) (const QList&lt; QgsFeature &gt; & features) <br> |
|  void | [**setInPlaceLayer**](#function-setinplacelayer) (QgsVectorLayer \* layer) <br> |
|  void | [**setParameters**](#function-setparameters) (const QVariantMap & parameters) <br> |
|  void | [**setPreview**](#function-setpreview) (bool preview) <br> |
|  QString | [**shortHelp**](#function-shorthelp-22) () const<br> |




























## Public Properties Documentation




### property displayName [1/2]

```C++
QString ProcessingAlgorithm::displayName;
```




<hr>



### property id [1/2]

```C++
QString ProcessingAlgorithm::id;
```




<hr>



### property inPlaceFeatures [1/2]

```C++
QList<QgsFeature> ProcessingAlgorithm::inPlaceFeatures;
```




<hr>



### property inPlaceLayer [1/2]

```C++
QgsVectorLayer * ProcessingAlgorithm::inPlaceLayer;
```




<hr>



### property isValid [1/2]

```C++
bool ProcessingAlgorithm::isValid;
```




<hr>



### property parameters [1/2]

```C++
QVariantMap ProcessingAlgorithm::parameters;
```




<hr>



### property preview [1/2]

```C++
bool ProcessingAlgorithm::preview;
```




<hr>



### property previewGeometries [1/2]

```C++
QList<QgsGeometry> ProcessingAlgorithm::previewGeometries;
```




<hr>



### property shortHelp [1/2]

```C++
QString ProcessingAlgorithm::shortHelp;
```




<hr>
## Public Signals Documentation




### signal idChanged 

```C++
void ProcessingAlgorithm::idChanged;
```



Emitted when the algorithm ID has changed 


        

<hr>



### signal inPlaceFeaturesChanged 

```C++
void ProcessingAlgorithm::inPlaceFeaturesChanged;
```



Emitted when the in place feature IDs list has changed 


        

<hr>



### signal inPlaceLayerChanged 

```C++
void ProcessingAlgorithm::inPlaceLayerChanged;
```



Emitted when the in place vector layer has changed 


        

<hr>



### signal parametersChanged 

```C++
void ProcessingAlgorithm::parametersChanged;
```



Emitted when the parameter model has changed 


        

<hr>



### signal previewChanged 

```C++
void ProcessingAlgorithm::previewChanged;
```



Emitted when the preview setting has changed 


        

<hr>



### signal previewGeometriesChanged 

```C++
void ProcessingAlgorithm::previewGeometriesChanged;
```



Emitted when the list of preview geometries has changed 


        

<hr>
## Public Functions Documentation




### function ProcessingAlgorithm 

```C++
explicit ProcessingAlgorithm::ProcessingAlgorithm (
    QObject * parent=nullptr
) 
```




<hr>



### function displayName [2/2]

```C++
QString ProcessingAlgorithm::displayName () const
```



Returns the display name of the algorithm. 


        

<hr>



### function id [2/2]

```C++
inline QString ProcessingAlgorithm::id () const
```



Returns the current algorithm ID from which parameters are taken from. 


        

<hr>



### function inPlaceFeatures [2/2]

```C++
inline QList< QgsFeature > ProcessingAlgorithm::inPlaceFeatures () const
```



Returns the vector _layer_ for in-place algorithm filter. 


        

<hr>



### function inPlaceLayer [2/2]

```C++
inline QgsVectorLayer * ProcessingAlgorithm::inPlaceLayer () const
```



Returns the vector _layer_ for in-place algorithm filter. 


        

<hr>



### function isValid [2/2]

```C++
inline bool ProcessingAlgorithm::isValid () const
```



Returns whether the current model refers to a valid algorithm. 


        

<hr>



### function parameters [2/2]

```C++
inline QVariantMap ProcessingAlgorithm::parameters () const
```



Returns the algorithm parameters as a map of parameter names as keys and values. 


        

<hr>



### function preview [2/2]

```C++
inline bool ProcessingAlgorithm::preview () const
```



Returns whether the algorithm will provide preview outputs such as modified geometries. from provided parameters. 


        

<hr>



### function previewGeometries [2/2]

```C++
inline QList< QgsGeometry > ProcessingAlgorithm::previewGeometries () const
```



Returns a list of geometries previewing the algorithm result using current parameters. 


        

<hr>



### function run 

```C++
Q_INVOKABLE bool ProcessingAlgorithm::run (
    bool previewMode=false
) 
```



Executes the algorithm. 


        

<hr>



### function setId 

```C++
void ProcessingAlgorithm::setId (
    const QString & id
) 
```



Sets the current algorithm _ID_ from which parameters are taken from. 


        

<hr>



### function setInPlaceFeatures 

```C++
void ProcessingAlgorithm::setInPlaceFeatures (
    const QList< QgsFeature > & features
) 
```



Sets the vector _layer_ for in-place algorithm filter. 


        

<hr>



### function setInPlaceLayer 

```C++
void ProcessingAlgorithm::setInPlaceLayer (
    QgsVectorLayer * layer
) 
```



Sets the vector _layer_ for in-place algorithm filter. 


        

<hr>



### function setParameters 

```C++
void ProcessingAlgorithm::setParameters (
    const QVariantMap & parameters
) 
```



Sets the algorithm parameters using a map with parameter names as keys and values. 


        

<hr>



### function setPreview 

```C++
void ProcessingAlgorithm::setPreview (
    bool preview
) 
```



Sets whether the algorithm will provide preview outputs such as modified geometries. from provided parameters. 


        

<hr>



### function shortHelp [2/2]

```C++
QString ProcessingAlgorithm::shortHelp () const
```



Returns a short description of the algorithm. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/processing/processingalgorithm.h`

