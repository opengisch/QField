

# Class ReferencingFeatureListModelBase



[**ClassList**](annotated.md) **>** [**ReferencingFeatureListModelBase**](classReferencingFeatureListModelBase.md)








Inherits the following classes: QAbstractItemModel


Inherited by the following classes: [OrderedRelationModel](classOrderedRelationModel.md)












## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**ReferencedFeatureListRoles**](#enum-referencedfeaturelistroles)  <br> |












## Public Properties

| Type | Name |
| ---: | :--- |
| property QString | [**currentNmRelationId**](classReferencingFeatureListModelBase.md#property-currentnmrelationid-12)  <br> |
| property QString | [**currentRelationId**](classReferencingFeatureListModelBase.md#property-currentrelationid-12)  <br> |
| property QgsFeature | [**feature**](classReferencingFeatureListModelBase.md#property-feature-12)  <br> |
| property bool | [**isLoading**](classReferencingFeatureListModelBase.md#property-isloading-12)  <br> |
| property QgsRelation | [**nmRelation**](classReferencingFeatureListModelBase.md#property-nmrelation-12)  <br> |
| property bool | [**parentPrimariesAvailable**](classReferencingFeatureListModelBase.md#property-parentprimariesavailable-12)  <br> |
| property QgsRelation | [**relation**](classReferencingFeatureListModelBase.md#property-relation-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**attributeFormModelChanged**](classReferencingFeatureListModelBase.md#signal-attributeformmodelchanged)  <br> |
| signal void | [**beforeModelUpdated**](classReferencingFeatureListModelBase.md#signal-beforemodelupdated)  <br> |
| signal void | [**featureChanged**](classReferencingFeatureListModelBase.md#signal-featurechanged)  <br> |
| signal void | [**isLoadingChanged**](classReferencingFeatureListModelBase.md#signal-isloadingchanged)  <br> |
| signal void | [**modelUpdated**](classReferencingFeatureListModelBase.md#signal-modelupdated)  <br> |
| signal void | [**nmRelationChanged**](classReferencingFeatureListModelBase.md#signal-nmrelationchanged)  <br> |
| signal void | [**parentPrimariesAvailableChanged**](classReferencingFeatureListModelBase.md#signal-parentprimariesavailablechanged)  <br> |
| signal void | [**relationChanged**](classReferencingFeatureListModelBase.md#signal-relationchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ReferencingFeatureListModelBase**](#function-referencingfeaturelistmodelbase) (QObject \* parent=nullptr) <br> |
|  int | [**columnCount**](#function-columncount) (const QModelIndex & parent=QModelIndex()) override const<br> |
|  QString | [**currentNmRelationId**](#function-currentnmrelationid-22) () const<br>_On many-to-many relations returns the second relation id connecting the children in the association table to their other parent._  |
|  QString | [**currentRelationId**](#function-currentrelationid-22) () const<br>_Returns the id of the relation connecting the parent feature with the children in this model._  |
|  QVariant | [**data**](#function-data) (const QModelIndex & index, int role=Qt::DisplayRole) override const<br> |
|  Q\_INVOKABLE bool | [**deleteFeature**](#function-deletefeature) (QgsFeatureId referencingFeatureId) <br> |
|  QgsFeature | [**feature**](#function-feature-22) () const<br> |
|  Q\_INVOKABLE int | [**getFeatureIdRow**](#function-getfeatureidrow) (QgsFeatureId featureId) <br> |
|  QModelIndex | [**index**](#function-index) (int row, int column, const QModelIndex & parent=QModelIndex()) override const<br> |
|  bool | [**isLoading**](#function-isloading-22) () const<br> |
|  QgsRelation | [**nmRelation**](#function-nmrelation-22) () const<br> |
|  QModelIndex | [**parent**](#function-parent) (const QModelIndex & index) override const<br> |
|  bool | [**parentPrimariesAvailable**](#function-parentprimariesavailable-22) () const<br> |
|  QgsRelation | [**relation**](#function-relation-22) () const<br> |
|  Q\_INVOKABLE void | [**reload**](#function-reload) () <br> |
|  QHash&lt; int, QByteArray &gt; | [**roleNames**](#function-rolenames) () override const<br> |
|  int | [**rowCount**](#function-rowcount) (const QModelIndex & parent=QModelIndex()) override const<br> |
|  void | [**setCurrentNmRelationId**](#function-setcurrentnmrelationid) (const QString & nmRelationId) <br>_On many-to-many relations sets the second relation connecting the children in the association table to their other parent._  |
|  void | [**setCurrentRelationId**](#function-setcurrentrelationid) (const QString & relationId) <br>_Sets the relation connecting the parent feature with the children in this model._  |
|  void | [**setFeature**](#function-setfeature) (const QgsFeature & feature) <br> |
|  void | [**setNmRelation**](#function-setnmrelation) (const QgsRelation & relation) <br> |
|  void | [**setParentPrimariesAvailable**](#function-setparentprimariesavailable) (const bool parentPrimariesAvailable) <br> |
|  void | [**setRelation**](#function-setrelation) (const QgsRelation & relation) <br> |




























## Public Types Documentation




### enum ReferencedFeatureListRoles 

```C++
enum ReferencingFeatureListModelBase::ReferencedFeatureListRoles {
    DisplayString = Qt::UserRole,
    ReferencingFeature,
    NmReferencedFeature,
    NmDisplayString
};
```




<hr>
## Public Properties Documentation




### property currentNmRelationId [1/2]

```C++
QString ReferencingFeatureListModelBase::currentNmRelationId;
```




<hr>



### property currentRelationId [1/2]

```C++
QString ReferencingFeatureListModelBase::currentRelationId;
```




<hr>



### property feature [1/2]

```C++
QgsFeature ReferencingFeatureListModelBase::feature;
```




<hr>



### property isLoading [1/2]

```C++
bool ReferencingFeatureListModelBase::isLoading;
```




<hr>



### property nmRelation [1/2]

```C++
QgsRelation ReferencingFeatureListModelBase::nmRelation;
```




<hr>



### property parentPrimariesAvailable [1/2]

```C++
bool ReferencingFeatureListModelBase::parentPrimariesAvailable;
```




<hr>



### property relation [1/2]

```C++
QgsRelation ReferencingFeatureListModelBase::relation;
```




<hr>
## Public Signals Documentation




### signal attributeFormModelChanged 

```C++
void ReferencingFeatureListModelBase::attributeFormModelChanged;
```




<hr>



### signal beforeModelUpdated 

```C++
void ReferencingFeatureListModelBase::beforeModelUpdated;
```




<hr>



### signal featureChanged 

```C++
void ReferencingFeatureListModelBase::featureChanged;
```




<hr>



### signal isLoadingChanged 

```C++
void ReferencingFeatureListModelBase::isLoadingChanged;
```




<hr>



### signal modelUpdated 

```C++
void ReferencingFeatureListModelBase::modelUpdated;
```




<hr>



### signal nmRelationChanged 

```C++
void ReferencingFeatureListModelBase::nmRelationChanged;
```




<hr>



### signal parentPrimariesAvailableChanged 

```C++
void ReferencingFeatureListModelBase::parentPrimariesAvailableChanged;
```




<hr>



### signal relationChanged 

```C++
void ReferencingFeatureListModelBase::relationChanged;
```




<hr>
## Public Functions Documentation




### function ReferencingFeatureListModelBase 

```C++
explicit ReferencingFeatureListModelBase::ReferencingFeatureListModelBase (
    QObject * parent=nullptr
) 
```




<hr>



### function columnCount 

```C++
int ReferencingFeatureListModelBase::columnCount (
    const QModelIndex & parent=QModelIndex()
) override const
```




<hr>



### function currentNmRelationId [2/2]

_On many-to-many relations returns the second relation id connecting the children in the association table to their other parent._ 
```C++
QString ReferencingFeatureListModelBase::currentNmRelationId () const
```




<hr>



### function currentRelationId [2/2]

_Returns the id of the relation connecting the parent feature with the children in this model._ 
```C++
QString ReferencingFeatureListModelBase::currentRelationId () const
```




<hr>



### function data 

```C++
QVariant ReferencingFeatureListModelBase::data (
    const QModelIndex & index,
    int role=Qt::DisplayRole
) override const
```




<hr>



### function deleteFeature 

```C++
Q_INVOKABLE bool ReferencingFeatureListModelBase::deleteFeature (
    QgsFeatureId referencingFeatureId
) 
```



Deletes a feature regarding the referencing layer and the feature id of the selected child 

**Parameters:**


* `referencingFeatureId` id of the selected child 




        

<hr>



### function feature [2/2]

```C++
QgsFeature ReferencingFeatureListModelBase::feature () const
```



The parent feature for which this model contains the children 

**Returns:**

the parent feature 




**See also:** [**setFeature**](classReferencingFeatureListModelBase.md#function-setfeature) 



        

<hr>



### function getFeatureIdRow 

```C++
Q_INVOKABLE int ReferencingFeatureListModelBase::getFeatureIdRow (
    QgsFeatureId featureId
) 
```



Returns the row number for a given feature id 

**Parameters:**


* `featureId` the feature id 




        

<hr>



### function index 

```C++
QModelIndex ReferencingFeatureListModelBase::index (
    int row,
    int column,
    const QModelIndex & parent=QModelIndex()
) override const
```




<hr>



### function isLoading [2/2]

```C++
bool ReferencingFeatureListModelBase::isLoading () const
```



Indicator if the model is currently performing any feature iteration in the background. 


        

<hr>



### function nmRelation [2/2]

```C++
QgsRelation ReferencingFeatureListModelBase::nmRelation () const
```



On many-to-many relations this is the second relation connecting the children in the associationtable to their other parent 

**Returns:**

associated relation 




**See also:** [**setNmRelation**](classReferencingFeatureListModelBase.md#function-setnmrelation) 



        

<hr>



### function parent 

```C++
QModelIndex ReferencingFeatureListModelBase::parent (
    const QModelIndex & index
) override const
```




<hr>



### function parentPrimariesAvailable [2/2]

```C++
bool ReferencingFeatureListModelBase::parentPrimariesAvailable () const
```



The status if the pk of the parent feature (this feature) are valid (not null) It's needed to check on opening a form to add a new child 

**Returns:**

parentPrimariesAvailable The status if the parent pks are available 




**See also:** [**setParentPrimariesAvailable**](classReferencingFeatureListModelBase.md#function-setparentprimariesavailable) 



        

<hr>



### function relation [2/2]

```C++
QgsRelation ReferencingFeatureListModelBase::relation () const
```



The relation connecting the parent feature with the children in this model 

**Returns:**

relation 




**See also:** [**setRelation**](classReferencingFeatureListModelBase.md#function-setrelation) 



        

<hr>



### function reload 

```C++
Q_INVOKABLE void ReferencingFeatureListModelBase::reload () 
```



Reloads the model by starting the reload functionality in the gatherer (seperate thread) Sets the property parentPrimariesAvailable 


        

<hr>



### function roleNames 

```C++
QHash< int, QByteArray > ReferencingFeatureListModelBase::roleNames () override const
```




<hr>



### function rowCount 

```C++
int ReferencingFeatureListModelBase::rowCount (
    const QModelIndex & parent=QModelIndex()
) override const
```




<hr>



### function setCurrentNmRelationId 

_On many-to-many relations sets the second relation connecting the children in the association table to their other parent._ 
```C++
void ReferencingFeatureListModelBase::setCurrentNmRelationId (
    const QString & nmRelationId
) 
```




<hr>



### function setCurrentRelationId 

_Sets the relation connecting the parent feature with the children in this model._ 
```C++
void ReferencingFeatureListModelBase::setCurrentRelationId (
    const QString & relationId
) 
```




<hr>



### function setFeature 

```C++
void ReferencingFeatureListModelBase::setFeature (
    const QgsFeature & feature
) 
```



The parent feature for which this model contains the children 

**Parameters:**


* `feature` 



**See also:** feature 



        

<hr>



### function setNmRelation 

```C++
void ReferencingFeatureListModelBase::setNmRelation (
    const QgsRelation & relation
) 
```



On many-to-many relations this is the second relation connecting the children in the associationtable to their other parent 

**Parameters:**


* `relation` The associated relation 



**See also:** nmRelation 



        

<hr>



### function setParentPrimariesAvailable 

```C++
void ReferencingFeatureListModelBase::setParentPrimariesAvailable (
    const bool parentPrimariesAvailable
) 
```



The status if the pk of the parent feature (this feature) are valid (not null) 

**Parameters:**


* `parentPrimariesAvailable` The status if the parent pks are available 



**See also:** parentPrimariesAvailable 



        

<hr>



### function setRelation 

```C++
void ReferencingFeatureListModelBase::setRelation (
    const QgsRelation & relation
) 
```



The relation connecting the parent feature with the children in this model 

**Parameters:**


* `relation` 



**See also:** relation 



        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/referencingfeaturelistmodel.h`

