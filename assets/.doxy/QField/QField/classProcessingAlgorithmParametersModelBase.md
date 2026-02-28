

# Class ProcessingAlgorithmParametersModelBase



[**ClassList**](annotated.md) **>** [**ProcessingAlgorithmParametersModelBase**](classProcessingAlgorithmParametersModelBase.md)



_A model for configuring processing algorithm parameters._ 

* `#include <processingalgorithmparametersmodel.h>`



Inherits the following classes: QAbstractListModel














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Role**](#enum-role)  <br>_Roles of the model._  |
















## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**algorithmIdChanged**](classProcessingAlgorithmParametersModelBase.md#signal-algorithmidchanged) (const QString & id) <br> |
| signal void | [**inPlaceLayerChanged**](classProcessingAlgorithmParametersModelBase.md#signal-inplacelayerchanged)  <br> |
| signal void | [**parametersChanged**](classProcessingAlgorithmParametersModelBase.md#signal-parameterschanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ProcessingAlgorithmParametersModelBase**](#function-processingalgorithmparametersmodelbase) (QObject \* parent=nullptr) <br> |
|  QString | [**algorithmDisplayName**](#function-algorithmdisplayname) () const<br> |
|  QString | [**algorithmId**](#function-algorithmid) () const<br> |
|  QString | [**algorithmShortHelp**](#function-algorithmshorthelp) () const<br> |
|  QVariant | [**data**](#function-data) (const QModelIndex & index, int role) override const<br> |
|  bool | [**hasAdvancedParameters**](#function-hasadvancedparameters) () const<br> |
|  bool | [**hasParameters**](#function-hasparameters) () const<br> |
|  QgsVectorLayer \* | [**inPlaceLayer**](#function-inplacelayer) () const<br> |
|  bool | [**isValid**](#function-isvalid) () const<br> |
|  QVariantMap | [**parameters**](#function-parameters) () <br> |
|  QHash&lt; int, QByteArray &gt; | [**roleNames**](#function-rolenames) () override const<br> |
|  int | [**rowCount**](#function-rowcount) (const QModelIndex & parent) override const<br> |
|  void | [**setAlgorithmId**](#function-setalgorithmid) (const QString & id) <br> |
|  bool | [**setData**](#function-setdata) (const QModelIndex & index, const QVariant & value, int role=Qt::EditRole) override<br> |
|  void | [**setInPlaceLayer**](#function-setinplacelayer) (QgsVectorLayer \* layer) <br> |
|  void | [**setParameters**](#function-setparameters) (const QVariantMap & parameters) <br> |




























## Public Types Documentation




### enum Role 

_Roles of the model._ 
```C++
enum ProcessingAlgorithmParametersModelBase::Role {
    ParameterTypeRole = Qt::UserRole,
    ParameterDescriptionRole,
    ParameterFlagsRole,
    ParameterDefaultValueRole,
    ParameterValueRole,
    ParameterConfigurationRole
};
```




<hr>
## Public Signals Documentation




### signal algorithmIdChanged 

```C++
void ProcessingAlgorithmParametersModelBase::algorithmIdChanged;
```



Emitted when the algorithm ID has changed. 


        

<hr>



### signal inPlaceLayerChanged 

```C++
void ProcessingAlgorithmParametersModelBase::inPlaceLayerChanged;
```



Emitted when the in place vector layer has changed 


        

<hr>



### signal parametersChanged 

```C++
void ProcessingAlgorithmParametersModelBase::parametersChanged;
```



Emitted when the parameters have changed. 


        

<hr>
## Public Functions Documentation




### function ProcessingAlgorithmParametersModelBase 

```C++
explicit ProcessingAlgorithmParametersModelBase::ProcessingAlgorithmParametersModelBase (
    QObject * parent=nullptr
) 
```




<hr>



### function algorithmDisplayName 

```C++
QString ProcessingAlgorithmParametersModelBase::algorithmDisplayName () const
```



Returns the display name of the algorithm. 


        

<hr>



### function algorithmId 

```C++
inline QString ProcessingAlgorithmParametersModelBase::algorithmId () const
```



Returns the current algorithm ID from which parameters are taken from. 


        

<hr>



### function algorithmShortHelp 

```C++
QString ProcessingAlgorithmParametersModelBase::algorithmShortHelp () const
```



Returns a short description of the algorithm. 


        

<hr>



### function data 

```C++
QVariant ProcessingAlgorithmParametersModelBase::data (
    const QModelIndex & index,
    int role
) override const
```




<hr>



### function hasAdvancedParameters 

```C++
inline bool ProcessingAlgorithmParametersModelBase::hasAdvancedParameters () const
```



Returns whether the current model has advanced parameters. 


        

<hr>



### function hasParameters 

```C++
inline bool ProcessingAlgorithmParametersModelBase::hasParameters () const
```



Returns whether the current model has parameters. 


        

<hr>



### function inPlaceLayer 

```C++
inline QgsVectorLayer * ProcessingAlgorithmParametersModelBase::inPlaceLayer () const
```



Returns the vector _layer_ for in-place algorithms for parameters to take details from. 


        

<hr>



### function isValid 

```C++
inline bool ProcessingAlgorithmParametersModelBase::isValid () const
```



Returns whether the current model refers to a valid algorithm. 


        

<hr>



### function parameters 

```C++
QVariantMap ProcessingAlgorithmParametersModelBase::parameters () 
```



Returns a variant map of parameter names and values. 


        

<hr>



### function roleNames 

```C++
QHash< int, QByteArray > ProcessingAlgorithmParametersModelBase::roleNames () override const
```




<hr>



### function rowCount 

```C++
int ProcessingAlgorithmParametersModelBase::rowCount (
    const QModelIndex & parent
) override const
```




<hr>



### function setAlgorithmId 

```C++
void ProcessingAlgorithmParametersModelBase::setAlgorithmId (
    const QString & id
) 
```



Sets the current algorithm _ID_ from which parameters are taken from. 


        

<hr>



### function setData 

```C++
bool ProcessingAlgorithmParametersModelBase::setData (
    const QModelIndex & index,
    const QVariant & value,
    int role=Qt::EditRole
) override
```




<hr>



### function setInPlaceLayer 

```C++
void ProcessingAlgorithmParametersModelBase::setInPlaceLayer (
    QgsVectorLayer * layer
) 
```



Sets the vector _layer_ for in-place algorithm filter for parameters to take details from. 


        

<hr>



### function setParameters 

```C++
void ProcessingAlgorithmParametersModelBase::setParameters (
    const QVariantMap & parameters
) 
```



Sets the values of the parameters model from variant map of parameter names and values. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/processing/processingalgorithmparametersmodel.h`

