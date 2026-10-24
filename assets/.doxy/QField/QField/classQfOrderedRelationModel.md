

# Class QfOrderedRelationModel



[**ClassList**](annotated.md) **>** [**QfOrderedRelationModel**](classQfOrderedRelationModel.md)








Inherits the following classes: [QfReferencingFeatureListModelBase](classQfReferencingFeatureListModelBase.md)














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Roles**](#enum-roles)  <br> |


## Public Types inherited from QfReferencingFeatureListModelBase

See [QfReferencingFeatureListModelBase](classQfReferencingFeatureListModelBase.md)

| Type | Name |
| ---: | :--- |
| enum  | [**ReferencedFeatureListRoles**](classQfReferencingFeatureListModelBase.md#enum-referencedfeaturelistroles)  <br> |






















## Public Properties

| Type | Name |
| ---: | :--- |
| property QString | [**description**](classQfOrderedRelationModel.md#property-description-12)  <br> |
| property QString | [**imagePath**](classQfOrderedRelationModel.md#property-imagepath-12)  <br> |
| property QString | [**orderingField**](classQfOrderedRelationModel.md#property-orderingfield-12)  <br> |


## Public Properties inherited from QfReferencingFeatureListModelBase

See [QfReferencingFeatureListModelBase](classQfReferencingFeatureListModelBase.md)

| Type | Name |
| ---: | :--- |
| property int | [**attachmentDocumentViewer**](classQfReferencingFeatureListModelBase.md#property-attachmentdocumentviewer-12)  <br> |
| property QString | [**attachmentFieldName**](classQfReferencingFeatureListModelBase.md#property-attachmentfieldname-12)  <br> |
| property QString | [**attachmentStorageAuthConfigId**](classQfReferencingFeatureListModelBase.md#property-attachmentstorageauthconfigid-12)  <br> |
| property QString | [**attachmentStorageType**](classQfReferencingFeatureListModelBase.md#property-attachmentstoragetype-12)  <br> |
| property QString | [**attachmentStorageUrl**](classQfReferencingFeatureListModelBase.md#property-attachmentstorageurl-12)  <br> |
| property QString | [**currentNmRelationId**](classQfReferencingFeatureListModelBase.md#property-currentnmrelationid-12)  <br> |
| property QString | [**currentRelationId**](classQfReferencingFeatureListModelBase.md#property-currentrelationid-12)  <br> |
| property QgsFeature | [**feature**](classQfReferencingFeatureListModelBase.md#property-feature-12)  <br> |
| property bool | [**isLoading**](classQfReferencingFeatureListModelBase.md#property-isloading-12)  <br> |
| property QgsRelation | [**nmRelation**](classQfReferencingFeatureListModelBase.md#property-nmrelation-12)  <br> |
| property bool | [**parentPrimariesAvailable**](classQfReferencingFeatureListModelBase.md#property-parentprimariesavailable-12)  <br> |
| property QgsRelation | [**relation**](classQfReferencingFeatureListModelBase.md#property-relation-12)  <br> |






## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**currentFeatureChanged**](classQfOrderedRelationModel.md#signal-currentfeaturechanged) (QgsFeature feature) <br> |
| signal void | [**descriptionChanged**](classQfOrderedRelationModel.md#signal-descriptionchanged)  <br> |
| signal void | [**failedReorder**](classQfOrderedRelationModel.md#signal-failedreorder)  <br> |
| signal void | [**imagePathChanged**](classQfOrderedRelationModel.md#signal-imagepathchanged)  <br> |
| signal void | [**orderingFieldChanged**](classQfOrderedRelationModel.md#signal-orderingfieldchanged)  <br> |


## Public Signals inherited from QfReferencingFeatureListModelBase

See [QfReferencingFeatureListModelBase](classQfReferencingFeatureListModelBase.md)

| Type | Name |
| ---: | :--- |
| signal void | [**attachmentDetailsChanged**](classQfReferencingFeatureListModelBase.md#signal-attachmentdetailschanged)  <br> |
| signal void | [**attributeFormModelChanged**](classQfReferencingFeatureListModelBase.md#signal-attributeformmodelchanged)  <br> |
| signal void | [**beforeModelUpdated**](classQfReferencingFeatureListModelBase.md#signal-beforemodelupdated)  <br> |
| signal void | [**featureChanged**](classQfReferencingFeatureListModelBase.md#signal-featurechanged)  <br> |
| signal void | [**isLoadingChanged**](classQfReferencingFeatureListModelBase.md#signal-isloadingchanged)  <br> |
| signal void | [**modelUpdated**](classQfReferencingFeatureListModelBase.md#signal-modelupdated)  <br> |
| signal void | [**nmRelationChanged**](classQfReferencingFeatureListModelBase.md#signal-nmrelationchanged)  <br> |
| signal void | [**parentPrimariesAvailableChanged**](classQfReferencingFeatureListModelBase.md#signal-parentprimariesavailablechanged)  <br> |
| signal void | [**relationChanged**](classQfReferencingFeatureListModelBase.md#signal-relationchanged)  <br> |






## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfOrderedRelationModel**](#function-qforderedrelationmodel) (QObject \* parent=nullptr) <br> |
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


## Public Functions inherited from QfReferencingFeatureListModelBase

See [QfReferencingFeatureListModelBase](classQfReferencingFeatureListModelBase.md)

| Type | Name |
| ---: | :--- |
|   | [**QfReferencingFeatureListModelBase**](classQfReferencingFeatureListModelBase.md#function-qfreferencingfeaturelistmodelbase) (QObject \* parent=nullptr) <br> |
|  int | [**attachmentDocumentViewer**](classQfReferencingFeatureListModelBase.md#function-attachmentdocumentviewer-22) () const<br> |
|  QString | [**attachmentFieldName**](classQfReferencingFeatureListModelBase.md#function-attachmentfieldname-22) () const<br> |
|  QString | [**attachmentStorageAuthConfigId**](classQfReferencingFeatureListModelBase.md#function-attachmentstorageauthconfigid-22) () const<br> |
|  QString | [**attachmentStorageType**](classQfReferencingFeatureListModelBase.md#function-attachmentstoragetype-22) () const<br> |
|  QString | [**attachmentStorageUrl**](classQfReferencingFeatureListModelBase.md#function-attachmentstorageurl-22) () const<br> |
|  int | [**columnCount**](classQfReferencingFeatureListModelBase.md#function-columncount) (const QModelIndex & parent=QModelIndex()) override const<br> |
|  QString | [**currentNmRelationId**](classQfReferencingFeatureListModelBase.md#function-currentnmrelationid-22) () const<br>_On many-to-many relations returns the second relation id connecting the children in the association table to their other parent._  |
|  QString | [**currentRelationId**](classQfReferencingFeatureListModelBase.md#function-currentrelationid-22) () const<br>_Returns the id of the relation connecting the parent feature with the children in this model._  |
|  QVariant | [**data**](classQfReferencingFeatureListModelBase.md#function-data) (const QModelIndex & index, int role=Qt::DisplayRole) override const<br> |
|  Q\_INVOKABLE bool | [**deleteFeature**](classQfReferencingFeatureListModelBase.md#function-deletefeature) (QgsFeatureId referencingFeatureId) <br> |
|  QgsFeature | [**feature**](classQfReferencingFeatureListModelBase.md#function-feature-22) () const<br> |
|  Q\_INVOKABLE int | [**getFeatureIdRow**](classQfReferencingFeatureListModelBase.md#function-getfeatureidrow) (QgsFeatureId featureId) <br> |
|  QModelIndex | [**index**](classQfReferencingFeatureListModelBase.md#function-index) (int row, int column, const QModelIndex & parent=QModelIndex()) override const<br> |
|  bool | [**isLoading**](classQfReferencingFeatureListModelBase.md#function-isloading-22) () const<br> |
|  QgsRelation | [**nmRelation**](classQfReferencingFeatureListModelBase.md#function-nmrelation-22) () const<br> |
|  QModelIndex | [**parent**](classQfReferencingFeatureListModelBase.md#function-parent) (const QModelIndex & index) override const<br> |
|  bool | [**parentPrimariesAvailable**](classQfReferencingFeatureListModelBase.md#function-parentprimariesavailable-22) () const<br> |
|  QgsRelation | [**relation**](classQfReferencingFeatureListModelBase.md#function-relation-22) () const<br> |
|  Q\_INVOKABLE void | [**reload**](classQfReferencingFeatureListModelBase.md#function-reload) () <br> |
|  QHash&lt; int, QByteArray &gt; | [**roleNames**](classQfReferencingFeatureListModelBase.md#function-rolenames) () override const<br> |
|  int | [**rowCount**](classQfReferencingFeatureListModelBase.md#function-rowcount) (const QModelIndex & parent=QModelIndex()) override const<br> |
|  void | [**setCurrentNmRelationId**](classQfReferencingFeatureListModelBase.md#function-setcurrentnmrelationid) (const QString & nmRelationId) <br>_On many-to-many relations sets the second relation connecting the children in the association table to their other parent._  |
|  void | [**setCurrentRelationId**](classQfReferencingFeatureListModelBase.md#function-setcurrentrelationid) (const QString & relationId) <br>_Sets the relation connecting the parent feature with the children in this model._  |
|  void | [**setFeature**](classQfReferencingFeatureListModelBase.md#function-setfeature) (const QgsFeature & feature) <br> |
|  void | [**setNmRelation**](classQfReferencingFeatureListModelBase.md#function-setnmrelation) (const QgsRelation & relation) <br> |
|  void | [**setParentPrimariesAvailable**](classQfReferencingFeatureListModelBase.md#function-setparentprimariesavailable) (const bool parentPrimariesAvailable) <br> |
|  void | [**setRelation**](classQfReferencingFeatureListModelBase.md#function-setrelation) (const QgsRelation & relation) <br> |






















































## Public Types Documentation




### enum Roles 

```C++
enum QfOrderedRelationModel::Roles {
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
QString QfOrderedRelationModel::description;
```




<hr>



### property imagePath [1/2]

```C++
QString QfOrderedRelationModel::imagePath;
```




<hr>



### property orderingField [1/2]

```C++
QString QfOrderedRelationModel::orderingField;
```




<hr>
## Public Signals Documentation




### signal currentFeatureChanged 

```C++
void QfOrderedRelationModel::currentFeatureChanged;
```




<hr>



### signal descriptionChanged 

```C++
void QfOrderedRelationModel::descriptionChanged;
```




<hr>



### signal failedReorder 

```C++
void QfOrderedRelationModel::failedReorder;
```




<hr>



### signal imagePathChanged 

```C++
void QfOrderedRelationModel::imagePathChanged;
```




<hr>



### signal orderingFieldChanged 

```C++
void QfOrderedRelationModel::orderingFieldChanged;
```




<hr>
## Public Functions Documentation




### function QfOrderedRelationModel 

```C++
explicit QfOrderedRelationModel::QfOrderedRelationModel (
    QObject * parent=nullptr
) 
```




<hr>



### function data 

```C++
QVariant QfOrderedRelationModel::data (
    const QModelIndex & index,
    int role=Qt::DisplayRole
) override const
```




<hr>



### function description [2/2]

```C++
QString QfOrderedRelationModel::description () const
```




<hr>



### function imagePath [2/2]

```C++
QString QfOrderedRelationModel::imagePath () const
```




<hr>



### function moveItems 

```C++
Q_INVOKABLE bool QfOrderedRelationModel::moveItems (
    const int fromIdx,
    const int toIdx
) 
```




<hr>



### function orderingField [2/2]

```C++
QString QfOrderedRelationModel::orderingField () const
```




<hr>



### function roleNames 

```C++
QHash< int, QByteArray > QfOrderedRelationModel::roleNames () override const
```




<hr>



### function setDescription 

```C++
void QfOrderedRelationModel::setDescription (
    const QString & description
) 
```




<hr>



### function setImagePath 

```C++
void QfOrderedRelationModel::setImagePath (
    const QString & imagePath
) 
```




<hr>



### function setOrderingField 

```C++
void QfOrderedRelationModel::setOrderingField (
    const QString & orderingField
) 
```




<hr>



### function triggerViewCurrentFeatureChange 

```C++
Q_INVOKABLE void QfOrderedRelationModel::triggerViewCurrentFeatureChange (
    int index
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/gui/qforderedrelationmodel.h`

