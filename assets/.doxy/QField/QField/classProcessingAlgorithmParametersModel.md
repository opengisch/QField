

# Class ProcessingAlgorithmParametersModel



[**ClassList**](annotated.md) **>** [**ProcessingAlgorithmParametersModel**](classProcessingAlgorithmParametersModel.md)



_A sort/filter proxy model for providers and algorithms available within QField which automatically sorts the toolbox in a logical fashion and supports filtering the results._ 

* `#include <processingalgorithmparametersmodel.h>`



Inherits the following classes: QSortFilterProxyModel














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Filter**](#enum-filter)  <br>_Available filter flags for filtering the model._  |












## Public Properties

| Type | Name |
| ---: | :--- |
| property QString | [**algorithmDisplayName**](classProcessingAlgorithmParametersModel.md#property-algorithmdisplayname-12)  <br> |
| property QString | [**algorithmId**](classProcessingAlgorithmParametersModel.md#property-algorithmid-12)  <br> |
| property QString | [**algorithmShortHelp**](classProcessingAlgorithmParametersModel.md#property-algorithmshorthelp-12)  <br> |
| property ProcessingAlgorithmParametersModel::Filters | [**filters**](classProcessingAlgorithmParametersModel.md#property-filters-12)  <br> |
| property bool | [**hasAdvancedParameters**](classProcessingAlgorithmParametersModel.md#property-hasadvancedparameters-12)  <br> |
| property bool | [**hasParameters**](classProcessingAlgorithmParametersModel.md#property-hasparameters-12)  <br> |
| property QgsVectorLayer \* | [**inPlaceLayer**](classProcessingAlgorithmParametersModel.md#property-inplacelayer-12)  <br> |
| property bool | [**isValid**](classProcessingAlgorithmParametersModel.md#property-isvalid-12)  <br> |
| property QVariantMap | [**parameters**](classProcessingAlgorithmParametersModel.md#property-parameters-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**algorithmIdChanged**](classProcessingAlgorithmParametersModel.md#signal-algorithmidchanged) (const QString & id) <br> |
| signal void | [**filtersChanged**](classProcessingAlgorithmParametersModel.md#signal-filterschanged)  <br> |
| signal void | [**inPlaceLayerChanged**](classProcessingAlgorithmParametersModel.md#signal-inplacelayerchanged)  <br> |
| signal void | [**parametersChanged**](classProcessingAlgorithmParametersModel.md#signal-parameterschanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ProcessingAlgorithmParametersModel**](#function-processingalgorithmparametersmodel) (QObject \* parent=nullptr) <br> |
|  QString | [**algorithmDisplayName**](#function-algorithmdisplayname-22) () const<br> |
|  QString | [**algorithmId**](#function-algorithmid-22) () const<br> |
|  QString | [**algorithmShortHelp**](#function-algorithmshorthelp-22) () const<br> |
|  bool | [**filterAcceptsRow**](#function-filteracceptsrow) (int sourceRow, const QModelIndex & sourceParent) override const<br> |
|  ProcessingAlgorithmParametersModel::Filters | [**filters**](#function-filters-22) () const<br> |
|  bool | [**hasAdvancedParameters**](#function-hasadvancedparameters-22) () const<br> |
|  bool | [**hasParameters**](#function-hasparameters-22) () const<br> |
|  QgsVectorLayer \* | [**inPlaceLayer**](#function-inplacelayer-22) () const<br> |
|  bool | [**isValid**](#function-isvalid-22) () const<br> |
|  QVariantMap | [**parameters**](#function-parameters-22) () <br> |
|  void | [**setAlgorithmId**](#function-setalgorithmid) (const QString & id) <br> |
|  void | [**setFilters**](#function-setfilters) (ProcessingAlgorithmParametersModel::Filters filters) <br> |
|  void | [**setInPlaceLayer**](#function-setinplacelayer) (QgsVectorLayer \* layer) <br> |
|  void | [**setParameters**](#function-setparameters) (const QVariantMap & parameters) <br> |




























## Public Types Documentation




### enum Filter 

_Available filter flags for filtering the model._ 
```C++
enum ProcessingAlgorithmParametersModel::Filter {
    GeneralParameterFilter = 1 << 1,
    AdvancedParameterFilter = 1 << 2
};
```




<hr>
## Public Properties Documentation




### property algorithmDisplayName [1/2]

```C++
QString ProcessingAlgorithmParametersModel::algorithmDisplayName;
```




<hr>



### property algorithmId [1/2]

```C++
QString ProcessingAlgorithmParametersModel::algorithmId;
```




<hr>



### property algorithmShortHelp [1/2]

```C++
QString ProcessingAlgorithmParametersModel::algorithmShortHelp;
```




<hr>



### property filters [1/2]

```C++
ProcessingAlgorithmParametersModel::Filters ProcessingAlgorithmParametersModel::filters;
```




<hr>



### property hasAdvancedParameters [1/2]

```C++
bool ProcessingAlgorithmParametersModel::hasAdvancedParameters;
```




<hr>



### property hasParameters [1/2]

```C++
bool ProcessingAlgorithmParametersModel::hasParameters;
```




<hr>



### property inPlaceLayer [1/2]

```C++
QgsVectorLayer * ProcessingAlgorithmParametersModel::inPlaceLayer;
```




<hr>



### property isValid [1/2]

```C++
bool ProcessingAlgorithmParametersModel::isValid;
```




<hr>



### property parameters [1/2]

```C++
QVariantMap ProcessingAlgorithmParametersModel::parameters;
```




<hr>
## Public Signals Documentation




### signal algorithmIdChanged 

```C++
void ProcessingAlgorithmParametersModel::algorithmIdChanged;
```



Emitted when the algorithm ID has changed. 


        

<hr>



### signal filtersChanged 

```C++
void ProcessingAlgorithmParametersModel::filtersChanged;
```



Emitted when the active filters have changed. 


        

<hr>



### signal inPlaceLayerChanged 

```C++
void ProcessingAlgorithmParametersModel::inPlaceLayerChanged;
```



Emitted when the in place vector layer has changed 


        

<hr>



### signal parametersChanged 

```C++
void ProcessingAlgorithmParametersModel::parametersChanged;
```



Emitted when the parameters have changed. 


        

<hr>
## Public Functions Documentation




### function ProcessingAlgorithmParametersModel 

```C++
explicit ProcessingAlgorithmParametersModel::ProcessingAlgorithmParametersModel (
    QObject * parent=nullptr
) 
```




<hr>



### function algorithmDisplayName [2/2]

```C++
QString ProcessingAlgorithmParametersModel::algorithmDisplayName () const
```



Returns the display name of the algorithm. 


        

<hr>



### function algorithmId [2/2]

```C++
QString ProcessingAlgorithmParametersModel::algorithmId () const
```



Returns the current algorithm ID from which parameters are taken from. 


        

<hr>



### function algorithmShortHelp [2/2]

```C++
QString ProcessingAlgorithmParametersModel::algorithmShortHelp () const
```



Returns a short description of the algorithm. 


        

<hr>



### function filterAcceptsRow 

```C++
bool ProcessingAlgorithmParametersModel::filterAcceptsRow (
    int sourceRow,
    const QModelIndex & sourceParent
) override const
```




<hr>



### function filters [2/2]

```C++
inline ProcessingAlgorithmParametersModel::Filters ProcessingAlgorithmParametersModel::filters () const
```



Returns any filters that affect how toolbox content is filtered. 

**See also:** [**setFilters()**](classProcessingAlgorithmParametersModel.md#function-setfilters) 



        

<hr>



### function hasAdvancedParameters [2/2]

```C++
bool ProcessingAlgorithmParametersModel::hasAdvancedParameters () const
```



Returns whether the current model has advanced parameters. 


        

<hr>



### function hasParameters [2/2]

```C++
bool ProcessingAlgorithmParametersModel::hasParameters () const
```



Returns whether the current model has parameters. 


        

<hr>



### function inPlaceLayer [2/2]

```C++
QgsVectorLayer * ProcessingAlgorithmParametersModel::inPlaceLayer () const
```



Returns the vector _layer_ for in-place algorithms for parameters to take details from. 


        

<hr>



### function isValid [2/2]

```C++
bool ProcessingAlgorithmParametersModel::isValid () const
```



Returns whether the current model refers to a valid algorithm. 


        

<hr>



### function parameters [2/2]

```C++
QVariantMap ProcessingAlgorithmParametersModel::parameters () 
```



Returns a variant map of parameter names and values. 


        

<hr>



### function setAlgorithmId 

```C++
void ProcessingAlgorithmParametersModel::setAlgorithmId (
    const QString & id
) 
```



Sets the current algorithm _ID_ from which parameters are taken from. 


        

<hr>



### function setFilters 

```C++
void ProcessingAlgorithmParametersModel::setFilters (
    ProcessingAlgorithmParametersModel::Filters filters
) 
```



Set _filters_ that affect how toolbox content is filtered. 

**See also:** filters() 



        

<hr>



### function setInPlaceLayer 

```C++
void ProcessingAlgorithmParametersModel::setInPlaceLayer (
    QgsVectorLayer * layer
) 
```



Sets the vector _layer_ for in-place algorithm filter for parameters to take details from. 


        

<hr>



### function setParameters 

```C++
void ProcessingAlgorithmParametersModel::setParameters (
    const QVariantMap & parameters
) 
```



Sets the values of the parameters model from variant map of parameter names and values. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/processing/processingalgorithmparametersmodel.h`

