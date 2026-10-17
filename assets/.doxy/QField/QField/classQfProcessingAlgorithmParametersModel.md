

# Class QfProcessingAlgorithmParametersModel



[**ClassList**](annotated.md) **>** [**QfProcessingAlgorithmParametersModel**](classQfProcessingAlgorithmParametersModel.md)



_A sort/filter proxy model for providers and algorithms available within QField which automatically sorts the toolbox in a logical fashion and supports filtering the results._ 

* `#include <qfprocessingalgorithmparametersmodel.h>`



Inherits the following classes: QSortFilterProxyModel














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Filter**](#enum-filter)  <br>_Available filter flags for filtering the model._  |












## Public Properties

| Type | Name |
| ---: | :--- |
| property QString | [**algorithmDisplayName**](classQfProcessingAlgorithmParametersModel.md#property-algorithmdisplayname-12)  <br> |
| property QString | [**algorithmId**](classQfProcessingAlgorithmParametersModel.md#property-algorithmid-12)  <br> |
| property QString | [**algorithmShortHelp**](classQfProcessingAlgorithmParametersModel.md#property-algorithmshorthelp-12)  <br> |
| property QfProcessingAlgorithmParametersModel::Filters | [**filters**](classQfProcessingAlgorithmParametersModel.md#property-filters-12)  <br> |
| property bool | [**hasAdvancedParameters**](classQfProcessingAlgorithmParametersModel.md#property-hasadvancedparameters-12)  <br> |
| property bool | [**hasParameters**](classQfProcessingAlgorithmParametersModel.md#property-hasparameters-12)  <br> |
| property QgsVectorLayer \* | [**inPlaceLayer**](classQfProcessingAlgorithmParametersModel.md#property-inplacelayer-12)  <br> |
| property bool | [**isValid**](classQfProcessingAlgorithmParametersModel.md#property-isvalid-12)  <br> |
| property QVariantMap | [**parameters**](classQfProcessingAlgorithmParametersModel.md#property-parameters-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**algorithmIdChanged**](classQfProcessingAlgorithmParametersModel.md#signal-algorithmidchanged) (const QString & id) <br> |
| signal void | [**filtersChanged**](classQfProcessingAlgorithmParametersModel.md#signal-filterschanged)  <br> |
| signal void | [**inPlaceLayerChanged**](classQfProcessingAlgorithmParametersModel.md#signal-inplacelayerchanged)  <br> |
| signal void | [**parametersChanged**](classQfProcessingAlgorithmParametersModel.md#signal-parameterschanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfProcessingAlgorithmParametersModel**](#function-qfprocessingalgorithmparametersmodel) (QObject \* parent=nullptr) <br> |
|  QString | [**algorithmDisplayName**](#function-algorithmdisplayname-22) () const<br> |
|  QString | [**algorithmId**](#function-algorithmid-22) () const<br> |
|  QString | [**algorithmShortHelp**](#function-algorithmshorthelp-22) () const<br> |
|  bool | [**filterAcceptsRow**](#function-filteracceptsrow) (int sourceRow, const QModelIndex & sourceParent) override const<br> |
|  QfProcessingAlgorithmParametersModel::Filters | [**filters**](#function-filters-22) () const<br> |
|  bool | [**hasAdvancedParameters**](#function-hasadvancedparameters-22) () const<br> |
|  bool | [**hasParameters**](#function-hasparameters-22) () const<br> |
|  QgsVectorLayer \* | [**inPlaceLayer**](#function-inplacelayer-22) () const<br> |
|  bool | [**isValid**](#function-isvalid-22) () const<br> |
|  QVariantMap | [**parameters**](#function-parameters-22) () <br> |
|  void | [**setAlgorithmId**](#function-setalgorithmid) (const QString & id) <br> |
|  void | [**setFilters**](#function-setfilters) (QfProcessingAlgorithmParametersModel::Filters filters) <br> |
|  void | [**setInPlaceLayer**](#function-setinplacelayer) (QgsVectorLayer \* layer) <br> |
|  void | [**setParameters**](#function-setparameters) (const QVariantMap & parameters) <br> |




























## Public Types Documentation




### enum Filter 

_Available filter flags for filtering the model._ 
```C++
enum QfProcessingAlgorithmParametersModel::Filter {
    GeneralParameterFilter = 1 << 1,
    AdvancedParameterFilter = 1 << 2
};
```




<hr>
## Public Properties Documentation




### property algorithmDisplayName [1/2]

```C++
QString QfProcessingAlgorithmParametersModel::algorithmDisplayName;
```




<hr>



### property algorithmId [1/2]

```C++
QString QfProcessingAlgorithmParametersModel::algorithmId;
```




<hr>



### property algorithmShortHelp [1/2]

```C++
QString QfProcessingAlgorithmParametersModel::algorithmShortHelp;
```




<hr>



### property filters [1/2]

```C++
QfProcessingAlgorithmParametersModel::Filters QfProcessingAlgorithmParametersModel::filters;
```




<hr>



### property hasAdvancedParameters [1/2]

```C++
bool QfProcessingAlgorithmParametersModel::hasAdvancedParameters;
```




<hr>



### property hasParameters [1/2]

```C++
bool QfProcessingAlgorithmParametersModel::hasParameters;
```




<hr>



### property inPlaceLayer [1/2]

```C++
QgsVectorLayer * QfProcessingAlgorithmParametersModel::inPlaceLayer;
```




<hr>



### property isValid [1/2]

```C++
bool QfProcessingAlgorithmParametersModel::isValid;
```




<hr>



### property parameters [1/2]

```C++
QVariantMap QfProcessingAlgorithmParametersModel::parameters;
```




<hr>
## Public Signals Documentation




### signal algorithmIdChanged 

```C++
void QfProcessingAlgorithmParametersModel::algorithmIdChanged;
```



Emitted when the algorithm ID has changed. 


        

<hr>



### signal filtersChanged 

```C++
void QfProcessingAlgorithmParametersModel::filtersChanged;
```



Emitted when the active filters have changed. 


        

<hr>



### signal inPlaceLayerChanged 

```C++
void QfProcessingAlgorithmParametersModel::inPlaceLayerChanged;
```



Emitted when the in place vector layer has changed 


        

<hr>



### signal parametersChanged 

```C++
void QfProcessingAlgorithmParametersModel::parametersChanged;
```



Emitted when the parameters have changed. 


        

<hr>
## Public Functions Documentation




### function QfProcessingAlgorithmParametersModel 

```C++
explicit QfProcessingAlgorithmParametersModel::QfProcessingAlgorithmParametersModel (
    QObject * parent=nullptr
) 
```




<hr>



### function algorithmDisplayName [2/2]

```C++
QString QfProcessingAlgorithmParametersModel::algorithmDisplayName () const
```



Returns the display name of the algorithm. 


        

<hr>



### function algorithmId [2/2]

```C++
QString QfProcessingAlgorithmParametersModel::algorithmId () const
```



Returns the current algorithm ID from which parameters are taken from. 


        

<hr>



### function algorithmShortHelp [2/2]

```C++
QString QfProcessingAlgorithmParametersModel::algorithmShortHelp () const
```



Returns a short description of the algorithm. 


        

<hr>



### function filterAcceptsRow 

```C++
bool QfProcessingAlgorithmParametersModel::filterAcceptsRow (
    int sourceRow,
    const QModelIndex & sourceParent
) override const
```




<hr>



### function filters [2/2]

```C++
inline QfProcessingAlgorithmParametersModel::Filters QfProcessingAlgorithmParametersModel::filters () const
```



Returns any filters that affect how toolbox content is filtered. 

**See also:** [**setFilters()**](classQfProcessingAlgorithmParametersModel.md#function-setfilters) 



        

<hr>



### function hasAdvancedParameters [2/2]

```C++
bool QfProcessingAlgorithmParametersModel::hasAdvancedParameters () const
```



Returns whether the current model has advanced parameters. 


        

<hr>



### function hasParameters [2/2]

```C++
bool QfProcessingAlgorithmParametersModel::hasParameters () const
```



Returns whether the current model has parameters. 


        

<hr>



### function inPlaceLayer [2/2]

```C++
QgsVectorLayer * QfProcessingAlgorithmParametersModel::inPlaceLayer () const
```



Returns the vector _layer_ for in-place algorithms for parameters to take details from. 


        

<hr>



### function isValid [2/2]

```C++
bool QfProcessingAlgorithmParametersModel::isValid () const
```



Returns whether the current model refers to a valid algorithm. 


        

<hr>



### function parameters [2/2]

```C++
QVariantMap QfProcessingAlgorithmParametersModel::parameters () 
```



Returns a variant map of parameter names and values. 


        

<hr>



### function setAlgorithmId 

```C++
void QfProcessingAlgorithmParametersModel::setAlgorithmId (
    const QString & id
) 
```



Sets the current algorithm _ID_ from which parameters are taken from. 


        

<hr>



### function setFilters 

```C++
void QfProcessingAlgorithmParametersModel::setFilters (
    QfProcessingAlgorithmParametersModel::Filters filters
) 
```



Set _filters_ that affect how toolbox content is filtered. 

**See also:** filters() 



        

<hr>



### function setInPlaceLayer 

```C++
void QfProcessingAlgorithmParametersModel::setInPlaceLayer (
    QgsVectorLayer * layer
) 
```



Sets the vector _layer_ for in-place algorithm filter for parameters to take details from. 


        

<hr>



### function setParameters 

```C++
void QfProcessingAlgorithmParametersModel::setParameters (
    const QVariantMap & parameters
) 
```



Sets the values of the parameters model from variant map of parameter names and values. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/processing/qfprocessingalgorithmparametersmodel.h`

