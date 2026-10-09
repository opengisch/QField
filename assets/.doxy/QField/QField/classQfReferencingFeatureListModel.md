

# Class QfReferencingFeatureListModel



[**ClassList**](annotated.md) **>** [**QfReferencingFeatureListModel**](classQfReferencingFeatureListModel.md)








Inherits the following classes: QSortFilterProxyModel


























## Public Properties

| Type | Name |
| ---: | :--- |
| property int | [**attachmentDocumentViewer**](classQfReferencingFeatureListModel.md#property-attachmentdocumentviewer-12)  <br> |
| property QString | [**attachmentFieldName**](classQfReferencingFeatureListModel.md#property-attachmentfieldname-12)  <br> |
| property QString | [**attachmentStorageAuthConfigId**](classQfReferencingFeatureListModel.md#property-attachmentstorageauthconfigid-12)  <br> |
| property QString | [**attachmentStorageType**](classQfReferencingFeatureListModel.md#property-attachmentstoragetype-12)  <br> |
| property QString | [**attachmentStorageUrl**](classQfReferencingFeatureListModel.md#property-attachmentstorageurl-12)  <br> |
| property QString | [**currentNmRelationId**](classQfReferencingFeatureListModel.md#property-currentnmrelationid-12)  <br> |
| property QString | [**currentRelationId**](classQfReferencingFeatureListModel.md#property-currentrelationid-12)  <br> |
| property QgsFeature | [**feature**](classQfReferencingFeatureListModel.md#property-feature-12)  <br> |
| property bool | [**isLoading**](classQfReferencingFeatureListModel.md#property-isloading-12)  <br> |
| property QgsRelation | [**nmRelation**](classQfReferencingFeatureListModel.md#property-nmrelation-12)  <br> |
| property bool | [**parentPrimariesAvailable**](classQfReferencingFeatureListModel.md#property-parentprimariesavailable-12)  <br> |
| property QgsRelation | [**relation**](classQfReferencingFeatureListModel.md#property-relation-12)  <br> |
| property Qt::SortOrder | [**sortOrder**](classQfReferencingFeatureListModel.md#property-sortorder-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**attachmentDetailsChanged**](classQfReferencingFeatureListModel.md#signal-attachmentdetailschanged)  <br> |
| signal void | [**attributeFormModelChanged**](classQfReferencingFeatureListModel.md#signal-attributeformmodelchanged)  <br> |
| signal void | [**beforeModelUpdated**](classQfReferencingFeatureListModel.md#signal-beforemodelupdated)  <br> |
| signal void | [**featureChanged**](classQfReferencingFeatureListModel.md#signal-featurechanged)  <br> |
| signal void | [**isLoadingChanged**](classQfReferencingFeatureListModel.md#signal-isloadingchanged)  <br> |
| signal void | [**modelUpdated**](classQfReferencingFeatureListModel.md#signal-modelupdated)  <br> |
| signal void | [**nmRelationChanged**](classQfReferencingFeatureListModel.md#signal-nmrelationchanged)  <br> |
| signal void | [**parentPrimariesAvailableChanged**](classQfReferencingFeatureListModel.md#signal-parentprimariesavailablechanged)  <br> |
| signal void | [**relationChanged**](classQfReferencingFeatureListModel.md#signal-relationchanged)  <br> |
| signal void | [**sortOrderChanged**](classQfReferencingFeatureListModel.md#signal-sortorderchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfReferencingFeatureListModel**](#function-qfreferencingfeaturelistmodel) (QObject \* parent=nullptr) <br> |
|  int | [**attachmentDocumentViewer**](#function-attachmentdocumentviewer-22) () const<br> |
|  QString | [**attachmentFieldName**](#function-attachmentfieldname-22) () const<br> |
|  QString | [**attachmentStorageAuthConfigId**](#function-attachmentstorageauthconfigid-22) () const<br> |
|  QString | [**attachmentStorageType**](#function-attachmentstoragetype-22) () const<br> |
|  QString | [**attachmentStorageUrl**](#function-attachmentstorageurl-22) () const<br> |
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
























## Protected Functions

| Type | Name |
| ---: | :--- |
|  bool | [**lessThan**](#function-lessthan) (const QModelIndex & left, const QModelIndex & right) override const<br> |




## Public Properties Documentation




### property attachmentDocumentViewer [1/2]

```C++
int QfReferencingFeatureListModel::attachmentDocumentViewer;
```




<hr>



### property attachmentFieldName [1/2]

```C++
QString QfReferencingFeatureListModel::attachmentFieldName;
```




<hr>



### property attachmentStorageAuthConfigId [1/2]

```C++
QString QfReferencingFeatureListModel::attachmentStorageAuthConfigId;
```




<hr>



### property attachmentStorageType [1/2]

```C++
QString QfReferencingFeatureListModel::attachmentStorageType;
```




<hr>



### property attachmentStorageUrl [1/2]

```C++
QString QfReferencingFeatureListModel::attachmentStorageUrl;
```




<hr>



### property currentNmRelationId [1/2]

```C++
QString QfReferencingFeatureListModel::currentNmRelationId;
```




<hr>



### property currentRelationId [1/2]

```C++
QString QfReferencingFeatureListModel::currentRelationId;
```




<hr>



### property feature [1/2]

```C++
QgsFeature QfReferencingFeatureListModel::feature;
```




<hr>



### property isLoading [1/2]

```C++
bool QfReferencingFeatureListModel::isLoading;
```




<hr>



### property nmRelation [1/2]

```C++
QgsRelation QfReferencingFeatureListModel::nmRelation;
```




<hr>



### property parentPrimariesAvailable [1/2]

```C++
bool QfReferencingFeatureListModel::parentPrimariesAvailable;
```




<hr>



### property relation [1/2]

```C++
QgsRelation QfReferencingFeatureListModel::relation;
```




<hr>



### property sortOrder [1/2]

```C++
Qt::SortOrder QfReferencingFeatureListModel::sortOrder;
```




<hr>
## Public Signals Documentation




### signal attachmentDetailsChanged 

```C++
void QfReferencingFeatureListModel::attachmentDetailsChanged;
```




<hr>



### signal attributeFormModelChanged 

```C++
void QfReferencingFeatureListModel::attributeFormModelChanged;
```




<hr>



### signal beforeModelUpdated 

```C++
void QfReferencingFeatureListModel::beforeModelUpdated;
```




<hr>



### signal featureChanged 

```C++
void QfReferencingFeatureListModel::featureChanged;
```




<hr>



### signal isLoadingChanged 

```C++
void QfReferencingFeatureListModel::isLoadingChanged;
```




<hr>



### signal modelUpdated 

```C++
void QfReferencingFeatureListModel::modelUpdated;
```




<hr>



### signal nmRelationChanged 

```C++
void QfReferencingFeatureListModel::nmRelationChanged;
```




<hr>



### signal parentPrimariesAvailableChanged 

```C++
void QfReferencingFeatureListModel::parentPrimariesAvailableChanged;
```




<hr>



### signal relationChanged 

```C++
void QfReferencingFeatureListModel::relationChanged;
```




<hr>



### signal sortOrderChanged 

```C++
void QfReferencingFeatureListModel::sortOrderChanged;
```




<hr>
## Public Functions Documentation




### function QfReferencingFeatureListModel 

```C++
explicit QfReferencingFeatureListModel::QfReferencingFeatureListModel (
    QObject * parent=nullptr
) 
```




<hr>



### function attachmentDocumentViewer [2/2]

```C++
int QfReferencingFeatureListModel::attachmentDocumentViewer () const
```



Returns the document viewer type of the first ExternalResource field found on the referencing layer. The value corresponds to the "DocumentViewer" configuration entry of the editor widget setup (0 = file, 1 = image, 3 = audio, 4 = video). Returns 0 if no ExternalResource field exists.




**See also:** QgsExternalResourceWidget::DocumentViewerContent in [https://github.com/qgis/QGIS/blob/6ca6cf1bab8e017355f7631115cf48bc3c6a4601/src/gui/qgsexternalresourcewidget.h#L72-L79](https://github.com/qgis/QGIS/blob/6ca6cf1bab8e017355f7631115cf48bc3c6a4601/src/gui/qgsexternalresourcewidget.h#L72-L79) 



        

<hr>



### function attachmentFieldName [2/2]

```C++
QString QfReferencingFeatureListModel::attachmentFieldName () const
```



Returns the field name of the first ExternalResource field found on the referencing layer, or an empty string if no such field exists. 


        

<hr>



### function attachmentStorageAuthConfigId [2/2]

```C++
QString QfReferencingFeatureListModel::attachmentStorageAuthConfigId () const
```



Returns the authentication configuration ID for the external storage configured on the attachment field, or an empty string if none is set. 


        

<hr>



### function attachmentStorageType [2/2]

```C++
QString QfReferencingFeatureListModel::attachmentStorageType () const
```



Returns the external storage type configured on the attachment field (e.g. "WebDAV"), or an empty string if none is set. 


        

<hr>



### function attachmentStorageUrl [2/2]

```C++
QString QfReferencingFeatureListModel::attachmentStorageUrl () const
```



Returns the external storage URL configured on the attachment field (e.g. "https://server.com/remote.php/dav/files/user/"), or an empty string if none is set. 


        

<hr>



### function currentNmRelationId [2/2]

_On many-to-many relations returns the second relation id connecting the children in the association table to their other parent._ 
```C++
QString QfReferencingFeatureListModel::currentNmRelationId () const
```




<hr>



### function currentRelationId [2/2]

_Returns the id of the relation connecting the parent feature with the children in this model._ 
```C++
QString QfReferencingFeatureListModel::currentRelationId () const
```




<hr>



### function deleteFeature 

```C++
Q_INVOKABLE bool QfReferencingFeatureListModel::deleteFeature (
    QgsFeatureId referencingFeatureId
) 
```



Deletes a feature regarding the referencing layer and the feature id of the selected child 

**Parameters:**


* `referencingFeatureId` id of the selected child 




        

<hr>



### function feature [2/2]

```C++
QgsFeature QfReferencingFeatureListModel::feature () const
```



The parent feature for which this model contains the children 

**Returns:**

the parent feature 




**See also:** [**setFeature**](classQfReferencingFeatureListModel.md#function-setfeature) 



        

<hr>



### function getFeatureIdRow 

```C++
Q_INVOKABLE int QfReferencingFeatureListModel::getFeatureIdRow (
    QgsFeatureId featureId
) 
```



Returns the row number for a given feature id 

**Parameters:**


* `featureId` the feature id 




        

<hr>



### function isLoading [2/2]

```C++
bool QfReferencingFeatureListModel::isLoading () const
```



Indicator if the model is currently performing any feature iteration in the background. 


        

<hr>



### function nmRelation [2/2]

```C++
QgsRelation QfReferencingFeatureListModel::nmRelation () const
```



On many-to-many relations this is the second relation connecting the children in the associationtable to their other parent 

**Returns:**

associated relation 




**See also:** [**setNmRelation**](classQfReferencingFeatureListModel.md#function-setnmrelation) 



        

<hr>



### function parentPrimariesAvailable [2/2]

```C++
bool QfReferencingFeatureListModel::parentPrimariesAvailable () const
```



The status if the pk of the parent feature (this feature) are valid (not null) It's needed to check on opening a form to add a new child 

**Returns:**

parentPrimariesAvailable The status if the parent pks are available 




**See also:** [**setParentPrimariesAvailable**](classQfReferencingFeatureListModel.md#function-setparentprimariesavailable) 



        

<hr>



### function relation [2/2]

```C++
QgsRelation QfReferencingFeatureListModel::relation () const
```



The relation connecting the parent feature with the children in this model 

**Returns:**

relation 




**See also:** [**setRelation**](classQfReferencingFeatureListModel.md#function-setrelation) 



        

<hr>



### function reload 

```C++
Q_INVOKABLE void QfReferencingFeatureListModel::reload () 
```



Reloads the model by starting the reload functionality in the gatherer (seperate thread) Sets the property parentPrimariesAvailable 


        

<hr>



### function setCurrentNmRelationId 

_On many-to-many relations sets the second relation connecting the children in the association table to their other parent._ 
```C++
void QfReferencingFeatureListModel::setCurrentNmRelationId (
    const QString & nmRelationId
) 
```




<hr>



### function setCurrentRelationId 

_Sets the relation connecting the parent feature with the children in this model._ 
```C++
void QfReferencingFeatureListModel::setCurrentRelationId (
    const QString & relationId
) 
```




<hr>



### function setFeature 

```C++
void QfReferencingFeatureListModel::setFeature (
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
void QfReferencingFeatureListModel::setNmRelation (
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
void QfReferencingFeatureListModel::setParentPrimariesAvailable (
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
void QfReferencingFeatureListModel::setRelation (
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
void QfReferencingFeatureListModel::setSortOrder (
    Qt::SortOrder sortOrder
) 
```





**Parameters:**


* `sortOrder` The new sort order to use. 




        

<hr>



### function sortOrder [2/2]

_Returns the current sort order (ascending or descending)._ 
```C++
Qt::SortOrder QfReferencingFeatureListModel::sortOrder () const
```




<hr>
## Protected Functions Documentation




### function lessThan 

```C++
bool QfReferencingFeatureListModel::lessThan (
    const QModelIndex & left,
    const QModelIndex & right
) override const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/gui/qfreferencingfeaturelistmodel.h`

