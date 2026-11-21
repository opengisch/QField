

# Class QfProcessingAlgorithm



[**ClassList**](annotated.md) **>** [**QfProcessingAlgorithm**](classQfProcessingAlgorithm.md)



_A processing algorithm item capable of runnning a given algorithm._ 

* `#include <qfprocessingalgorithm.h>`



Inherits the following classes: QObject


























## Public Properties

| Type | Name |
| ---: | :--- |
| property QString | [**displayName**](classQfProcessingAlgorithm.md#property-displayname-12)  <br> |
| property QString | [**id**](classQfProcessingAlgorithm.md#property-id-12)  <br> |
| property QList&lt; QgsFeature &gt; | [**inPlaceFeatures**](classQfProcessingAlgorithm.md#property-inplacefeatures-12)  <br> |
| property QgsVectorLayer \* | [**inPlaceLayer**](classQfProcessingAlgorithm.md#property-inplacelayer-12)  <br> |
| property bool | [**isValid**](classQfProcessingAlgorithm.md#property-isvalid-12)  <br> |
| property QVariantMap | [**parameters**](classQfProcessingAlgorithm.md#property-parameters-12)  <br> |
| property bool | [**preview**](classQfProcessingAlgorithm.md#property-preview-12)  <br> |
| property QList&lt; QgsGeometry &gt; | [**previewGeometries**](classQfProcessingAlgorithm.md#property-previewgeometries-12)  <br> |
| property QString | [**shortHelp**](classQfProcessingAlgorithm.md#property-shorthelp-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**idChanged**](classQfProcessingAlgorithm.md#signal-idchanged) (const QString & id) <br> |
| signal void | [**inPlaceFeaturesChanged**](classQfProcessingAlgorithm.md#signal-inplacefeatureschanged)  <br> |
| signal void | [**inPlaceLayerChanged**](classQfProcessingAlgorithm.md#signal-inplacelayerchanged)  <br> |
| signal void | [**parametersChanged**](classQfProcessingAlgorithm.md#signal-parameterschanged)  <br> |
| signal void | [**previewChanged**](classQfProcessingAlgorithm.md#signal-previewchanged)  <br> |
| signal void | [**previewGeometriesChanged**](classQfProcessingAlgorithm.md#signal-previewgeometrieschanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfProcessingAlgorithm**](#function-qfprocessingalgorithm) (QObject \* parent=nullptr) <br> |
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
QString QfProcessingAlgorithm::displayName;
```




<hr>



### property id [1/2]

```C++
QString QfProcessingAlgorithm::id;
```




<hr>



### property inPlaceFeatures [1/2]

```C++
QList<QgsFeature> QfProcessingAlgorithm::inPlaceFeatures;
```




<hr>



### property inPlaceLayer [1/2]

```C++
QgsVectorLayer * QfProcessingAlgorithm::inPlaceLayer;
```




<hr>



### property isValid [1/2]

```C++
bool QfProcessingAlgorithm::isValid;
```




<hr>



### property parameters [1/2]

```C++
QVariantMap QfProcessingAlgorithm::parameters;
```




<hr>



### property preview [1/2]

```C++
bool QfProcessingAlgorithm::preview;
```




<hr>



### property previewGeometries [1/2]

```C++
QList<QgsGeometry> QfProcessingAlgorithm::previewGeometries;
```




<hr>



### property shortHelp [1/2]

```C++
QString QfProcessingAlgorithm::shortHelp;
```




<hr>
## Public Signals Documentation




### signal idChanged 

```C++
void QfProcessingAlgorithm::idChanged;
```



Emitted when the algorithm ID has changed 


        

<hr>



### signal inPlaceFeaturesChanged 

```C++
void QfProcessingAlgorithm::inPlaceFeaturesChanged;
```



Emitted when the in place feature IDs list has changed 


        

<hr>



### signal inPlaceLayerChanged 

```C++
void QfProcessingAlgorithm::inPlaceLayerChanged;
```



Emitted when the in place vector layer has changed 


        

<hr>



### signal parametersChanged 

```C++
void QfProcessingAlgorithm::parametersChanged;
```



Emitted when the parameter model has changed 


        

<hr>



### signal previewChanged 

```C++
void QfProcessingAlgorithm::previewChanged;
```



Emitted when the preview setting has changed 


        

<hr>



### signal previewGeometriesChanged 

```C++
void QfProcessingAlgorithm::previewGeometriesChanged;
```



Emitted when the list of preview geometries has changed 


        

<hr>
## Public Functions Documentation




### function QfProcessingAlgorithm 

```C++
explicit QfProcessingAlgorithm::QfProcessingAlgorithm (
    QObject * parent=nullptr
) 
```




<hr>



### function displayName [2/2]

```C++
QString QfProcessingAlgorithm::displayName () const
```



Returns the display name of the algorithm. 


        

<hr>



### function id [2/2]

```C++
inline QString QfProcessingAlgorithm::id () const
```



Returns the current algorithm ID from which parameters are taken from. 


        

<hr>



### function inPlaceFeatures [2/2]

```C++
inline QList< QgsFeature > QfProcessingAlgorithm::inPlaceFeatures () const
```



Returns the vector _layer_ for in-place algorithm filter. 


        

<hr>



### function inPlaceLayer [2/2]

```C++
inline QgsVectorLayer * QfProcessingAlgorithm::inPlaceLayer () const
```



Returns the vector _layer_ for in-place algorithm filter. 


        

<hr>



### function isValid [2/2]

```C++
inline bool QfProcessingAlgorithm::isValid () const
```



Returns whether the current model refers to a valid algorithm. 


        

<hr>



### function parameters [2/2]

```C++
inline QVariantMap QfProcessingAlgorithm::parameters () const
```



Returns the algorithm parameters as a map of parameter names as keys and values. 


        

<hr>



### function preview [2/2]

```C++
inline bool QfProcessingAlgorithm::preview () const
```



Returns whether the algorithm will provide preview outputs such as modified geometries. from provided parameters. 


        

<hr>



### function previewGeometries [2/2]

```C++
inline QList< QgsGeometry > QfProcessingAlgorithm::previewGeometries () const
```



Returns a list of geometries previewing the algorithm result using current parameters. 


        

<hr>



### function run 

```C++
Q_INVOKABLE bool QfProcessingAlgorithm::run (
    bool previewMode=false
) 
```



Executes the algorithm. 


        

<hr>



### function setId 

```C++
void QfProcessingAlgorithm::setId (
    const QString & id
) 
```



Sets the current algorithm _ID_ from which parameters are taken from. 


        

<hr>



### function setInPlaceFeatures 

```C++
void QfProcessingAlgorithm::setInPlaceFeatures (
    const QList< QgsFeature > & features
) 
```



Sets the vector _layer_ for in-place algorithm filter. 


        

<hr>



### function setInPlaceLayer 

```C++
void QfProcessingAlgorithm::setInPlaceLayer (
    QgsVectorLayer * layer
) 
```



Sets the vector _layer_ for in-place algorithm filter. 


        

<hr>



### function setParameters 

```C++
void QfProcessingAlgorithm::setParameters (
    const QVariantMap & parameters
) 
```



Sets the algorithm parameters using a map with parameter names as keys and values. 


        

<hr>



### function setPreview 

```C++
void QfProcessingAlgorithm::setPreview (
    bool preview
) 
```



Sets whether the algorithm will provide preview outputs such as modified geometries. from provided parameters. 


        

<hr>



### function shortHelp [2/2]

```C++
QString QfProcessingAlgorithm::shortHelp () const
```



Returns a short description of the algorithm. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/processing/qfprocessingalgorithm.h`

