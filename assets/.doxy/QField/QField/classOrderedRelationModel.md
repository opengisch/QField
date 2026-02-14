

# Class OrderedRelationModel



[**ClassList**](annotated.md) **>** [**OrderedRelationModel**](classOrderedRelationModel.md)








Inherits the following classes: [ReferencingFeatureListModelBase](classReferencingFeatureListModelBase.md)














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Roles**](#enum-roles)  <br> |


## Public Types inherited from ReferencingFeatureListModelBase

See [ReferencingFeatureListModelBase](classReferencingFeatureListModelBase.md)

| Type | Name |
| ---: | :--- |
| enum  | [**ReferencedFeatureListRoles**](classReferencingFeatureListModelBase.md#enum-referencedfeaturelistroles)  <br> |






















## Public Properties

| Type | Name |
| ---: | :--- |
| property QString | [**description**](classOrderedRelationModel.md#property-description-12)  <br> |
| property QString | [**imagePath**](classOrderedRelationModel.md#property-imagepath-12)  <br> |
| property QString | [**orderingField**](classOrderedRelationModel.md#property-orderingfield-12)  <br> |


## Public Properties inherited from ReferencingFeatureListModelBase

See [ReferencingFeatureListModelBase](classReferencingFeatureListModelBase.md)

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
| signal void | [**currentFeatureChanged**](classOrderedRelationModel.md#signal-currentfeaturechanged) (QgsFeature feature) <br> |
| signal void | [**descriptionChanged**](classOrderedRelationModel.md#signal-descriptionchanged)  <br> |
| signal void | [**failedReorder**](classOrderedRelationModel.md#signal-failedreorder)  <br> |
| signal void | [**imagePathChanged**](classOrderedRelationModel.md#signal-imagepathchanged)  <br> |
| signal void | [**orderingFieldChanged**](classOrderedRelationModel.md#signal-orderingfieldchanged)  <br> |


## Public Signals inherited from ReferencingFeatureListModelBase

See [ReferencingFeatureListModelBase](classReferencingFeatureListModelBase.md)

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
|   | [**OrderedRelationModel**](#function-orderedrelationmodel) (QObject \* parent=nullptr) <br> |
|  QVariant | [**data**](#function-data) (const QModelIndex & index, int role=Qt::DisplayRole) override const<br> |
|  QString | [**description**](#function-description-22) () const<br> |
|  QString | [**imagePath**](#function-imagepath-22) () const<br> |
|  Q\_INVOKABLE bool | [**moveItems**](#function-moveitems) (const int fromIdx, const int toIdx) <br> |
|  QString | [**orderingField**](#function-orderingfield-22) () const<br> |
|  QHash&lt; int, QByteArray &gt; | [**roleNames**](#function-rolenames) () override const<br> |
|  void | [**setDescription**](#function-setdescription) (const QString & description) <br> |
|  void | [**setImagePath**](#function-setimagepath) (const QString & imagePath) <br> |
|  void | [**setOrderingField**](#function-setorderingfield) (const QString & orderingField) <br> |
|  Q\_INVOKABLE void | [**triggerViewCurrentFeatureChange**](#function-triggerviewcurrentfeaturechange) (int index) <br> |


## Public Functions inherited from ReferencingFeatureListModelBase

See [ReferencingFeatureListModelBase](classReferencingFeatureListModelBase.md)

| Type | Name |
| ---: | :--- |
|   | [**ReferencingFeatureListModelBase**](classReferencingFeatureListModelBase.md#function-referencingfeaturelistmodelbase) (QObject \* parent=nullptr) <br> |
|  int | [**columnCount**](classReferencingFeatureListModelBase.md#function-columncount) (const QModelIndex & parent=QModelIndex()) override const<br> |
|  QString | [**currentNmRelationId**](classReferencingFeatureListModelBase.md#function-currentnmrelationid-22) () const<br>_On many-to-many relations returns the second relation id connecting the children in the association table to their other parent._  |
|  QString | [**currentRelationId**](classReferencingFeatureListModelBase.md#function-currentrelationid-22) () const<br>_Returns the id of the relation connecting the parent feature with the children in this model._  |
|  QVariant | [**data**](classReferencingFeatureListModelBase.md#function-data) (const QModelIndex & index, int role=Qt::DisplayRole) override const<br> |
|  Q\_INVOKABLE bool | [**deleteFeature**](classReferencingFeatureListModelBase.md#function-deletefeature) (QgsFeatureId referencingFeatureId) <br> |
|  QgsFeature | [**feature**](classReferencingFeatureListModelBase.md#function-feature-22) () const<br> |
|  Q\_INVOKABLE int | [**getFeatureIdRow**](classReferencingFeatureListModelBase.md#function-getfeatureidrow) (QgsFeatureId featureId) <br> |
|  QModelIndex | [**index**](classReferencingFeatureListModelBase.md#function-index) (int row, int column, const QModelIndex & parent=QModelIndex()) override const<br> |
|  bool | [**isLoading**](classReferencingFeatureListModelBase.md#function-isloading-22) () const<br> |
|  QgsRelation | [**nmRelation**](classReferencingFeatureListModelBase.md#function-nmrelation-22) () const<br> |
|  QModelIndex | [**parent**](classReferencingFeatureListModelBase.md#function-parent) (const QModelIndex & index) override const<br> |
|  bool | [**parentPrimariesAvailable**](classReferencingFeatureListModelBase.md#function-parentprimariesavailable-22) () const<br> |
|  QgsRelation | [**relation**](classReferencingFeatureListModelBase.md#function-relation-22) () const<br> |
|  Q\_INVOKABLE void | [**reload**](classReferencingFeatureListModelBase.md#function-reload) () <br> |
|  QHash&lt; int, QByteArray &gt; | [**roleNames**](classReferencingFeatureListModelBase.md#function-rolenames) () override const<br> |
|  int | [**rowCount**](classReferencingFeatureListModelBase.md#function-rowcount) (const QModelIndex & parent=QModelIndex()) override const<br> |
|  void | [**setCurrentNmRelationId**](classReferencingFeatureListModelBase.md#function-setcurrentnmrelationid) (const QString & nmRelationId) <br>_On many-to-many relations sets the second relation connecting the children in the association table to their other parent._  |
|  void | [**setCurrentRelationId**](classReferencingFeatureListModelBase.md#function-setcurrentrelationid) (const QString & relationId) <br>_Sets the relation connecting the parent feature with the children in this model._  |
|  void | [**setFeature**](classReferencingFeatureListModelBase.md#function-setfeature) (const QgsFeature & feature) <br> |
|  void | [**setNmRelation**](classReferencingFeatureListModelBase.md#function-setnmrelation) (const QgsRelation & relation) <br> |
|  void | [**setParentPrimariesAvailable**](classReferencingFeatureListModelBase.md#function-setparentprimariesavailable) (const bool parentPrimariesAvailable) <br> |
|  void | [**setRelation**](classReferencingFeatureListModelBase.md#function-setrelation) (const QgsRelation & relation) <br> |






















































## Public Types Documentation




### enum Roles 

```C++
enum OrderedRelationModel::Roles {
    ImagePathRole = Qt::UserRole + 100,
    DescriptionRole,
    FeatureIdRole,
    OrderingValueRole
};
```




<hr>
## Public Properties Documentation




### property description [1/2]

```C++
QString OrderedRelationModel::description;
```




<hr>



### property imagePath [1/2]

```C++
QString OrderedRelationModel::imagePath;
```




<hr>



### property orderingField [1/2]

```C++
QString OrderedRelationModel::orderingField;
```




<hr>
## Public Signals Documentation




### signal currentFeatureChanged 

```C++
void OrderedRelationModel::currentFeatureChanged;
```




<hr>



### signal descriptionChanged 

```C++
void OrderedRelationModel::descriptionChanged;
```




<hr>



### signal failedReorder 

```C++
void OrderedRelationModel::failedReorder;
```




<hr>



### signal imagePathChanged 

```C++
void OrderedRelationModel::imagePathChanged;
```




<hr>



### signal orderingFieldChanged 

```C++
void OrderedRelationModel::orderingFieldChanged;
```




<hr>
## Public Functions Documentation




### function OrderedRelationModel 

```C++
explicit OrderedRelationModel::OrderedRelationModel (
    QObject * parent=nullptr
) 
```




<hr>



### function data 

```C++
QVariant OrderedRelationModel::data (
    const QModelIndex & index,
    int role=Qt::DisplayRole
) override const
```




<hr>



### function description [2/2]

```C++
QString OrderedRelationModel::description () const
```




<hr>



### function imagePath [2/2]

```C++
QString OrderedRelationModel::imagePath () const
```




<hr>



### function moveItems 

```C++
Q_INVOKABLE bool OrderedRelationModel::moveItems (
    const int fromIdx,
    const int toIdx
) 
```




<hr>



### function orderingField [2/2]

```C++
QString OrderedRelationModel::orderingField () const
```




<hr>



### function roleNames 

```C++
QHash< int, QByteArray > OrderedRelationModel::roleNames () override const
```




<hr>



### function setDescription 

```C++
void OrderedRelationModel::setDescription (
    const QString & description
) 
```




<hr>



### function setImagePath 

```C++
void OrderedRelationModel::setImagePath (
    const QString & imagePath
) 
```




<hr>



### function setOrderingField 

```C++
void OrderedRelationModel::setOrderingField (
    const QString & orderingField
) 
```




<hr>



### function triggerViewCurrentFeatureChange 

```C++
Q_INVOKABLE void OrderedRelationModel::triggerViewCurrentFeatureChange (
    int index
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/orderedrelationmodel.h`

