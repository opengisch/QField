

# Class QfProcessingAlgorithmParametersModelBase



[**ClassList**](annotated.md) **>** [**QfProcessingAlgorithmParametersModelBase**](classQfProcessingAlgorithmParametersModelBase.md)



_A model for configuring processing algorithm parameters._ 

* `#include <qfprocessingalgorithmparametersmodel.h>`



Inherits the following classes: QAbstractListModel














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Role**](#enum-role)  <br>_Roles of the model._  |
















## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**algorithmIdChanged**](classQfProcessingAlgorithmParametersModelBase.md#signal-algorithmidchanged) (const QString & id) <br> |
| signal void | [**inPlaceLayerChanged**](classQfProcessingAlgorithmParametersModelBase.md#signal-inplacelayerchanged)  <br> |
| signal void | [**parametersChanged**](classQfProcessingAlgorithmParametersModelBase.md#signal-parameterschanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfProcessingAlgorithmParametersModelBase**](#function-qfprocessingalgorithmparametersmodelbase) (QObject \* parent=nullptr) <br> |
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
enum QfProcessingAlgorithmParametersModelBase::Role {
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
void QfProcessingAlgorithmParametersModelBase::algorithmIdChanged;
```



Emitted when the algorithm ID has changed. 


        

<hr>



### signal inPlaceLayerChanged 

```C++
void QfProcessingAlgorithmParametersModelBase::inPlaceLayerChanged;
```



Emitted when the in place vector layer has changed 


        

<hr>



### signal parametersChanged 

```C++
void QfProcessingAlgorithmParametersModelBase::parametersChanged;
```



Emitted when the parameters have changed. 


        

<hr>
## Public Functions Documentation




### function QfProcessingAlgorithmParametersModelBase 

```C++
explicit QfProcessingAlgorithmParametersModelBase::QfProcessingAlgorithmParametersModelBase (
    QObject * parent=nullptr
) 
```




<hr>



### function algorithmDisplayName 

```C++
QString QfProcessingAlgorithmParametersModelBase::algorithmDisplayName () const
```



Returns the display name of the algorithm. 


        

<hr>



### function algorithmId 

```C++
inline QString QfProcessingAlgorithmParametersModelBase::algorithmId () const
```



Returns the current algorithm ID from which parameters are taken from. 


        

<hr>



### function algorithmShortHelp 

```C++
QString QfProcessingAlgorithmParametersModelBase::algorithmShortHelp () const
```



Returns a short description of the algorithm. 


        

<hr>



### function data 

```C++
QVariant QfProcessingAlgorithmParametersModelBase::data (
    const QModelIndex & index,
    int role
) override const
```




<hr>



### function hasAdvancedParameters 

```C++
inline bool QfProcessingAlgorithmParametersModelBase::hasAdvancedParameters () const
```



Returns whether the current model has advanced parameters. 


        

<hr>



### function hasParameters 

```C++
inline bool QfProcessingAlgorithmParametersModelBase::hasParameters () const
```



Returns whether the current model has parameters. 


        

<hr>



### function inPlaceLayer 

```C++
inline QgsVectorLayer * QfProcessingAlgorithmParametersModelBase::inPlaceLayer () const
```



Returns the vector _layer_ for in-place algorithms for parameters to take details from. 


        

<hr>



### function isValid 

```C++
inline bool QfProcessingAlgorithmParametersModelBase::isValid () const
```



Returns whether the current model refers to a valid algorithm. 


        

<hr>



### function parameters 

```C++
QVariantMap QfProcessingAlgorithmParametersModelBase::parameters () 
```



Returns a variant map of parameter names and values. 


        

<hr>



### function roleNames 

```C++
QHash< int, QByteArray > QfProcessingAlgorithmParametersModelBase::roleNames () override const
```




<hr>



### function rowCount 

```C++
int QfProcessingAlgorithmParametersModelBase::rowCount (
    const QModelIndex & parent
) override const
```




<hr>



### function setAlgorithmId 

```C++
void QfProcessingAlgorithmParametersModelBase::setAlgorithmId (
    const QString & id
) 
```



Sets the current algorithm _ID_ from which parameters are taken from. 


        

<hr>



### function setData 

```C++
bool QfProcessingAlgorithmParametersModelBase::setData (
    const QModelIndex & index,
    const QVariant & value,
    int role=Qt::EditRole
) override
```




<hr>



### function setInPlaceLayer 

```C++
void QfProcessingAlgorithmParametersModelBase::setInPlaceLayer (
    QgsVectorLayer * layer
) 
```



Sets the vector _layer_ for in-place algorithm filter for parameters to take details from. 


        

<hr>



### function setParameters 

```C++
void QfProcessingAlgorithmParametersModelBase::setParameters (
    const QVariantMap & parameters
) 
```



Sets the values of the parameters model from variant map of parameter names and values. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/processing/qfprocessingalgorithmparametersmodel.h`

