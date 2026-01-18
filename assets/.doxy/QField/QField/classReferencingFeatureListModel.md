

# Class ReferencingFeatureListModel



[**ClassList**](annotated.md) **>** [**ReferencingFeatureListModel**](classReferencingFeatureListModel.md)








Inherits the following classes: QSortFilterProxyModel


























## Public Properties

| Type | Name |
| ---: | :--- |
| property QString | [**currentNmRelationId**](classReferencingFeatureListModel.md#property-currentnmrelationid-12)  <br> |
| property QString | [**currentRelationId**](classReferencingFeatureListModel.md#property-currentrelationid-12)  <br> |
| property QgsFeature | [**feature**](classReferencingFeatureListModel.md#property-feature-12)  <br> |
| property bool | [**isLoading**](classReferencingFeatureListModel.md#property-isloading-12)  <br> |
| property QgsRelation | [**nmRelation**](classReferencingFeatureListModel.md#property-nmrelation-12)  <br> |
| property bool | [**parentPrimariesAvailable**](classReferencingFeatureListModel.md#property-parentprimariesavailable-12)  <br> |
| property QgsRelation | [**relation**](classReferencingFeatureListModel.md#property-relation-12)  <br> |
| property Qt::SortOrder | [**sortOrder**](classReferencingFeatureListModel.md#property-sortorder-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**attributeFormModelChanged**](classReferencingFeatureListModel.md#signal-attributeformmodelchanged)  <br> |
| signal void | [**beforeModelUpdated**](classReferencingFeatureListModel.md#signal-beforemodelupdated)  <br> |
| signal void | [**featureChanged**](classReferencingFeatureListModel.md#signal-featurechanged)  <br> |
| signal void | [**isLoadingChanged**](classReferencingFeatureListModel.md#signal-isloadingchanged)  <br> |
| signal void | [**modelUpdated**](classReferencingFeatureListModel.md#signal-modelupdated)  <br> |
| signal void | [**nmRelationChanged**](classReferencingFeatureListModel.md#signal-nmrelationchanged)  <br> |
| signal void | [**parentPrimariesAvailableChanged**](classReferencingFeatureListModel.md#signal-parentprimariesavailablechanged)  <br> |
| signal void | [**relationChanged**](classReferencingFeatureListModel.md#signal-relationchanged)  <br> |
| signal void | [**sortOrderChanged**](classReferencingFeatureListModel.md#signal-sortorderchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ReferencingFeatureListModel**](#function-referencingfeaturelistmodel) (QObject \* parent=nullptr) <br> |
|  QString | [**currentNmRelationId**](#function-currentnmrelationid-22) () const<br>_On many-to-many relations returns the second relation id connecting the children in the association table to their other parent._  |
|  QString | [**currentRelationId**](#function-currentrelationid-22) () const<br>_Returns the id of the relation connecting the parent feature with the children in this model._  |
|  Q\_INVOKABLE bool | [**deleteFeature**](#function-deletefeature) (QgsFeatureId referencingFeatureId) <br> |
|  QgsFeature | [**feature**](#function-feature-22) () const<br> |
|  Q\_INVOKABLE int | [**getFeatureIdRow**](#function-getfeatureidrow) (QgsFeatureId featureId) <br> |
|  bool | [**isLoading**](#function-isloading-22) () const<br> |
|  QgsRelation | [**nmRelation**](#function-nmrelation-22) () const<br> |
|  bool | [**parentPrimariesAvailable**](#function-parentprimariesavailable-22) () const<br> |
|  QgsRelation | [**relation**](#function-relation-22) () const<br> |
|  Q\_INVOKABLE void | [**reload**](#function-reload) () <br> |
|  void | [**setCurrentNmRelationId**](#function-setcurrentnmrelationid) (const QString & nmRelationId) <br>_On many-to-many relations sets the second relation connecting the children in the association table to their other parent._  |
|  void | [**setCurrentRelationId**](#function-setcurrentrelationid) (const QString & relationId) <br>_Sets the relation connecting the parent feature with the children in this model._  |
|  void | [**setFeature**](#function-setfeature) (const QgsFeature & feature) <br> |
|  void | [**setNmRelation**](#function-setnmrelation) (const QgsRelation & relation) <br> |
|  void | [**setParentPrimariesAvailable**](#function-setparentprimariesavailable) (const bool parentPrimariesAvailable) <br> |
|  void | [**setRelation**](#function-setrelation) (const QgsRelation & relation) <br> |
|  void | [**setSortOrder**](#function-setsortorder) (Qt::SortOrder sortOrder) <br>_Sets the sort order and re-applies sorting._  |
|  Qt::SortOrder | [**sortOrder**](#function-sortorder-22) () const<br>_Returns the current sort order (ascending or descending)._  |




























## Public Properties Documentation




### property currentNmRelationId [1/2]

```C++
QString ReferencingFeatureListModel::currentNmRelationId;
```




<hr>



### property currentRelationId [1/2]

```C++
QString ReferencingFeatureListModel::currentRelationId;
```




<hr>



### property feature [1/2]

```C++
QgsFeature ReferencingFeatureListModel::feature;
```




<hr>



### property isLoading [1/2]

```C++
bool ReferencingFeatureListModel::isLoading;
```




<hr>



### property nmRelation [1/2]

```C++
QgsRelation ReferencingFeatureListModel::nmRelation;
```




<hr>



### property parentPrimariesAvailable [1/2]

```C++
bool ReferencingFeatureListModel::parentPrimariesAvailable;
```




<hr>



### property relation [1/2]

```C++
QgsRelation ReferencingFeatureListModel::relation;
```




<hr>



### property sortOrder [1/2]

```C++
Qt::SortOrder ReferencingFeatureListModel::sortOrder;
```




<hr>
## Public Signals Documentation




### signal attributeFormModelChanged 

```C++
void ReferencingFeatureListModel::attributeFormModelChanged;
```




<hr>



### signal beforeModelUpdated 

```C++
void ReferencingFeatureListModel::beforeModelUpdated;
```




<hr>



### signal featureChanged 

```C++
void ReferencingFeatureListModel::featureChanged;
```




<hr>



### signal isLoadingChanged 

```C++
void ReferencingFeatureListModel::isLoadingChanged;
```




<hr>



### signal modelUpdated 

```C++
void ReferencingFeatureListModel::modelUpdated;
```




<hr>



### signal nmRelationChanged 

```C++
void ReferencingFeatureListModel::nmRelationChanged;
```




<hr>



### signal parentPrimariesAvailableChanged 

```C++
void ReferencingFeatureListModel::parentPrimariesAvailableChanged;
```




<hr>



### signal relationChanged 

```C++
void ReferencingFeatureListModel::relationChanged;
```




<hr>



### signal sortOrderChanged 

```C++
void ReferencingFeatureListModel::sortOrderChanged;
```




<hr>
## Public Functions Documentation




### function ReferencingFeatureListModel 

```C++
explicit ReferencingFeatureListModel::ReferencingFeatureListModel (
    QObject * parent=nullptr
) 
```




<hr>



### function currentNmRelationId [2/2]

_On many-to-many relations returns the second relation id connecting the children in the association table to their other parent._ 
```C++
QString ReferencingFeatureListModel::currentNmRelationId () const
```




<hr>



### function currentRelationId [2/2]

_Returns the id of the relation connecting the parent feature with the children in this model._ 
```C++
QString ReferencingFeatureListModel::currentRelationId () const
```




<hr>



### function deleteFeature 

```C++
Q_INVOKABLE bool ReferencingFeatureListModel::deleteFeature (
    QgsFeatureId referencingFeatureId
) 
```



Deletes a feature regarding the referencing layer and the feature id of the selected child 

**Parameters:**


* `referencingFeatureId` id of the selected child 




        

<hr>



### function feature [2/2]

```C++
QgsFeature ReferencingFeatureListModel::feature () const
```



The parent feature for which this model contains the children 

**Returns:**

the parent feature 




**See also:** [**setFeature**](classReferencingFeatureListModel.md#function-setfeature) 



        

<hr>



### function getFeatureIdRow 

```C++
Q_INVOKABLE int ReferencingFeatureListModel::getFeatureIdRow (
    QgsFeatureId featureId
) 
```



Returns the row number for a given feature id 

**Parameters:**


* `featureId` the feature id 




        

<hr>



### function isLoading [2/2]

```C++
bool ReferencingFeatureListModel::isLoading () const
```



Indicator if the model is currently performing any feature iteration in the background. 


        

<hr>



### function nmRelation [2/2]

```C++
QgsRelation ReferencingFeatureListModel::nmRelation () const
```



On many-to-many relations this is the second relation connecting the children in the associationtable to their other parent 

**Returns:**

associated relation 




**See also:** [**setNmRelation**](classReferencingFeatureListModel.md#function-setnmrelation) 



        

<hr>



### function parentPrimariesAvailable [2/2]

```C++
bool ReferencingFeatureListModel::parentPrimariesAvailable () const
```



The status if the pk of the parent feature (this feature) are valid (not null) It's needed to check on opening a form to add a new child 

**Returns:**

parentPrimariesAvailable The status if the parent pks are available 




**See also:** [**setParentPrimariesAvailable**](classReferencingFeatureListModel.md#function-setparentprimariesavailable) 



        

<hr>



### function relation [2/2]

```C++
QgsRelation ReferencingFeatureListModel::relation () const
```



The relation connecting the parent feature with the children in this model 

**Returns:**

relation 




**See also:** [**setRelation**](classReferencingFeatureListModel.md#function-setrelation) 



        

<hr>



### function reload 

```C++
Q_INVOKABLE void ReferencingFeatureListModel::reload () 
```



Reloads the model by starting the reload functionality in the gatherer (seperate thread) Sets the property parentPrimariesAvailable 


        

<hr>



### function setCurrentNmRelationId 

_On many-to-many relations sets the second relation connecting the children in the association table to their other parent._ 
```C++
void ReferencingFeatureListModel::setCurrentNmRelationId (
    const QString & nmRelationId
) 
```




<hr>



### function setCurrentRelationId 

_Sets the relation connecting the parent feature with the children in this model._ 
```C++
void ReferencingFeatureListModel::setCurrentRelationId (
    const QString & relationId
) 
```




<hr>



### function setFeature 

```C++
void ReferencingFeatureListModel::setFeature (
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
void ReferencingFeatureListModel::setNmRelation (
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
void ReferencingFeatureListModel::setParentPrimariesAvailable (
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
void ReferencingFeatureListModel::setRelation (
    const QgsRelation & relation
) 
```



The relation connecting the parent feature with the children in this model 

**Parameters:**


* `relation` 



**See also:** relation 



        

<hr>



### function setSortOrder 

_Sets the sort order and re-applies sorting._ 
```C++
void ReferencingFeatureListModel::setSortOrder (
    Qt::SortOrder sortOrder
) 
```





**Parameters:**


* `sortOrder` The new sort order to use. 




        

<hr>



### function sortOrder [2/2]

_Returns the current sort order (ascending or descending)._ 
```C++
Qt::SortOrder ReferencingFeatureListModel::sortOrder () const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/referencingfeaturelistmodel.h`

