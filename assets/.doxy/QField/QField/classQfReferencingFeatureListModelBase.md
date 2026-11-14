

# Class QfReferencingFeatureListModelBase



[**ClassList**](annotated.md) **>** [**QfReferencingFeatureListModelBase**](classQfReferencingFeatureListModelBase.md)








Inherits the following classes: QAbstractItemModel


Inherited by the following classes: [QfOrderedRelationModel](classQfOrderedRelationModel.md)












## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**ReferencedFeatureListRoles**](#enum-referencedfeaturelistroles)  <br> |












## Public Properties

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
|   | [**QfReferencingFeatureListModelBase**](#function-qfreferencingfeaturelistmodelbase) (QObject \* parent=nullptr) <br> |
|  int | [**attachmentDocumentViewer**](#function-attachmentdocumentviewer-22) () const<br> |
|  QString | [**attachmentFieldName**](#function-attachmentfieldname-22) () const<br> |
|  QString | [**attachmentStorageAuthConfigId**](#function-attachmentstorageauthconfigid-22) () const<br> |
|  QString | [**attachmentStorageType**](#function-attachmentstoragetype-22) () const<br> |
|  QString | [**attachmentStorageUrl**](#function-attachmentstorageurl-22) () const<br> |
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
enum QfReferencingFeatureListModelBase::ReferencedFeatureListRoles {
    DisplayString = Qt::UserRole,
    ReferencingFeature,
    NmReferencedFeature,
    NmDisplayString,
    AttachmentPath
};
```




<hr>
## Public Properties Documentation




### property attachmentDocumentViewer [1/2]

```C++
int QfReferencingFeatureListModelBase::attachmentDocumentViewer;
```




<hr>



### property attachmentFieldName [1/2]

```C++
QString QfReferencingFeatureListModelBase::attachmentFieldName;
```




<hr>



### property attachmentStorageAuthConfigId [1/2]

```C++
QString QfReferencingFeatureListModelBase::attachmentStorageAuthConfigId;
```




<hr>



### property attachmentStorageType [1/2]

```C++
QString QfReferencingFeatureListModelBase::attachmentStorageType;
```




<hr>



### property attachmentStorageUrl [1/2]

```C++
QString QfReferencingFeatureListModelBase::attachmentStorageUrl;
```




<hr>



### property currentNmRelationId [1/2]

```C++
QString QfReferencingFeatureListModelBase::currentNmRelationId;
```




<hr>



### property currentRelationId [1/2]

```C++
QString QfReferencingFeatureListModelBase::currentRelationId;
```




<hr>



### property feature [1/2]

```C++
QgsFeature QfReferencingFeatureListModelBase::feature;
```




<hr>



### property isLoading [1/2]

```C++
bool QfReferencingFeatureListModelBase::isLoading;
```




<hr>



### property nmRelation [1/2]

```C++
QgsRelation QfReferencingFeatureListModelBase::nmRelation;
```




<hr>



### property parentPrimariesAvailable [1/2]

```C++
bool QfReferencingFeatureListModelBase::parentPrimariesAvailable;
```




<hr>



### property relation [1/2]

```C++
QgsRelation QfReferencingFeatureListModelBase::relation;
```




<hr>
## Public Signals Documentation




### signal attachmentDetailsChanged 

```C++
void QfReferencingFeatureListModelBase::attachmentDetailsChanged;
```




<hr>



### signal attributeFormModelChanged 

```C++
void QfReferencingFeatureListModelBase::attributeFormModelChanged;
```




<hr>



### signal beforeModelUpdated 

```C++
void QfReferencingFeatureListModelBase::beforeModelUpdated;
```




<hr>



### signal featureChanged 

```C++
void QfReferencingFeatureListModelBase::featureChanged;
```




<hr>



### signal isLoadingChanged 

```C++
void QfReferencingFeatureListModelBase::isLoadingChanged;
```




<hr>



### signal modelUpdated 

```C++
void QfReferencingFeatureListModelBase::modelUpdated;
```




<hr>



### signal nmRelationChanged 

```C++
void QfReferencingFeatureListModelBase::nmRelationChanged;
```




<hr>



### signal parentPrimariesAvailableChanged 

```C++
void QfReferencingFeatureListModelBase::parentPrimariesAvailableChanged;
```




<hr>



### signal relationChanged 

```C++
void QfReferencingFeatureListModelBase::relationChanged;
```




<hr>
## Public Functions Documentation




### function QfReferencingFeatureListModelBase 

```C++
explicit QfReferencingFeatureListModelBase::QfReferencingFeatureListModelBase (
    QObject * parent=nullptr
) 
```




<hr>



### function attachmentDocumentViewer [2/2]

```C++
int QfReferencingFeatureListModelBase::attachmentDocumentViewer () const
```



Returns the document viewer type of the first ExternalResource field found on the referencing layer. The value corresponds to the "DocumentViewer" configuration entry of the editor widget setup (0 = file, 1 = image, 3 = audio, 4 = video). Returns 0 if no ExternalResource field exists.




**See also:** QgsExternalResourceWidget::DocumentViewerContent in [https://github.com/qgis/QGIS/blob/6ca6cf1bab8e017355f7631115cf48bc3c6a4601/src/gui/qgsexternalresourcewidget.h#L72-L79](https://github.com/qgis/QGIS/blob/6ca6cf1bab8e017355f7631115cf48bc3c6a4601/src/gui/qgsexternalresourcewidget.h#L72-L79) 



        

<hr>



### function attachmentFieldName [2/2]

```C++
QString QfReferencingFeatureListModelBase::attachmentFieldName () const
```



Returns the field name of the first ExternalResource field found on the referencing layer, or an empty string if no such field exists. The value is cached when the relation is set. 


        

<hr>



### function attachmentStorageAuthConfigId [2/2]

```C++
QString QfReferencingFeatureListModelBase::attachmentStorageAuthConfigId () const
```



Returns the authentication configuration ID for the external storage configured on the attachment field, or an empty string if none is set. 


        

<hr>



### function attachmentStorageType [2/2]

```C++
QString QfReferencingFeatureListModelBase::attachmentStorageType () const
```



Returns the external storage type configured on the attachment field (e.g. "WebDAV"), or an empty string if none is set. 


        

<hr>



### function attachmentStorageUrl [2/2]

```C++
QString QfReferencingFeatureListModelBase::attachmentStorageUrl () const
```



Returns the external storage URL configured on the attachment field (e.g. "https://server.com/remote.php/dav/files/user/"), or an empty string if none is set. 


        

<hr>



### function columnCount 

```C++
int QfReferencingFeatureListModelBase::columnCount (
    const QModelIndex & parent=QModelIndex()
) override const
```




<hr>



### function currentNmRelationId [2/2]

_On many-to-many relations returns the second relation id connecting the children in the association table to their other parent._ 
```C++
QString QfReferencingFeatureListModelBase::currentNmRelationId () const
```




<hr>



### function currentRelationId [2/2]

_Returns the id of the relation connecting the parent feature with the children in this model._ 
```C++
QString QfReferencingFeatureListModelBase::currentRelationId () const
```




<hr>



### function data 

```C++
QVariant QfReferencingFeatureListModelBase::data (
    const QModelIndex & index,
    int role=Qt::DisplayRole
) override const
```




<hr>



### function deleteFeature 

```C++
Q_INVOKABLE bool QfReferencingFeatureListModelBase::deleteFeature (
    QgsFeatureId referencingFeatureId
) 
```



Deletes a feature regarding the referencing layer and the feature id of the selected child 

**Parameters:**


* `referencingFeatureId` id of the selected child 




        

<hr>



### function feature [2/2]

```C++
QgsFeature QfReferencingFeatureListModelBase::feature () const
```



The parent feature for which this model contains the children 

**Returns:**

the parent feature 




**See also:** [**setFeature**](classQfReferencingFeatureListModelBase.md#function-setfeature) 



        

<hr>



### function getFeatureIdRow 

```C++
Q_INVOKABLE int QfReferencingFeatureListModelBase::getFeatureIdRow (
    QgsFeatureId featureId
) 
```



Returns the row number for a given feature id 

**Parameters:**


* `featureId` the feature id 




        

<hr>



### function index 

```C++
QModelIndex QfReferencingFeatureListModelBase::index (
    int row,
    int column,
    const QModelIndex & parent=QModelIndex()
) override const
```




<hr>



### function isLoading [2/2]

```C++
bool QfReferencingFeatureListModelBase::isLoading () const
```



Indicator if the model is currently performing any feature iteration in the background. 


        

<hr>



### function nmRelation [2/2]

```C++
QgsRelation QfReferencingFeatureListModelBase::nmRelation () const
```



On many-to-many relations this is the second relation connecting the children in the associationtable to their other parent 

**Returns:**

associated relation 




**See also:** [**setNmRelation**](classQfReferencingFeatureListModelBase.md#function-setnmrelation) 



        

<hr>



### function parent 

```C++
QModelIndex QfReferencingFeatureListModelBase::parent (
    const QModelIndex & index
) override const
```




<hr>



### function parentPrimariesAvailable [2/2]

```C++
bool QfReferencingFeatureListModelBase::parentPrimariesAvailable () const
```



The status if the pk of the parent feature (this feature) are valid (not null) It's needed to check on opening a form to add a new child 

**Returns:**

parentPrimariesAvailable The status if the parent pks are available 




**See also:** [**setParentPrimariesAvailable**](classQfReferencingFeatureListModelBase.md#function-setparentprimariesavailable) 



        

<hr>



### function relation [2/2]

```C++
QgsRelation QfReferencingFeatureListModelBase::relation () const
```



The relation connecting the parent feature with the children in this model 

**Returns:**

relation 




**See also:** [**setRelation**](classQfReferencingFeatureListModelBase.md#function-setrelation) 



        

<hr>



### function reload 

```C++
Q_INVOKABLE void QfReferencingFeatureListModelBase::reload () 
```



Reloads the model by starting the reload functionality in the gatherer (seperate thread) Sets the property parentPrimariesAvailable 


        

<hr>



### function roleNames 

```C++
QHash< int, QByteArray > QfReferencingFeatureListModelBase::roleNames () override const
```




<hr>



### function rowCount 

```C++
int QfReferencingFeatureListModelBase::rowCount (
    const QModelIndex & parent=QModelIndex()
) override const
```




<hr>



### function setCurrentNmRelationId 

_On many-to-many relations sets the second relation connecting the children in the association table to their other parent._ 
```C++
void QfReferencingFeatureListModelBase::setCurrentNmRelationId (
    const QString & nmRelationId
) 
```




<hr>



### function setCurrentRelationId 

_Sets the relation connecting the parent feature with the children in this model._ 
```C++
void QfReferencingFeatureListModelBase::setCurrentRelationId (
    const QString & relationId
) 
```




<hr>



### function setFeature 

```C++
void QfReferencingFeatureListModelBase::setFeature (
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
void QfReferencingFeatureListModelBase::setNmRelation (
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
void QfReferencingFeatureListModelBase::setParentPrimariesAvailable (
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
void QfReferencingFeatureListModelBase::setRelation (
    const QgsRelation & relation
) 
```



The relation connecting the parent feature with the children in this model 

**Parameters:**


* `relation` 



**See also:** relation 



        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/gui/qfreferencingfeaturelistmodel.h`

